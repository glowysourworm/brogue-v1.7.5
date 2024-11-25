#pragma once

#include "simple.h"

using namespace brogueHd::simple;

namespace brogueHd::component
{
	template<isHashable TNode>
	struct graphEdge : hashable
	{
	public:

		TNode node1;
		TNode node2;

		graphEdge()
		{
			node1 = default_value::value<TNode>();
			node2 = default_value::value<TNode>();
		}
		graphEdge(const TNode& anode1, const TNode& anode2)
		{
			node1 = anode1;
			node2 = anode2;
		}
		graphEdge(const graphEdge& copy)
		{
			node1 = copy.node1;
			node2 = copy.node2;
		}
		void operator=(const graphEdge& copy)
		{
			node1 = copy.node1;
			node2 = copy.node2;
		}

		virtual bool operator==(const graphEdge& edge) const
		{
			return node1 == edge.node1 &&
					node2 == edge.node2;
		}
		virtual bool operator!=(const graphEdge& edge) const
		{
			return node1 != edge.node1 ||
				node2 != edge.node2;
		}

		virtual size_t getHash() const override
		{
			return hashGenerator::generateHash(node1, node2);
		}
	};
}