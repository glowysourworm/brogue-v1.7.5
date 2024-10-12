#pragma once

#include "graphNode.h"
#include "simpleMath.h"
#include "simpleExt.h"

using namespace brogueHd::simple;

namespace brogueHd::component
{
	struct gridLocator : graphNode
	{
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

		gridLocator add(const gridLocator& cell)
		{
			return gridLocator(cell.column + column, cell.row + row);
		}
		gridLocator subtract(const gridLocator& cell)
		{
			return gridLocator(column - cell.column, row - cell.row);
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

		virtual const char* getString() const
		{
			std::string result = simpleExt::format("Column={}, Row={}", column, row);

			return result.c_str();
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(column, row);
		}
	};
}