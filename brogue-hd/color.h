#pragma once

#include "simple.h"

using namespace brogueHd::simple;

namespace brogueHd::backend::model::game
{
	struct color : hashable
	{
	public:

		// base RGB components: (0, 1) -> (0, 255)
		float red;
		float green;
		float blue;

		color() 
		{
			red = 0;
			green = 0;
			blue = 0;
		}
		color(const color& color)
		{
			red = color.red;
			green = color.green;
			blue = color.blue;
		}
		color(short ared, short agreen, short ablue)
		{
			red = ared;
			green = agreen;
			blue = ablue;
		}
		bool compare(const color& color) const
		{
			return red == color.red &&
					green == color.green &&
					blue == color.blue;
		}
		bool operator ==(const color& other) const
		{
			return compare(other);
		}
		bool operator !=(const color& other) const
		{
			return !compare(other);
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(red, green, blue);
		}
	};
}