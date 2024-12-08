#pragma once

#include <simple.h>
#include <simpleGraphEdge.h>

#include "gridDefinitions.h"
#include "gridRegionGraphNode.h"

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;

	class gridRegionGraphEdge : public simpleGraphEdge<gridRegionGraphNode>
	{
	public:

		gridRegionGraphEdge();
		gridRegionGraphEdge(const gridRegionGraphNode& node1, const gridRegionGraphNode& node2);
		gridRegionGraphEdge(const gridRegionGraphEdge& copy);
		~gridRegionGraphEdge() override;

		virtual float weight() const override;
	};

	gridRegionGraphEdge::gridRegionGraphEdge()
	{}
	gridRegionGraphEdge::gridRegionGraphEdge(const gridRegionGraphNode& node1, const gridRegionGraphNode& node2)
		: simpleGraphEdge<gridRegionGraphNode>(node1, node2)
	{}

	gridRegionGraphEdge::~gridRegionGraphEdge()
	{
		// Be aware of grid region memory. There is no deletion until the layout is understood.		
	}

	gridRegionGraphEdge::gridRegionGraphEdge(const gridRegionGraphEdge& copy)
	{
		this->node1 = copy.node1;
		this->node2 = copy.node2;
	}

	float gridRegionGraphEdge::weight() const
	{
		// Distance calculation from the region largest-sub-rectangle centers
		return (float)this->node1.getLocator().distance(this->node2.getLocator());
	}
}