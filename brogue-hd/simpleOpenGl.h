#pragma once

#include <concepts>
#include "brogueView.h"
#include "brogueViewContainer.h"
#include "brogueListView.h"

namespace brogueHd::frontend::opengl
{
	template<typename T>
	concept isBrogueView = std::convertible_to<T, brogueView> ||
						   std::convertible_to<T, brogueViewContainer>;
}