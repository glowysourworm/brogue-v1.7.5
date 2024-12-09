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
									const gridLocator& interruptingLocation);

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

		/// <summary>
		/// Returns true if the partial connection was completed successfully, either by reconciling
		/// or by direct-means.
		/// </summary>
		bool getReconciled() const;

	private:

		gridRegionGraphNode* _interruptingRegion;
		gridLocator _locationInterrupted;

		bool _reconciled;
	};

	layoutPartialConnectionData::layoutPartialConnectionData(const gridRegionGraphNode& source,
															 const gridRegionGraphNode& destination,
															 const gridRegionGraphNode& interruptingRegion,
															 const gridLocator& interruptingLocation) :
	layoutConnectionData(source, destination)
	{
		_interruptingRegion = new gridRegionGraphNode(interruptingRegion);
		_locationInterrupted = interruptingLocation;
		_reconciled = false;
	}
	layoutPartialConnectionData::layoutPartialConnectionData(const layoutConnectionData* connection,
															 const gridRegionGraphNode& interruptingRegion,
															 const gridLocator& interruptingLocation) :
	layoutPartialConnectionData(connection->getNode1(),
								connection->getNode2(),
								interruptingRegion,
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

		// Go ahead and mark "reconciled"
		_reconciled = true;

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