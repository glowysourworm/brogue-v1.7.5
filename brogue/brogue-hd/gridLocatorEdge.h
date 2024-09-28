#pragma once

#include "graphEdge.h"
#include "gridLocator.h"
#include "point.h"

using namespace brogueHd::backend::math;

namespace brogueHd::backend::model::layout
{
	struct gridLocatorEdge : public graphEdge<gridLocator>
	{
		gridLocator node1;
		gridLocator node2;

		gridLocatorEdge()
		{}

		gridLocatorEdge(gridLocator anode1, gridLocator anode2)
		{
			node1 = anode1;
			node2 = anode2;
		}
		
		/// <summary>
		/// Returns the graph weight (distance) for this edge
		/// </summary>
		short weight()
		{
			return node1.distance(node2);
		}

		template<typename T>
		bool isEquivalent(point<T> point1, point<T> point2)
		{
			return (node1.column == point1.x &&
				    node1.row == point1.y &&
				    node2.column == point2.x &&
				    node2.row == point2.y) ||
				   (node1.column == point2.x &&
					node1.row == point2.y &&
					node2.column == point1.x &&
					node2.row == point1.y);
		}
	};
}
