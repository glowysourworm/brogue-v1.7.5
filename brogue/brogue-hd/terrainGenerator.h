#pragma once

#include "dungeonDeclaration.h"
#include "cellularAutomataParameters.h"
#include "gridRect.h"
#include "grid.h"
#include "gridCell.h"
#include "brogueCell.h"
#include "randomGenerator.h"

using namespace brogueHd::backend::model::layout;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::generator
{
	class terrainGenerator
	{
	public:

		terrainGenerator(randomGenerator* randomGenerator);
		~terrainGenerator();

		array2D<bool>& createCellularAutomata(grid<brogueCell*>* grid, const cellularAutomataParameters& parameters);

	private:

		void cellularAutomataIteration(array2D<bool>& resultGrid, const cellularAutomataParameters& parameters);

	private:

		randomGenerator* _randomGenerator;
	};
}