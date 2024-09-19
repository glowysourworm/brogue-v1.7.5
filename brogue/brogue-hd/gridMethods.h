#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "broguedef.h"
#include "iteratordef.h"
#include <functional>

using namespace std;

using namespace brogueHd::backend::extension;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model::layout
{

	/*
	
		const short nbDirs[8][2] = { {0,-1}, {0,1}, {-1,0}, {1,0}, {-1,-1}, {-1,1}, {1,-1}, {1,1} };
		const short cDirs[8][2] = { {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1} };

	*/

	template<gridCellConstraint T>
	static void iterate(const grid<T>* grid, gridDelegates::gridIterationCallback callback)
	{
		bool userBreak = false;

		for (short i = 0; i < grid->columnCount() && !userBreak; i++)
		{
			for (short j = 0; j < grid->rowCount() && !userBreak; j++)
			{
				if (callback(i, j) == iterationCallback::breakAndReturn)
					userBreak = true;
			}
		}
	}

	template<gridCellConstraint T>
	static void iterate(const gridRect& boundary, gridDelegates::gridIterationCallback callback)
	{
		bool userBreak = false;

		for (short i = boundary.left(); i <= boundary.right() && !userBreak; i++)
		{
			for (short j = boundary.top(); j <= boundary.bottom() && !userBreak; j++)
			{
				if (callback(i, j) == iterationCallback::breakAndReturn)
					userBreak = true;
			}
		}
	}

	template<gridCellConstraint T>
	static void iterateOutward(const grid<T>* grid, 
							   short centerColumn, 
							   short centerRow, 
							   short distance, 
							   gridDelegates::gridIterationCallback callback)
	{
		bool userBreak = false;

		gridRect boundary = grid->getBoundary();
		short left = clamp(centerColumn - distance, boundary.left(), boundary.right());
		short right = clamp(centerColumn + distance, boundary.left(), boundary.right());
		short top = clamp(centerRow - distance, boundary.top(), boundary.bottom());
		short bottom = clamp(centerColumn + distance, boundary.top(), boundary.bottom());

		for (short i = left; i <= right && !userBreak; i++)
		{
			for (short j = top; j <= bottom && !userBreak; j++)
			{
				if (callback(i, j) == iterationCallback::breakAndReturn)
					userBreak = true;
			}
		}
	}

	template<gridCellConstraint T>
	static void iterateIn(const grid<T>* grid, gridRect boundary, gridDelegates::gridIterationCallback callback)
	{
		bool userBreak = false;

		short maxWidth = min(boundary.right() + 1, grid->columnCount());
		short maxHeight = min(boundary.top() + 1, grid->rowCount());

		for (short i = column; i < maxWidth && !userBreak; i++)
		{
			for (short j = row; j < maxHeight && !userBreak; j++)
			{
				if (callback(i, j) == iterationCallback::breakAndReturn)
					userBreak = true;
			}
		}
	}

	template<gridCellConstraint T>
	static void iterateIn(const gridRegion<T>* grid, gridRect boundary, gridDelegates::gridIterationCallback callback)
	{
		bool userBreak = false;

		short column = max(boundary.left(), grid->getBoundary().left());
		short row = max(boundary.top(), grid->getBoundary().top());
		short maxWidth = min(boundary.right() + 1, grid->getBoundary().right());
		short maxHeight = min(boundary.top() + 1, grid->getBoundary().bottom());

		for (short i = column; i < maxWidth && !userBreak; i++)
		{
			for (short j = row; j < maxHeight && !userBreak; j++)
			{
				if (callback(i, j) == iterationCallback::breakAndReturn)
					userBreak = true;
			}
		}
	}

	template<gridCellConstraint T>
	static void iterateInCircle(const grid<T>* grid, gridRect boundary, gridDelegates::gridIterationCallback callback)
	{
		bool userBreak = false;

		for (short i = boundary.left(); i <= boundary.right() && !userBreak; i++)
		{
			for (short j = boundary.top(); j < boundary.bottom() && !userBreak; j++)
			{
				short dx = boundary.centerX() - i;
				short dy = boundary.centerY() - j;

				if (((dx * dx) + (dy * dy) <= boundary.width * boundary.width))
				{
					userBreak = callback(i, j);
				}
					
			}
		}
	}

	template<gridCellConstraint T>
	static void iterateAround(const grid<T>* grid, short column, short row, bool withinBounds, gridDelegates::gridIterationCallback callback)
	{
		short newX, newY;

		bool userBreak = false;

		for (short dir = 0; dir < DIRECTION_COUNT && !userBreak; dir++)
		{
			newX = i + nbDirs[dir][0];
			newY = j + nbDirs[dir][1];

			if (withinBounds)
			{
				if (grid->isInBounds(newX, newY))
				{
					userBreak = callback(newX, newY);
				}
					
			}

			// Unsafe
			else
			{
				userBreak = callback(newX, newY);
			}
		}
	}

	template<gridCellConstraint T>
	static void iterateAroundCardinal(const grid<T>* grid, short column, short row, bool withinBounds, gridDelegates::gridIterationCallback callback)
	{
		short newX, newY;

		bool userBreak = false;

		for (short dir = 0; dir < 4 && !userBreak; dir++) 
		{
			newX = column + nbDirs[dir][0];
			newY = row + nbDirs[dir][1];

			if (withinBounds)
			{
				if (grid->isInBounds(newX, newY))
				{
					userBreak = callback(newX, newY);
				}
			}

			// Unsafe
			else
			{
				userBreak = callback(newX, newY);
			}
		}
	}
}