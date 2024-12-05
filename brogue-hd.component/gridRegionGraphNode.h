#pragma once

#include <simpleGraphNode.h>

#include "gridDefinitions.h"
#include "gridRegion.h"

namespace brogueHd::component
{
	using namespace simple::math;

	template<isGridLocator T>
	class gridRegionGraphNode : public simpleGraphNode
	{
	public:

		gridRegionGraphNode(gridRegion<T>* region);
		gridRegionGraphNode(const gridRegionGraphNode& copy);
		~gridRegionGraphNode();

		void operator=(const gridRegionGraphNode& copy);

		gridLocator getNode() const;
		gridRegion<T>* getRegion() const;

		size_t getHash() const override;

	private:

		gridRegion<T>* _region;
	};

	template<isGridLocator T>
	gridRegionGraphNode<T>::gridRegionGraphNode(gridRegion<T>* region)
	{
		_region = region;
	}

	template<isGridLocator T>
	gridRegionGraphNode<T>::~gridRegionGraphNode()
	{
		// Be aware of region memory. There is no deletion until the final layout is understood.		
	}

	template<isGridLocator T>
	gridRegionGraphNode<T>::gridRegionGraphNode(const gridRegionGraphNode& copy)
	{
		_region = copy.getRegion();
	}

	template<isGridLocator T>
	void gridRegionGraphNode<T>::operator=(const gridRegionGraphNode& copy)
	{
		_region = copy.getRegion();
	}

	template<isGridLocator T>
	gridLocator gridRegionGraphNode<T>::getNode() const
	{
		return _region->getLargestSubRectangle().center();
	}

	template<isGridLocator T>
	gridRegion<T>* gridRegionGraphNode<T>::getRegion() const
	{
		return _region;
	}

	template<isGridLocator T>
	size_t gridRegionGraphNode<T>::getHash() const
	{
		// Use region pointer for hash code
		return hashGenerator::generateHash(_region);
	}
}