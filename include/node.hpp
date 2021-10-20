#ifndef MORPHEUS_NODE_HPP
#define MORPHEUS_NODE_HPP

#include <map>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <vector>

namespace morph
{
	template<typename VALUE_TYPE>
    class node
    {
    public:
        using node_ptr = std::shared_ptr<node>;

        node() = default;
        explicit node(std::size_t level) : level{level} {};

        void add(std::string name, node_ptr newNode)
        {
        	availableNodes.push_back(name);
        	childs[name] = newNode;
        }

        node_ptr find(std::string name)
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
        node* findParentOf(std::string name)
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
        std::vector<node*> findAllParentsOf(std::string name)
        {
        	std::cout << name << std::endl;
        	std::vector<node*> parents;
	            auto it = std::find_if(childs.cbegin(), childs.cend(), [node_name = name](const auto& pair)
	            {
	                return pair.first == node_name;
	            });
        	if (it != childs.cend())
        	{
        		parents.push_back(this);
        	}
        	for (const auto& elem : childs)
        	{
        		if (elem.second != nullptr)
        		{
        			std::cerr << elem.second->getLevel() << " " << level << std::endl;
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

        node_ptr get(std::string name)
        {
        	auto it = std::find_if(childs.cbegin(), childs.cend(), [node_name = std::move(name)](const auto& pair)
        	{
				return pair.first == node_name;
        	});
        	if (it == childs.cend()) return nullptr;
        	return (*it).second;
        }

        std::map<std::string, node_ptr>& getChilds()
        {
        	return childs;
        }

	    const std::weak_ptr<node>& getParent() const
	    {
		    return parent;
	    }

	    void setParent(const std::weak_ptr<node> &_parent)
	    {
		    this->parent = _parent;
	    }

	    std::size_t size() const noexcept {return childs.size();}

	    const std::vector<std::string>& getAvailableNodes() const
	    {
		    return availableNodes;
	    }
	    size_t getLevel() const
	    {
        	return level;
	    }

	    void setLevel(size_t level)
	    {
        	node::level = level;
	    }
    public:
		VALUE_TYPE value;
    private:
    	std::vector<std::string> availableNodes;
        std::map<std::string, node_ptr> childs;
        std::weak_ptr<node> parent;
        std::size_t level{};
    };

	template<typename T>
	using node_ptr = std::shared_ptr<node<T>>;

	template <typename T>
	std::ostream& operator << (std::ostream& os, const node_ptr<T> node)
	{
		os << "{" << node->value << "}";
		return os;
	}
}

#endif //MORPHEUS_NODE_HPP
