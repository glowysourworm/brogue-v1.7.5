#pragma once

#include <simpleGraphEdge.h>

#include "gridRegionGraphNode.h"

namespace brogueHd::component
{
	using namespace simple::math;

	class gridRegionGraphEdge : public simpleGraphEdge<gridRegionGraphNode>
	{
	public:

		gridRegionGraphEdge(gridRegionGraphNode* node1, gridRegionGraphNode* node2);
		gridRegionGraphEdge(const gridRegionGraphEdge& copy);
		~gridRegionGraphEdge();

		void operator=(const gridRegionGraphEdge& copy);

		virtual float weight() const override;
	};

	gridRegionGraphEdge::gridRegionGraphEdge(gridRegionGraphNode* node1, gridRegionGraphNode* node2)
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
	void gridRegionGraphEdge::operator=(const gridRegionGraphEdge& copy)
	{
		this->node1 = copy.node1;
		this->node2 = copy.node2;
	}

	float gridRegionGraphEdge::weight() const
	{
		// Distance calculation from the region largest-sub-rectangle centers
		return node1->getNode().distance(node2->getNode());
	}
}