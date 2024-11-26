#pragma once

#include "brogueCellQuad.h"
#include "brogueColorQuad.h"
#include "brogueImageQuad.h"
#include "brogueLine.h"
#include "simple.h"
#include "simpleGlData.h"
#include <concepts>

namespace brogueHd::frontend
{
	template<typename T>
	concept isGLUniform = std::same_as<T, int> ||
		std::same_as<T, float> ||
		std::same_as<T, vec2> ||
		std::same_as<T, vec3> ||
		std::same_as<T, ivec2> ||
		std::same_as<T, vec4>;

	template<typename T>
	concept isGLStream = isHashable<T> &&
		(std::same_as<T, brogueCellQuad> ||
		 std::same_as<T, brogueColorQuad> ||
		 std::same_as<T, brogueImageQuad> ||
		 std::same_as<T, brogueLine>);
}