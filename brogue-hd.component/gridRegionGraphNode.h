#pragma once

#include "gridLocator.h"
#include "gridLocatorNode.h"
#include "gridRegion.h"

namespace brogueHd::component
{
	using namespace simple::math;

	class gridRegionGraphNode : public gridLocatorNode<gridRegion*>
	{
	public:

		gridRegionGraphNode();
		gridRegionGraphNode(gridRegion* region, const gridLocator& node);
		gridRegionGraphNode(const gridRegionGraphNode& copy);
		~gridRegionGraphNode() override;
	};

	gridRegionGraphNode::gridRegionGraphNode()
	{
	}
	gridRegionGraphNode::gridRegionGraphNode(gridRegion* region, const gridLocator& nodeLocation)
		: gridLocatorNode<gridRegion*>(region, nodeLocation)
	{
	}

	gridRegionGraphNode::~gridRegionGraphNode()
	{
		// Be aware of region memory. There is no deletion until the final layout is understood.		
	}

	gridRegionGraphNode::gridRegionGraphNode(const gridRegionGraphNode& copy)
		: gridLocatorNode<gridRegion*>(copy.getData(), copy.getLocator())
	{
	}
}