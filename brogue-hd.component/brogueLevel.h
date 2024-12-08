#pragma once

#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "brogueContentGrid.h"
#include "brogueLayout.h"

#include <brogueCell.h>
#include <simpleGraph.h>
#include <simpleList.h>

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;
	using namespace brogueHd::model;

	class brogueLevel
	{
	public:
		brogueLevel(int depth, brogueLayout* layout, brogueContentGrid* content);
		~brogueLevel();

		int getDepth() const;

		void iterateWhereDefined(gridCallbackConst<brogueCell*> callback) const;
		void iterateRoomConnections(graphSimpleEdgeIterator<gridLocatorNode<gridLocator>, gridLocatorEdge<gridLocator>> callback) const;

		//simpleList<gridLocatorEdge> getCorridorConnections() const
		//{
		//	return _layout->getCorridorConnections();
		//}

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

	void brogueLevel::iterateWhereDefined(gridCallbackConst<brogueCell*> callback) const
	{
		_layout->iterateWhereDefined(callback);
	}

	void brogueLevel::iterateRoomConnections(graphSimpleEdgeIterator<gridLocatorNode<gridLocator>, gridLocatorEdge<gridLocator>> callback) const
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
