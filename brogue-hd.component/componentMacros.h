#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

namespace brogueHd::component
{
	// STD Library / substitutions
	#define SIZEOF(x)						sizeof(x)			// Size of an array
	#define STRCMP(x, y)					strcmp(x,y)			// String compare
	#define STRCPY(dest, src, count)		strncpy_s(dest, src, count)
}