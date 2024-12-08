#pragma once

#include <simpleGraphNode.h>

#include "gridLocator.h"
#include "gridRegion.h"
#include "gridRegionGraphNode.h"

namespace brogueHd::component
{
	using namespace simple::math;

	class gridConnectionNode : public gridRegionGraphNode
	{
	public:

		gridConnectionNode();
		gridConnectionNode(gridRegion* region, const gridLocator& connectionPoint);
		gridConnectionNode(const gridConnectionNode& copy);
		~gridConnectionNode() override;
	};

	gridConnectionNode::gridConnectionNode()
	{
	}
	gridConnectionNode::gridConnectionNode(gridRegion* region, const gridLocator& connectionPoint)
		: gridRegionGraphNode(region, connectionPoint)
	{
	}
	gridConnectionNode::gridConnectionNode(const gridConnectionNode& copy)
		: gridRegionGraphNode(copy.getData(), copy.getLocator())
	{
	}
	gridConnectionNode::~gridConnectionNode() 
	{
		// Region gets handled elsewhere
	}
}
