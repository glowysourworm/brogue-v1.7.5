#pragma once

#include "color.h"
#include "creatureDeclaration.h"
#include "creatureBehaviorDeclaration.h"

namespace brogueHd::backend::model::effect
{
	enum statusEffects 
	{
		STATUS_SEARCHING = 0,
		STATUS_DONNING,
		STATUS_WEAKENED,
		STATUS_TELEPATHIC,
		STATUS_HALLUCINATING,
		STATUS_LEVITATING,
		STATUS_SLOWED,
		STATUS_HASTED,
		STATUS_CONFUSED,
		STATUS_BURNING,
		STATUS_PARALYZED,
		STATUS_POISONED,
		STATUS_STUCK,
		STATUS_NAUSEOUS,
		STATUS_DISCORDANT,
		STATUS_IMMUNE_TO_FIRE,
		STATUS_EXPLOSION_IMMUNITY,
		STATUS_NUTRITION,
		STATUS_ENTERS_LEVEL_IN,
		STATUS_MAGICAL_FEAR,
		STATUS_ENTRANCED,
		STATUS_DARKNESS,
		STATUS_LIFESPAN_REMAINING,
		STATUS_SHIELDED,
		STATUS_INVISIBLE,
		STATUS_AGGRAVATING,
		NUMBER_OF_STATUS_EFFECTS
	};

	// This is just termporary
	struct characterStatusEffect
	{
		statusEffects status[NUMBER_OF_STATUS_EFFECTS];
		short maxStatus[NUMBER_OF_STATUS_EFFECTS];		// used to set the max point on the status bars
	};

	// TODO
	struct characterEffect
	{
		short flashStrength;							// monster will flash soon; this indicates the percent strength of flash
		color flashColor;								// the color that the monster will flash
	};
}