#pragma once

#include "graphNode.h"

namespace brogueHd::component
{
	template<typename TNode>
	struct graphEdge
	{
		TNode node1;
		TNode node2;
	};
}