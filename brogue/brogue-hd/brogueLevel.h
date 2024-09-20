#pragma once

#include "brogueGrid.h"
#include "brogueContentGrid.h"
#include "gridCell.h"

using namespace std;

using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model
{
	class brogueLevel
	{
	public:

		brogueLevel(short depth, unsigned long levelSeed);
		~brogueLevel();

		short getDepth() const;

	public:

		brogueGrid* levelGrid;

		brogueContentGrid* contentGrid;

		// (old variable name)
		// pcell mapStorage[DCOLS][DROWS];
		// short** scentMap;

	private:

		bool _visited;
		short _depth;
		unsigned long _levelSeed;
		//unsigned long _awaySince;		// Parameter to "burn in" level. Simulate it for N turns so that swamp gas accumulates, swamps percolate, etc...

		gridCell* _upStairsLoc;
		gridCell* _downStairsLoc;
		gridCell* _playerExitedVia;
	};
}

