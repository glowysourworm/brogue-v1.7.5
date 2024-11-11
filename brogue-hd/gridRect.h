#pragma once

#include "gridLocator.h"
#include "simple.h"
#include <functional>

namespace brogueHd::component
{
	/// <summary>
	/// Iterator delegate for providing feedback to/from gridRect iterator functions.
	/// </summary>
	using gridRectIterator = std::function<iterationCallback(short column, short row)>;

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
			return column + (width / 2);
		}
		short centerY() const
		{
			return row + (height / 2);
		}
		long area() const
		{
			return width * height;
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
	};
}