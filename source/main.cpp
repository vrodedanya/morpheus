#include <iostream>
#include <morpheus/morpheus.hpp>

class Scenario : public morph::HasDataType<Scenario>
{
public:
	void registerData() override
	{
		setData(Mandatory::MANDATORY, "message1", &Scenario::message1);
		setData(Mandatory::MANDATORY, "message2", &Scenario::message2);
	}
	std::string message1;
	std::string message2;
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
		auto t = morph::Loader<Scenario>::parseFromFile("file.scen");
		t->print(std::cerr);

	std::string answer;
	while (true)
	{
		std::cout << "Message 1: " << t->value.message1 << "\n";
		std::cout << "Message 2: " << t->value.message2;
		std::cout << std::endl;
		if (t->isEmpty()) break;
		std::cout << "Available answers: "<< t->getAvailableNodes() << std::endl;
		std::getline(std::cin, answer);
		auto buf = t->get(answer);
		if (buf == nullptr) std::cout << "Wrong answer! I can't continue dialogue" << std::endl;
		else t = buf;
	}
    return 0;
}
