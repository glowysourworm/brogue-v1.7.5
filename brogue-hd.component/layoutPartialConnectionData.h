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
		layoutPartialConnectionData(layoutDesignRect* source,
		                            layoutDesignRect* destination,
									layoutDesignRect* interruptingRegion,
									const gridLocator& locationSource, 
									const gridLocator& locationDest,
									const gridLocator& locationInterrupted);
		layoutPartialConnectionData(layoutConnectionData* connection,
									layoutDesignRect* interruptingRegion,
									const gridLocator& interruptingLocation);
		~layoutPartialConnectionData();

		void completePartial(layoutDesignRect* interruptingRegion, const simpleArray<gridLocator>& pathData);

		layoutDesignRect* getInterruptingRegion() const;
		gridLocator getInterruptingLocation() const;

		bool hasOriginalConnection(const gridLocator& locationSource, const gridLocator& locationDest);

		void setReconciled();
		bool getReconciled() const;

	private:

		layoutDesignRect* _interruptingRegion;
		gridLocator _locationInterrupted;

		bool _reconciled;
	};

	layoutPartialConnectionData::layoutPartialConnectionData(layoutDesignRect* source,
															 layoutDesignRect* destination,
															 layoutDesignRect* interruptingRegion,
															 const gridLocator& locationSource,
															 const gridLocator& locationDest,
															 const gridLocator& locationInterrupted) : 
	layoutConnectionData(source, destination, locationSource, locationDest)
	{
		_interruptingRegions = interruptingRegion;
		_locationInterrupted = locationInterrupted;

		_interruptingRegions->add(interruptingRegion);

		_reconciled = false;
	}
	layoutPartialConnectionData::layoutPartialConnectionData(layoutConnectionData* connection,
															 layoutDesignRect* interruptingRegion,
															 const gridLocator& interruptingLocation) :
	layoutPartialConnectionData(connection->getRegion1Rect(),
								connection->getRegion2Rect(),
								interruptingRegion,
								connection->getConnectionPoint1(),
								connection->getConnectionPoint2(),
								interruptingLocation)
	{}
	layoutPartialConnectionData::~layoutPartialConnectionData()
	{
	}

	void layoutPartialConnectionData::completePartial(layoutDesignRect* interruptingRegion, const simpleArray<gridLocator>& pathData)
	{
		if (_reconciled)
			throw simpleException("Trying to complete a connection marked 'reconciled'. This implies that the connection was already completed");

		_interruptingRegionOnCompletion = interruptingRegion;

		layoutConnectionData::complete(pathData);
	}
	layoutDesignRect* layoutPartialConnectionData::getInterruptingRegionOnCompletion() const
	{
		return _interruptingRegionOnCompletion;
	}
	int layoutPartialConnectionData::getInterruptingRegionCount() const
	{
		return _interruptingRegions->count();
	}
	gridLocator layoutPartialConnectionData::getInterruptingLocation() const
	{
		return _locationInterrupted;
	}
	layoutDesignRect* layoutPartialConnectionData::getInterruptingRegion(int index) const
	{
		return _interruptingRegions->get(index);
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