#pragma once

#include <functional>

using namespace std;

namespace brogueHd::backend::extension
{
	/// <summary>
	/// Instructs the iterator to either continue or break out of its loop
	/// </summary>
	enum iterationCallback
	{
		iterate = 0,
		breakAndReturn = 1
	};

	// Constraint that the type T must inherit from gridOperator<T>
	template<typename T>
	concept comparable = requires(T a, T b)
	{
		{ a <= b } -> std::convertible_to<bool>;
		{ a < b }  -> std::convertible_to<bool>;
		{ a > b }  -> std::convertible_to<bool>;
		{ a >= b } -> std::convertible_to<bool>;
		{ a == b } -> std::convertible_to<bool>;
	};

	/// <summary>
	/// Definitions for most of the basic delegates in the extensions namespace
	/// </summary>
	template<typename T>
	struct extensionDelegates
	{
	public:

		/// <summary>
		/// Definition of function to provide callback: 1) user can return iterationCallback 
		/// value to either break, or continue the loop.
		/// </summary>
		/// <param name="value">callback (current) value</param>
		typedef std::function<iterationCallback(T item)> simpleCallback;

		/// <summary>
		/// Definition of simple predicate (decision making function) for most collection types
		/// </summary>
		typedef std::function<bool(T item)> simplePredicate;

		/// <summary>
		/// Definition of selector for a value V from an item T.
		/// </summary>
		template<typename V>
		typedef std::function<V(T item)> simpleSelector;
	};
	
	template<typename K, typename V>
	struct extensionMapDelegates
	{
		/// <summary>
		/// Definition of simple predicate for any key-value map
		/// </summary>
		typedef std::function<bool(K key, V value)> simplePredicate;

		/// <summary>
		/// Definition of function to provide callback: 1) user can return iterationCallback 
		/// value to either break, or continue the loop.
		/// </summary>
		typedef std::function<iterationCallback(K key, V value)> simpleCallback;
	};

	template<typename T>
	struct extensionArray2DDelegates
	{
	public:

		/// <summary>
		/// Definition of function to provide feedback based on current grid (iteration) values
		/// </summary>
		typedef std::function<bool(short column, short row, T value)> simplePredicate;

		/// <summary>
		/// Definition of function for standard iteration callbacks. Return value instructs the
		/// parent loop to either continue or break and return.
		/// </summary>
		typedef std::function<iterationCallback(short column, short row)> simpleCallback;

		/// <summary>
		/// Searches grid for requested value based on comparison. Each truthy aggregateComparator result will store
		/// the next value as the going compare-ee. This compare-ee will be passed as the first argument of the comparator.
		/// The second argument will be the current grid value.
		/// </summary>
		typedef std::function<bool(T aggregate, T value)> aggregateComparer;

		/// <summary>
		/// Definition of function to select a value from the grid cell type.
		/// </summary>
		template<typename TResult>
		typedef std::function<TResult(T item)> simpleSelector;

		/// <summary>
		/// Defines a constructor (or getter) method for grid cells
		/// </summary>
		typedef std::function<T(short, short)> simpleConstructor;
	};
}