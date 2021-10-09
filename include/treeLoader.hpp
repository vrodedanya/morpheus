#ifndef MORPHEUS_TREELOADER_HPP
#define MORPHEUS_TREELOADER_HPP

#include <node.hpp>
#include <fstream>

namespace morph
{
	template <typename T>
	class treeLoader
	{
	public:
		static node_ptr<T> load(const std::string& file)
		{
			std::ifstream f(file);

			node_ptr<T> root = nullptr;

			std::string line;
			while (std::getline(f, line))
			{
				parse(root, line);
			}
			return root;
		}
	private:
		static void parse(node_ptr<T>& root, std::string line)
		{
			node_ptr<T> newNode = std::make_shared<node<T>>();

			std::string node_name = line.substr(1, line.find(']') - 1);
			line = line.substr(node_name.size());

			std::string value = line.substr(line.find('{') + 1, line.find('}') - line.find('{') - 1 );
			newNode->value = value;

			add_childs(newNode, line);

			if (root == nullptr)
			{
				root = newNode;
			}
			else
			{
				auto buf = root->findParentNode(node_name);
				if (buf == nullptr) throw std::runtime_error("Dangling node");
				buf->getChilds()[node_name] = newNode;
			}
		}
		static void add_childs(node_ptr<T>& newNode, std::string& line)
		{
			if (line.find(':') != std::string::npos)
			{
				line = line.substr(line.find(':') + 1);
				bool isNode = false;
				std::string subNode_name;
				for (const auto& sym : line)
				{
					if (isNode)
					{
						if (sym == ']')
						{
							newNode->add(subNode_name, nullptr);
							subNode_name.clear();
							isNode = false;
							continue;
						}
						subNode_name += sym;
					}
					else
					{
						if (sym == '[') isNode = true;
					}
				}
			}
		}
	};
}

#endif //MORPHEUS_TREELOADER_HPP
