#pragma once

#include "brogueCellDisplay.h"
#include "color.h"
#include "gl.h"
#include "simple.h"
#include "simpleDataStream.h"
#include "simpleException.h"
#include "simpleGlData.h"

using namespace brogueHd::simple;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	struct brogueFlameQuad : simpleGlData
	{
		brogueFlameQuad()
		{
			vertexCurrent = default_value::value<simpleQuad>();
			textureCurrent = default_value::value<simpleQuad>();
			backgroundColor1 = default_value::value<vec4>();
			backgroundColor2 = default_value::value<vec4>();
			backgroundColor3 = default_value::value<vec4>();
		}
		brogueFlameQuad(const brogueFlameQuad& copy)
		{
			copyImpl(copy);
		}
		brogueFlameQuad(const color& color1,
						const color& color2,
						const color& color3,
						const simpleQuad& vertex,
						const simpleQuad& texture)
		{
			// Consider translating these perhaps INTO the cell display - the view bounds.
			//
			backgroundColor1 = vec4(color1.red, color1.green, color1.blue, color1.alpha);
			backgroundColor2 = vec4(color2.red, color2.green, color2.blue, color2.alpha);
			backgroundColor3 = vec4(color3.red, color3.green, color3.blue, color3.alpha);
			
			vertexCurrent = vertex;
			textureCurrent = texture;
		}
		void operator=(const brogueFlameQuad& copy)
		{
			copyImpl(copy);
		}

		int getElementVertexSize(GLenum primitiveType) const override
		{
			// Total # of calls to the shader
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 6;
				default:
					throw simpleException("Unhandled primitive type for GLQuad:  {}", primitiveType);
					break;
			}
		}
		int getStreamSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 96 * sizeof(float);
				default:
					throw simpleException("Unhandled primitive type for GLQuad:  {}", primitiveType);
					break;
			}
		}
		void streamBuffer(GLenum primitiveType, simpleDataStream* outputStream) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
				{
					// Need to map out the vertex shader's "layout" parameters. Each shader call is 
					// to the current vertex. So, for a triangle pair, each of the values must go out
					// for each vertex - including the adjacent texture coordinates.
					//

					// Triangle 1:  top-left, top-right, bottom-right
					// Triangle 2:  top-left, bottom-right, bottom-left
					//
					// Data:		vertex, texture
					//
					// Total Data:  16 floats per vertex * 6 vertices = 96 (floats)

					// Triangle 1: Top Left (32 floats total)
					vertexCurrent.topLeft.streamBuffer(primitiveType, outputStream);				// vec2
					textureCurrent.topLeft.streamBuffer(primitiveType, outputStream);				// vec2
					backgroundColor1.streamBuffer(primitiveType, outputStream);						// vec4
					backgroundColor2.streamBuffer(primitiveType, outputStream);						// vec4
					backgroundColor3.streamBuffer(primitiveType, outputStream);						// vec4

					// Triangle 1:  Top Right
					vertexCurrent.topRight.streamBuffer(primitiveType, outputStream);				// vec2
					textureCurrent.topRight.streamBuffer(primitiveType, outputStream);				// vec2
					backgroundColor1.streamBuffer(primitiveType, outputStream);						// vec4
					backgroundColor2.streamBuffer(primitiveType, outputStream);						// vec4
					backgroundColor3.streamBuffer(primitiveType, outputStream);						// vec4

					// Triangle 1:  Bottom Right
					vertexCurrent.bottomRight.streamBuffer(primitiveType, outputStream);			// vec2
					textureCurrent.bottomRight.streamBuffer(primitiveType, outputStream);			// vec2
					backgroundColor1.streamBuffer(primitiveType, outputStream);						// vec4
					backgroundColor2.streamBuffer(primitiveType, outputStream);						// vec4
					backgroundColor3.streamBuffer(primitiveType, outputStream);						// vec4

					// Triangle 2: Top Left
					vertexCurrent.topLeft.streamBuffer(primitiveType, outputStream);				// vec2
					textureCurrent.topLeft.streamBuffer(primitiveType, outputStream);				// vec2
					backgroundColor1.streamBuffer(primitiveType, outputStream);						// vec4
					backgroundColor2.streamBuffer(primitiveType, outputStream);						// vec4
					backgroundColor3.streamBuffer(primitiveType, outputStream);						// vec4

					// Triangle 2:  Bottom Right
					vertexCurrent.bottomRight.streamBuffer(primitiveType, outputStream);			// vec2
					textureCurrent.bottomRight.streamBuffer(primitiveType, outputStream);			// vec2
					backgroundColor1.streamBuffer(primitiveType, outputStream);						// vec4
					backgroundColor2.streamBuffer(primitiveType, outputStream);						// vec4
					backgroundColor3.streamBuffer(primitiveType, outputStream);						// vec4

					// Triangle 2:  Bottom Left
					vertexCurrent.bottomLeft.streamBuffer(primitiveType, outputStream);				// vec2
					textureCurrent.bottomLeft.streamBuffer(primitiveType, outputStream);			// vec2
					backgroundColor1.streamBuffer(primitiveType, outputStream);						// vec4
					backgroundColor2.streamBuffer(primitiveType, outputStream);						// vec4
					backgroundColor3.streamBuffer(primitiveType, outputStream);						// vec4
				}
				break;
				default:
					throw simpleException("Unhandled primitive type for GLQuad:  {}", primitiveType);
					break;
			}
		}

		size_t getHash() const override
		{
			// Wouldn't worry about hashing the whole data structure
			//
			return hashGenerator::generateHash(vertexCurrent, textureCurrent, backgroundColor1, backgroundColor2, backgroundColor3);
		}

		const char* toString() const override
		{
			return textureCurrent.toString();
		}

	private:

		void copyImpl(const brogueFlameQuad& copy)
		{
			vertexCurrent = copy.vertexCurrent;
			textureCurrent = copy.textureCurrent;
			backgroundColor1 = copy.backgroundColor1;
			backgroundColor2 = copy.backgroundColor2;
			backgroundColor3 = copy.backgroundColor3;
		}

	public:

		simpleQuad vertexCurrent;
		simpleQuad textureCurrent;

		vec4 backgroundColor1;
		vec4 backgroundColor2;
		vec4 backgroundColor3;
	};
}