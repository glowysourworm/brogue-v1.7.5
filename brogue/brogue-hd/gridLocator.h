#pragma once

#include <string>
#include <format>
#include <iostream>
#include "graphNode.h"
#include "stringExtension.h"
#include "typeConverter.h"
#include "brogueMath.h"

using namespace std;
using namespace brogueHd::backend::math;
using namespace brogueHd::component;

namespace brogueHd::backend::model::layout
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

		bool operator==(const gridLocator& cell)
		{
			return column == cell.column &&
				   row == cell.row;
		}

		bool operator!=(const gridLocator& cell)
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

			return brogueMath<double>::sqrt((dx * dx) + (dy * dy));
		}

		bool isEmpty()
		{
			return row == -1 && column == -1;
		}

		static gridLocator getEmpty()
		{
			return gridLocator(-1, -1);
		}

		std::string getString()
		{
			//return std::format("(Col={}, Row={})", typeConverter::shortToString(column), typeConverter::shortToString(row));
			return std::string("TODO");
		}
	};
}