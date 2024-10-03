#pragma once

#include <concepts>

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

	template<typename T>
	concept isNumber = requires(T a)
	{
		std::same_as<T, double>;
		std::same_as<T, float>;
		std::same_as<T, unsigned long>;
		std::same_as<T, long>;
		std::same_as<T, int>;
		std::same_as<T, short>;
		std::same_as<T, bool>;
	};
}