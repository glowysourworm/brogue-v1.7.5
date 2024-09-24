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

	private:

		randomGenerator* _randomGenerator;
	};
}