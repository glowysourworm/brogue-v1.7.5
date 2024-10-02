#pragma once

#include <string>
#include <iostream>

using namespace std;

namespace brogueHd
{
	class brogueLogger
	{
	public:

		static void output(const std::string& message)
		{
			std::cout << "Log Message:  " << message << std::endl;
		}
	};
}