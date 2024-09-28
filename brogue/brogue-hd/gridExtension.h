#pragma once

#include "grid.h"
#include "simple.h"
#include "gridRect.h"
#include "brogueMath.h"
#include <functional>

using namespace std;

using namespace brogueHd::backend::model::layout;
using namespace brogueHd::backend::math;

namespace brogueHd::backend::extension
{

	/*
	
		const short nbDirs[8][2] = { {0,-1}, {0,1}, {-1,0}, {1,0}, {-1,-1}, {-1,1}, {1,-1}, {1,1} };
		const short cDirs[8][2] = { {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1} };

	*/

	/// <summary>
	/// Delegate definitions for most of the grid classes to share. This should
	/// facilitate iterators on these classes - apart from array2D, and std collections.
	/// </summary>
	template<typename T>
	struct gridDelegates
	{
		/// <summary>
		/// Simple iteration callback. Set the return value to either continue / break.
		/// </summary>
		typedef std::function<iterationCallback(short column, short row, T item)> callback;

		/// <summary>
		/// Simple predicate decision making function that includes the item coordinates
		/// </summary>
		typedef std::function<bool(short column, short row, T item)> predicate;

		/// <summary>
		/// Simple predicate decision making function
		/// </summary>
		typedef std::function<bool(T item)> simplePredicate;

		/// <summary>
		/// Searches grid for requested value based on comparison. Each truthy aggregateComparator result will store
		/// the next value as the going compare-ee. This compare-ee will be passed as the first argument of the comparator.
		/// The second argument will be the current grid value.
		/// </summary>
		typedef std::function<bool(T aggregate, T value)> aggregateComparer;

		/// <summary>
		/// Defines a constructor (or getter) method for grid cells
		/// </summary>
		typedef std::function<T(short, short)> constructor;
	};

	template<typename T, typename TResult>
	struct gridSelectorDelegates
	{
		/// <summary>
		/// Selector of a value from the current item that includes the grid coordinates
		/// </summary>
		typedef std::function<TResult(short column, short row, T item)> selector;

		/// <summary>
		/// Definition of function to select a value from the grid type.
		/// </summary>
		typedef std::function<TResult(T item)> simpleSelector;
	};

	struct gridRectDelegates
	{
		/// <summary>
		/// Simple iteration callback. Set the return value to either continue / break.
		/// </summary>
		typedef std::function<iterationCallback(short column, short row)> callback;
	};

	template<typename T>
	struct gridExtension
	{
		template<typename T>
		static void iterate(const grid<T>& grid, gridDelegates<T>::callback callback)
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

		template<typename T>
		static void iterateOutward(const grid<T>& grid,
								   short centerColumn,
								   short centerRow,
								   short distance,
								   gridDelegates<T>::callback callback)
		{
			bool userBreak = false;

			gridRect boundary = grid->getBoundary();

			short left = brogueMath<short>::clamp(centerColumn - distance, boundary.left(), boundary.right());
			short right = brogueMath<short>::clamp(centerColumn + distance, boundary.left(), boundary.right());
			short top = brogueMath<short>::clamp(centerRow - distance, boundary.top(), boundary.bottom());
			short bottom = brogueMath<short>::clamp(centerColumn + distance, boundary.top(), boundary.bottom());

			for (short i = left; i <= right && !userBreak; i++)
			{
				for (short j = top; j <= bottom && !userBreak; j++)
				{
					if (callback(i, j) == iterationCallback::breakAndReturn)
						userBreak = true;
				}
			}
		}

		template<typename T>
		static void iterateIn(const grid<T>& grid, gridRect boundary, gridDelegates<T>::callback callback)
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

		template<typename T>
		static void iterateAround(const grid<T>& grid, short column, short row, bool withinBounds, gridDelegates<T>::callback callback)
		{
			short newX, newY;

			bool userBreak = false;

			for (short i = column - 1; i < column + 1 && !userBreak; i++)
			{
				for (short j = row - 1; j < row + 1 && !userBreak; j++)
				{
					if (withinBounds)
					{
						if (grid.isInBounds(i, j))
							userBreak = callback(grid->get(i, j));
					}
					else
					{
						userBreak = callback(grid->get(i, j));
					}
				}
			}
		}

		template<typename T>
		static void iterateAroundCardinal(const grid<T>& grid, short column, short row, bool withinBounds, gridDelegates<T>::callback callback)
		{
			iterationCallback response = iterationCallback::iterate;

			// North
			T north = grid->getAdjacentUnsafe(column, row - 1);

			if (grid->isInBounds(column, row - 1))
				response = callback(column, row - 1, north);

			else if (!withinBounds)
				response = callback(column, row - 1, north);

			if (response == iterationCallback::breakAndReturn)
				return;

			// South
			T south = grid->getAdjacentUnsafe(column, row + 1);

			if (grid->isInBounds(column, row + 1))
				response = callback(column, row + 1, south);

			else if (!withinBounds)
				response = callback(column, row + 1, south);

			if (response == iterationCallback::breakAndReturn)
				return;

			// East
			T east = grid->getAdjacentUnsafe(column + 1, row);

			if (grid->isInBounds(column + 1, row))
				response = callback(column + 1, row, east);

			else if (!withinBounds)
				response = callback(column + 1, row, east);

			if (response == iterationCallback::breakAndReturn)
				return;

			// West
			T west = grid->getAdjacentUnsafe(column - 1, row);

			if (grid->isInBounds(column - 1, row))
				response = callback(column - 1, row, west);

			else if (!withinBounds)
				response = callback(column - 1, row, west);

			if (response == iterationCallback::breakAndReturn)
				return;
		}
	};

	struct gridRectExtension
	{
		static void iterate(const gridRect& boundary, gridRectDelegates::callback callback)
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
		static void iterateInCircle(gridRect boundary, gridRectDelegates::callback callback)
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
	};
}