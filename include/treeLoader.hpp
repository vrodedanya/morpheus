#ifndef MORPHEUS_TREELOADER_HPP
#define MORPHEUS_TREELOADER_HPP

#include <node.hpp>
#include <fstream>
#include <type_traits>
#include <ValueType.hpp>

namespace morph
{
	template <typename T>
	class treeLoader
	{
	public:
		static node_ptr<T> load(const std::string& file)
		{
			std::ifstream f(file);

			node_ptr<T> root = std::make_shared<node<T>>();

			std::string line;
			while (std::getline(f, line, ';'))
			{
				if (!line.empty())
				{
					parse(root, line);
					line.clear();
				}
			}
			return root;
		}
	private:
		static void parse(node_ptr<T>& root, std::string line)
		{
			node_ptr<T> newNode = std::make_shared<node<T>>();

			std::string node_name = line.substr(line.find('[') + 1, line.find(']') - line.find('[') -1);
			line = line.substr(node_name.size());

			std::string value = line.substr(line.find('{') + 1, line.find('}') - line.find('{') - 1 );
			if constexpr(std::is_base_of<ValueType, T>::value)
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
				if (bufVector.empty()) throw std::runtime_error("Dangling node '" + node_name + "'");
				int level = 0;
				for (auto& buf : bufVector)
				{
					if (buf->getLevel() > level) level = buf->getLevel();
					buf->getChilds()[node_name] = newNode;
				}
				newNode->setLevel(level + 1);
			}
			add_childs(root,newNode, line);
		}
		static void add_childs(node_ptr<T>& root, node_ptr<T>& newNode, std::string& line)
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
	};
}

#endif //MORPHEUS_TREELOADER_HPP
