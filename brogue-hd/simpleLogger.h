#pragma once

#include "simple.h"
#include <iostream>

namespace brogueHd::simple
{
	enum brogueConsoleColor : int
	{
		White = 97,
		Green = 92,
		Yellow = 93,
		Red = 91,
		Blue = 94
	};

	class simpleLogger
	{
	public:

		/*

			TODO: Wrap terminal stream in a class

			30	Black	90	Bright Black
			31	Red		91	Bright Red
			32	Green	92	Bright Green
			33	Yellow	93	Bright Yellow
			34	Blue	94	Bright Blue
			35	Magenta	95	Bright Magenta
			36	Cyan	96	Bright Cyan
			37	White	97	Bright White

		*/

		static void log(const simpleString& message)
		{
			std::cout << message << std::endl;
		}

		template<isNumber T>
		static void log(const simpleString& formatStr, const T& param)
		{
			simpleString message = simpleString::format(formatStr, param);

			std::cout << message << std::endl;
		}

		template<isStringLike T>
		static void log(const simpleString& formatStr, const T& param)
		{
			simpleString message = simpleString::format(formatStr, param);

			std::cout << message << std::endl;
		}

		template<isStringConvertible T1, isStringConvertible T2>
		static void log(const simpleString& formatStr, const T1& param0, const T2& param1)
		{
			simpleString message = simpleString::format(formatStr, param0, param1);

			std::cout << message << std::endl;
		}

		template<isStringConvertible T1, isStringConvertible T2, isStringConvertible T3>
		static void log(const simpleString& formatStr, const T1& param0, const T2& param1, const T3& param2)
		{
			simpleString message = simpleString::format(formatStr, param0, param1, param2);

			std::cout << message << std::endl;
		}

		template<isStringConvertible T1, isStringConvertible T2, isStringConvertible T3, isStringConvertible T4>
		static void log(const simpleString& formatStr, const T1& param0, const T2& param1, const T3& param2, const T4& param3)
		{
			simpleString message = simpleString::format(formatStr, param0, param1, param2, param3);

			std::cout << message << std::endl;
		}

		static void logColor(const brogueConsoleColor& color, const simpleString& message)
		{
			setColor(color);
			std::cout << message << std::endl;
			setColor(brogueConsoleColor::White);
		}

		template<isNumber T>
		static void logColor(const brogueConsoleColor& color, const simpleString& formatStr, const T& param)
		{
			simpleString message = simpleString::format(formatStr, param);

			setColor(color);
			std::cout << message << std::endl;
			setColor(brogueConsoleColor::White);
		}

		template<isStringLike T>
		static void logColor(const brogueConsoleColor& color, const simpleString& formatStr, const T& param)
		{
			simpleString message = simpleString::format(formatStr, param);

			setColor(color);
			std::cout << message << std::endl;
			setColor(brogueConsoleColor::White);
		}

		template<isStringConvertible T1, isStringConvertible T2>
		static void logColor(const brogueConsoleColor& color, const simpleString& formatStr, const T1& param0, const T2& param1)
		{
			simpleString message = simpleString::format(formatStr, param0, param1);

			setColor(color);
			std::cout << message << std::endl;
			setColor(brogueConsoleColor::White);
		}

		template<isStringConvertible T1, isStringConvertible T2, isStringConvertible T3>
		static void logColor(const brogueConsoleColor& color, const simpleString& formatStr, const T1& param0, const T2& param1, const T3& param2)
		{
			simpleString message = simpleString::format(formatStr, param0, param1, param2);

			setColor(color);
			std::cout << message << std::endl;
			setColor(brogueConsoleColor::White);
		}

		template<isStringConvertible T1, isStringConvertible T2, isStringConvertible T3, isStringConvertible T4>
		static void logColor(const brogueConsoleColor& color, const simpleString& formatStr, const T1& param0, const T2& param1, const T3& param2, const T4& param3)
		{
			simpleString message = simpleString::format(formatStr, param0, param1, param2, param3);

			setColor(color);
			std::cout << message << std::endl;
			setColor(brogueConsoleColor::White);
		}

	private:

		static void setColor(int textColor)
		{
			std::cout << "\033[" << textColor << "m";
		}
	};
}