#pragma once

#include "gridLocator.h"

namespace brogueHd::component
{
	template <typename T>
	struct gridCell : gridLocator
	{
	public:
		gridLocator locator;
		T value;

		gridCell()
		{
		}

		gridCell(int column, int row, T cellValue)
		{
			locator.column = column;
			locator.row = row;
			value = cellValue;
		}
	};
}
