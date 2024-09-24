#pragma once

namespace brogueHd::backend::model::layout
{
	struct gridRect
	{
	public:
		short column;
		short row;
		short width;
		short height;

		gridRect()
		{
			column = -1;
			row = -1;
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

			if (arow > top())
				return false;

			return true;
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
	};
}