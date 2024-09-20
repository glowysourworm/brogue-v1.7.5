#pragma once

#include "array2D.h"
#include "array2DExtension.h"
#include "extensionDefinitions.h"
#include "gridRect.h"
#include <functional>

using namespace std;

using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::extension
{

	/*
	
		const short nbDirs[8][2] = { {0,-1}, {0,1}, {-1,0}, {1,0}, {-1,-1}, {-1,1}, {1,-1}, {1,1} };
		const short cDirs[8][2] = { {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1} };

	*/

	template<typename T>
	struct array2DExtension
	{
		static void iterate(const array2D<T>* grid, extensionArray2DDelegates::simpleCallback callback)
		{
			bool userBreak = false;

			gridRect boundary = grid->getBoundary();

			for (short i = boundary.left(); i <= boundary.right() && !userBreak; i++)
			{
				for (short j = boundary.top(); j <= boundary.bottom() && !userBreak; j++)
				{
					if (callback(i, j) == iterationCallback::breakAndReturn)
						userBreak = true;
				}
			}
		}

		static void iterate(const gridRect& boundary, extensionArray2DDelegates::simpleCallback callback)
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

		static void iterateOutward(const array2D<T>* grid,
									short centerColumn,
									short centerRow,
									short distance,
									extensionArray2DDelegates::simpleCallback callback)
		{
			bool userBreak = false;

			gridRect boundary = grid->getBoundary();

			short left = brogueMath::clamp(centerColumn - distance, boundary.left(), boundary.right());
			short right = brogueMath::clamp(centerColumn + distance, boundary.left(), boundary.right());
			short top = brogueMath::clamp(centerRow - distance, boundary.top(), boundary.bottom());
			short bottom = brogueMath::clamp(centerColumn + distance, boundary.top(), boundary.bottom());

			for (short i = left; i <= right && !userBreak; i++)
			{
				for (short j = top; j <= bottom && !userBreak; j++)
				{
					if (callback(i, j) == iterationCallback::breakAndReturn)
						userBreak = true;
				}
			}
		}

		static void iterateIn(const array2D<T>* grid, gridRect boundary, extensionArray2DDelegates::simpleCallback callback)
		{
			bool userBreak = false;

			gridRect gridBoundary = grid->getBoundary();

			short safeRight = brogueMath::clamp(boundary.right(), gridBoundary.left(), gridBoundary.right());
			short safeBottom = brogueMath::clamp(boundary.bottom(), gridBoundary.top(), gridBoundary.bottom());

			for (short i = boundary.left(); i <= safeRight && !userBreak; i++)
			{
				for (short j = boundary.top(); j <= safeBottom && !userBreak; j++)
				{
					if (callback(i, j) == iterationCallback::breakAndReturn)
						userBreak = true;
				}
			}
		}

		static void iterateInCircle(const array2D<T>* grid, gridRect boundary, extensionArray2DDelegates::simpleCallback callback)
		{
			bool userBreak = false;

			for (short i = boundary.left(); i <= boundary.right() && !userBreak; i++)
			{
				for (short j = boundary.top(); j <= boundary.bottom() && !userBreak; j++)
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

		static void iterateAround(const array2D<T>* grid, short column, short row, bool withinBounds, extensionArray2DDelegates::simpleCallback callback)
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

		static void iterateAroundCardinal(const array2D<T>* grid, short column, short row, bool withinBounds, extensionArray2DDelegates::simpleCallback callback)
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
	};
}