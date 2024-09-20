#pragma once

#include "broguedef.h"

namespace brogueHd::backend::model::game
{
	struct color 
	{
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
		boolean colorDances;

		color() {}
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
		color(short ared, short agreen, short ablue, short rred, short rgreen, short rblue, short rrand, boolean acolorDances)
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
		bool compare(const color& color)
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
	};
}