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
		static simpleArray<std::string> split(const std::string& input, const char* tokens)
		{	
			simpleList<std::string> result;
			simpleList<std::string> list;
			
			list.add(input);

			for (int index = 0; index < sizeof(tokens); index++)
			{
				for (int listIndex = 0; listIndex < list.count(); listIndex++)
				{
					int lastIndex = -1;

					// Scan the string for the next token
					for (int strIndex = 0; strIndex < list.get(listIndex).size(); strIndex++)
					{
						// Found token!
						if (list.get(listIndex).at(strIndex) == tokens[index])
						{
							std::string subString = (lastIndex == -1) ? list.get(listIndex).substr(0, strIndex) : 
																		list.get(listIndex).substr(lastIndex, strIndex - lastIndex);

							// Remove white space
							if (!isWhitespace(subString))
								result.add(subString);

							lastIndex = strIndex;
						}
					}

					// No match found
					if (lastIndex == -1)
						result.add(list.get(listIndex));
				}

				// Setup results for next iteration
				if (index < sizeof(tokens) - 1)
				{
					list.clear();
					list.addRange(result);

					result.clear();			// Search for smaller splits
				}
			}
			
			return result.toArray();
		}
	};
}