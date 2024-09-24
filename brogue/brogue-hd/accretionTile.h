#pragma once

#include "gridRect.h"
#include "gridRegion.h"
#include "brogueCell.h"

namespace brogueHd::backend::model::layout
{
	/// <summary>
	/// Accretion tile is used to help design the layout. The connection points are desired
	/// locations for doors - which may or may not be on the edge of the region
	/// </summary>
	struct accretionTile : public gridRect
	{
		// Tile has been connected to another tile
		bool hasNorthConnection;
		bool hasSouthConnection;
		bool hasEastConnection;
		bool hasWestConnection;

		// Connection points - not necessarily on the border
		gridLocator connectionPointN;
		gridLocator connectionPointS;
		gridLocator connectionPointE;
		gridLocator connectionPointW;

		gridRegion<gridLocator>* region;
	};
}