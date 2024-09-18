#pragma once

namespace brogueHd::backend::model::layout
{
	struct gridRect
	{
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

		gridRect(short acolumn, short arow, short awidth, short aheight)
		{
			column = acolumn;
			row = arow;
			width = awidth;
			height = aheight;
		}

		short left()
		{
			return column;
		}
		short right()
		{
			return column + width - 1;
		}
		short top()
		{
			return row;
		}
		short bottom()
		{
			return row + height - 1;
		}
		short centerX()
		{
			return column + (width / 2);
		}
		short centerY()
		{
			return row + (height / 2);
		}
		long area()
		{
			return width * height;
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
	};
}