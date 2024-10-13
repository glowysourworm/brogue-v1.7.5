#pragma once

#include "simple.h"

using namespace brogueHd::simple;

namespace brogueHd::backend::model::game
{
	struct color : hashable
	{
	public:

		// base RGB components:
		short red;
		short green;
		short blue;

		// random RGB components to add to base components:
		short redRand;
		short greenRand;
		short blueRand;

		// random scalar to add to all components:
		short rand;

		// Flag: this color "dances" with every refresh:
		bool colorDances;

		color() 
		{
			red = 0;
			green = 0;
			blue = 0;
			redRand = 0;
			greenRand = 0;
			blueRand = 0;
			rand = 0;
			colorDances = false;
		}
		color(const color& color)
		{
			red = color.red;
			green = color.green;
			blue = color.blue;
			redRand = color.redRand;
			greenRand = color.greenRand;
			blueRand = color.blueRand;
			rand = color.rand;
			colorDances = color.colorDances;
		}
		color(short ared, short agreen, short ablue, short rred, short rgreen, short rblue, short rrand, bool acolorDances)
		{
			red = ared;
			green = agreen;
			blue = ablue;
			redRand = rred;
			greenRand = rgreen;
			blueRand = rblue;
			rand = rrand;
			colorDances = acolorDances;
		}
		bool compare(const color& color) const
		{
			return red == color.red &&
					green == color.green &&
					blue == color.blue &&
					redRand == color.redRand &&
					greenRand == color.greenRand &&
					blueRand == color.blueRand &&
					rand == color.rand &&
					colorDances == color.colorDances;
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
			return hashGenerator::generateHash(red, green, blue, redRand, greenRand, blueRand, rand, colorDances);
		}
	};
}