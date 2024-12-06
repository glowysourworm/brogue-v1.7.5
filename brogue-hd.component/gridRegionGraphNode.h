#pragma once

#include <simpleGraphNode.h>

#include "gridDefinitions.h"
#include "gridRegion.h"

namespace brogueHd::component
{
	using namespace simple::math;

	class gridRegionGraphNode : public simpleGraphNode
	{
	public:

		gridRegionGraphNode(gridRegion<gridLocator>* region, const gridLocator& node);
		gridRegionGraphNode(const gridRegionGraphNode& copy);
		~gridRegionGraphNode();

		void operator=(const gridRegionGraphNode& copy);
		bool operator==(const gridRegionGraphNode& other) const;
		bool operator!=(const gridRegionGraphNode& other) const;

		gridLocator getNode() const;
		gridRegion<gridLocator>* getRegion() const;

		size_t getHash() const override;

	private:

		gridRegion<gridLocator>* _region;

		gridLocator _nodeLocation;
	};

	gridRegionGraphNode::gridRegionGraphNode(gridRegion<gridLocator>* region, const gridLocator& nodeLocation)
	{
		_region = region;
		_nodeLocation = nodeLocation;
	}

	gridRegionGraphNode::~gridRegionGraphNode()
	{
		// Be aware of region memory. There is no deletion until the final layout is understood.		
	}

	gridRegionGraphNode::gridRegionGraphNode(const gridRegionGraphNode& copy)
	{
		_region = copy.getRegion();
		_nodeLocation = copy.getNode();
	}

	void gridRegionGraphNode::operator=(const gridRegionGraphNode& copy)
	{
		_region = copy.getRegion();
		_nodeLocation = copy.getNode();
	}

	bool gridRegionGraphNode::operator==(const gridRegionGraphNode& other) const
	{
		return _region == other.getRegion() && _nodeLocation == other.getNode();
	}

	bool gridRegionGraphNode::operator!=(const gridRegionGraphNode& other) const
	{
		return _region != other.getRegion() || _nodeLocation != other.getNode();	
	}

	gridLocator gridRegionGraphNode::getNode() const
	{
		return _nodeLocation;
	}

	gridRegion<gridLocator>* gridRegionGraphNode::getRegion() const
	{
		return _region;
	}

	size_t gridRegionGraphNode::getHash() const
	{
		// Use region pointer for hash code
		return hashGenerator::generateHash(_region, _nodeLocation);
	}
}