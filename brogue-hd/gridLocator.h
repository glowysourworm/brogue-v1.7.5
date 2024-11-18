#pragma once

#include "graphNode.h"
#include "simple.h"
#include "simpleExt.h"
#include "simpleMath.h"
#include <string>

using namespace brogueHd::simple;

namespace brogueHd::component
{
	struct gridLocator : graphNode
	{
	public:

		short column;
		short row;

		gridLocator()
		{
			column = -1;
			row = -1;
		}
		gridLocator(short acol, short arow)
		{
			column = acol;
			row = arow;
		}

		bool operator==(const gridLocator& cell) const
		{
			return column == cell.column &&
				   row == cell.row;
		}

		bool operator!=(const gridLocator& cell) const
		{
			return column != cell.column ||
				   row != cell.row;
		}
		void translate(short columnOffset, short rowOffset)
		{
			column += columnOffset;
			row += rowOffset;
		}
		gridLocator add(const gridLocator& cell)
		{
			return gridLocator(cell.column + column, cell.row + row);
		}
		gridLocator add(short acolumn, short arow)
		{
			return gridLocator(acolumn + column, arow + row);
		}
		gridLocator subtract(const gridLocator& cell)
		{
			return gridLocator(column - cell.column, row - cell.row);
		}
		gridLocator subtract(short acolumn, short arow)
		{
			return gridLocator(column - acolumn, row - arow);
		}
		double distance(const gridLocator& location)
		{
			double dx = location.column - column;
			double dy = location.row - row;

			return simpleMath::sqrt((dx * dx) + (dy * dy));
		}

		bool isEmpty()
		{
			return row == -1 && column == -1;
		}

		static gridLocator getEmpty()
		{
			return gridLocator(-1, -1);
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(column, row);
		}
	};
}