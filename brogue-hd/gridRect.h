#pragma once

#include "gridDefinitions.h"
#include "gridLocator.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleMath.h"
#include <functional>

namespace brogueHd::component
{
	/// <summary>
	/// Iterator delegate for providing feedback to/from gridRect iterator functions.
	/// </summary>
	using gridRectIterator = std::function<iterationCallback(short column, short row)>;

	/// <summary>
	/// Iterator delegate for providing feedback to/from gridRect iterator functions. The direction 
	/// is specific to the "adjacent side" along which iteration was happening.
	/// </summary>
	using gridRectDirectionalIterator = std::function<iterationCallback(short column, short row, brogueCompass direction)>;

	struct gridRect : hashable
	{
	public:
		short column;
		short row;
		short width;
		short height;

		gridRect()
		{
			column = 0;
			row = 0;
			width = 0;
			height = 0;
		}

		gridRect(const gridRect& copy)
		{
			column = copy.column;
			row = copy.row;
			width = copy.width;
			height = copy.height;
		}

		gridRect(short acolumn, short arow, short awidth, short aheight)
		{
			column = acolumn;
			row = arow;
			width = awidth;
			height = aheight;
		}
		void operator=(const gridRect& copy)
		{
			column = copy.column;
			row = copy.row;
			width = copy.width;
			height = copy.height;
		}

		friend gridRect operator+(const gridRect& instance, const gridLocator& translation)
		{
			return gridRect(instance.column + translation.column, instance.row + translation.row, instance.width, instance.height);
		}

		bool operator==(const gridRect& rect) const
		{
			return column == rect.column &&
				row == rect.row &&
				width == rect.width &&
				height == rect.height;
		}

		bool operator!=(const gridRect& rect) const
		{
			return column != rect.column ||
				row != rect.row ||
				width != rect.width ||
				height != rect.height;
		}

		/// <summary>
		/// Returns true if the grid rect represents a valid rectangle in UI coordinates
		/// </summary>
		/// <returns></returns>
		bool validateUI() const
		{
			if (column < 0)
				return false;

			if (row < 0)
				return false;

			if (width <= 0)
				return false;

			if (height <= 0)
				return false;

			return true;
		}

		virtual size_t getHash() const override
		{
			return hashGenerator::generateHash(column, row, width, height);
		}

		static gridRect fromCircle(short centerColumn, short centerRow, short radiusX, short radiusY)
		{
			gridRect result;

			result.column = centerColumn - radiusX;
			result.row = centerRow - radiusY;
			result.width = radiusX;
			result.height = radiusY;

			return result;
		}

		short left() const

		{
			return column;
		}
		short right() const
		{
			return column + width - 1;
		}
		short top() const
		{
			return row;
		}
		short bottom() const
		{
			return row + height - 1;
		}
		short centerX() const
		{
			return column + simpleMath::floor(width / 2.0f);
		}
		short centerY() const
		{
			return row + simpleMath::floor(height / 2.0f);
		}
		long area() const
		{
			return width * height;
		}

		gridLocator topLeft() const
		{
			return gridLocator(left(), top());
		}
		gridLocator topRight() const
		{
			return gridLocator(right(), top());
		}
		gridLocator bottomLeft() const
		{
			return gridLocator(left(), bottom());
		}
		gridLocator bottomRight() const
		{
			return gridLocator(right(), bottom());
		}
		gridLocator center() const
		{
			return gridLocator(centerX(), centerY());
		}
		
		/// <summary>
		/// Creates a new gridRect, expanded in all directions, by the specified amount.
		/// </summary>
		gridRect createExpanded(int amount) const
		{
			gridRect expandedRect = gridRect(column - amount, row - amount, width + (2 * amount), height + (2 * amount));

			if (expandedRect.width <= 0 ||
				expandedRect.height <= 0)
				throw simpleException("Trying to create expanded gridRect created invalid rectangle");

			return expandedRect;
		}

		/// <summary>
		/// Creates a new gridRect, shrunk in all directions, by the specified amount.
		/// </summary>
		gridRect createPadded(int padding) const
		{
			gridRect paddedRect = gridRect(column + padding, row + padding, width - (2 * padding), height - (2 * padding));

			if (paddedRect.width <= 0 ||
				paddedRect.height <= 0)
				throw simpleException("Trying to create padded gridRect created invalid rectangle");

			return paddedRect;
		}

		bool contains(short acolumn, short arow) const
		{
			if (acolumn < left())
				return false;

			if (acolumn > right())
				return false;

			if (arow < top())
				return false;

			if (arow > bottom())
				return false;

			return true;
		}

		bool contains(const gridLocator& location) const
		{
			return contains(location.column, location.row);
		}

		bool contains(const gridRect& rect) const
		{
			if (!contains(rect.left(), rect.top()))
				return false;

			if (!contains(rect.left(), rect.bottom()))
				return false;

			if (!contains(rect.right(), rect.top()))
				return false;

			if (!contains(rect.right(), rect.bottom()))
				return false;

			return true;
		}

		bool overlaps(const gridRect& rect) const
		{
			return !(right() < rect.left() ||
					 left() > rect.right() ||
					 top() > rect.bottom() ||
					 bottom() < rect.top());
		}

