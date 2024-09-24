#pragma once

#include <string>
#include <format>
#include <iostream>

using namespace std;

namespace brogueHd::backend::model::layout
{
	struct gridLocator
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

		static bool operator==(const gridLocator& cell1, const gridLocator& cell2)
		{
			return cell1.column == cell2.column &&
				cell1.row == cell2.row;
		}

		static bool operator!=(const gridLocator& cell1, const gridLocator& cell2)
		{
			return cell1.column != cell2.column ||
				cell1.row != cell2.row;
		}

		gridLocator add(const gridLocator& cell)
		{
			return gridLocator(cell.column + column, cell.row + row);
		}
		gridLocator subtract(const gridLocator& cell)
		{
			return gridLocator(column - cell.column, row - cell.row);
		}

		bool isEmpty()
		{
			return row == -1 && column == -1;
		}

		static gridLocator getEmpty()
		{
			return gridLocator(-1, -1);
		}

		char* getString()
		{
			return std::format("(Col={}, Row={})", column, row);
		}
	};
}