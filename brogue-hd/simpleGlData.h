#pragma once

/*
	Had to write this because of missing opengl data types commonly used. These
	will work better with our backend:  vec2, vec4, ...
*/

#include "brogueGlobal.h"
#include "simpleDataStream.h"
#include "simpleArray.h"
#include "gl.h"
#include "typeConverter.h"

using namespace brogueHd::component;

namespace brogueHd::frontend::opengl
{
	struct simpleGlData
	{
	public:

		virtual float* getBuffer()
		{
			return NULL;
		}
		virtual int getElementSize(GLenum primitiveType)
		{
			return NULL;
		}
		virtual int getStreamSize()
		{
			return 0;
		}
		virtual void streamBuffer(simpleDataStream<float>& outputStream)
		{

		}
	};

	struct vec2
	{
		float x;
		float y;

		vec2(){}
		vec2(float ax, float ay)
		{
			x = ax;
			y = ay;
		}
	};

	struct vec4
	{
		float x;
		float y;
		float z;
		float w;

		vec4(){}
		vec4(float ax, float ay, float az, float aw)
		{
			x = ax;
			y = ay;
			z = az;
			w = aw;
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

		simpleQuad(){}
		simpleQuad(float aleft, float atop, float aright, float abottom)
		{
			left = aleft;
			top = atop;
			right = aright;
			bottom = abottom;
		}

		float* getBuffer()
		{
			// topLeft: [x, y], topRight: [x, x], ...
			float result[8] { left, top, right, top, right, bottom, left, bottom };

			return result;
		}

		int getElementSize(GLenum primitiveType) override
		{
			// These should probably be tested! :)
			switch (primitiveType)
			{
			case GL_QUADS:
				return 4;
			default:
				brogueException::show(simpleString("Unhandled primitive type for GLQuad:  ") + typeConverter::intToString(primitiveType));
			}
		}
		int getStreamSize() override
		{
			// 8 TOTAL FLOATS
			return 8;
		}

		void streamBuffer(simpleDataStream<float>& outputStream) override
		{
			outputStream.write(left);
			outputStream.write(top);
			outputStream.write(right);
			outputStream.write(top);
			outputStream.write(right);
			outputStream.write(bottom);
			outputStream.write(left);
			outputStream.write(bottom);
		}
	};

	template<typename T>
	struct uniformData
	{
		uniformData(){}
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
	};

	struct vertexAttributeData
	{
		vertexAttributeData()
		{
			index = -1;
			name = default_value<simpleString>::value;
			type = default_value<GLenum>::value;
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

	private:

		void copyImpl(const vertexAttributeData& copy)
		{
			index = copy.index;
			name = copy.name;
			type = copy.type;
		}
	};
}