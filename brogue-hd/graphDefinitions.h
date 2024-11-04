#pragma once

#include "graphEdge.h"
#include "graphNode.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include <functional>
#include <type_traits>

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

	template<typename TEdge, typename TNode>
	concept isGridLocatorEdge = requires(TEdge a, TNode b)
	{
		{ a } -> std::convertible_to<gridLocatorEdge>;
		{ b } -> std::convertible_to<gridLocator>;
	};

	// Graph Delegates
	template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
	using graphNodeConstructor = std::function<TNode(gridLocator)>;

	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	using graphEdgeConstructor = std::function<TEdge(TNode node1, TNode node2)>;
}