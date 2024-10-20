#pragma once

#include "simple.h"
#include "gridLocator.h"
#include <array>
#include <functional>

namespace brogueHd::component
{
	/// <summary>
	/// Defines directions for character movement
	/// </summary>
	enum brogueCompass
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
	template<typename T>
	concept isGridLocator = std::convertible_to<T, gridLocator>;

	/// <summary>
	/// Simple iteration callback. Set the return value to either continue / break.
	/// </summary>
	template<typename T>
	using gridSimpleCallback = std::function<iterationCallback(T)>;

	/// <summary>
	/// Iteration callback that includes the indices. Set the return value to either continue / break.
	/// </summary>
	template<typename T>
	using gridCallback = std::function<iterationCallback(short, short, T)>;

	/// <summary>
	/// Iteration callback that includes the indices. Set the return value to either continue / break. Adds the
	/// compass for adjacent cells.
	/// </summary>
	template<typename T>
	using gridCallbackAdjacent = std::function<iterationCallback(short, short, brogueCompass, T)>;

	/// <summary>
	/// Simple predicate decision making function that includes the item coordinates
	/// </summary>
	template<typename T>
	using gridPredicate = std::function<bool(short, short, T)>;

	/// <summary>
	/// Simple predicate decision making function
	/// </summary>
	template<typename T>
	using gridSimplePredicate = std::function<bool(T)>;

	/// <summary>
	/// Searches grid for requested value based on comparison. Each truthy aggregateComparator result will store
	/// the next value as the going compare-ee. This compare-ee will be passed as the first argument of the comparator.
	/// The second argument will be the current grid value.
	/// </summary>
	template<typename T>
	using gridAggregateComparer = std::function<bool(T, T)>;

	/// <summary>
	/// Defines a constructor (or getter) method for grid cells
	/// </summary>
	template<typename T>
	using gridElementConstructor = std::function<T(short, short)>;


	/// <summary>
	/// Selector of a value from the current item that includes the grid coordinates
	/// </summary>
	template<typename T, typename TResult>
	using gridSelector = std::function<TResult(short, short, T)>;

	/// <summary>
	/// Definition of function to select a value from the grid type.
	/// </summary>
	template<typename T, typename TResult>
	using gridSimpleSelectlr = std::function<TResult(T)>;

	/// <summary>
	/// Simple iteration callback. Set the return value to either continue / break.
	/// </summary>
	template<typename T>
	using gridIterator = std::function<iterationCallback(short, short)>;
}