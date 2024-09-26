#pragma once

#include "color.h"
#include "creature.h"
#include "creatureBehavior.h"
#include "statusEffectConstants.h"

using namespace brogueHd::backend::modelConstant;

namespace brogueHd::backend::model::effect
{
	// This is just termporary
	struct characterStatusEffect
	{
		statusEffects status;
		short maxStatus;								// used to set the max point on the status bars
	};

	// TODO
	struct characterEffect
	{
		short flashStrength;							// monster will flash soon; this indicates the percent strength of flash
		color flashColor;								// the color that the monster will flash
	};
}