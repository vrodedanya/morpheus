#ifndef MORPHEUS_LOADER_HPP
#define MORPHEUS_LOADER_HPP

#include <morpheus/Node.hpp>
#include <fstream>
#include <type_traits>
#include <morpheus/HasDataType.hpp>
#include <sstream>

namespace morph
{
	/*
	 *  Loads all data from file and returns std::string
	 */
	std::string loadDataFromFile(const std::string& file);

	template <typename DATA_TYPE>
	class Loader
	{
	public:
		Loader() = delete;

		/*
		 *  Creates tree from data
		 */
		static nodeTemplatePtr<DATA_TYPE> parseFromData(const std::string& data);
		static nodeTemplatePtr<DATA_TYPE> parseFromFile(const std::string& file);

	private:
		static void parse(nodeTemplatePtr<DATA_TYPE>& root, std::string line);

		static void addChildren(nodeTemplatePtr<DATA_TYPE>& root, nodeTemplatePtr<DATA_TYPE>& newNode, std::string& line);
	};

	std::string loadDataFromFile(const std::string& file)
	{
		std::ifstream f(file);
		if (!f.is_open()) throw std::ios_base::failure("Failed to open file: " + file);
		std::stringstream bufferString;
		bufferString << f.rdbuf();
		return bufferString.str();
	}

	template<typename T>
	nodeTemplatePtr<T> Loader<T>::parseFromData(const std::string& data)
	{
		std::stringstream sheetStream;

		sheetStream << data;
		nodeTemplatePtr<T> root = std::make_shared<Node<T>>();

		std::string line;
		while (std::getline(sheetStream, line, ';'))
		{
			if (!line.empty())
			{
				parse(root, line);
				line.clear();
			}
		}
		return root;
	}


	template<typename T>
	nodeTemplatePtr<T> Loader<T>::parseFromFile(const std::string& file)
	{
		std::string sheet = loadDataFromFile(file);
		std::stringstream sheetStream;
		sheetStream << sheet;

		nodeTemplatePtr<T> root = std::make_shared<Node<T>>();

		std::string line;
		while (std::getline(sheetStream, line, ';'))
		{
			if (!line.empty())
			{
				parse(root, line);
				line.clear();
			}
		}
		return root;
	}

	template<typename T>
	void Loader<T>::parse(nodeTemplatePtr<T>& root, std::string line)
	{
		nodeTemplatePtr<T> newNode = std::make_shared<Node<T>>();

		std::string node_name = line.substr(line.find('[') + 1, line.find(']') - line.find('[') -1);
		line = line.substr(node_name.size());

		std::string value = line.substr(line.find('{') + 1, line.find('}') - line.find('{') - 1 );
		if constexpr (std::is_base_of<HasDataType, T>::value)
		{
			newNode->value.registerData();
			newNode->value.getData(value);
		}
		else
		{
			newNode->value = value;
		}

		if (!root->count())
		{
			newNode->setLevel(1);
			root->add(node_name, newNode);
		}
		else
		{
			auto bufVector = root->findAllParentsOf(node_name);
			if (bufVector.empty()) throw std::runtime_error("Dangling Node '" + node_name + "'");
			int level = 0;
			for (auto& buf : bufVector)
			{
				if (buf->getLevel() > level) level = buf->getLevel();
				buf->getChilds()[node_name] = newNode;
			}
			newNode->setLevel(level + 1);
		}
		addChildren(root, newNode, line);
	}

	template<typename T>
	void Loader<T>::addChildren(nodeTemplatePtr<T>& root, nodeTemplatePtr<T>& newNode, std::string& line)
	{
		if (line.find("->") != std::string::npos)
		{
			line = line.substr(line.find("->") + 1);
			std::string subNode_name;
			while (line.find('[') != std::string::npos)
			{
				subNode_name = line.substr(line.find('[') + 1, line.find(']') - line.find('[') - 1);
				line = line.substr(line.find(']') + 1);
				auto subNode_node = root->find(subNode_name);
				newNode->add(subNode_name, subNode_node);
			}
		}
	}
}

#endif //MORPHEUS_LOADER_HPP
