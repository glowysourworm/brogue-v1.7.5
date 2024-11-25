#pragma once

#include "color.h"
#include "statusEffectConstants.h"

namespace brogueHd::backend::model
{
	// This is just termporary
	struct characterStatusEffect
	{
		statusEffects status;
		int maxStatus;								// used to set the max point on the status bars
	};

	// TODO
	struct characterEffect
	{
		int flashStrength;							// monster will flash soon; this indicates the percent strength of flash
		color flashColor;								// the color that the monster will flash
	};
}