#pragma once

#include "simple.h"

#include <stdexcept>
#include <string>
#include <ctime>

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
		static std::string toStringImpl(const T& param)
		{
			return std::to_string(param);
		}

		template<isChar T>
		static std::string toStringImpl(const T& param)
		{
			std::string result;
			result += param;
			return result;
		}

		template<isStringLike T>
		static std::string toStringImpl(const T& param)
		{
			return std::string(param);
		}

	public:

		template<isStringConvertible T>
		static std::string format(const char* formatStr, const T& param)
		{
			// Format String (recurse)
			std::string result(formatStr);
			std::string paramStr = simpleExt::toStringImpl(param);
			std::string replaceStr = "{}";

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
		static const char* toString(const T& param)
		{
			if (isNumber<T>)
			{
				return simpleExt::toStringImpl(param).c_str();
			}
			else if (isStringLike<T>)
			{
				return simpleExt::toStringImpl(param).c_str();
			}
			else
				showError("Invalid use of concept constraints:  simpleExt::format");
		}

		static const char* formatDate(time_t time)
		{
			char buffer[80];
			tm timeValue;

			localtime_s(&timeValue, &time);

			std::strftime(buffer, 80, "%Y-%m-%d-%H:%M:%S", &timeValue);

			return buffer;
		}
	};
}

