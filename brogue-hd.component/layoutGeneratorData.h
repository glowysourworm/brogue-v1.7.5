#pragma once

#include <simpleGraph.h>

#include "brogueCell.h"
#include "dijkstra.h"
#include "grid.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRegionGraphEdge.h"
#include "gridRegionGraphNode.h"
#include "gridConnectionNode.h"
#include "gridConnectionEdge.h"
#include "layoutConnectionBuilder.h"
#include "layoutDesignRect.h"
#include "layoutPartialConnectionData.h"

namespace brogueHd::component
{
	/// <summary>
	/// Data store for collecting output from layout generator. This will show which pieces
	/// are "garbage collected" afterwards; and which are left for other components to manage.
	/// </summary>
	class layoutGeneratorData
	{
	public:

		layoutGeneratorData(brogueLevelProfile* profile, const gridRect& layoutParentBoundary, const gridRect& layoutRelativeBoundary);
		~layoutGeneratorData();

		brogueLevelProfile* getProfile() const;

		gridRect getParentBoundary() const;
		gridRect getBoundary() const;

		layoutConnectionBuilder* getConnectionBuilder() const;

		layoutDijkstraParameters<gridLocator>* getTrialDijkstraParameters() const;

		grid<gridLocator>* getTrialGrid() const;
		simpleList<gridRegionGraphNode>* getRoomNodes() const;
		simpleList<gridRegionGraphEdge>* getRoomNearestNeighbors() const;
		simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>* getRoomGraph() const;
		simpleGraph<gridConnectionNode, gridConnectionEdge>* getConnectionGraph() const;

	private:	// Dijkstra's map predicates (sometimes it's nice to have actual functions!)

		bool trialLayoutInclusionPredicate(int column, int row);
		int trialLayoutCostPredicate(int column, int row);
		gridLocator trialLayoutLocatorCallback(int column, int row);

	private:

		brogueLevelProfile* _profile;

		layoutConnectionBuilder* _connectionBuilder;
		layoutDijkstraParameters<gridLocator>* _trialDijkstraParameters;

		grid<gridLocator>* _trialGrid;
		simpleList<gridRegionGraphNode>* _roomNodes;
		simpleList<gridRegionGraphEdge>* _roomNearestNeighbors;
		simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>* _roomGraph;
		simpleGraph<gridConnectionNode, gridConnectionEdge>* _connectionGraph;
	};

	layoutGeneratorData::layoutGeneratorData(brogueLevelProfile* profile, const gridRect& layoutParentBoundary, const gridRect& layoutRelativeBoundary)
	{
		_profile = profile;
		_connectionBuilder = new layoutConnectionBuilder();

		_trialGrid = new grid<gridLocator>(layoutParentBoundary, layoutRelativeBoundary);
		_roomNodes = new simpleList<gridRegionGraphNode>();
		_roomNearestNeighbors = new simpleList<gridRegionGraphEdge>();

		// Graph instances are created by the triangulator algorithms
		_roomGraph = new simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>();
		_connectionGraph = new simpleGraph<gridConnectionNode, gridConnectionEdge>();

		_trialDijkstraParameters = new layoutDijkstraParameters<gridLocator>(layoutParentBoundary,
																			 layoutRelativeBoundary,
																			 true,
																			 std::bind(&layoutGeneratorData::trialLayoutInclusionPredicate, this, std::placeholders::_1, std::placeholders::_2),
																			 std::bind(&layoutGeneratorData::trialLayoutCostPredicate, this, std::placeholders::_1, std::placeholders::_2),
																			 std::bind(&layoutGeneratorData::trialLayoutLocatorCallback, this, std::placeholders::_1, std::placeholders::_2));
	};
	layoutGeneratorData::~layoutGeneratorData()
	{
		delete _connectionBuilder;

		// Layout Grid is passed on (no deletion here)
		// Connection Graph is pass on (no deletion here)

		delete _trialGrid;
		delete _roomGraph;
		delete _roomNodes;
		delete _roomNearestNeighbors;
		delete _trialDijkstraParameters;
	}
	brogueLevelProfile* layoutGeneratorData::getProfile() const
	{
		return _profile;
	}
	gridRect layoutGeneratorData::getParentBoundary() const
	{
		return _trialGrid->getParentBoundary();
	}
	gridRect layoutGeneratorData::getBoundary() const
	{
		return _trialGrid->getRelativeBoundary();
	}
	layoutDijkstraParameters<gridLocator>* layoutGeneratorData::getTrialDijkstraParameters() const
	{
		return _trialDijkstraParameters;
	}
	bool layoutGeneratorData::trialLayoutInclusionPredicate(int column, int row)
	{
		return true;
	}
	int layoutGeneratorData::trialLayoutCostPredicate(int column, int row)
	{
		// TODO: Padding (Define a padding around the region cells in the planning stage)
		//
		bool collision = false;

		_roomGraph->iterateNodes([&collision, &column, &row] (const gridRegionGraphNode& node)
		{
			if (node.getData()->getRegion()->isDefined(column, row))
			{
				collision = true;
				return iterationCallback::breakAndReturn;
			}

			return iterationCallback::iterate;
		});

		// Takes extra steps to forcibly enter another region.
		return collision ? 5 : 1;
	}
	gridLocator layoutGeneratorData::trialLayoutLocatorCallback(int column, int row)
	{
		// Trial grid is not yet filled in
		return gridLocator(column, row);
	}

	grid<gridLocator>* layoutGeneratorData::getTrialGrid() const
	{
		return _trialGrid;
	}
	simpleList<gridRegionGraphNode>* layoutGeneratorData::getRoomNodes() const
	{
		return _roomNodes;
	}
	simpleList<gridRegionGraphEdge>* layoutGeneratorData::getRoomNearestNeighbors() const
	{
		return _roomNearestNeighbors;
	}
	layoutConnectionBuilder* layoutGeneratorData::getConnectionBuilder() const
	{
		return _connectionBuilder;
	}
	simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>* layoutGeneratorData::getRoomGraph() const
	{
		return _roomGraph;
	}
	simpleGraph<gridConnectionNode, gridConnectionEdge>* layoutGeneratorData::getConnectionGraph() const
	{	
		return _connectionGraph;
	}
}