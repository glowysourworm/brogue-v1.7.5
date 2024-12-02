#pragma once

#include "graphEdge.h"
#include "gridLocator.h"
#include <simple.h>
#include <simplePoint.h>
#include <simpleGraphEdge.h>

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;

	struct gridLocatorEdge : simpleGraphEdge<gridLocator>
	{
	public:
		gridLocatorEdge()
		{
			node1 = default_value::value<gridLocator>();
			node2 = default_value::value<gridLocator>();
		}

		gridLocatorEdge(const gridLocatorEdge& copy)
		{
			node1 = copy.node1;
			node2 = copy.node2;
		}

		gridLocatorEdge(gridLocator anode1, gridLocator anode2)
		{
			node1 = anode1;
			node2 = anode2;
		}

		void operator=(const gridLocatorEdge& copy)
		{
			node1 = copy.node1;
			node2 = copy.node2;
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
		int weight() const
		{
			return (int)node1.distance(node2);
		}

		template <typename T>
		bool isEquivalent(simplePoint<T> point1, simplePoint<T> point2) const
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

		virtual size_t getHash() const override
		{
			return hashGenerator::generateHash(node1, node2);
		}
	};
}
