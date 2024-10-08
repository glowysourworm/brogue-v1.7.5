#pragma once

#include "cellularAutomataParameters.h"
#include "gridRect.h"
#include "grid.h"
#include "gridDefinitions.h"
#include "randomGenerator.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::generator
{
	class noiseGenerator
	{
	public:

		noiseGenerator(randomGenerator* randomGenerator);
		~noiseGenerator();

		void cellularAutomata(const cellularAutomataParameters& parameters, gridCallback<bool> callback);

	private:

		void cellularAutomataIteration(grid<bool>& resultGrid, const cellularAutomataParameters& parameters);

	private:

		randomGenerator* _randomGenerator;
	};
}