#pragma once

#include <simpleGraphEdge.h>

#include "gridConnectionNode.h"
#include "gridDefinitions.h"
#include "gridRegionGraphNode.h"

namespace brogueHd::component
{
	using namespace simple::math;

	class gridConnectionEdge : public simpleGraphEdge<gridConnectionNode>
	{
	public:

		gridConnectionEdge();
		gridConnectionEdge(const gridConnectionNode& anode1, const gridConnectionNode& anode2);
		gridConnectionEdge(const gridConnectionNode& anode1, const gridConnectionNode& anode2, simpleArray<gridLocator>* pathData);
		gridConnectionEdge(const gridConnectionEdge& copy);
		~gridConnectionEdge() override;

		void operator=(const gridConnectionEdge& copy);
		bool operator==(const gridConnectionEdge& other) const;
		bool operator!=(const gridConnectionEdge& other) const;

		simpleArray<gridLocator>* getPathData() const;
		void setPathData(const simpleArray<gridLocator>& pathData);

		virtual float weight() const override;

	private:

		bool compareImpl(const gridConnectionEdge& other) const;

	private:

		simpleArray<gridLocator>* _pathData;

	};

	gridConnectionEdge::gridConnectionEdge()
	{
		_pathData = new simpleArray<gridLocator>();
	}
	gridConnectionEdge::gridConnectionEdge(const gridConnectionNode& anode1, const gridConnectionNode& anode2)
		: simpleGraphEdge<gridConnectionNode>(anode1, anode2)
	{
		_pathData = new simpleArray<gridLocator>();
	}
	gridConnectionEdge::gridConnectionEdge(const gridConnectionNode& anode1, const gridConnectionNode& anode2, simpleArray<gridLocator>* pathData)
		: simpleGraphEdge<gridConnectionNode>(anode1, anode2)
	{
		_pathData = new simpleArray<gridLocator>(*pathData);
	}

	gridConnectionEdge::~gridConnectionEdge()
	{
		// Be aware of grid region memory.
		delete _pathData;
	}

	gridConnectionEdge::gridConnectionEdge(const gridConnectionEdge& copy)
	{
		this->node1 = copy.node1;
		this->node2 = copy.node2;
		_pathData = new simpleArray<gridLocator>(*copy.getPathData());
	}

	void gridConnectionEdge::operator=(const gridConnectionEdge& copy)
	{
		delete _pathData;

		this->node1 = copy.node1;
		this->node2 = copy.node2;

		// Go ahead and let this hand off happen, for now.
		_pathData = new simpleArray<gridLocator>(*copy.getPathData());
	}

	bool gridConnectionEdge::operator==(const gridConnectionEdge& other) const
	{
		return compareImpl(other);
	}

	bool gridConnectionEdge::operator!=(const gridConnectionEdge& other) const
	{
		return !compareImpl(other);
	}

	float gridConnectionEdge::weight() const
	{
		// Distance calculation from the region largest-sub-rectangle centers
		return (float)this->node1.getLocator().distance(this->node2.getLocator());
	}

	simpleArray<gridLocator>* gridConnectionEdge::getPathData() const
	{
		return _pathData;
	}
	void gridConnectionEdge::setPathData(const simpleArray<gridLocator>& pathData)
	{
		delete _pathData;

		_pathData = new simpleArray<gridLocator>(pathData);
	}
	bool gridConnectionEdge::compareImpl(const gridConnectionEdge& other) const
	{
		simpleArray<gridLocator> reversePath = _pathData->reverse();

		bool valueEquivalent = (*_pathData) == *other.getPathData() ||
							    reversePath == *other.getPathData();

		// Let the pointer comparison for path data work, for now.
		return ((this->node1 == other.node1 && this->node2 == other.node2) ||
				(this->node1 == other.node2 && this->node2 == other.node1)) && valueEquivalent;
	}
}