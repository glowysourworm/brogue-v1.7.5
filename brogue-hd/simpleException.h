#pragma once

#include "simple.h"
#include "simpleExt.h"
//#include "simpleString.h"
#include <stdexcept>

namespace brogueHd::simple
{
	class simpleException
	{
	public:

		//static void show(const simpleString& message)
		//{
		//	throw std::exception(message.c_str());
		//}

		static void showCstr(const char* message)
		{
			throw std::exception(message);

			throw;
		}
		static void show(const char* message)
		{
			throw std::exception(message);

			throw;
		}

	public:

		template<isStringConvertible T>
		static void show(const char* formatStr, const T& param)
		{
			std::string message = simpleExt::format(formatStr, param);

			showCstr(message.c_str());
		}

		template<isStringConvertible T, isStringConvertible...Args>
		static void show(const char* formatStr, const T& param, const Args&...rest)
		{
			std::string message = simpleExt::format(formatStr, param, rest...);

			showCstr(message.c_str());
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