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
				if (line.size()) parse(root, line);
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
				auto bufVector = root->findAllParentsNode(node_name);
				for (auto& buf : bufVector)
				{
					if (buf == nullptr) throw std::runtime_error("Dangling node '" + node_name + "'");
					buf->getChilds()[node_name] = newNode;
				}
			}
		}
		static void add_childs(node_ptr<T>& newNode, std::string& line)
		{
			if (line.find(':') != std::string::npos)
			{
				line = line.substr(line.find(':') + 1);
				std::string subNode;
				while (line.find('[') != std::string::npos)
				{
					subNode = line.substr(line.find('[') + 1, line.find(']') - line.find('[') - 1);
					line = line.substr(line.find(']') + 1);
					newNode->add(subNode, nullptr);
				}
			}
		}
	};
}

#endif //MORPHEUS_TREELOADER_HPP
