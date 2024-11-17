#pragma once

#include "brogueCell.h"
#include "brogueContentGrid.h"
#include "brogueLayout.h"
#include "gridDefinitions.h"
#include "gridLocator.h"

using namespace brogueHd::component;

namespace brogueHd::backend::model
{
	class brogueLevel
	{
	public:

		brogueLevel(short depth, brogueLayout* layout, brogueContentGrid* content);
		~brogueLevel();

		short getDepth() const;

		void iterate(gridCallback<brogueCell*> callback) const;

	private:

		brogueLayout* _layout;
		brogueContentGrid* _contentGrid;

		// (old variable name)
		// pcell mapStorage[DCOLS][DROWS];
		// short** scentMap;

		bool _visited;
		short _depth;
		unsigned long _levelSeed;
		//unsigned long _awaySince;		// Parameter to "burn in" level. Simulate it for N turns so that swamp gas accumulates, swamps percolate, etc...

		gridLocator* _upStairsLoc;
		gridLocator* _downStairsLoc;
		gridLocator* _playerExitedVia;
	};

	brogueLevel::brogueLevel(short depth, brogueLayout* layout, brogueContentGrid* content)
	{
		_depth = depth;
		_layout = layout;
		_contentGrid = content;
	}
	brogueLevel::~brogueLevel()
	{
	}
	short brogueLevel::getDepth() const
	{
		return _depth;
	}
	void brogueLevel::iterate(gridCallback<brogueCell*> callback) const
	{
		_layout->iterate(callback);
	}
}

