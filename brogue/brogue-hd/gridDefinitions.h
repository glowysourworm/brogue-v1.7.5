#pragma once

#include "gridCell.h"
#include <array>
#include <functional>

using namespace std;

namespace brogueHd::backend::model::layout
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
	concept gridCellConstraint = requires(T a)
	{
		{ a } -> std::convertible_to<gridCell*>;
	};

	template<gridCellConstraint T>
	struct gridDelegates
	{
	public:

		/// <summary>
		/// Definition of function to provide feedback based on current grid (iteration) values
		/// </summary>
		/// <param name="column">callback (current) column</param>
		/// <param name="row">callback (current) row</param>
		/// <param name="value">callback (current) value</param>
		typedef std::function<bool(short column, short row, T value)> gridPredicate;

		/// <summary>
		/// Definition of function for standard iteration callbacks. Return value instructs the
		/// parent loop to either continue or break and return.
		/// </summary>
		typedef std::function<iterationCallback(short column, short row)> gridIterationCallback;

		/// <summary>
		/// Searches grid for requested value based on comparison. Each truthy aggregateComparator result will store
		/// the next value as the going compare-ee. This compare-ee will be passed as the first argument of the comparator.
		/// The second argument will be the current grid value.
		/// </summary>
		typedef std::function<bool(T aggregate, T value)> gridAggregateComparer;

		/// <summary>
		/// Definition of function to select a value from the grid cell type.
		/// </summary>
		template<typename TResult>
		typedef std::function<TResult(T)> gridSelector;

		/// <summary>
		/// Defines a constructor (or getter) method for grid cells
		/// </summary>
		typedef std::function<T(short, short)> gridCellConstructor;
	};
}