#pragma once

#include "gridRegion.h"
#include "gridLocator.h"
#include "layoutDesignRect.h"
#include "gridRegionGraphNode.h"

namespace brogueHd::component
{
	class layoutConnectionData
	{
	public:

		layoutConnectionData(const gridRegionGraphNode& node1,
							 const gridRegionGraphNode& node2,
							 const gridLocator& connection1,
							 const gridLocator& connection2);
		~layoutConnectionData();

		gridLocator getConnectionPoint1() const;
		gridLocator getConnectionPoint2() const;

		gridRegionGraphNode getNode1() const;
		gridRegionGraphNode getNode2() const;

		simpleArray<gridLocator> getPathData() const;

		void complete(const simpleList<gridLocator>& pathData);
		void fail();

		bool isComplete() const;
		bool isFailed() const;

	private:

		gridRegionGraphNode* _regionNode1;
		gridRegionGraphNode* _regionNode2;
		
		gridLocator _connectionPoint1;
		gridLocator _connectionPoint2;

		simpleList<gridLocator>* _pathData;

		bool _complete;
		bool _failed;
	};

	layoutConnectionData::layoutConnectionData(const gridRegionGraphNode& node1,
											   const gridRegionGraphNode& node2,
											   const gridLocator& connection1,
											   const gridLocator& connection2)
	{
		_regionNode1 = new gridRegionGraphNode(node1);
		_regionNode2 = new gridRegionGraphNode(node2);
		_connectionPoint1 = connection1;
		_connectionPoint2 = connection2;
		_pathData = new simpleList<gridLocator>();
		_complete = false;
		_failed = false;
	}
	layoutConnectionData::~layoutConnectionData()
	{
		delete _pathData;
		delete _regionNode1;		// Leave rest of region data for other components
		delete _regionNode2;		// Leave rest of region data for other components
	}

	gridLocator layoutConnectionData::getConnectionPoint1() const
	{
		return _connectionPoint1;
	}
	gridLocator layoutConnectionData::getConnectionPoint2() const
	{
		return _connectionPoint2;
	}

	gridRegionGraphNode layoutConnectionData::getNode1() const
	{
		return *_regionNode1;
	}
	gridRegionGraphNode layoutConnectionData::getNode2() const
	{
		return *_regionNode2;
	}

	void layoutConnectionData::complete(const simpleList<gridLocator>& pathData)
	{
		if (_complete)
			throw simpleException("Trying to re-complete a completed connection. Must re-start the layout process.");

		if (_failed)
			throw simpleException("Trying to utilize failed connection. Must re-start the layout process.");

		_pathData->addRange(pathData);
		_complete = true;
	}

	simpleArray<gridLocator> layoutConnectionData::getPathData() const
	{
		if (!_complete || _failed)
			throw simpleException("Trying to access path data for a layout connection that either failed, or is not yet complete.");

		return _pathData->toArray();
	}

	void layoutConnectionData::fail()
	{
		_failed = true;
	}

	bool layoutConnectionData::isComplete() const
	{
		return _complete;
	}
	bool layoutConnectionData::isFailed() const
	{
		return _failed;
	}
}