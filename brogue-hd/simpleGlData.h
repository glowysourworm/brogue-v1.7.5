#pragma once

/*
	Had to write this because of missing opengl data types commonly used. These
	will work better with our backend:  vec2, vec4, ...
*/

#include "simple.h"
#include "simpleString.h"
#include "simpleDataStream.h"
#include "simpleArray.h"
#include "gl.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
	struct simpleGlData : hashable
	{
	public:

		virtual int getElementSize(GLenum primitiveType)
		{
			return NULL;
		}
		virtual int getStreamSize(GLenum primitiveType)
		{
			return 0;
		}
		virtual void streamBuffer(GLenum primitiveType, simpleDataStream<float>& outputStream)
		{

		}

		virtual size_t getHash() const override
		{
			return 0;
		}
	};

	struct vec2 : hashable
	{
		float x;
		float y;

		vec2()
		{
			x = -1;
			y = -1;
		}
		vec2(const vec2& copy)
		{
			x = copy.x;
			y = copy.y;
		}
		vec2(float ax, float ay)
		{
			x = ax;
			y = ay;
		}
		void operator=(const vec2& other)
		{
			x = other.x;
			y = other.y;
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(x, y);
		}
	};

	struct vec4 : hashable
	{
		float x;
		float y;
		float z;
		float w;

		vec4()
		{
			x = -1;
			y = -1;
			z = -1;
			w = -1;
		}
		vec4(const vec4& copy)
		{
			x = copy.x;
			y = copy.y;
			w = copy.w;
			z = copy.z;
		}
		vec4(float ax, float ay, float az, float aw)
		{
			x = ax;
			y = ay;
			z = az;
			w = aw;
		}
		void operator=(const vec4& other)
		{
			x = other.x;
			y = other.y;
			w = other.w;
			z = other.z;
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(x, y, z, w);
		}
	};

	struct simpleQuad : simpleGlData
	{
		// Memory Layout:  { top-left, top-right, bottom-right, bottom-left }

	public:

		float left;
		float top;
		float right;
		float bottom;
		GLenum primitiveType;

		simpleQuad()
		{
			primitiveType = GL_TRIANGLES;
			left = 0;
			top = 0;
			right = 0;
			bottom = 0;
		}
		simpleQuad(const simpleQuad& copy)
		{
			primitiveType = copy.primitiveType;
			left = copy.left;
			top = copy.top;
			right = copy.right;
			bottom = copy.bottom;
		}
		simpleQuad(GLenum aprimitiveType, float aleft, float atop, float aright, float abottom)
		{
			primitiveType = aprimitiveType;
			left = aleft;
			top = atop;
			right = aright;
			bottom = abottom;
		}

		int getElementSize(GLenum primitiveType) override
		{
			// These should probably be tested! :)
			switch (primitiveType)
			{
			case GL_QUADS:
				return 4;
			case GL_TRIANGLES:
				return 6;
			default:
				simpleException::show("Unhandled primitive type for GLQuad:  {}", primitiveType);
				break;
			}
		}
		int getStreamSize(GLenum primitiveType) override
		{
			// Total # of floats
			switch (primitiveType)
			{
			case GL_QUADS:
				return 8;
			case GL_TRIANGLES:
				return 12;
			default:
				simpleException::show("Unhandled primitive type for GLQuad:  {}", primitiveType);
				break;
			}
		}

		void streamBuffer(GLenum primitiveType, simpleDataStream<float>& outputStream) override
		{
			// These should probably be tested! :)
			switch (primitiveType)
			{
			case GL_QUADS:
			{
				// topLeft: [x, y], topRight: [x, y], ...
				outputStream.write(left);
				outputStream.write(top);
				outputStream.write(right);
				outputStream.write(top);
				outputStream.write(right);
				outputStream.write(bottom);
				outputStream.write(left);
				outputStream.write(bottom);
			}
			break;
			case GL_TRIANGLES:
			{
				// (Triangle 1) topLeft, topRight, bottomRight, (Triangle 2) topLeft, bottomRight, bottomLeft

				outputStream.write(left);
				outputStream.write(top);
				outputStream.write(right);
				outputStream.write(top);
				outputStream.write(right);
				outputStream.write(bottom);

				outputStream.write(left);
				outputStream.write(top);
				outputStream.write(right);
				outputStream.write(bottom);
				outputStream.write(left);
				outputStream.write(bottom);
			}
			break;
			default:
				simpleException::show("Unhandled primitive type for GLQuad:  {}", primitiveType);
				break;
			}
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(left, top, right, bottom);
		}
	};

	template<isHashable T>
	struct uniformData : hashable
	{
		uniformData()
		{
			name = default_value::value<simpleString>();
			type = default_value::value<GLenum>();
			value = default_value::value<T>();
		}
		uniformData(const uniformData<T>& copy)
		{
			name = copy.name;
			type = copy.type;
			value = copy.value;
		}
		uniformData(const simpleString& aname, GLenum atype)
		{
			name = aname;
			type = atype;
			value = default_value<T>::value;
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

	struct vertexAttributeData : hashable
	{
		vertexAttributeData()
		{
			index = -1;
			name = default_value::value<simpleString>();
			type = default_value::value<GLenum>();
		}
		vertexAttributeData(const vertexAttributeData& copy)
		{
			copyImpl(copy);
		}
		vertexAttributeData(int aindex, const simpleString& aname, GLenum atype)
		{
			index = aindex;
			name = aname;
			type = atype;
		}
		~vertexAttributeData()
		{

		}

		void operator=(const vertexAttributeData& copy)
		{
			copyImpl(copy);
		}

		int index;
		simpleString name;

		/// <summary>
		/// Type of input data for the shader - this would be some sort of GLSL supported data type.
		/// </summary>
		GLenum type;

		size_t getHash() const override
		{
			return hashGenerator::generateHash(name, type, index);
		}

	private:

		void copyImpl(const vertexAttributeData& copy)
		{
			index = copy.index;
			name = copy.name;
			type = copy.type;
		}
	};
}