#pragma once

#include "cellularAutomataParameters.h"
#include "gridRect.h"
#include "grid.h"
#include "gridDefinitions.h"
#include "randomGenerator.h"

using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::generator
{
	class noiseGenerator
	{
	public:

		noiseGenerator(randomGenerator* randomGenerator);
		~noiseGenerator();

		void cellularAutomata(const cellularAutomataParameters& parameters, gridDelegates<bool>::callback callback);

	private:

		void cellularAutomataIteration(grid<bool>& resultGrid, const cellularAutomataParameters& parameters);

	private:

		randomGenerator* _randomGenerator;
	};
}