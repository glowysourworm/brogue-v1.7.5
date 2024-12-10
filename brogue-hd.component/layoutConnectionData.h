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
							 const gridRegionGraphNode& node2);
		~layoutConnectionData();

		gridRegionGraphNode getNode1() const;
		gridRegionGraphNode getNode2() const;

		simpleArray<gridLocator>* getPathData() const;

		void complete(const simpleList<gridLocator>& pathData);
		void fail();

		bool isComplete() const;
		bool isFailed() const;

	private:

		gridRegionGraphNode* _regionNode1;
		gridRegionGraphNode* _regionNode2;
		
		simpleArray<gridLocator>* _pathData;

		bool _complete;
		bool _failed;
	};

	layoutConnectionData::layoutConnectionData(const gridRegionGraphNode& node1,
											   const gridRegionGraphNode& node2)
	{
		_regionNode1 = new gridRegionGraphNode(node1);
		_regionNode2 = new gridRegionGraphNode(node2);
		_pathData = new simpleArray<gridLocator>();
		_complete = false;
		_failed = false;
	}
	layoutConnectionData::~layoutConnectionData()
	{
		delete _pathData;
		delete _regionNode1;		// Leave rest of region data for other components
		delete _regionNode2;		// Leave rest of region data for other components
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

		delete _pathData;

		_pathData = new simpleArray<gridLocator>(pathData.count());

		for (int index = 0; index < pathData.count() ;index++)
		{
			_pathData->set(index, pathData.get(index));
		}

		_complete = true;
	}

	simpleArray<gridLocator>* layoutConnectionData::getPathData() const
	{
		if (!_complete || _failed)
			throw simpleException("Trying to access path data for a layout connection that either failed, or is not yet complete.");

		return _pathData;
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