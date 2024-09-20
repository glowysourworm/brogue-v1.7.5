#pragma once

#include "graphNode.h"
#include "graphEdge.h"
#include <functional>

namespace brogueHd::backend::math
{
	template<typename TNode, typename TValue>
	concept graphNodeType = requires(TNode a, TValue b)
	{
		{ a } -> std::convertible_to<graphNode<TValue>>;
	};

	template<typename TEdge, typename TNode, typename TWeight, typename TValue>
	concept graphEdgeType = requires(TEdge a, TNode b)
	{
		{ a } -> std::convertible_to<graphEdge<TWeight, TValue>>;
		{ b } -> std::convertible_to<graphNode<TValue>>;
	};

	// This template got a little unruly
	#define GraphTemplate typename TValue, typename TWeight, graphNodeType<TValue> TNode, graphEdgeType<TNode, TWeight, TValue> TEdge

	// This template got a little unruly
	#define GraphEdgeCollectionTemplate typename TWeight, typename TValue, graphNodeType<TValue> TNode, graphEdgeType<TNode, TWeight, TValue> TEdge
}