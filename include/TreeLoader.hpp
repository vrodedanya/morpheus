#ifndef MORPHEUS_TREELOADER_HPP
#define MORPHEUS_TREELOADER_HPP

#include <Node.hpp>
#include <fstream>
#include <type_traits>
#include <ValueType.hpp>
#include <sstream>

namespace morph
{
	/*
	 *  Loads all data from file and returns std::string
	 */
	std::string loadDataFromFile(const std::string& file);

	template <typename T>
	class TreeLoader
	{
	public:
		TreeLoader() = delete;

		/*
		 *  Creates tree from data
		 */
		static nodeTemplatePtr<T> parseFromData(const std::string& file);

	private:
		static void parse(nodeTemplatePtr<T>& root, std::string line);

		static void addChilds(nodeTemplatePtr<T>& root, nodeTemplatePtr<T>& newNode, std::string& line);
	};

	std::string loadDataFromFile(const std::string& file)
	{
		std::ifstream f(file);
		std::string sheet;
		std::string buffer;
		while (std::getline(f, buffer)){sheet += buffer;}
		return sheet;
	}

	template<typename T>
	nodeTemplatePtr<T> TreeLoader<T>::parseFromData(const std::string& file)
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
	void TreeLoader<T>::parse(nodeTemplatePtr<T>& root, std::string line)
	{
		nodeTemplatePtr<T> newNode = std::make_shared<Node<T>>();

		std::string node_name = line.substr(line.find('[') + 1, line.find(']') - line.find('[') -1);
		line = line.substr(node_name.size());

		std::string value = line.substr(line.find('{') + 1, line.find('}') - line.find('{') - 1 );
		if constexpr (std::is_base_of<ValueType, T>::value)
		{
			newNode->value.registerData();
			newNode->value.getData(value);
		}
		else
		{
			newNode->value = value;
		}

		if (!root->size())
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
		addChilds(root, newNode, line);
	}

	template<typename T>
	void TreeLoader<T>::addChilds(nodeTemplatePtr<T>& root, nodeTemplatePtr<T>& newNode, std::string& line)
	{
		if (line.find(':') != std::string::npos)
		{
			line = line.substr(line.find(':') + 1);
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

#endif //MORPHEUS_TREELOADER_HPP
