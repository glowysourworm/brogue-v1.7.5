#pragma once

#include "graphNode.h"

namespace brogueHd::backend::math
{
	template<typename TWeight, typename TValue>
	struct graphEdge
	{
		graphNode<TValue> node1;
		graphNode<TValue> node2;

		TWeight weight;
	};
}