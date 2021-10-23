#include <iostream>
#include <treeLoader.hpp>

class Scenario : public morph::ValueType
{
public:
	void registerData() override
	{
		setData(Mandatory::MANDATORY, "answer", &test1);
		setData(Mandatory::OPTIONAL, "filename", &test2);
	}
	std::string test1;
	std::string test2;
};

template<typename T>
std::ostream& operator << (std::ostream& os, const std::vector<T> vec)
{
	for (auto it = vec.cbegin() ; it != vec.cend() ; it++)
	{
		os << *it;
		if (it != (vec.cend() - 1) ) os << " ";
	}
	return os;
}

int main()
{
	auto t = morph::treeLoader<Scenario>::load("scenario.cfg");
	std::string answer;
	while (true)
	{
		std::cout << t->value.test1 << " with " << t->value.test2 << std::endl;
		if (!t->size()) break;
		std::cout << "Available answers: "<< t->getAvailableNodes() << std::endl;
		std::getline(std::cin, answer);
		auto buf = t->get(answer);
		if (buf == nullptr) std::cout << "Wrong answer! I can't continue dialogue" << std::endl;
		else t = buf;
	}
    return 0;
}
