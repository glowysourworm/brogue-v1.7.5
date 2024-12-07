#pragma once

#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridLayer.h"

#include <simpleGraph.h>

#include "gridRegionGraphNode.h"
#include "gridRegionGraphEdge.h"

namespace brogueHd::component
{
	class gridConnectionLayer
	{
	public:
		gridConnectionLayer(simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>* connectionGraph, gridLayer* cellLayer);
		~gridConnectionLayer();

	private:

		simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>* _connectionGraph;

		// Permanent Cell Layer (shared pointer)(this may change during a topology change)
		gridLayer* _cellLayer;
	};

	gridConnectionLayer::gridConnectionLayer(simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>* connectionGraph, gridLayer* cellLayer)
	{
		_connectionGraph = connectionGraph;
		_cellLayer = cellLayer;
	}
	gridConnectionLayer::~gridConnectionLayer()
	{
		
	}
}
