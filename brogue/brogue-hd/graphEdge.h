#pragma once

#include "graphNode.h"

namespace brogueHd::backend::math
{
	template<typename TNode>
	struct graphEdge
	{
		TNode node1;
		TNode node2;
	};
}