#pragma once

#include <concepts>

#include "simple.h"
#include "simpleGlData.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
	template<typename T>
	concept isOpenGlUniform =	isHashable<T> &&
							   (std::same_as<T, int> ||
								std::same_as<T, float> ||
								std::same_as<T, vec2> ||
								std::same_as<T, vec3> ||
								std::same_as<T, vec4>);


	template<isOpenGlUniform T>
	struct simpleUniform : hashable
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


	/// <summary>
	/// Data structure to avoid template issues
	/// </summary>
	struct uniformData
	{
	public:

		uniformData(simpleString aname, GLenum atype, int avalue)
		{
			name = aname;
			type = atype;
			valueInt = avalue;
		}
		uniformData(simpleString aname, GLenum atype, float avalue)
		{
			name = aname;
			type = atype;
			valueFloat = avalue;
		}
		uniformData(simpleString aname, GLenum atype, vec2 avalue)
		{
			name = aname;
			type = atype;
			valueVec2 = avalue;
		}
		uniformData(simpleString aname, GLenum atype, vec4 avalue)
		{
			name = aname;
			type = atype;
			valueVec4 = avalue;
		}

		uniformData(const simpleUniform<int>& uniform)
		{
			name = uniform.name;
			type = uniform.type;
			valueInt = uniform.value;
		}

		uniformData(const simpleUniform<float>& uniform)
		{
			name = uniform.name;
			type = uniform.type;
			valueFloat = uniform.value;
		}

		uniformData(const simpleUniform<vec2>& uniform)
		{
			name = uniform.name;
			type = uniform.type;
			valueVec2 = uniform.value;
		}

		uniformData(const simpleUniform<vec4>& uniform)
		{
			name = uniform.name;
			type = uniform.type;
			valueVec4 = uniform.value;
		}

		uniformData(const uniformData& copy)
		{
			name = copy.name;
			type = copy.type;
			valueInt = copy.valueInt;
			valueFloat = copy.valueFloat;
			valueVec2 = copy.valueVec2;
			valueVec4 = copy.valueVec4;
		}

		simpleString name;
		GLenum type;

		int valueInt;
		float valueFloat;
		vec2 valueVec2;
		vec4 valueVec4;
	};
}