		/// <summary>
		/// Checks adjacency with the other gridRect. The condition is that it must share
		/// an edge; and that corners don't count.
		/// </summary>
		bool isAdjacent(const gridRect& rect) const
		{
			int adjacentEdges = 0;

			if (left() == rect.right() + 1)
				adjacentEdges++;

			if (right() == rect.left() - 1)
				adjacentEdges++;

			if (top() == rect.bottom() + 1)
				adjacentEdges++;

			if (bottom() == rect.top() - 1)
				adjacentEdges++;

			bool indicesOverlapX = (rect.left() >= left() && rect.left() <= right()) ||
								   (rect.right() <= right() && rect.right() >= left());

			bool indicesOverlapY = (rect.top() >= top() && rect.top() <= bottom()) ||
								   (rect.bottom() <= bottom() && rect.bottom() >= top());

			return !overlaps(rect) && adjacentEdges > 0 && (indicesOverlapX || indicesOverlapY);
		}

		void expand(const gridRect& rect)
		{
			expand(rect.left(), rect.top());
			expand(rect.right(), rect.bottom());
		}

		void expand(short acolumn, short arow)
		{
			if (acolumn < left())
			{
				width += column - acolumn;
				column = acolumn;
			}
			if (arow < top())
			{
				height += row - arow;
				row = arow;
			}
			if (acolumn > right())
			{
				width += acolumn - right();
			}
			if (arow > bottom())
			{
				height += arow - bottom();
			}
		}

		void translate(short columnOffset, short rowOffset)
		{
			column += columnOffset;
			row += rowOffset;
		}

		void translate(const gridLocator& translation)
		{
			translate(translation.column, translation.row);
		}

		gridLocator getTranslation(const gridRect& rect)
		{
			return gridLocator(rect.column - column, rect.row - row);
		}


		void iterate(gridRectIterator callback) const
		{
			bool userBreak = false;

			for (short i = left(); i <= right() && !userBreak; i++)
			{
				for (short j = top(); j <= bottom() && !userBreak; j++)
				{
					if (callback(i, j) == iterationCallback::breakAndReturn)
						userBreak = true;
				}
			}
		}

		void iterateBoundary(gridRectIterator callback) const
		{
			bool userBreak = false;

			for (short i = left(); i <= right() && !userBreak; i++)
			{
				if (callback(i, top()) == iterationCallback::breakAndReturn)
					userBreak = true;
			}

			for (short i = left(); i <= right() && !userBreak; i++)
			{
				if (callback(i, bottom()) == iterationCallback::breakAndReturn)
					userBreak = true;
			}

			for (short j = top() + 1; j <= bottom() - 1 && !userBreak; j++)
			{
				if (callback(left(), j) == iterationCallback::breakAndReturn)
					userBreak = true;

				if (userBreak)
					break;

				if (callback(right(), j) == iterationCallback::breakAndReturn)
					userBreak = true;
			}
		}

		void iterateAdjacent(short column, short row, gridRectIterator callback) const
		{
			for (int columnIndex = column - 1; columnIndex != column + 1; columnIndex++)
			{
				for (int rowIndex = row - 1; rowIndex != row + 1; rowIndex++)
				{
					if (contains(columnIndex, rowIndex))
					{
						if (callback(columnIndex, rowIndex) == iterationCallback::breakAndReturn)
							return;
					}
				}
			}
		}
		void iterateRowsFirst_BottomToTop(gridRectIterator callback) const
		{
			bool userBreak = false;

			for (short j = bottom(); j >= top() && !userBreak; j--)
			{
				for (short i = left(); i <= right() && !userBreak; i++)
				{
					if (callback(i, j) == iterationCallback::breakAndReturn)
						userBreak = true;
				}
			}
		}
		void iterateInCircle(gridRectIterator callback) const
		{
			bool userBreak = false;

			for (short i = left(); i <= right() && !userBreak; i++)
			{
				for (short j = top(); j <= bottom() && !userBreak; j++)
				{
					short dx = centerX() - i;
					short dy = centerY() - j;

					if (((dx * dx) + (dy * dy) <= (width * width)))
					{
						userBreak = callback(i, j);
					}
				}
			}
		}

		/// <summary>
		/// Iterates outside of the rectangle; but within the bounds of the second rectangle. Iteration
		/// will continue for the specified distance.
		/// </summary>
		void iterateOutsideAdjacent(int distance, const gridRect& constraint, gridRectDirectionalIterator callback) const
		{
			// Start adjacent to the outer boundary
			//
			int count = 1;

			while (count <= distance) 
			{
				// Top
				for (int column = left() - count; column <= right() + count && constraint.contains(column, top() - count); column++)
				{
					// Repeat (from bottom iterator)
					if (column == left() - count)
						continue;

					if (callback(column, top() - count, brogueCompass::N) == iterationCallback::breakAndReturn)
						return;
				}

				// Right
				for (int row = top() - count; row <= bottom() + count && constraint.contains(right() + count, row); row++)
				{
					// Repeat
					if (row == top() - count)
						continue;

					if (callback(right() + count, row, brogueCompass::E) == iterationCallback::breakAndReturn)
						return;
				}

				// Bottom
				for (int column = right() + count; column >= left() - count && constraint.contains(column, bottom() + count); column--)
				{
					// Repeat
					if (column == right() + count)
						continue;

					if (callback(column, bottom() + count, brogueCompass::S) == iterationCallback::breakAndReturn)
						return;
				}

				// Left
				for (int row = bottom() + count; row >= top() - count && constraint.contains(left() - count, row); row--)
				{
					// Repeat
					if (row == bottom() + count)
						continue;

					if (callback(left() - count, row, brogueCompass::W) == iterationCallback::breakAndReturn)
						return;
				}

				count++;
			}
		}
	};
}