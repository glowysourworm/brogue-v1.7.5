#pragma once

#include <simpleGraph.h>

#include "grid.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRegionGraphNode.h"
#include "gridRegionGraphEdge.h"
#include "brogueCell.h"
#include "layoutConnectionBuilder.h"
#include "layoutDesignPartialConnection.h"
#include "layoutDesignRect.h"

namespace brogueHd::component
{
	/// <summary>
	/// Data store for collecting output from layout generator. This will show which pieces
	/// are "garbage collected" afterwards; and which are left for other components to manage.
	/// </summary>
	class layoutGeneratorData
	{
	public:

		layoutGeneratorData(const gridRect& layoutParentBoundary, const gridRect& layoutRelativeBoundary);
		~layoutGeneratorData();

		layoutConnectionBuilder* getConnectionBuilder() const;

		layoutDijkstraParameters<gridLocator>* getTrialDijkstraParameters() const;
		layoutDijkstraParameters<brogueCell>* getLayoutDijkstraParameters() const;

		grid<brogueCell*>* getLayoutGrid() const;
		grid<gridLocator>* getTrialGrid() const;
		simpleList<layoutDesignRect*>* getRoomTiles() const;
		simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>* getRoomGraph() const;
		simpleGraph<gridLocator, gridLocatorEdge>* getConnectionGraph() const;

	private:	// Dijkstra's map predicates (sometimes it's nice to have actual functions!)

		bool trialLayoutInclusionPredicate(int column, int row);
		int trialLayoutCostPredicate(int column, int row);
		gridLocator trialLayoutLocatorCallback(int column, int row);

		bool layoutInclusionPredicate(int column, int row);
		int layoutCostPredicate(int column, int row);
		gridLocator layoutLocatorCallback(int column, int row);

	private:

		layoutConnectionBuilder* _connectionBuilder;
		layoutDijkstraParameters<gridLocator>* _trialDijkstraParameters;
		layoutDijkstraParameters<brogueCell>* _layoutDijkstraParameters;

		grid<brogueCell*>* _layoutGrid;
		grid<gridLocator>* _trialGrid;
		simpleList<layoutDesignRect*>* _roomTiles;
		simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>* _roomGraph;
		simpleGraph<gridLocator, gridLocatorEdge>* _connectionGraph;
	};

	layoutGeneratorData::layoutGeneratorData(const gridRect& layoutParentBoundary, const gridRect& layoutRelativeBoundary)
	{
		_connectionBuilder = new layoutConnectionBuilder();

		_layoutGrid = new grid<brogueCell*>(layoutParentBoundary, layoutRelativeBoundary);
		_trialGrid = new grid<gridLocator>(layoutParentBoundary, layoutRelativeBoundary);
		_roomTiles = new simpleList<layoutDesignRect*>();
		_roomGraph = new simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>();
		_connectionGraph = new simpleGraph<gridLocator, gridLocatorEdge>();

		_trialDijkstraParameters = new layoutDijkstraParameters<gridLocator>(layoutParentBoundary,
																			 layoutRelativeBoundary,
																			 true,
																			 &trialLayoutInclusionPredicate,
																			 &trialLayoutCostPredicate,
																			 &trialLayoutLocatorCallback);

		_layoutDijkstraParameters = new layoutDijkstraParameters<gridLocator>(layoutParentBoundary,
																			  layoutRelativeBoundary,
																			  true,
																			  &layoutInclusionPredicate,
																			  &layoutCostPredicate,
																			  &layoutLocatorCallback);
	};
	layoutGeneratorData::~layoutGeneratorData()
	{
		if (_connectionBuilder != nullptr)
			delete _connectionBuilder;

		// Layout Grid is passed on (no deletion here)

		if (_trialGrid != nullptr)
			delete _trialGrid;

		if (_roomTiles != nullptr)
			delete _roomTiles;

		if (_roomGraph != nullptr)
			delete _roomGraph;

		if (_connectionGraph != nullptr)
			delete _connectionGraph;

		if (_roomNearestNeighbors != nullptr)
			delete _roomNearestNeighbors;
	}
	layoutDijkstraParameters<gridLocator>* layoutGeneratorData::getTrialDijkstraParameters() const
	{
		return _trialDijkstraParameters;
	}
	layoutDijkstraParameters<brogueCell>* layoutGeneratorData::getLayoutDijkstraParameters() const
	{
		return _layoutDijkstraParameters;
	}
	bool layoutGeneratorData::trialLayoutInclusionPredicate(int column, int row)
	{
		return true;
	}
	int layoutGeneratorData::trialLayoutCostPredicate(int column, int row)
	{
		for (int index = 0; index < _roomTiles->count(); index++)
		{
			layoutDesignRect* rect = _roomTiles->get(index);

			// TODO: (padding)
			if (rect->getRegion()->isDefined(column, row))
				return 10;
		}
		return 1;
	}
	gridLocator layoutGeneratorData::trialLayoutLocatorCallback(int column, int row)
	{
		return _trialGrid->get(column, row);
	}

	bool layoutGeneratorData::layoutInclusionPredicate(int column, int row)
	{
		return true;
	}
	int layoutGeneratorData::layoutCostPredicate(int column, int row)
	{
		for (int index = 0; index < _roomTiles->count(); index++)
		{
			layoutDesignRect* rect = _roomTiles->get(index);

			// TODO: (padding)
			if (rect->getRegion()->isDefined(column, row))
				return 10;
		}
		return 1;
	}
	gridLocator layoutGeneratorData::layoutLocatorCallback(int column, int row)
	{
		return _layoutGrid->get(column, row);
	}

	grid<brogueCell*>* layoutGeneratorData::getLayoutGrid() const
	{
		return _layoutGrid;
	}
	grid<gridLocator>* layoutGeneratorData::getTrialGrid() const
	{
		return _trialGrid;
	}
	simpleList<layoutDesignRect*>* layoutGeneratorData::getRoomTiles() const
	{
		return _roomTiles;
	}
	layoutConnectionBuilder* layoutGeneratorData::getConnectionBuilder() const
	{
		return _connectionBuilder;
	}
	simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>* layoutGeneratorData::getRoomGraph() const
	{
		return _roomGraph;
	}
	simpleGraph<gridLocator, gridLocatorEdge>* layoutGeneratorData::getConnectionGraph() const
	{
		return _connectionGraph;
	}
}