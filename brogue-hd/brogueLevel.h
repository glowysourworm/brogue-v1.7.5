#pragma once

#include "brogueContentGrid.h"
#include "brogueLayout.h"
#include "gridLocator.h"

using namespace brogueHd::component;
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

		brogueLayout* layout;

		brogueContentGrid* contentGrid;

		// (old variable name)
		// pcell mapStorage[DCOLS][DROWS];
		// short** scentMap;

	private:

		bool _visited;
		short _depth;
		unsigned long _levelSeed;
		//unsigned long _awaySince;		// Parameter to "burn in" level. Simulate it for N turns so that swamp gas accumulates, swamps percolate, etc...

		gridLocator* _upStairsLoc;
		gridLocator* _downStairsLoc;
		gridLocator* _playerExitedVia;
	};

	brogueLevel::brogueLevel(short depth, unsigned long levelSeed)
	{

	}
	brogueLevel::~brogueLevel()
	{
	}
	short brogueLevel::getDepth() const
	{
		return 1;
	}
}

