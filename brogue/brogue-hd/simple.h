#pragma once

namespace brogueHd::component
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
	concept isComparable = requires(T a, T b)
	{
		{ a <= b } -> std::convertible_to<bool>;
		{ a < b }  -> std::convertible_to<bool>;
		{ a > b }  -> std::convertible_to<bool>;
		{ a >= b } -> std::convertible_to<bool>;
		{ a == b } -> std::convertible_to<bool>;
	};
}