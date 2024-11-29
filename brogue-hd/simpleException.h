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

		simpleException(const std::string& message) : simpleException(message.c_str())
		{
		}

		simpleException(const char* message) :  std::exception(message)
		{
			std::string result = message;

			result += "\r\n";

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

	private:

		std::string _message;
	};
}