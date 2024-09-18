#pragma once

#include "broguedef.h"
#include "gridRect.h"
#include <array>

using namespace std;

namespace broguehHd
{
	struct cellularAutomataParameters 
	{
		gridRect boundary;
		short fillValue;
		float fillRatio;
		boolean birthParameters[9];
		boolean survivalParameters[9];
		short smoothingIterations;

		cellularAutomataParameters()
		{
			boundary = gridRect();
			fillValue = 0;
			fillRatio = 0;
			//birthParameters = "ttttttttt";
			//survivalParameters = "ttttttttt";
			smoothingIterations = 0;
		}

		cellularAutomataParameters(gridRect aboundary, short afillValue, float afillRatio, boolean abirthParameters[9], boolean asurvivalParameters[9], short asmoothingIterations)
		{
			boundary = aboundary;
			fillValue = afillValue;
			fillRatio = afillRatio;

			// TODO: Use std library
			strcpy(birthParameters, abirthParameters);
			strcpy(survivalParameters, asurvivalParameters);
			
			smoothingIterations = asmoothingIterations;
		}

	};
}