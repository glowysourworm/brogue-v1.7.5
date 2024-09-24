#pragma once

#include "accretionTiling.h"
#include "extensionDefinitions.h"
#include "array2DExtension.h"
#include "exceptionHandler.h"
#include "brogueMath.h"
#include "gridRect.h"

using namespace brogueHd::backend::math;
using namespace brogueHd::backend::extension;

namespace brogueHd::backend::model::layout
{
	accretionTiling::accretionTiling(gridRect boundary, gridRect minBoundary)
	{
		_usedGrid = new array2D<bool>(boundary, boundary);

		_minBoundary = minBoundary;
	}
	accretionTiling::~accretionTiling()
	{
		delete _usedGrid;
	}

	/// <summary>
	/// Adds tile to the accretion and marks used space
	/// </summary>
	void accretionTiling::addTile(const accretionTile& tile)
	{
		array2D<bool>* usedGrid = _usedGrid;

		// Mark Cells
		array2DExtension<bool>::iterateIn(_usedGrid, tile, [&usedGrid](short column, short row)
		{
			usedGrid->set(column, row, true);
		});

		// Add Tile
		_tiles->push_back(tile);
	}

	bool accretionTiling::hasAvailableSpace(const accretionTile& from, brogueCompass direction)
	{
		switch (direction)
		{
		case brogueCompass::N:
			return _nextNorth.area() >= _minBoundary.area();

		case brogueCompass::S:
			return _nextSouth.area() >= _minBoundary.area();

		case brogueCompass::E:
			return _nextEast.area() >= _minBoundary.area();

		case brogueCompass::W:
			return _nextWest.area() >= _minBoundary.area();

		default:
			brogueException::show("Invalid brogueCompass direction:  accretionTiling.getAvailableSpace");
		}
	}

	gridRect accretionTiling::getAvailableSpace(const accretionTile& from, brogueCompass direction)
	{
		if (!this->hasAvailableSpace(direction))
			brogueException::show("Not enough space to continue accretion:  accretionTiling.cpp");

		switch (direction)
		{
		case brogueCompass::N:
			return _nextNorth;
		case brogueCompass::S:
			return _nextSouth;
		case brogueCompass::E:
			return _nextEast;
		case brogueCompass::W:
			return _nextWest;
		default:
			brogueException::show("Invalid brogueCompass direction:  accretionTiling.getAvailableSpace");
		}
	}

	gridRect accretionTiling::calculateAvailableSpace(const accretionTile& from, brogueCompass direction)
	{
		// Procedure
		//
		// 1) Look to the {direction} of from tile - just outside the boundary
		// 
		// 2) Divide the grid based on that line (to minimize iterating); calculate sub-rectangles
		//
		// 3) Iterate the edge along the {from} tile in {direction} and look for the best rectangle
		//	  that fills the space and touches the {from} tile.
		//

		gridRect boundary = _usedGrid->getBoundary();
		gridRect constraint;

		switch (direction)
		{
		case brogueCompass::N:
		{
			short outsideRow = brogueMath<short>::clamp(from.row - 1, from.row - 1, boundary.top());

			constraint = gridRect()

			_usedGrid->calculateSubRectangles()

			// Iterate along N edge -> W
			for (left = from.column; left >= boundary.left() && !found; left--)
			{
				// Existing Tile
				if (_usedGrid->get(left, outsideRow))
					break;
			}

			// Iterate

		}
			break;
		case brogueCompass::S:
			break;
		case brogueCompass::E:
			break;
		case brogueCompass::W:
			break;
		default:
			break;
		}
	}

	std::vector<gridRect> accretionTiling::calculateUnusedSpace()
	{

	}
}