#pragma once

#include <concepts>

#include "gl.h"
#include "gl.h"
#include "simple.h"
#include "simpleGlData.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	template<typename T>
	concept isOpenGlUniform = isHashable<T> &&
								(std::same_as<T, int> ||
								 std::same_as<T, float> ||
								 std::same_as<T, vec2> ||
								 std::same_as<T, ivec2> ||
								 std::same_as<T, vec3> ||
								 std::same_as<T, vec4>);


	template<isOpenGlUniform T>
	struct simpleUniform : simpleStruct
	{
	public:

		simpleUniform()
		{
			name = default_value::value<simpleString>();
			type = default_value::value<GLenum>();
			value = default_value::value<T>();
		}
		simpleUniform(const simpleUniform<T>& copy)
		{
			name = copy.name;
			type = copy.type;
			value = copy.value;
		}
		simpleUniform(const simpleString& aname, GLenum atype, T defaultValue)
		{
			name = aname;
			type = atype;
			value = defaultValue;
		}
		void operator=(const simpleUniform& copy)
		{
			name = copy.name;
			type = copy.type;
			value = copy.value;
		}

		simpleString name;
		GLenum type;

		/// <summary>
		/// NOTE:  Can also be the index of the texture! used for sampler2D
		/// </summary>
		T value;

		size_t getHash() const override
		{
			return hashGenerator::generateHash(name, type, value);
		}
	};
}

