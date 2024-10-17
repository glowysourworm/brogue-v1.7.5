#pragma once

#include <array>
#include <concepts>
#include <type_traits>

#include "brogueCellDisplay.h"

using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model
{

	template<typename T>
	concept isBrogueDisplayCell = std::convertible_to<T, brogueCellDisplay>;
}