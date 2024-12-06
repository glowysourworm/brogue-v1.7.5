#pragma once

#include <simpleGraphEdge.h>

#include "gridDefinitions.h"
#include "gridRegionGraphNode.h"

namespace brogueHd::component
{
	using namespace simple::math;

	class gridRegionGraphEdge : public simpleGraphEdge<gridRegionGraphNode>
	{
	public:

		gridRegionGraphEdge();
		gridRegionGraphEdge(const gridRegionGraphNode& anode1, const gridRegionGraphNode& anode2);
		gridRegionGraphEdge(const gridRegionGraphEdge& copy);
		~gridRegionGraphEdge();

		void operator=(const gridRegionGraphEdge& copy);
		bool operator==(const gridRegionGraphEdge& other) const;
		bool operator!=(const gridRegionGraphEdge& other) const;

		virtual float weight() const override;

	private:

		bool compareImpl(const gridRegionGraphEdge& other) const;
	};

	gridRegionGraphEdge::gridRegionGraphEdge()
	{}
	gridRegionGraphEdge::gridRegionGraphEdge(const gridRegionGraphNode& anode1, const gridRegionGraphNode& anode2)
		: simpleGraphEdge<gridRegionGraphNode>(anode1, anode2)
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

	bool gridRegionGraphEdge::operator==(const gridRegionGraphEdge& other) const
	{
		return compareImpl(other);
	}

	bool gridRegionGraphEdge::operator!=(const gridRegionGraphEdge& other) const
	{
		return !compareImpl(other);
	}

	float gridRegionGraphEdge::weight() const
	{
		// Distance calculation from the region largest-sub-rectangle centers
		return (float)this->node1.getNode().distance(this->node2.getNode());
	}

	bool gridRegionGraphEdge::compareImpl(const gridRegionGraphEdge& other) const
	{
		return (this->node1 == other.node1 && this->node2 == other.node2) ||
			   (this->node1 == other.node2 && this->node2 == other.node1);
	}
}