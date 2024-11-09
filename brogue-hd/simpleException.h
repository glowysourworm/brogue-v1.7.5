#pragma once

#include "simple.h"
#include "simpleExt.h"
#include <exception>
#include <iostream>
#include <stacktrace>
#include <string>

namespace brogueHd::simple
{
	class simpleException : std::exception
	{

	public:

		simpleException() : simpleException("BrogueHd Error (No Message Specified)")
		{
		}

		simpleException(const char* message) :  std::exception(message)
		{
			std::string result = message;

			for (int index = 0; index < std::stacktrace::current().size(); index++)
			{
				const std::stacktrace_entry stack = std::stacktrace::current().at(index);

				const char* format1 = "Source File:  {}  Line:  {}  \r\n";
				const char* format2 = "Description:  {} \r\n";

				result += simpleExt::format(format1, stack.source_file().c_str(), stack.source_line());
				result += simpleExt::format(format2, stack.description().c_str());
			}

#ifdef _DEBUG
			std::cout << result << std::endl;
#endif
			_message = result;
		}

		virtual const char* what() const noexcept
		{
			return _message.c_str();
		}



	public:

		//static void show(const simpleString& message)
		//{
		//	throw std::exception(message.c_str());
		//}

		static void showCstr(const char* message) throw()
		{
			std::cout << message << std::endl;
			std::cout << std::stacktrace::current() << std::endl;

			throw message;
		}
		static void show(const char* message) throw()
		{
			std::cout << message << std::endl;
			std::cout << std::stacktrace::current() << std::endl;

			throw message;
		}

	public:

		template<isStringConvertible T>
		static void show(const char* formatStr, const T& param) throw()
		{
			std::string message = simpleExt::format(formatStr, param);

			showCstr(message.c_str());
		}

		template<isStringConvertible T, isStringConvertible...Args>
		static void show(const char* formatStr, const T& param, const Args&...rest) throw()
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

	private:

		std::string _message;
	};
}