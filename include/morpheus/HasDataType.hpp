#ifndef MORPHEUS_HASDATATYPE_HPP
#define MORPHEUS_HASDATATYPE_HPP

#include <map>
#include <string>
#include <stdexcept>
#include <algorithm>

namespace morph
{
	template <typename SCENARIO_CLASS>
	class HasDataType
	{
	protected: // METHODS
		enum class Mandatory
		{
			OPTIONAL,
			MANDATORY,
		};

		// Implement this method for your _data
		virtual void registerData() = 0;

		// Use this method for register _data in registerData
		void setData(Mandatory type, const std::string& name, std::string SCENARIO_CLASS::* element);

	private: // METHODS
		void getData(const std::string& line);
		static void clear();

		static int getKey(const std::string& line, const std::string& key, std::string& keyBuffer);
		static int getValue(const std::string& line, std::string& value, std::size_t position);
		static int reduceLine(std::string& line);


	private: // DATA
		struct Value
		{
			Mandatory type;
			std::string SCENARIO_CLASS::* data;
		};

		static inline std::map<std::string, Value> _data;


		template <typename T>
		friend class Loader;
	};

	template <typename SCENARIO_CLASS>
	void HasDataType<SCENARIO_CLASS>::setData(Mandatory type, const std::string& name, std::string SCENARIO_CLASS::* element)
	{
		_data[name] = Value{type, element};
	}

	template <typename SCENARIO_CLASS>
	void HasDataType<SCENARIO_CLASS>::clear()
	{
		_data.clear();
	}

	template <typename SCENARIO_CLASS>
	void morph::HasDataType<SCENARIO_CLASS>::getData(const std::string& line)
	{
		for (auto& elem : _data)
		{
			std::string bufferLine = line;
			bool isInited = false;
			while (bufferLine.size() > elem.first.size())
			{
				std::string key;
				int keyEnd = getKey(bufferLine, elem.first, key);
				if (keyEnd == -1) break;
				if (elem.first == key)
				{
					std::string value;
					int valueEnd = getValue(bufferLine, value, keyEnd);
					if (valueEnd == -1) throw std::runtime_error("getData: Bad string format for " + key);
					//*elem.second.data = value;
					(*static_cast<SCENARIO_CLASS*>(this)).*elem.second.data = value;
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
				if (elem.second.type == Mandatory::MANDATORY) throw std::runtime_error(elem.first + " is mandatory!\nCan't find in " + line);
			}
		}
	}


	template <typename SCENARIO_CLASS>
	int morph::HasDataType<SCENARIO_CLASS>::getKey(const std::string& line, const std::string& key, std::string& keyBuffer)
	{
		std::size_t begin = line.find(key);
		std::size_t end = line.find('=');
		if (begin == std::string::npos || end == std::string::npos) return -1;
		keyBuffer = line.substr(begin, end - begin);
		keyBuffer.erase(std::remove(keyBuffer.begin(), keyBuffer.end(), ' '), keyBuffer.end());
		return static_cast<int>(end);
	}

	template <typename SCENARIO_CLASS>
	int morph::HasDataType<SCENARIO_CLASS>::getValue(const std::string& line, std::string& value, std::size_t position)
	{
		std::size_t valueBegin = line.find('"', position) + 1;
		std::size_t valueEnd  = line.find('"', valueBegin + 1);
		if (valueBegin == std::string::npos || valueEnd == std::string::npos)
		{
			return -1;
		}
		value = line.substr(valueBegin, valueEnd - valueBegin);
		return static_cast<int>(valueEnd);
	}

	template <typename SCENARIO_CLASS>
	int morph::HasDataType<SCENARIO_CLASS>::reduceLine(std::string& line)
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

}

#endif //MORPHEUS_HASDATATYPE_HPP
