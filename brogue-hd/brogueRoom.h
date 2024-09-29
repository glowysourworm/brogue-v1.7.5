#pragma once

#include <gridLocator.h>
#include <gridRegion.h>
#include "brogueCell.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model
{
	class brogueRoom
	{
		brogueRoom(gridRegion<brogueCell> region,
					gridLocator connectionPointN,
					gridLocator connectionPointS,
					gridLocator connectionPointE,
					gridLocator connectionPointW)
		{
			_region = region;
			_connectionPointN = connectionPointN;
			_connectionPointS = connectionPointS;
			_connectionPointE = connectionPointE;
			_connectionPointW = connectionPointW;
		}

		~brogueRoom(){};

	private:

		gridRegion<brogueCell> _region;

		// Connection points are inside the region:  THEY ARE NOT DOORS!
		gridLocator _connectionPointN;
		gridLocator _connectionPointS;
		gridLocator _connectionPointE;
		gridLocator _connectionPointW;
	};
}