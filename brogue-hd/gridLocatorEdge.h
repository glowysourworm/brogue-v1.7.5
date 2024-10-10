#pragma once

#include "graphEdge.h"
#include "gridLocator.h"
#include "simplePoint.h"

using namespace brogueHd::simple;

namespace brogueHd::component
{
	struct gridLocatorEdge : graphEdge<gridLocator>
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

		bool operator==(const gridLocatorEdge& edge) const
		{
			return node1 == edge.node1 &&
				   node2 == edge.node2;
		}

		bool operator!=(const gridLocatorEdge& edge) const
		{
			return node1 != edge.node1 ||
				   node2 != edge.node2;
		}
		
		/// <summary>
		/// Returns the graph weight (distance) for this edge
		/// </summary>
		short weight()
		{
			return (short)node1.distance(node2);
		}

		template<typename T>
		bool isEquivalent(simplePoint<T> point1, simplePoint<T> point2)
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

		virtual size_t getHash() const
		{
			return 0;
		}
	};
}
