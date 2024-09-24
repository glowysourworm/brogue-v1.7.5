#pragma once

#include "mathdef.h"
#include "color.h"

using namespace brogueHd::backend::math;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model::lighting
{
	typedef struct lightSource {
		const color* lightColor;
		randomRange lightRadius;
		short radialFadeToPercent;
		bool passThroughCreatures;			// generally no, but miner light does
	} lightSource;
}