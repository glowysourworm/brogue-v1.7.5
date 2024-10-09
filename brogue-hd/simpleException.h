#pragma once

#include <stdexcept>

namespace brogueHd::simple
{
	class simpleException
	{
	public:

		static void show(const char* message)
		{
			throw std::runtime_error(message);
		}

		static void showCstr(const char* message)
		{
			throw std::runtime_error(message);
		}
		//
		//		static void show(const simpleString& message)
		//		{
		//
		//#ifdef _DEBUG
		//			// Send to the console
		//			//simpleLogger::log<simpleString>("Brogue Error Handler:  {}", message);
		//
		//			//if (true)
		//			//	//throw std::runtime_error(message);
		//			//	throw std::exception(message.c_str());
		//#else
		//			// TODO: figure out way to show portable Brogue message box. Then use the halt variable
		//
		//			// Show the user a message box
		//			throw std::runtime_error(message);
		//#endif
		//		}
	};
}