#pragma once

#include "graphNode.h"
#include "graphEdge.h"
#include <functional>

namespace brogueHd::backend::math
{
	// Abstract graph constraints
	template<typename TNode>
	concept graphNodeType = requires(TNode a)
	{
		{ a } -> std::convertible_to<graphNode>;
	};

	template<typename TEdge>
	concept graphEdgeType = requires(TEdge a)
	{
		{ a } -> std::convertible_to<graphEdge>;
	};

	// Grid specific graph constraints
	template<typename TNode>
	concept isGridLocatorNode = requires(TNode a)
	{
		{ a } -> std::convertible_to<gridLocatorNode>;
	};

	template<typename TEdge>
	concept isGridLocatorEdge = requires(TEdge a)
	{
		{ a } -> std::convertible_to<gridLocatorEdge>;
	};

	// Graph Delegates
	template<typename TNode, typename TEdge>
	struct graphDelegates
	{
		typedef function<TEdge(TNode node1, TNode node2)> edgeConstructor;
	};
}