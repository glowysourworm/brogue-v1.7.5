#pragma once

#include "simpleGlData.h"

namespace brogueHd::frontend::opengl
{
	struct brogueValueQuad : simpleGlData
	{
	public:

		float left;
		float top;
		float right;
		float bottom;
		float value;

		brogueValueQuad()
		{
			left = 0;
			top = 0;
			right = 0;
			bottom = 0;
			value = 0;
		}
		brogueValueQuad(const simpleQuad& quad, float avalue)
		{
			left = quad.left;
			top = quad.top;
			right = quad.right;
			bottom = quad.bottom;
			value = avalue;
		}
		brogueValueQuad(const brogueValueQuad& copy)
		{
			left = copy.left;
			top = copy.top;
			right = copy.right;
			bottom = copy.bottom;
			value = copy.value;
		}
		brogueValueQuad(float aleft, float atop, float aright, float abottom, float avalue)
		{
			left = aleft;
			top = atop;
			right = aright;
			bottom = abottom;
			value = avalue;
		}

		int getElementSize(GLenum primitiveType) override
		{
			// 6 elements, each with 3 * sizeof(float) byte size
			switch (primitiveType)
			{
			case GL_TRIANGLES:
				return 6;
			default:
				simpleException::show("Unhandled primitive type for GLQuad:  {}", primitiveType);
				break;
			}
		}
		int getStreamSize(GLenum primitiveType) override
		{
			// Total # of floats: 6 Points * 3 Values per point
			switch (primitiveType)
			{
			case GL_TRIANGLES:
				return 18;
			default:
				simpleException::show("Unhandled primitive type for GLQuad:  {}", primitiveType);
				break;
			}

			return 0;
		}

		void streamBuffer(GLenum primitiveType, simpleDataStream<float>& outputStream) override
		{
			switch (primitiveType)
			{
			case GL_TRIANGLES:
			{	
				// (Triangle 1) topLeft, Value, topRight, Value, bottomRight, Value .. (Triangle 2) topLeft, Value, bottomRight, Value, bottomLeft

				outputStream.write(left);
				outputStream.write(top);

				outputStream.write(value);

				outputStream.write(right);
				outputStream.write(top);

				outputStream.write(value);

				outputStream.write(right);
				outputStream.write(bottom);

				outputStream.write(value);

				outputStream.write(left);
				outputStream.write(top);

				outputStream.write(value);

				outputStream.write(right);
				outputStream.write(bottom);

				outputStream.write(value);

				outputStream.write(left);
				outputStream.write(bottom);

				outputStream.write(value);
			}
			break;
			break;
			default:
				simpleException::show("Unhandled primitive type for GLQuad:  {}", primitiveType);
				break;
			}
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(left, top, right, bottom, value);
		}
	};
}
