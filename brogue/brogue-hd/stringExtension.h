#pragma once

#include "broguedef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string>
#include <vector>

using namespace std;

namespace brogueHd::backend::extension
{
	struct stringExtension
	{
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