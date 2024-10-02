#pragma once

#include "simpleList.h"
#include "simpleArray.h"
#include "brogueGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <format>

using namespace std;

namespace brogueHd::component
{
	struct stringExtension
	{
		static std::string formatDate(time_t time)
		{
			char buffer[80];
			tm timeValue;

			localtime_s(&timeValue, &time);

			std::strftime(buffer, 80, "%Y-%m-%d-%H:%M:%S", &timeValue);
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

			for (int index = 0; index < sizeof(strings); index++)
			{
				result.push_back(std::string(strings[index]));
			}

			return result;
		}
		static std::string join(char* strings[], int arrayLength)
		{
			std::string result;
			

			for (int index = 0; index < arrayLength; index++)
			{
				result += strings[index];
			}

			return result;
		}
		static bool isWhitespace(const std::string& s)
		{
			for (int index = 0; index < s.size(); index++)
			{
				if (isspace(s[index]))
					return false;
			}

			return true;
		}
		static simpleArray<std::string> split(const std::string& input, const simpleArray<char>& tokens)
		{
			simpleList<std::string> result;
			simpleList<std::string> list;

			result.add(input);

			for (int index = 0; index < tokens.count(); index++)
			{
				char token = tokens.get(index);

				list.clear();

				// Iterate the result list, swap back to list
				result.forEach([&token, &list](std::string str)
				{
					simpleArray<std::string> strings = split(str, token);

					list.addRange(strings);

					return iterationCallback::iterate;
				});

				result.clear();
				result.addRange(list);
				list.clear();
			}

			return result.toArray();
		}
		static simpleArray<std::string> split(const std::string& input, const char token)
		{	
			simpleList<std::string> result;
			
			int lastIndex = -1;

			for (int strIndex = 0; strIndex < input.size(); strIndex++)
			{
				// Found token!
				if (input[strIndex] == token)
				{
					std::string subString = (lastIndex == -1) ? input.substr(0, strIndex) : input.substr(lastIndex + 1, strIndex - lastIndex - 1);

					result.add(subString);

					lastIndex = strIndex;
				}
			}

			// Add the last sub-string
			if (lastIndex != -1)
				result.add(input.substr(lastIndex + 1, input.size() - lastIndex - 1));

			else
				result.add(input);

			return result.toArray();
		}
	};
}