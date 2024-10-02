#pragma once

#include "stringExtension.h"
#include <string>

namespace brogueHd::component
{
	/// <summary>
	/// Converts between value types, and reference types are handled with 
	/// the proper _cast keywords: dynamic_cast, static_cast, etc..
	/// </summary>
	class typeConverter
	{
	public:

		static char intToChar(int in)
		{
			return toString(in).at(0);
		}
		static char shortToChar(short in)
		{
			return toString(in).at(0);
		}
		static char longToChar(long in)
		{
			return toString(in).at(0);
		}
		static char ulongToChar(unsigned long in)
		{
			return toString(in).at(0);
		}

		static int stringToInt(const std::string& input)
		{
			return std::stoi(input);
		}
		static bool tryStringToInt(const std::string& input, int& result)
		{
			try
			{
				result = std::stoi(input);

				return true;
			}
			catch (std::exception ex)
			{
				return false;
			}
		}
		static long stringTolong(const std::string& input)
		{
			return std::stol(input);
		}
		static short stringToshort(const std::string& input)
		{
			return (short)std::stoi(input);
		}
		static float stringTofloat(const std::string& input)
		{
			return std::stof(input);
		}
		static double stringTodouble(const std::string& input)
		{
			return std::stod(input);
		}
		static bool stringToBool(const std::string& input)
		{
			std::string upper = stringExtension::toUpper(input);

			if (upper == "TRUE")
				return true;

			return false;
		}

		template<isNumber T>
		static std::string toString(T in)
		{
			return std::to_string(in);
		}

		static std::string intToString(int in)
		{
			return std::to_string(in);
		}
		static std::string shortToString(short in)
		{
			return std::to_string(in);
		}
		static std::string longToString(long in)
		{
			return std::to_string(in);
		}
		static std::string ulongToString(unsigned long in)
		{
			return std::to_string(in);
		}
	};
}