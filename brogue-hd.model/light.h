#pragma once

#include "color.h"
#include "randomRange.h"

using namespace brogueHd::simple;

namespace brogueHd::model
{
	struct light
	{
		int red;
		int green;
		int blue;

		light()
		{
		}

		light(int ared, int agreen, int ablue)
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
		int radialFadeToPercent;
		bool passThroughCreatures; // generally no, but miner light does
	};
}
