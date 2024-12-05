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

		layoutConnectionData(layoutDesignRect* region1,
							 layoutDesignRect* region2,
							 const gridLocator& connection1,
							 const gridLocator& connection2);
		~layoutConnectionData();

		gridLocator getConnectionPoint1() const;
		gridLocator getConnectionPoint2() const;

		gridRegionGraphNode* getRegion1Rect() const;
		gridRegionGraphNode* getRegion2Rect() const;

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

	layoutConnectionData::layoutConnectionData(layoutDesignRect* region1,
												 layoutDesignRect* region2,
												 const gridLocator& connection1,
												 const gridLocator& connection2)
	{
		_regionNode1 = new gridRegionGraphNode(region1->getRegion());
		_regionNode2 = new gridRegionGraphNode(region2->getRegion());
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

	gridRegionGraphNode* layoutConnectionData::getNode1() const
	{
		return _regionNode1;
	}
	gridRegionGraphNode* layoutConnectionData::getNode2() const
	{
		return _regionNode2;
	}

	void layoutConnectionData::complete(const simpleList<gridLocator>& pathData)
	{
		if (_complete)
			throw simpleException("Trying to re-complete a completed connection. Must re-start the layout process.");

		if (_failed)
			throw simpleException("Trying to utilize failed connection. Must re-start the layout process.");

		for (int index = 0; index < pathData.count(); index++)
		{
			_pathData->set(index, pathData.get(index));
		}
		_complete = true;
	}

	void layoutConnectionData::fail()
	{
		_failed = true;
	}

	bool layoutConnectionData::isComplete() const
	{
		return _complete;
	}
	bool isFailed() const
	{
		return _failed;
	}
}