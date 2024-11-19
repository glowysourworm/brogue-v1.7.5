#pragma once


using namespace brogueHd::component;

namespace brogueHd::backend::model
{
	struct cellularAutomataParameters
	{
		float fillRatio;
		short birthCount;
		short survivalCount;
		short smoothingIterations;

		cellularAutomataParameters()
		{
			fillRatio = 0.55f;
			birthCount = 3;
			survivalCount = 5;
			smoothingIterations = 5;
		}

		cellularAutomataParameters(float afillRatio, short abirthCount, short asurvivalCount, short asmoothingIterations)
		{
			fillRatio = afillRatio;
			birthCount = abirthCount;
			survivalCount = asurvivalCount;
			smoothingIterations = asmoothingIterations;
		}
	};
}