#pragma once

#include "gridLocator.h"
#include "gridRect.h"
#include "gridRegion.h"
#include "simple.h"

using namespace brogueHd::component;

namespace brogueHd::backend::model
{
	/// <summary>
	/// Accretion tile is used to help design the layout. The connection points are desired
	/// locations for doors - which may or may not be on the edge of the region
	/// </summary>
	struct accretionTile : gridRect
	{
	public:

		accretionTile()
		{
			region = nullptr;

			hasNorthConnection = false;
			hasSouthConnection = false;
			hasEastConnection = false;
			hasWestConnection = false;

			connectionPointN = default_value::value<gridLocator>();
			connectionPointS = default_value::value<gridLocator>();
			connectionPointE = default_value::value<gridLocator>();
			connectionPointW = default_value::value<gridLocator>();
		}
		~accretionTile()
		{
		}

		/// <summary>
		/// Attempts to translate the region, and connection points, using the specified 
		/// amount. 
		/// </summary>
		bool attemptTranslastion(const gridLocator& translation)
		{
			gridRect parentBoundary = region->getParentBoundary();

			// Must be in bounds in order to translate
			if (!parentBoundary.contains(region->getBoundary() + translation) ||
				!parentBoundary.contains(connectionPointN.add(translation)) ||
				!parentBoundary.contains(connectionPointS.add(translation)) ||
				!parentBoundary.contains(connectionPointE.add(translation)) ||
				!parentBoundary.contains(connectionPointW.add(translation)))
				return false;

			region->translate_StackLike(translation.column, translation.row);

			connectionPointN.translate(translation.column, translation.row);
			connectionPointS.translate(translation.column, translation.row);
			connectionPointE.translate(translation.column, translation.row);
			connectionPointW.translate(translation.column, translation.row);

			return true;
		}

		// Tile has been connected to another tile
		bool hasNorthConnection;
		bool hasSouthConnection;
		bool hasEastConnection;
		bool hasWestConnection;

		// Connection points - these lie just outside the region; and are
		// shared with the connecting tile.
		gridLocator connectionPointN;
		gridLocator connectionPointS;
		gridLocator connectionPointE;
		gridLocator connectionPointW;

		gridRegion<gridLocator>* region;

		size_t getHash() const override
		{
			return hashGenerator::generateHash(region);
		}
	};
}