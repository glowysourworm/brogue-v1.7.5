#pragma once

#include "brogueLogger.h"
#include "simpleString.h"
#include <stdexcept>
#include <iostream>

namespace brogueHd::component
{
	class brogueException
	{
	public:

		static void show(const simpleString& message)
		{
			show(message, false);
		}

		static void show(const simpleString& message, bool halt = false)
		{

#ifdef _DEBUG
			// Send to the console
			brogueLogger::logRed("Brogue Error Handler:  " + message);

			if (halt)
				//throw std::runtime_error(message);
				throw std::exception(message.c_str());
#else
			// TODO: figure out way to show portable Brogue message box. Then use the halt variable

			// Show the user a message box
			throw std::runtime_error(message);
#endif
		}

		static void show(const char* message, bool halt = false)
		{
			show(simpleString(message), halt);
		}
	};
}