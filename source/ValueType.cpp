#include "ValueType.hpp"

#include <stdexcept>
#include <algorithm>

void morph::ValueType::setData(Mandatory type, const std::string& name, std::string* newData)
{
	data[name] = Value{type, newData};
}

void morph::ValueType::getData(const std::string& line)
{
	for (auto& elem : data)
	{
		std::string bufferLine = line;
		bool isInited = false;
		while (bufferLine.size() > elem.first.size())
		{
			std::size_t begin = bufferLine.find(elem.first);
			std::size_t end = bufferLine.find('=');
			if (begin == std::string::npos || end == std::string::npos) break;
			std::string buffer = bufferLine.substr(begin, end - begin);
			buffer.erase(std::remove(buffer.begin(), buffer.end(), ' '), buffer.end());
			if (elem.first == buffer)
			{
				std::size_t valueBegin = bufferLine.find('"', end);
				std::size_t valueEnd  = bufferLine.find('"', valueBegin + 1);
				if (valueBegin == std::string::npos || valueEnd == std::string::npos)
				{
					throw std::runtime_error("getData: Bad string format for " + buffer);
				}
				else
				{
					std::string value = bufferLine.substr(valueBegin + 1, valueEnd - valueBegin - 1);
					*elem.second.data = value;
				}
				isInited = true;
				break;
			}
			else
			{
				std::size_t valueType = bufferLine.find(',');
				if (valueType == std::string::npos)
				{
					break;
				}
				std::string value = bufferLine.substr(valueType + 1);
				bufferLine = value;
			}
		}
		if (!isInited)
		{
			if (elem.second.type == Mandatory::MANDATORY) throw std::runtime_error(elem.first + " is mandatory!");
		}
	}
}