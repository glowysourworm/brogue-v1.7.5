#pragma once

#include "gridLocator.h"
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
	concept isGridLocator = requires(T a)
	{
		{ a } -> std::convertible_to<gridLocator>;
	};
}