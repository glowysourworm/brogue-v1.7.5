#pragma once

#include "gridLocator.h"
#include <simple.h>
#include <simpleGraphDefinitions.h>
#include <functional>
#include <type_traits>

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;

	/// <summary>
	/// Defines directions for character movement
	/// </summary>
	enum class brogueCompass
	{
		None = 0,
		N = 1,
		S = 2,
		E = 4,
		W = 8,
		NW = 16,
		NE = 32,
		SW = 64,
		SE = 128
	};

	// Constraint for template function to help cast the brogue item details
	//
	template <typename T>
	concept isGridLocator = std::convertible_to<T, gridLocator>;

	/// <summary>
	/// Delegate for mapping different types of grid locators.
	/// </summary>
	/// <typeparam name="TResult">Result type</typeparam>
	/// <typeparam name="T">Source type</typeparam>
	template <isGridLocator TResult, isGridLocator T>
	using gridLocatorMapper = std::function<TResult(const T& item)>;

	/// <summary>
	/// Simple graph (.lib) extension for the grid components. Defines a gridLocator node.
	/// </summary>
	template<typename TNode, typename TEdge>
	concept isGridLocatorNode = isGraphNode<TNode> && isGridLocator<TNode>;

	/// <summary>
	/// Simple graph (.lib) extension for the grid components. Defines a gridLocator edge.
	/// </summary>
	template<typename TNode, typename TEdge>
	concept isGridLocatorEdge = isGraphEdge<TNode, TEdge> && isGridLocator<TNode>;

	/// <summary>
	/// Simple iteration callback. Set the return value to either continue / break.
	/// </summary>
	template <typename T>
	using gridSimpleCallback = std::function<iterationCallback(const T& item)>;

	/// <summary>
	/// Iteration callback that includes the indices. Set the return value to either continue / break.
	/// </summary>
	template <typename T>
	using gridCallback = std::function<iterationCallback(int column, int row, const T& current)>;

	/// <summary>
	/// Iteration callback that includes the indices. Set the return value to either continue / break. Adds the
	/// compass for adjacent cells.
	/// </summary>
	template <typename T>
	using gridCallbackAdjacent = std::function<iterationCallback(int column, int row, brogueCompass direction,
	                                                             const T& current)>;

	/// <summary>
	/// Simple predicate decision making function that includes the item coordinates
	/// </summary>
	template <typename T>
	using gridPredicate = std::function<bool(int column, int row, const T& current)>;

	/// <summary>
	/// Simple predicate decision making function
	/// </summary>
	template <typename T>
	using gridSimplePredicate = std::function<bool(const T& current)>;

	/// <summary>
	/// Searches grid for requested value based on comparison. Each truthy aggregateComparator result will store
	/// the next value as the going compare-ee. This compare-ee will be passed as the first argument of the comparator.
	/// The second argument will be the current grid value.
	/// </summary>
	template <typename T>
	using gridAggregateComparer = std::function<bool(T& seed, const T& currentItem)>;

	/// <summary>
	/// Defines a constructor (or getter) method for grid cells
	/// </summary>
	template <typename T>
	using gridElementConstructor = std::function<T(int column, int row)>;


	/// <summary>
	/// Selector of a value from the current item that includes the grid coordinates
	/// </summary>
	template <typename T, typename TResult>
	using gridSelector = std::function<TResult(int column, int row, const T& current)>;

	/// <summary>
	/// Definition of function to select a value from the grid type.
	/// </summary>
	template <typename T, typename TResult>
	using gridSimpleSelector = std::function<TResult(const T& current)>;
}
