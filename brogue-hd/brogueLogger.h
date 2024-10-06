#pragma once

#include "simpleString.h"
#include <iostream>

namespace brogueHd::component
{
	class brogueLogger
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

		static const int White = 97;
		static const int Green = 92;
		static const int Yellow = 93;
		static const int Red = 91;
		static const int Blue = 94;

		static void log(const simpleString& message, const bool& endline = true)
		{
			if (endline)
				std::cout << message.c_str() << std::endl;
			else
				std::cout << message.c_str();
		}
		static void logGreen(const simpleString& message, const bool& endline = true)
		{
			setColor(Green);

			if (endline)
				std::cout << message.c_str() << std::endl;
			else
				std::cout << message.c_str();

			setColor(White);
		}
		static void logRed(const simpleString& message, const bool& endline = true)
		{
			setColor(Red);

			if (endline)
				std::cout << message.c_str() << std::endl;
			else
				std::cout << message.c_str();

			setColor(White);
		}
		static void logBlue(const simpleString& message, const bool& endline = true)
		{
			setColor(Blue);

			if (endline)
				std::cout << message.c_str() << std::endl;
			else
				std::cout << message.c_str();

			setColor(White);
		}
		static void logYellow(const simpleString& message, const bool& endline = true)
		{
			setColor(Yellow);

			if (endline)
				std::cout << message.c_str() << std::endl;
			else
				std::cout << message.c_str();

			setColor(White);
		}

	private:

		static void setColor(int textColor)
		{
			std::cout << "\033[" << textColor << "m";
		}
	};
}