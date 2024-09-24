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
		template<typename TResult>
		typedef std::function<TResult(T item)> simpleSelector;

		/// <summary>
		/// Defines a pair of items for use with a user callback
		/// </summary>
		typedef std::function<void(T item1, T item2)> pairs;
	};
	
	template<typename K, typename V, typename VResult>
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

		/// <summary>
		/// Definition of selector for the value type
		/// </summary>
		typedef std::function<VResult(V value)> valueSelector;
	};
}