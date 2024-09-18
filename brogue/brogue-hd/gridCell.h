#pragma once

#include "hashGenerator.h"

using namespace brogueHd::backend::generator;

namespace brogueHd::backend::model::layout
{
	class gridCell 
	{
		short column;
		short row;

		gridCell()
		{
			column = -1;
			row = -1;
		}
		gridCell(short acol, short arow)
		{
			column = acol;
			row = arow;
		}

		static bool operator==(const gridCell& cell1, const gridCell& cell2)
		{
			return cell1.column == cell2.column &&
				   cell1.row == cell2.row;
		}

		static bool operator!=(const gridCell& cell1, const gridCell& cell2)
		{
			return cell1.column != cell2.column ||
				   cell1.row != cell2.row;
		}

		unsigned long getHashCode()
		{
			short values[] = {column, row};

			return hashGenerator::createHash<short>(values);
		}
	};
}