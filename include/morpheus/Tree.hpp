#ifndef MORPHEUS_TREE_HPP
#define MORPHEUS_TREE_HPP

#include "Node.hpp"

namespace morph
{
	template <typename SCENARIO_CLASS>
	class Tree
	{
	public:
		bool switchNode(std::string newNode)
		{
			_currentNode = _currentNode->get(newNode);
			return _currentNode != nullptr;
		}
		const node_templatePtr<SCENARIO_CLASS>& node() const {return _currentNode;}
		node_templatePtr<SCENARIO_CLASS>& node() {return _currentNode;}

		node_templatePtr<SCENARIO_CLASS> root() {return _root;}
	private:
		node_templatePtr<SCENARIO_CLASS> _root;
		node_templatePtr<SCENARIO_CLASS> _currentNode;

		template <typename T>
		friend class Loader;
	};
}

#endif //MORPHEUS_TREE_HPP
