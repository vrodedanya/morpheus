#ifndef MORPHEUS_VALUETYPE_HPP
#define MORPHEUS_VALUETYPE_HPP

#include <map>
#include <string>

namespace morph
{
	class ValueType
	{
	protected: // METHODS
		enum class Mandatory
		{
			OPTIONAL,
			MANDATORY,
		};

		// Implement this method for your data
		virtual void registerData() = 0;

		// Use this method for register data in registerData
		void setData(Mandatory type, const std::string& name, std::string* element);

	private: // METHODS
		void getData(const std::string& line);

	private: // DATA
		struct Value
		{
			Mandatory type;
			std::string* data;
		};

		std::map<std::string, Value> data;

		template<typename T>
		friend class TreeLoader;
	};
}

#endif //MORPHEUS_VALUETYPE_HPP
