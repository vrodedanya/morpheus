#include <morpheus/HasDataType.hpp>

#include <stdexcept>
#include <algorithm>

void morph::HasDataType::setData(Mandatory type, const std::string& name, std::string* newData)
{
	data[name] = Value{type, newData};
}

// Returns -1 if '=' isn't in line
static std::size_t getKey(const std::string& line, const std::string& key, std::string& keyBuffer)
{
	std::size_t begin = line.find(key);
	std::size_t end = line.find('=');
	if (begin == std::string::npos || end == std::string::npos) return -1;
	keyBuffer = line.substr(begin, end - begin);
	keyBuffer.erase(std::remove(keyBuffer.begin(), keyBuffer.end(), ' '), keyBuffer.end());
	return end;
}

// Return -1 if '"' aren't in line
static std::size_t getValue(const std::string& line, std::string& value, std::size_t position)
{
	std::size_t valueBegin = line.find('"', position) + 1;
	std::size_t valueEnd  = line.find('"', valueBegin + 1);
	if (valueBegin == std::string::npos || valueEnd == std::string::npos)
	{
		return -1;
	}
	value = line.substr(valueBegin, valueEnd - valueBegin);
	return valueEnd;
}

// Return -1 if line ends
static std::size_t reduceLine(std::string& line)
{
	std::size_t valueType = line.find(',');
	if (valueType == std::string::npos)
	{
		return -1;
	}
	std::string value = line.substr(valueType + 1);
	line = value;
	return 1;
}

void morph::HasDataType::getData(const std::string& line)
{
	for (auto& elem : data)
	{
		std::string bufferLine = line;
		bool isInited = false;
		while (bufferLine.size() > elem.first.size())
		{
			std::string key;
			std::size_t keyEnd = getKey(bufferLine, elem.first, key);
			if (keyEnd == -1) break;
			if (elem.first == key)
			{
				std::string value;
				std::size_t valueEnd = getValue(bufferLine, value, keyEnd);
				if (valueEnd == -1) throw std::runtime_error("getData: Bad string format for " + key);
				*elem.second.data = value;
				isInited = true;
				break;
			}
			else
			{
				if (reduceLine(bufferLine) == -1) break;
			}
		}
		if (!isInited)
		{
			if (elem.second.type == Mandatory::MANDATORY) throw std::runtime_error(elem.first + " is mandatory!");
		}
	}
}