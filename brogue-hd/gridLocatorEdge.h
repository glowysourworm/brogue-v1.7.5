#pragma once

#include "graphEdge.h"
#include "gridLocator.h"
#include "mathPoint.h"

using namespace brogueHd::component::math;

namespace brogueHd::component
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
			return (short)node1.distance(node2);
		}

		template<typename T>
		bool isEquivalent(mathPoint<T> point1, mathPoint<T> point2)
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
