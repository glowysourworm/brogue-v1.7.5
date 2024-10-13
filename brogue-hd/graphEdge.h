#pragma once

#include "simple.h"
#include "brogueGlobal.h"
#include "graphNode.h"

namespace brogueHd::component
{
	template<isHashable TNode>
	struct graphEdge : hashable
	{
	public:

		TNode node1;
		TNode node2;

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