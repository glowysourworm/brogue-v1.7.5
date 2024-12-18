#pragma once

#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridLayer.h"

#include <simpleGraph.h>
#include <simpleGraphDefinitions.h>

#include "gridConnectionEdge.h"
#include "gridConnectionNode.h"
#include "gridRegionGraphNode.h"
#include "gridRegionGraphEdge.h"

namespace brogueHd::component
{
	using namespace simple::math;

	class gridConnectionLayer
	{
	public:
		gridConnectionLayer(simpleGraph<gridConnectionNode, gridConnectionEdge>* connectionGraph, gridLayer* cellLayer);
		~gridConnectionLayer();

		void iterateConnections(graphSimpleEdgeIterator<gridConnectionNode, gridConnectionEdge> callback) const;

	private:

		simpleGraph<gridConnectionNode, gridConnectionEdge>* _connectionGraph;

		// Permanent Cell Layer (shared pointer)(this may change during a topology change)
		gridLayer* _cellLayer;
	};

	gridConnectionLayer::gridConnectionLayer(simpleGraph<gridConnectionNode, gridConnectionEdge>* connectionGraph, gridLayer* cellLayer)
	{
		_connectionGraph = connectionGraph;
		_cellLayer = cellLayer;
	}
	gridConnectionLayer::~gridConnectionLayer()
	{
		// NOTE: This is an artifact of the build process. So, this component is the one responsible for deleting
		//		 it.
		delete _connectionGraph;
		delete _cellLayer;
	}

	void gridConnectionLayer::iterateConnections(graphSimpleEdgeIterator<gridConnectionNode, gridConnectionEdge> callback) const
	{
		_connectionGraph->iterateEdges(callback);
	}
}
