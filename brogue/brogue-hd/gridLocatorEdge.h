#pragma once

#include "graphEdge.h"
#include "gridLocator.h"
#include "point.h"

using namespace brogueHd::backend::math;

namespace brogueHd::backend::model::layout
{
	struct gridLocatorEdge : graphEdge
	{
		gridLocatorNode node1;
		gridLocatorNode node2;

		gridLocatorEdge()
		{}

		gridLocatorEdge(gridLocatorNode anode1, gridLocatorNode anode2)
		{
			node1 = anode1;
			node2 = anode2;
		}
		
		/// <summary>
		/// Returns the graph weight (distance) for this edge
		/// </summary>
		short weight()
		{
			return node1.calculateDistance(node2);
		}

		template<typename T>
		bool isEquivalent(point<T> point1, point<T> point2)
		{
			return (node1.locator.column == point1.x &&
				    node1.locator.row == point1.y &&
				    node2.locator.column == point2.x &&
				    node2.locator.row == point2.y) ||
				   (node1.locator.column == point2.x &&
					node1.locator.row == point2.y &&
					node2.locator.column == point1.x &&
					node2.locator.row == point1.y);
		}
	};
}
