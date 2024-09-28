#pragma once

#include <stdexcept>
#include <string>
#include <iostream>

using namespace std;

namespace brogueHd::component
{
	class brogueException
	{
	public:

		static void show(const std::string& message, bool halt = false)
		{
			show(message.c_str(), halt);
		}

		static void show(const char* message, bool halt = false)
		{

#ifdef DEBUG
			// Send to the console
			cout << "Brogue Error Handler:  " << message;

			if (halt)
				//throw std::runtime_error(message);
				throw std::exception(message);
#else
			// TODO: figure out way to show portable Brogue message box. Then use the halt variable

			// Show the user a message box
			throw std::runtime_error(message);
#endif
		}
	};
}