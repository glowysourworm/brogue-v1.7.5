#pragma once

#include "creatureBehaviorConstants.h"

using namespace brogueHd::backend::modelConstant;

namespace brogueHd::backend::model::creature
{
	struct creatureBehavior
	{
		creatureStates creatureState;		// current behavioral state
		creatureModes creatureMode;			// current behavioral mode (higher-level than state)
	};
}