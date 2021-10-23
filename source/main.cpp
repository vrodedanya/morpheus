#include <iostream>
#include <TreeLoader.hpp>

class Scenario : public morph::ValueType
{
public:
	void registerData() override
	{
		setData(Mandatory::MANDATORY, "message1", &message1);
		setData(Mandatory::MANDATORY, "message2", &message2);
		setData(Mandatory::OPTIONAL, "message3", &message3);
	}
	std::string message1;
	std::string message2;
	std::string message3;
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
	auto t = morph::TreeLoader<Scenario>::parseFromData("scenario1.cfg");
	std::string answer;
	t = t->get("begin");
	while (true)
	{
		std::cout << "Message 1: " << t->value.message1 << "\n";
		std::cout << "Message 2: " << t->value.message2;
		if (!t->value.message3.empty())
		{
			std::cout << "\nMessage 3: " << t->value.message3;
		}
		std::cout << std::endl;
		if (!t->size()) break;
		std::cout << "Available answers: "<< t->getAvailableNodes() << std::endl;
		std::getline(std::cin, answer);
		auto buf = t->get(answer);
		if (buf == nullptr) std::cout << "Wrong answer! I can't continue dialogue" << std::endl;
		else t = buf;
	}
    return 0;
}
