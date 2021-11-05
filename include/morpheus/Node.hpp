#ifndef MORPHEUS_NODE_HPP
#define MORPHEUS_NODE_HPP

#include <ostream>
#include <map>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <vector>

namespace morph
{
	template<typename VALUE_TYPE>
    class Node
    {
    public:
        using nodePtr = std::shared_ptr<Node>;

        Node() = default;

        explicit Node(std::size_t level) : level {level} {};

        /*
         *  Adds new child 'newNode' with name 'name'
         */
        void add(std::string name, nodePtr newNode);

        /*
         *  Finds child with 'name' and returns it
         */
        nodePtr find(std::string name) const;

        /*
         *  Finds parent of child with name 'name' and returns it
         */
        Node* findParentOf(std::string name) const;

        /*
         *  Finds all parents of child with name 'name' and returns vector of them
         */
        std::vector<Node*> findAllParentsOf(std::string name) const;

        /*
         *  Returns current node childs
         */
        nodePtr get(std::string name) const;

        /*
         *  Returns childs of node
         */
        std::map<std::string, nodePtr>& getChilds() { return childs; }
        const std::map<std::string, nodePtr>& getChilds() const { return childs; }

        /*
         *  Returns childs count
         */
	    std::size_t count() const noexcept { return childs.size(); }

	    bool isEmpty() const noexcept { return childs.size() == 0; }

	    /*
	     *  Returns available nodes names
	     */
	    const std::vector<std::string>& getAvailableNodes() const { return availableNodes; }

	    /*
	     *  Returns level of node
	     */
	    size_t getLevel() const { return level; }

	    /*
	     *  Sets level
	     */
	    void setLevel(size_t newLevel) { level =  newLevel; }

	    nodePtr getRoot() const;

	    void setRoot(nodePtr root);

    public:
    	VALUE_TYPE value;
    private:
    	std::vector<std::string> availableNodes;
        std::map<std::string, nodePtr> childs;
        nodePtr root;
        std::size_t level{};
    };

	template<typename T>
	using nodeTemplatePtr = std::shared_ptr<Node<T>>;

	template <typename T>
	std::ostream& operator << (std::ostream& os, const nodeTemplatePtr<T> node)
	{
		os << "{" << node->value << "}";
		return os;
	}

	template<typename T>
	void Node<T>::add(std::string name, nodePtr newNode)
	{
		availableNodes.push_back(name);
		childs[name] = newNode;
	}

	template<typename T>
	nodeTemplatePtr<T> Node<T>::find(std::string name) const
	{
		auto it = std::find_if(childs.cbegin(), childs.cend(), [node_name = name](const auto& pair)
		{
			return pair.first == node_name;
		});
		if (it != childs.cend()) return (*it).second;
		for (const auto& elem : childs)
		{
			if (elem.second != nullptr)
			{
				if (elem.second->getLevel() < level) break;
				auto buf = elem.second->find(name);
				if (buf != nullptr) return buf;
			}
		}
		return nullptr;
	}

	template<typename T>
	Node<T>* Node<T>::findParentOf(std::string name) const
	{
		auto it = std::find_if(childs.cbegin(), childs.cend(), [node_name = name](const auto& pair)
		{
			return pair.first == node_name;
		});
		if (it != childs.cend()) return this;
		for (const auto& elem : childs)
		{
			if (elem.second != nullptr)
			{
				if (elem.second->getLevel() < level) break;
				auto buf = elem.second->findParentOf(name);
				if (buf != nullptr) return buf;
			}
		}
		return nullptr;
	}

	template<typename T>
	std::vector<Node<T>*> Node<T>::findAllParentsOf(std::string name) const
	{
		std::vector<Node*> parents;
		auto it = std::find_if(childs.cbegin(), childs.cend(), [node_name = name](const auto& pair)
		{
			return pair.first == node_name;
		});
		if (it != childs.cend())
		{
			parents.push_back(const_cast<morph::Node<T>*>(this));
		}
		for (const auto& elem : childs)
		{
			if (elem.second != nullptr)
			{
				if (elem.second->getLevel() < level) break;
				auto bufVector = elem.second->findAllParentsOf(name);
				for (const auto subElem : bufVector)
				{
					parents.push_back(subElem);
				}
			}
		}
		return parents;
	}

	template<typename T>
	nodeTemplatePtr<T> Node<T>::get(std::string name) const
	{
		auto it = std::find_if(childs.cbegin(), childs.cend(), [node_name = std::move(name)](const auto& pair)
		{
			return pair.first == node_name;
		});
		if (it == childs.cend()) return nullptr;
		return (*it).second;
	}

	template<typename VALUE_TYPE>
	nodeTemplatePtr<VALUE_TYPE> Node<VALUE_TYPE>::getRoot() const
	{
		return root;
	}

	template<typename VALUE_TYPE>
	void Node<VALUE_TYPE>::setRoot(nodeTemplatePtr<VALUE_TYPE> root)
	{
		Node::root = root;
	}
}

#endif //MORPHEUS_NODE_HPP
