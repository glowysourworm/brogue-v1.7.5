#pragma once

#include <simpleGraphNode.h>
#include <simple.h>
#include <simpleMath.h>

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;

	struct gridLocator : public simpleGraphNode
	{
	public:
		int column;
		int row;

		gridLocator()
		{
			column = -1;
			row = -1;
		}

		gridLocator(int acol, int arow)
		{
			column = acol;
			row = arow;
		}

		gridLocator(const gridLocator& copy)
		{
			column = copy.column;
			row = copy.row;
		}

		void operator=(const gridLocator& copy)
		{
			column = copy.column;
			row = copy.row;
		}

		virtual bool operator==(const gridLocator& cell) const
		{
			return column == cell.column &&
				row == cell.row;
		}

		virtual bool operator!=(const gridLocator& cell) const
		{
			return column != cell.column ||
				row != cell.row;
		}

		void translate(int columnOffset, int rowOffset)
		{
			column += columnOffset;
			row += rowOffset;
		}

		gridLocator add(const gridLocator& cell)
		{
			return gridLocator(cell.column + column, cell.row + row);
		}

		gridLocator add(int acolumn, int arow)
		{
			return gridLocator(acolumn + column, arow + row);
		}

		gridLocator subtract(const gridLocator& cell)
		{
			return gridLocator(column - cell.column, row - cell.row);
		}

		gridLocator subtract(int acolumn, int arow)
		{
			return gridLocator(column - acolumn, row - arow);
		}

		double distance(const gridLocator& location) const
		{
			double dx = location.column - column;
			double dy = location.row - row;

			return simpleMath::squareRoot((dx * dx) + (dy * dy));
		}

		bool isEmpty()
		{
			return row == -1 && column == -1;
		}

		static gridLocator getEmpty()
		{
			return gridLocator(-1, -1);
		}

		virtual size_t getHash() const override
		{
			return hashGenerator::generateHash(column, row);
		}
	};
}
