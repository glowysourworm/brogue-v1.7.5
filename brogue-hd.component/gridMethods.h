#pragma once

#include "gridLocator.h"
#include "gridDefinitions.h"
#include <simpleException.h>

namespace brogueHd::component
{
	class gridMethods
	{
	public:
		static bool isAdjacent(const gridLocator& location, const gridLocator& adjacentLocation,
		                       bool cardinalOnly = false)
		{
			bool north = location.row - 1 == adjacentLocation.row;
			bool south = location.row + 1 == adjacentLocation.row;
			bool east = location.column + 1 == adjacentLocation.column;
			bool west = location.column - 1 == adjacentLocation.column;
			bool equalsColumn = location.column == adjacentLocation.column;
			bool equalsRow = location.row == adjacentLocation.row;

			bool cardinallyAdjacent = (north && equalsColumn) ||
				(south && equalsColumn) ||
				(east && equalsRow) ||
				(west && equalsRow);
			if (cardinalOnly)
				return cardinallyAdjacent;
			else
			{
				return cardinallyAdjacent ||
					(north && east) ||
					(north && west) ||
					(south && east) ||
					(south && west);
			}
		}

		static brogueCompass getAdjacentDirection(const gridLocator& location, const gridLocator& adjacentLocation)
		{
			if (!isAdjacent(location, adjacentLocation))
			{
				return brogueCompass::None;
			}

			bool north = location.row - 1 == adjacentLocation.row;
			bool south = location.row + 1 == adjacentLocation.row;
			bool east = location.column + 1 == adjacentLocation.column;
			bool west = location.column - 1 == adjacentLocation.column;

			if (north)
			{
				if (east)
					return brogueCompass::NE;
				else if (west)
					return brogueCompass::NW;
				else
					return brogueCompass::N;
			}
			else if (south)
			{
				if (east)
					return brogueCompass::SE;
				else if (west)
					return brogueCompass::SW;
				else
					return brogueCompass::S;
			}
			else if (east)
				return brogueCompass::E;
			else if (west)
				return brogueCompass::W;
			else
				throw simpleException("Mishandled adjacency calculation:  gridMethods.h");
		}
	};
}
