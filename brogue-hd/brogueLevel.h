#pragma once

#include "brogueCell.h"
#include "brogueContentGrid.h"
#include "brogueLayout.h"
#include "graph.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"

using namespace brogueHd::component;

namespace brogueHd::backend::model
{
	class brogueLevel
	{
	public:

		brogueLevel(int depth, brogueLayout* layout, brogueContentGrid* content);
		~brogueLevel();

		int getDepth() const;

		void iterateWhereDefined(gridCallback<brogueCell*> callback) const;
		void iterateRoomGraph(graphIterator<gridLocator, gridLocatorEdge> callback) const;
		void iterateRoomConnections(graphSimpleEdgeIterator<gridLocator, gridLocatorEdge> callback) const;
		void clearUpdate();
		bool needsUpdate();

	private:

		brogueLayout* _layout;
		brogueContentGrid* _contentGrid;

		// Primary invalid flag for the level
		bool _invalid;

		// (old variable name)
		// pcell mapStorage[DCOLS][DROWS];
		// int** scentMap;

		bool _visited;
		int _depth;
		unsigned long _levelSeed;
		//unsigned long _awaySince;		// Parameter to "burn in" level. Simulate it for N turns so that swamp gas accumulates, swamps percolate, etc...

		gridLocator* _upStairsLoc;
		gridLocator* _downStairsLoc;
		gridLocator* _playerExitedVia;
	};

	brogueLevel::brogueLevel(int depth, brogueLayout* layout, brogueContentGrid* content)
	{
		_depth = depth;
		_layout = layout;
		_contentGrid = content;

		_invalid = true;
	}
	brogueLevel::~brogueLevel()
	{
	}
	int brogueLevel::getDepth() const
	{
		return _depth;
	}
	void brogueLevel::iterateWhereDefined(gridCallback<brogueCell*> callback) const
	{
		_layout->iterateWhereDefined(callback);
	}
	void brogueLevel::iterateRoomGraph(graphIterator<gridLocator, gridLocatorEdge> callback) const
	{
		_layout->iterateRoomGraph(callback);
	}
	void brogueLevel::iterateRoomConnections(graphSimpleEdgeIterator<gridLocator, gridLocatorEdge> callback) const
	{
		_layout->iterateRoomConnections(callback);
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

