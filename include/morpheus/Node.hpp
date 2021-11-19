#ifndef MORPHEUS_NODE_HPP
#define MORPHEUS_NODE_HPP

#include <ostream>
#include <map>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>

namespace morph
{
	template <typename VALUE_TYPE>
	class Node : public std::enable_shared_from_this<Node<VALUE_TYPE>>
    {
    public:
        using node_ptr = std::shared_ptr<Node>;
        using node_weakPtr = std::weak_ptr<Node>;

        Node() = default;

        explicit Node(std::size_t level) : _level {level} {};

        /*
         *  Adds new child 'newNode' with name 'name'
         */
        inline void add(std::string name, node_ptr newNode);

        /*
         *  Finds child with 'name' and returns it
         */
        node_ptr find(std::string name) const noexcept;

        /*
         *  Finds parent of child with name 'name' and returns it
         */
        node_ptr findParentOf(std::string name) const noexcept;

        /*
         *  Finds all parents of child with name 'name' and returns vector of them
         */
        std::vector<node_ptr> findAllParentsOf(std::string name) noexcept;

        /*
         *  Returns current node _childs
         */
        inline node_ptr get(std::string name) const noexcept;

        /*
         *  Returns _childs of node
         */
        std::map<std::string, node_ptr>& getChilds() { return _childs; }
        const std::map<std::string, node_ptr>& getChilds() const { return _childs; }

        /*
         *  Returns _childs count
         */
	    std::size_t count() const { return _childs.size(); }

	    bool isEmpty() const { return _childs.size() == 0; }

	    /*
	     *  Returns available nodes names
	     */
	    const std::vector<std::string>& getAvailableNodes() const { return _availableNodes; }

	    /*
	     *  Returns _level of node
	     */
	    size_t getLevel() const { return _level; }

	    /*
	     *  Sets _level
	     */
	    void setLevel(size_t newLevel) { _level =  newLevel; }

	    node_ptr getRoot() const;

	    void setRoot(node_ptr root);

		const std::string &getName() const;

		void setName(const std::string &name);

	public:
    	VALUE_TYPE value;
    private:
    	std::vector<std::string> _availableNodes;
        std::map<std::string, node_ptr> _childs;
        std::string _name;
        node_weakPtr _root;
        std::size_t _level{};
    };

	template <typename T>
	using node_templatePtr = std::shared_ptr<Node<T>>;
	template <typename T>
	using node_templateWeakPtr = std::weak_ptr<Node<T>>;

	template <typename T>
	std::ostream& operator << (std::ostream& os, const node_templatePtr<T> node)
	{
		os << "{" << node->value << "}";
		return os;
	}

	template <typename T>
	void Node<T>::add(std::string name, node_ptr newNode)
	{
		if (std::find(_availableNodes.cbegin(), _availableNodes.cend(), name) != _availableNodes.cend())
		{
			throw std::runtime_error("morpheus: failed to add new node " + name + " to " + _name + ".\nIt's already exists!");
		}
		_availableNodes.push_back(name);
		_childs[name] = newNode;
	}

	template <typename T>
	node_templatePtr<T> Node<T>::find(std::string name) const noexcept
	{
		auto it = std::find_if(_childs.cbegin(), _childs.cend(), [node_name = name](const auto& pair)
		{
			return pair.first == node_name;
		});
		if (it != _childs.cend()) return (*it).second;
		for (const auto& elem : _childs)
		{
			if (elem.second != nullptr)
			{
				if (elem.second->getLevel() < _level) break;
				auto buf = elem.second->find(name);
				if (buf != nullptr) return buf;
			}
		}
		return nullptr;
	}

	template <typename T>
	node_templatePtr<T> Node<T>::findParentOf(std::string name) const noexcept
	{
		auto it = std::find_if(_childs.cbegin(), _childs.cend(), [node_name = name](const auto& pair)
		{
			return pair.first == node_name;
		});
		if (it != _childs.cend()) return this;
		for (const auto& elem : _childs)
		{
			if (elem.second != nullptr)
			{
				if (elem.second->getLevel() < _level) break;
				auto buf = elem.second->findParentOf(name);
				if (buf != nullptr) return buf;
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<node_templatePtr<T>> Node<T>::findAllParentsOf(std::string name) noexcept
	{
		std::vector<node_ptr> parents;
		auto it = std::find_if(_childs.cbegin(), _childs.cend(), [node_name = name](const auto& pair)
		{
			return pair.first == node_name;
		});
		if (it != _childs.cend())
		{
			parents.push_back(this->shared_from_this());
		}
		for (const auto& elem : _childs)
		{
			if (elem.second != nullptr)
			{
				if (elem.second->getLevel() < _level) break;
				auto bufVector = elem.second->findAllParentsOf(name);
				for (const auto& subElem : bufVector)
				{
					parents.push_back(subElem);
				}
			}
		}
		return parents;
	}

	template <typename T>
	node_templatePtr<T> Node<T>::get(std::string name) const noexcept
	{
		auto it = std::find_if(_childs.cbegin(), _childs.cend(), [node_name = std::move(name)](const auto& pair)
		{
			return pair.first == node_name;
		});
		if (it == _childs.cend()) return nullptr;
		return (*it).second;
	}

	template <typename VALUE_TYPE>
	node_templatePtr<VALUE_TYPE> Node<VALUE_TYPE>::getRoot() const
	{
		return _root;
	}

	template <typename VALUE_TYPE>
	void Node<VALUE_TYPE>::setRoot(node_templatePtr<VALUE_TYPE> root)
	{
		Node::_root = root;
	}

	template <typename VALUE_TYPE>
	const std::string& Node<VALUE_TYPE>::getName() const
	{
		return _name;
	}

	template <typename VALUE_TYPE>
	void Node<VALUE_TYPE>::setName(const std::string &name)
	{
		_name = name;
	}
}

#endif //MORPHEUS_NODE_HPP
