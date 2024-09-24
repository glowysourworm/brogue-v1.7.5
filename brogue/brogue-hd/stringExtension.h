#pragma once

#include "broguedef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <format>

using namespace std;

namespace brogueHd::backend::extension
{
	struct stringExtension
	{
		template<typename T>
		static T convert(std::string input)
		{
			if (std::is_convertible<std::string>())
				return dynamic_cast<T>(input);

			else if (std::is_convertible<int>())
				return dynamic_cast<T>(atoi(input.c_str()));

			else if (std::is_convertible<short>())
				return dynamic_cast<T>(atoi(input.c_str()));

			else if (std::is_convertible<float>())
				return dynamic_cast<T>(atof(input.c_str()));

			else if (std::is_convertible<bool>())
				return dynamic_cast<T>(convertBool(input));

			else
				brogueException::show(std::format("Unhandled type:  stringExtension::convert  {}", typeid(T)));
				
		}
		static bool convertBool(const std::string& input)
		{
			if (toUpper(input) == "TRUE")
				return true;

			return false;
		}
		static std::string formatDate(time_t time)
		{
			char buffer[80];
			std::strftime(buffer, 80, "%Y-%m-%d-%H:%M:%S", localtime(&time));
			std::string result(buffer);

			return result;
		}
		static std::string toUpper(const string& str) 
		{
			std::string result;

			for (std::string::const_iterator beg = str.begin(); beg != str.end(); ++beg) 
			{
				result.push_back(toupper(*beg));
			}

			return result;
		}
		static std::vector<std::string> toVector(char** strings)
		{
			std::vector<std::string> result;

			for (int index = 0; index < SIZEOF(strings); index++)
			{
				result.push_back(std::string(strings[index]));
			}

			return result;
		}
		static std::string join(char** strings)
		{
			std::string result;

			for (int index = 0; index < SIZEOF(strings); index++)
			{
				result += strings[index];
			}
		}
		static std::string* split(const std::string& input, const char* tokens)
		{
			std::vector<std::string> strings;

			char* inputPtr = (char*)input.c_str();

			do
			{
				inputPtr = strtok(inputPtr, tokens);

			} while(inputPtr != NULL);

			return strings.data();
		}
	};
}