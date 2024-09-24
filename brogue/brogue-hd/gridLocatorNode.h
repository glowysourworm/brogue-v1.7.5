#pragma once

#include "graphNode.h"
#include "gridLocator.h"

using namespace brogueHd::backend::math;

namespace brogueHd::backend::model::layout
{
	struct gridLocatorNode : graphNode
	{
		gridLocator locator;

		gridLocatorNode()
		{
			locator.column = -1;
			locator.row = -1;
		}

		gridLocatorNode(gridLocator alocator)
		{
			locator.column = alocator.column;
			locator.row = alocator.row;
		}

		gridLocatorNode(short column, short row)
		{
			locator.column = column;
			locator.row = row;
		}

		short calculateDistance(const gridLocatorNode& node)
		{
			short dx = node.locator.column - locator.column;
			short dy = node.locator.row - locator.row;

			return sqrt((dx * dx) + (dy * dy));
		}
	};
}
