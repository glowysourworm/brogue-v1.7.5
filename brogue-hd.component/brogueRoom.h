#pragma once
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridRegion.h"

#include <simple.h>
#include <brogueCell.h>

namespace brogueHd::component
{
	using namespace simple;

	class brogueRoom : public simpleObject
	{
	public:
		template <isGridLocator T>
		brogueRoom(gridRegion* region,
		           const gridLocator& connectionPointN,
		           const gridLocator& connectionPointS,
		           const gridLocator& connectionPointE,
		           const gridLocator& connectionPointW)
		{
			//// (MEMORY!)
			//_region = gridRegionExtension::map(region, [] (const T& locator)
			//{
			//	return new brogueCell(locator.column, locator.row);
			//});

			_connectionPointN = new gridLocator(connectionPointN);
			_connectionPointS = new gridLocator(connectionPointS);
			_connectionPointE = new gridLocator(connectionPointE);
			_connectionPointW = new gridLocator(connectionPointW);
		}

		~brogueRoom()
		{
			//_region->iterateLocations([] (int column, int row, brogueCell* item)
			//{
			//	delete item;

			//	return iterationCallback::iterate;
			//});

			// (MEMORY!)
			delete _region;

			// local memory
			delete _connectionPointN;
			delete _connectionPointS;
			delete _connectionPointE;
			delete _connectionPointW;
		};

	public:
		size_t getHash() const override
		{
			return 0;
		}

	private:
		gridRegion* _region;

		// Connection points are inside the region:  THEY ARE NOT DOORS!
		gridLocator* _connectionPointN;
		gridLocator* _connectionPointS;
		gridLocator* _connectionPointE;
		gridLocator* _connectionPointW;
	};
}
