#pragma once

#include "randomRange.h"
#include "dungeonTemplate.h"
#include <string>

using namespace brogueHd::component::math;

namespace brogueHd::backend::model::gameTemplate
{
	struct itemTemplate
	{
		keyLocation location;
		short originDepth;
	};
}