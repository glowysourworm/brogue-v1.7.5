#pragma once

#include "stringExtension.h"
#include "simpleString.h"

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
			return intToString(in).get(0);
		}
		static char shortToChar(short in)
		{
			return shortToString(in).get(0);
		}
		static char longToChar(long in)
		{
			return longToString(in).get(0);
		}
		static char ulongToChar(unsigned long in)
		{
			return ulongToString(in).get(0);
		}

		static int stringToInt(const simpleString& input)
		{
			return atoi(input.c_str());
		}
		static bool tryStringToInt(const simpleString& input, int& result)
		{
			try
			{
				result = atoi(input.c_str());

				return true;
			}
			catch (std::exception ex)
			{
				return false;
			}
		}
		static long stringTolong(const simpleString& input)
		{
			return atol(input.c_str());
		}
		static short stringToshort(const simpleString& input)
		{
			return (short)atoi(input.c_str());
		}
		static float stringTofloat(const simpleString& input)
		{
			return atof(input.c_str());
		}
		static double stringTodouble(const simpleString& input)
		{		
			return (double)atof(input.c_str());
		}
		static bool stringToBool(const simpleString& input)
		{
			simpleString upper = input.toUpper();

			if (upper == "TRUE")
				return true;

			return false;
		}

		static simpleString intToString(int in)
		{
			return std::to_string(in).c_str();
		}
		static simpleString shortToString(short in)
		{
			return std::to_string(in).c_str();
		}
		static simpleString longToString(long in)
		{
			return std::to_string(in).c_str();
		}
		static simpleString ulongToString(unsigned long in)
		{
			return std::to_string(in).c_str();
		}
	};
}