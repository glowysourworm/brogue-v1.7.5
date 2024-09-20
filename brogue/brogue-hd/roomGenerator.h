#pragma once

#include "brogueCell.h"
#include "grid.h"
#include "gridDefinitions.h"

using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::generator
{
	class roomGenerator
	{
	public:

		roomGenerator();
		~roomGenerator();

		void designRandomRoom(grid<brogueCell*>* grid, const gridRect& roomBoundary, const dungeonProfile& roomProfile);
		void designCavern(grid<brogueCell*>* grid, const gridRect& roomBoundary, const dungeonProfile& roomProfile);
		void designEntranceRoom(grid<brogueCell*>* grid, const gridRect& roomBoundary, const dungeonProfile& roomProfile);
		void designCrossRoom(grid<brogueCell*>* grid, const gridRect& roomBoundary, const dungeonProfile& roomProfile);
		void designSymmetricalCrossRoom(grid<brogueCell*>* grid, const gridRect& roomBoundary, const dungeonProfile& roomProfile);
		void designSmallRoom(grid<brogueCell*>* grid, const gridRect& roomBoundary, const dungeonProfile& roomProfile);
		void designCircularRoom(grid<brogueCell*>* grid, const gridRect& roomBoundary, const dungeonProfile& roomProfile);
		void designChunkyRoom(grid<brogueCell*>* grid, const gridRect& roomBoundary, const dungeonProfile& roomProfile);

	};
}

