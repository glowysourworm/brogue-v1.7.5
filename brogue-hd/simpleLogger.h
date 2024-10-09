#pragma once

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


		//template<isStringConvertible T, isStringConvertible...Rest>
		//static void log(const T& format, const T& param, Rest...rest)
		//{
		//	setColor(color);

		//	simpleString message = simpleString::format(format, param, ...rest);

		//	std::cout << message;

		//	setColor(brogueConsoleColor::White);
		//}

	private:

		static void setColor(int textColor)
		{
			std::cout << "\033[" << textColor << "m";
		}
	};
}