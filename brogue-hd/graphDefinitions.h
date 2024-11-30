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
	concept isGraphNode = requires(TNode a)
	{
		{ a } -> std::convertible_to<graphNode>;
	};

	template<typename TEdge, typename TNode>
	concept isGraphEdge = requires(TEdge a, TNode b)
	{
		{ a } -> std::convertible_to<graphEdge<TNode>>;
		{ b } -> std::convertible_to<graphNode>;
	};

	//// Grid specific graph constraints
	//template<typename TNode>
	//concept isGridLocatorNode = requires(TNode a)
	//{
	//	{ a } -> std::convertible_to<gridLocator>;
	//};

	//template<typename TEdge, typename TNode>
	//concept isGridLocatorEdge = requires(TEdge a, TNode b)
	//{
	//	{ a } -> std::convertible_to<gridLocatorEdge>;
	//	{ b } -> std::convertible_to<gridLocator>;
	//};

	// Graph Delegates
	template<isGraphNode TNode1, isGraphNode TNode2>
	using graphNodeConstructor = std::function<TNode2(const TNode1& node)>;

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	using graphEdgeConstructor = std::function<TEdge(const TNode& node1, const TNode& node2)>;
}