#pragma once

#include "color.h"
#include "randomRange.h"

using namespace brogueHd::component::math;

namespace brogueHd::backend::model::game
{
	struct light 
	{

		short red;
		short green;
		short blue;

		light() {}
		light(short ared, short agreen, short ablue)
		{
			red = ared;
			green = agreen;
			blue = ablue;
		}
		bool compare(const light& light)
		{
			return red == light.red && blue == light.blue && green == light.green;
		}

	};

	struct lightSource
	{
		color lightColor;
		randomRange lightRadius;
		short radialFadeToPercent;
		bool passThroughCreatures;			// generally no, but miner light does
	};
}