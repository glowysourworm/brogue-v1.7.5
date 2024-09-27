#pragma once

#include "broguedef.h"
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
			char buffer[SIZEOF(int)];
			itoa(in, buffer, 10);
			return *buffer;
		}
		static char shortToChar(short in)
		{
			int upcast = (int)in;
			return intToChar(upcast);
		}
		static char longToChar(long in)
		{
			char buffer[SIZEOF(long)];
			ltoa(in, buffer, 10);
			return *buffer;
		}
		static char ulongToChar(unsigned long in)
		{
			char buffer[SIZEOF(unsigned long)];
			ltoa(in, buffer, 10);
			return *buffer;
		}

		static std::string intToString(int in)
		{
			char buffer[SIZEOF(int)];
			itoa(in, buffer, 10);
			return std::string(buffer);
		}
		static std::string shortToString(short in)
		{
			int upcast = (int)in;
			return intToString(upcast);
		}
		static std::string longToString(long in)
		{
			char buffer[SIZEOF(long)];
			ltoa(in, buffer, 10);
			return std::string(buffer);
		}
		static std::string ulongToString(unsigned long in)
		{
			char buffer[SIZEOF(unsigned long)];
			ltoa(in, buffer, 10);
			return std::string(buffer);
		}

	private:

		template<typename Tout, typename Tin>
		static Tout valueCast(const Tin& in)
		{
			return dynamic_cast<Tout>(in);
		}
	};
}