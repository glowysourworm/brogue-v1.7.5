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

		//template<isNumber T>
		//static void toStringImpl(const T& param, std::string& result)
		//{
		//	result = std::to_string(param);
		//}

		//template<isChar T>
		//static void toStringImpl(const T& param, std::string& result)
		//{
		//	result += param;
		//}

		//template<isSimpleObject T>
		//static void toStringImpl(const T& param, std::string& result)
		//{
		//	result = std::string(param.toString());
		//}

		//template<isStringLike T>
		//static void toStringImpl(const T& param, std::string& result)
		//{
		//	result = std::string(param);
		//}

		//template<isPointer T>
		//static void toStringImpl(const T& param, std::string& result)
		//{
		//	result = std::string("(pointer)");
		//}

	public:

		template<isStringConvertible T>
		static std::string format(const char* formatStr, const T& param)
		{
			// Format String (recurse)
			std::string result(formatStr);
			std::string replaceStr = "{}";

			std::string paramStr = simpleExt::toString(param);

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

		//template<isStringConvertible T1, isStringConvertible T2>
		//static std::string format(const char* formatStr, const T1& param1, const T2& param2)
		//{
		//	// Use the single param function overload
		//	std::string result = format(formatStr, param1);

		//	return format(result, param2);
		//}


		//template<isStringConvertible T1, isStringConvertible T2, isStringConvertible T3>
		//static std::string format(const char* formatStr, const T1& param1, const T2& param2, const T3& param3)
		//{
		//	// Use the single param function overload
		//	std::string result = format(formatStr, param1);

		//	return format(result, param2, param3);
		//}

		//template<isStringConvertible T1, isStringConvertible T2, isStringConvertible T3, isStringConvertible T4>
		//static std::string format(const char* formatStr, const T1& param1, const T2& param2, const T3& param3, const T4& param4)
		//{
		//	// Use the single param function overload
		//	std::string result = format(formatStr, param1);

		//	return format(result, param2, param3, param4);
		//}

		//template<isStringConvertible T1, isStringConvertible T2, isStringConvertible T3, isStringConvertible T4, isStringConvertible T5>
		//static std::string format(const char* formatStr, const T1& param1, const T2& param2, const T3& param3, const T4& param4, const T5& param5)
		//{
		//	// Use the single param function overload
		//	std::string result = format(formatStr, param1);

		//	return format(result, param2, param3, param4, param5);
		//}

		template<isStringConvertible T>
		static std::string toString(const T& param)
		{
			std::string result;

			if constexpr (isNumber<T>)
			{
				return std::to_string(param);
			}
			else if constexpr (std::same_as<T, char>)
			{
				result += param;
				return result;
			}
			else if constexpr (std::same_as<T, const char*>)
			{
				return std::string(param);
			}
			else if constexpr (isStringLike<T>)
			{
				return std::string(param);
			}
			else if constexpr (isSimpleObject<T>)
			{
				return std::string(param.getString());
			}
			else if constexpr (isPointer<T>)
			{
				return std::string("(pointer)");
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

