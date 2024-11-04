#pragma once

#include "simple.h"

#include <corecrt.h>
#include <ctime>
#include <stdexcept>
#include <string>

namespace brogueHd::simple
{
	class simpleExt
	{

	private:

		/// <summary>
		/// Use this error handler here because of compilation issues with simpleException. simpleString extension
		/// methods have exceptions to throw too :)
		/// </summary>
		static void showError(const char* error)
		{
			throw std::runtime_error(error);
		}

	private:

		template<isNumber T>
		static void toStringImpl(const T& param, std::string& result)
		{
			result = std::to_string(param);
		}

		template<isChar T>
		static void toStringImpl(const T& param, std::string& result)
		{
			result += param;
		}

		template<isStringLike T>
		static void toStringImpl(const T& param, std::string& result)
		{
			result = std::string(param);
		}

	public:

		template<isStringConvertible T>
		static std::string format(const char* formatStr, const T& param)
		{
			// Format String (recurse)
			std::string result(formatStr);
			std::string paramStr;
			std::string replaceStr = "{}";

			simpleExt::toStringImpl(param, paramStr);

			// Find!
			size_t index = result.find(replaceStr.c_str());

			// Nothing to do...!
			if (index < 0 || index >= result.length())
				return result;

			// Replace!
			result.replace(index, replaceStr.length(), paramStr.c_str());

			return result;
		}

		template<isStringConvertible T, isStringConvertible...Args>
		static std::string format(const char* formatStr, const T& param, const Args&...rest)
		{
			// Use the single param function overload
			std::string result = format(formatStr, param);

			return format(result.c_str(), rest...);
		}

		template<isStringConvertible T>
		static std::string toString(const T& param)
		{
			std::string result;

			if (isNumber<T>)
			{
				simpleExt::toStringImpl(param, result);
			}
			else if (isStringLike<T>)
			{
				simpleExt::toStringImpl(param, result);
			}
			else
				showError("Invalid use of concept constraints:  simpleExt::format");

			return result;
		}

		static std::string formatDate(time_t time, bool shortFormat)
		{
			char buffer[80];
			tm timeValue;

			localtime_s(&timeValue, &time);

			if (shortFormat)
				std::strftime(buffer, 80, "%m/%d/%Y", &timeValue);
			else
				std::strftime(buffer, 80, "%m/%d/%Y %H:%M:%S", &timeValue);

			return std::string(buffer);
		}
	};
}

