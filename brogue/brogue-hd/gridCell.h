#pragma once

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
	};
}