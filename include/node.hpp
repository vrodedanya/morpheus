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

        void add(std::string name, node_ptr newNode)
        {
        	availableNodes.push_back(name);
        	childs[name] = newNode;
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
        			auto buf = elem.second->findParentOf(name);
        			if (buf != nullptr) return buf;
        		}
        	}
        	return nullptr;
        }
        std::vector<node*> findAllParentsOf(std::string name)
        {
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
        			auto buf = elem.second->findParentOf(name);
        			if (buf != nullptr) parents.push_back(buf);
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
        	if (it == childs.cend()) throw std::runtime_error("No node with this value");
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

	    void setParent(const std::weak_ptr<node> &parent)
	    {
		    node::parent = parent;
	    }

	    std::size_t size() const noexcept {return childs.size();}

	    const std::vector<std::string>& getAvailableNodes() const
	    {
		    return availableNodes;
	    }

    public:
		VALUE_TYPE value;
    private:
    	std::vector<std::string> availableNodes;
        std::map<std::string, node_ptr> childs;
        std::weak_ptr<node> parent;
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
