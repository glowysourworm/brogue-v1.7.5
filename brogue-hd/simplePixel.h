#pragma once

#include "bitmap.h"
#include "simple.h"

namespace brogueHd::simple
{
	struct simplePixel : hashable
	{
		int32_t red;
		int32_t green;
		int32_t blue;
		int32_t alpha;

		simplePixel()
		{
			red = 0;
			green = 0;
			blue = 0;
			alpha = 0;
		}
		simplePixel(int32_t r, int32_t g, int32_t b, int32_t a)
		{
			red = r;
			green = g;
			blue = b;
			alpha = a;
		}
		simplePixel(const simplePixel& copy)
		{
			red = copy.red;
			green = copy.green;
			blue = copy.blue;
			alpha = copy.alpha;
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(red, green, blue, alpha);
		}
	};
}