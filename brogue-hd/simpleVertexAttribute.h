#pragma once

#include "gl.h"
#include "simple.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	/// <summary>
	/// Describes a single vertex attribute "in" parameter for a shader
	/// </summary>
	struct simpleVertexAttribute : simpleStruct
	{
	public:
		simpleVertexAttribute()
		{
			_index = 0;
			_name = default_value::value<simpleString>();
			_attributeType = 0;
		}
		simpleVertexAttribute(const simpleVertexAttribute& copy)
		{
			copyImpl(copy);
		}
		simpleVertexAttribute(GLuint index, const simpleString& name, int glActiveUniformType)
		{
			_index = index;
			_name = name;
			_attributeType = glActiveUniformType;

			// NO ENUM SUPPORT

			//glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
			//printf("Active Uniforms: %d\n", count);

			//for (i = 0; i < count; i++)
			//{
			//    glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);

			//    printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
			//}
		}

		GLuint getIndex() const
		{
			return _index;
		}
		simpleString getName() const
		{
			return _name;
		}
		GLenum getAttributeType() const
		{
			return _attributeType;
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(_index, _name, _attributeType);
		}

	private:

		void copyImpl(const simpleVertexAttribute& copy)
		{
			_index = copy.getIndex();
			_name = copy.getName();
			_attributeType = copy.getAttributeType();
		}

	private:

		GLuint _index;
		simpleString _name;

		/// <summary>
		/// Type of input data for the shader - this would be some sort of GLSL supported data type.
		/// </summary>
		GLenum _attributeType;
	};
}