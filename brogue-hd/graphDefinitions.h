#pragma once

#include "graphNode.h"
#include "graphEdge.h"
#include "gridLocatorEdge.h"
#include <functional>

using namespace std;

namespace brogueHd::component
{
	// Abstract graph constraints
	template<typename TNode>
	concept graphNodeType = requires(TNode a)
	{
		{ a } -> std::convertible_to<graphNode>;
	};

	template<typename TEdge, typename TNode>
	concept graphEdgeType = requires(TEdge a, TNode b)
	{
		{ a } -> std::convertible_to<graphEdge<TNode>>;
		{ b } -> std::convertible_to<graphNode>;
	};

	// Grid specific graph constraints
	template<typename TNode>
	concept isGridLocatorNode = requires(TNode a)
	{
		{ a } -> std::convertible_to<gridLocator>;
	};

	template<typename TEdge>
	concept isGridLocatorEdge = requires(TEdge a)
	{
		{ a } -> std::convertible_to<gridLocatorEdge>;
	};

	// Graph Delegates
	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	using graphEdgeConstructor = std::function<TEdge(TNode node1, TNode node2)>;
}