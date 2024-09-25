#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "noiseGenerator.h"
#include "gridRegionLocator.h"
#include "cellularAutomataParameters.h"

using namespace brogueHd::backend::model::construction;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::generator
{
	class roomGenerator
	{
	public:

		roomGenerator(noiseGenerator* noiseGenerator, randomGenerator* randomGenerator);
		~roomGenerator();

		void designCavern(gridRegionConstructor<gridLocator>& regionConstructor, 
							const gridRect& parentBoundary, 
							const gridRect& relativeBoundary, 
							const gridRect& minSize, 
							const gridRect& maxSize);

		void designEntranceRoom(gridRegionConstructor<gridLocator>& regionConstructor);

		void designCrossRoom(gridRegionConstructor<gridLocator>& regionConstructor);

		void designSymmetricalCrossRoom(gridRegionConstructor<gridLocator>& regionConstructor);

		void designSmallRoom(gridRegionConstructor<gridLocator>& regionConstructor);

		void designCircularRoom(gridRegionConstructor<gridLocator>& regionConstructor, const gridRect& parentBoundary, const gridRect& relativeBoundary);

		void designChunkyRoom(gridRegionConstructor<gridLocator>& regionConstructor, const gridRect& parentBoundary, const gridRect& relativeBoundary);

	private:

		noiseGenerator* _noiseGenerator;
		randomGenerator* _randomGenerator;

		cellularAutomataParameters* _cavernParameters;
	};
}

