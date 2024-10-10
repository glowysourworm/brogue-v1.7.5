#pragma once

#include "simple.h"
#include "simpleString.h"
#include <stdexcept>

namespace brogueHd::simple
{
	class simpleException
	{
	public:

		static void show(const simpleString& message)
		{
			//throw std::exception(message.c_str());
		}

		static void showCstr(const char* message)
		{
			//throw std::exception(message);
		}

		template<isNumber T>
		static void show(const simpleString& formatStr, const T& param)
		{
			simpleString result = formatStr;

			result.replaceFirst("{}", simpleString::toStringNumber(param));

			simpleException::show(result);
		}

		template<isStringLike T>
		static void show(const simpleString& formatStr, const T& param)
		{
			simpleString result = formatStr;

			result.replaceFirst("{}", simpleString(param));

			simpleException::show(result);
		}

		template<isStringConvertible T1, isStringConvertible T2>
		static void show(const simpleString& formatStr, const T1& param0, const T2& param1)
		{
			simpleString result = formatStr;

			result = simpleString::format<T1>(formatStr, param0);
			result = simpleString::format<T2>(result, param1);

			simpleException::show(result);
		}

		template<isStringConvertible T1, isStringConvertible T2, isStringConvertible T3>
		static void show(const simpleString& formatStr, const T1& param0, const T2& param1, const T3& param2)
		{
			simpleString result = formatStr;

			result = simpleString::format<T1>(formatStr, param0);
			result = simpleString::format<T2>(result, param1);
			result = simpleString::format<T3>(result, param2);

			simpleException::show(result);
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