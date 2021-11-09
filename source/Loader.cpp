#include <morpheus/Loader.hpp>

std::string morph::loadDataFromFile(const std::string& file)
{
	std::ifstream f(file);
	if (!f.is_open()) throw std::ios_base::failure("Failed to open file: " + file);
	std::stringstream bufferString;
	bufferString << f.rdbuf();
	return bufferString.str();
}