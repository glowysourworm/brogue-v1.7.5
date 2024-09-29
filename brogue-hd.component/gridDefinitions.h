#pragma once

#include "simple.h"
#include "gridLocator.h"
#include <array>
#include <functional>

using namespace std;

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
	concept isGridLocator = requires(T a)
	{
		{ a } -> std::convertible_to<gridLocator>;
	};

	/// <summary>
	/// Delegate definitions for most of the grid classes to share. This should
	/// facilitate iterators on these classes - apart from array2D, and std collections.
	/// </summary>
	template<typename T>
	struct gridDelegates
	{
		/// <summary>
		/// Simple iteration callback. Set the return value to either continue / break.
		/// </summary>
		typedef std::function<iterationCallback(T)> simpleCallback;

		/// <summary>
		/// Iteration callback that includes the indices. Set the return value to either continue / break.
		/// </summary>
		typedef std::function<iterationCallback(short, short, T)> callback;

		/// <summary>
		/// Simple predicate decision making function that includes the item coordinates
		/// </summary>
		typedef std::function<bool(short, short, T)> predicate;

		/// <summary>
		/// Simple predicate decision making function
		/// </summary>
		typedef std::function<bool(T)> simplePredicate;

		/// <summary>
		/// Searches grid for requested value based on comparison. Each truthy aggregateComparator result will store
		/// the next value as the going compare-ee. This compare-ee will be passed as the first argument of the comparator.
		/// The second argument will be the current grid value.
		/// </summary>
		typedef std::function<bool(T, T)> aggregateComparer;

		/// <summary>
		/// Defines a constructor (or getter) method for grid cells
		/// </summary>
		typedef std::function<T(short, short)> constructor;
	};

	template<typename T, typename TResult>
	struct gridSelectorDelegates
	{
		/// <summary>
		/// Selector of a value from the current item that includes the grid coordinates
		/// </summary>
		typedef std::function<TResult(short, short, T)> selector;

		/// <summary>
		/// Definition of function to select a value from the grid type.
		/// </summary>
		typedef std::function<TResult(T)> simpleSelector;
	};

	struct gridRectDelegates
	{
		/// <summary>
		/// Simple iteration callback. Set the return value to either continue / break.
		/// </summary>
		typedef std::function<iterationCallback(short, short)> callback;
	};
}