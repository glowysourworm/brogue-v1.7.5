#pragma once

#include "creatureBehaviorConstants.h"

namespace brogueHd::model
{
	struct creatureBehavior
	{
		creatureStates creatureState; // current behavioral state
		creatureModes creatureMode; // current behavioral mode (higher-level than state)
	};
}
