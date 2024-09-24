#pragma once

#include "dungeonDeclaration.h"
#include "cellularAutomataParameters.h"
#include "gridRect.h"
#include "grid.h"
#include "gridCell.h"
#include "gridDefinitions.h"
#include "brogueCell.h"
#include "randomGenerator.h"

using namespace brogueHd::backend::model::layout;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::generator
{
	class noiseGenerator
	{
	public:

		noiseGenerator(randomGenerator* randomGenerator);
		~noiseGenerator();

		void cellularAutomata(const cellularAutomataParameters& parameters, gridDelegates<bool>::callback callback);

	private:

		void cellularAutomataIteration(array2D<bool>& resultGrid, const cellularAutomataParameters& parameters);

	private:

		randomGenerator* _randomGenerator;
	};
}