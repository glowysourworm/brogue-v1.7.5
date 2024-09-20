#pragma once

#include "gridRect.h"
#include "broguedef.h"

namespace brogueHd::backend::model::layout
{
	struct cellularAutomataParameters
	{
		gridRect boundary;
		float fillRatio;
		short birthCount;
		short survivalCount;
		short smoothingIterations;

		cellularAutomataParameters()
		{
			boundary = gridRect(0, 0, DCOLS, DROWS);
			fillRatio = 0.55;
			birthCount = 3;
			survivalCount = 5;
			smoothingIterations = 5;
		}

		cellularAutomataParameters(gridRect aboundary, float afillRatio, short abirthCount, short asurvivalCount, short asmoothingIterations)
		{
			boundary = aboundary;
			fillRatio = afillRatio;
			birthCount = abirthCount;
			survivalCount = asurvivalCount;
			smoothingIterations = asmoothingIterations;
		}
	};
}