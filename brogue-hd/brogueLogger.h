#pragma once

#include <string>
#include <iostream>

namespace brogueHd
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

		static void log(const std::string& message, const bool& endline = true)
		{
			if (endline)
				std::cout << message << std::endl;
			else
				std::cout << message;
		}
		static void logGreen(const std::string& message, const bool& endline = true)
		{
			setColor(Green);

			if (endline)
				std::cout << message << std::endl;
			else
				std::cout << message;

			setColor(White);
		}
		static void logRed(const std::string& message, const bool& endline = true)
		{
			setColor(Red);

			if (endline)
				std::cout << message << std::endl;
			else
				std::cout << message;

			setColor(White);
		}
		static void logBlue(const std::string& message, const bool& endline = true)
		{
			setColor(Blue);

			if (endline)
				std::cout << message << std::endl;
			else
				std::cout << message;

			setColor(White);
		}
		static void logYellow(const std::string& message, const bool& endline = true)
		{
			setColor(Yellow);

			if (endline)
				std::cout << message << std::endl;
			else
				std::cout << message;

			setColor(White);
		}

	private:

		static void setColor(int textColor)
		{
			std::cout << "\033[" << textColor << "m";
		}
	};
}