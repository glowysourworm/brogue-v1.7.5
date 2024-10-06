#pragma once

#include "simpleString.h"
#include "brogueGlobal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <format>

namespace brogueHd::component
{
	struct stringExtension
	{
		static simpleString formatDate(time_t time)
		{
			char buffer[80];
			tm timeValue;

			localtime_s(&timeValue, &time);

			std::strftime(buffer, 80, "%Y-%m-%d-%H:%M:%S", &timeValue);
			simpleString result(buffer);

			return result;
		}
		static simpleString join(char* strings[], int arrayLength)
		{
			simpleString result;
			
			for (int index = 0; index < arrayLength; index++)
			{
				result += strings[index];
			}

			return result;
		}
	};
}