#include <iostream>
#include <fstream>
#include <treeLoader.hpp>

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
	auto t = morph::treeLoader<std::string>::load("scenario.cfg");
	std::string answer;
	while (true)
	{
		std::cout << t->value << std::endl;
		if (!t->size()) break;
		std::cout << "Available answers: "<< t->getAvailableNodes() << std::endl;
		std::cin >> answer;
		try
		{
			t = t->get(answer);
		}
		catch (const std::exception& except)
		{
			std::cerr << "Wrong answer! I can't continue dialogue" << std::endl;
		}
	}
    return 0;
}
