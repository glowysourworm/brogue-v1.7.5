#pragma once

#include "gridLocator.h"
#include "gridLocatorNode.h"
#include "gridRegion.h"
#include "layoutDesignRect.h"

namespace brogueHd::component
{
	using namespace simple::math;

	class gridRegionGraphNode : public gridLocatorNode<layoutDesignRect*>
	{
	public:

		gridRegionGraphNode();
		gridRegionGraphNode(layoutDesignRect* region, const gridLocator& node);
		gridRegionGraphNode(const gridRegionGraphNode& copy);
		~gridRegionGraphNode() override;
	};

	gridRegionGraphNode::gridRegionGraphNode()
	{
	}
	gridRegionGraphNode::gridRegionGraphNode(layoutDesignRect* region, const gridLocator& nodeLocation)
		: gridLocatorNode<layoutDesignRect*>(region, nodeLocation)
	{
	}

	gridRegionGraphNode::~gridRegionGraphNode()
	{
		// Be aware of region memory. There is no deletion until the final layout is understood.		
	}

	gridRegionGraphNode::gridRegionGraphNode(const gridRegionGraphNode& copy)
		: gridLocatorNode<layoutDesignRect*>(copy.getData(), copy.getLocator())
	{
	}
}
