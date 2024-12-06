#pragma once

#include <simpleList.h>

#include "gridLocator.h"

#include "layoutDesignRect.h"
#include "layoutConnectionData.h"

namespace brogueHd::component
{
	using namespace simple;

	class layoutPartialConnectionData : public layoutConnectionData
	{
	public:
		layoutPartialConnectionData(const gridRegionGraphNode& source,
									 const gridRegionGraphNode& destination,
									 const gridRegionGraphNode& interruptingRegion,
									 const gridLocator& locationSource,
									 const gridLocator& locationDest,
									 const gridLocator& locationInterrupted);

		layoutPartialConnectionData(const layoutConnectionData* connection,
									const gridRegionGraphNode& interruptingRegion,
									const gridLocator& interruptingLocation);
		~layoutPartialConnectionData();

		/// <summary>
		/// Sets the data for the path of the source -> the location interrupted. This is saved for path
		/// reconciliation with the graph.
		/// </summary>
		void completePartial(const simpleArray<gridLocator>& pathData);

		gridRegionGraphNode getInterruptingRegion() const;
		gridLocator getInterruptingLocation() const;

		void setReconciled(bool value);
		bool getReconciled() const;

	private:

		gridRegionGraphNode* _interruptingRegion;
		gridLocator _locationInterrupted;

		bool _reconciled;
	};

	layoutPartialConnectionData::layoutPartialConnectionData(const gridRegionGraphNode& source,
															 const gridRegionGraphNode& destination,
															 const gridRegionGraphNode& interruptingRegion,
															 const gridLocator& locationSource,
															 const gridLocator& locationDest,
															 const gridLocator& locationInterrupted) : 
	layoutConnectionData(source, destination, locationSource, locationDest)
	{
		_interruptingRegion = new gridRegionGraphNode(interruptingRegion);
		_locationInterrupted = locationInterrupted;
		_reconciled = false;
	}
	layoutPartialConnectionData::layoutPartialConnectionData(const layoutConnectionData* connection,
															 const gridRegionGraphNode& interruptingRegion,
															 const gridLocator& interruptingLocation) :
	layoutPartialConnectionData(connection->getNode1(),
								connection->getNode2(),
								interruptingRegion,
								connection->getConnectionPoint1(),
								connection->getConnectionPoint2(),
								interruptingLocation)
	{}
	layoutPartialConnectionData::~layoutPartialConnectionData()
	{
		delete _interruptingRegion;
	}

	void layoutPartialConnectionData::completePartial(const simpleArray<gridLocator>& pathData)
	{
		if (_reconciled)
			throw simpleException("Trying to complete a connection marked 'reconciled'. This implies that the connection was already completed");

		layoutConnectionData::complete(pathData);
	}
	gridLocator layoutPartialConnectionData::getInterruptingLocation() const
	{
		return _locationInterrupted;
	}
	gridRegionGraphNode layoutPartialConnectionData::getInterruptingRegion() const
	{
		return *_interruptingRegion;
	}
	void layoutPartialConnectionData::setReconciled(bool value)
	{
		_reconciled = value;
	}
	bool layoutPartialConnectionData::getReconciled() const
	{
		return _reconciled;
	}
}