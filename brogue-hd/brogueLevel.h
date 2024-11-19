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

		void iterateWhereDefined(gridCallback<brogueCell*> callback) const;
		void clearUpdate();
		bool needsUpdate();

	private:

		brogueLayout* _layout;
		brogueContentGrid* _contentGrid;

		// Primary invalid flag for the level
		bool _invalid;

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

		_invalid = true;
	}
	brogueLevel::~brogueLevel()
	{
	}
	short brogueLevel::getDepth() const
	{
		return _depth;
	}
	void brogueLevel::iterateWhereDefined(gridCallback<brogueCell*> callback) const
	{
		_layout->iterateWhereDefined(callback);
	}
	bool brogueLevel::needsUpdate()
	{
		return _invalid;
	}
	void brogueLevel::clearUpdate()
	{
		_invalid = false;
	}
}

