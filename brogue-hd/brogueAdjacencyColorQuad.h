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
	struct brogueAdjacencyColorQuad : simpleGlData
	{
		brogueAdjacencyColorQuad()
		{
			vertexCurrent = default_value::value<simpleQuad>();
			textureCurrent = default_value::value<simpleQuad>();
			textureS = default_value::value<simpleQuad>();
			textureSE = default_value::value<simpleQuad>();
			textureSW = default_value::value<simpleQuad>();
			backgroundColor = default_value::value<vec4>();
		}
		brogueAdjacencyColorQuad(const brogueAdjacencyColorQuad& copy)
		{
			copyImpl(copy);
		}
		brogueAdjacencyColorQuad(const brogueCellDisplay& cell,
								 const simpleQuad& vertexXY,
								 const simpleQuad& tex,
								 const simpleQuad& texS,
								 const simpleQuad& texSE,
								 const simpleQuad& texSW)
		{
			// Consider translating these perhaps INTO the cell display - the view bounds.
			//
			backgroundColor = vec4(cell.backColor.red, cell.backColor.green, cell.backColor.blue, cell.backColor.alpha);
			
			vertexCurrent = vertexXY;
			textureCurrent = tex;
			textureS = texS;
			textureSE = texSE;
			textureSW = texSW;
		}
		void operator=(const brogueAdjacencyColorQuad& copy)
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
					simpleException::show("Unhandled primitive type for GLQuad:  {}", primitiveType);
					break;
			}
		}
		int getStreamSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 84 * sizeof(float);
				default:
					simpleException::show("Unhandled primitive type for GLQuad:  {}", primitiveType);
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
					// Total Data:  14 floats per vertex * 6 vertices = 84 (floats)

					// Triangle 1: Top Left (32 floats total)
					vertexCurrent.topLeft.streamBuffer(primitiveType, outputStream);				// vec2
					textureCurrent.topLeft.streamBuffer(primitiveType, outputStream);				// vec2
					textureS.topLeft.streamBuffer(primitiveType, outputStream);						// vec2
					textureSE.topLeft.streamBuffer(primitiveType, outputStream);					// vec2
					textureSW.topLeft.streamBuffer(primitiveType, outputStream);					// vec2
					backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

					// Triangle 1:  Top Right
					vertexCurrent.topRight.streamBuffer(primitiveType, outputStream);				// vec2
					textureCurrent.topRight.streamBuffer(primitiveType, outputStream);				// vec2
					textureS.topRight.streamBuffer(primitiveType, outputStream);					// vec2
					textureSE.topRight.streamBuffer(primitiveType, outputStream);					// vec2
					textureSW.topRight.streamBuffer(primitiveType, outputStream);					// vec2
					backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

					// Triangle 1:  Bottom Right
					vertexCurrent.bottomRight.streamBuffer(primitiveType, outputStream);			// vec2
					textureCurrent.bottomRight.streamBuffer(primitiveType, outputStream);			// vec2
					textureS.bottomRight.streamBuffer(primitiveType, outputStream);					// vec2
					textureSE.bottomRight.streamBuffer(primitiveType, outputStream);				// vec2
					textureSW.bottomRight.streamBuffer(primitiveType, outputStream);				// vec2
					backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

					// Triangle 2: Top Left
					vertexCurrent.topLeft.streamBuffer(primitiveType, outputStream);				// vec2
					textureCurrent.topLeft.streamBuffer(primitiveType, outputStream);				// vec2
					textureS.topLeft.streamBuffer(primitiveType, outputStream);						// vec2
					textureSE.topLeft.streamBuffer(primitiveType, outputStream);					// vec2
					textureSW.topLeft.streamBuffer(primitiveType, outputStream);					// vec2
					backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

					// Triangle 2:  Bottom Right
					vertexCurrent.bottomRight.streamBuffer(primitiveType, outputStream);			// vec2
					textureCurrent.bottomRight.streamBuffer(primitiveType, outputStream);			// vec2
					textureS.bottomRight.streamBuffer(primitiveType, outputStream);					// vec2
					textureSE.bottomRight.streamBuffer(primitiveType, outputStream);				// vec2
					textureSW.bottomRight.streamBuffer(primitiveType, outputStream);				// vec2
					backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

					// Triangle 2:  Bottom Left
					vertexCurrent.bottomLeft.streamBuffer(primitiveType, outputStream);				// vec2
					textureCurrent.bottomLeft.streamBuffer(primitiveType, outputStream);			// vec2
					textureS.bottomLeft.streamBuffer(primitiveType, outputStream);					// vec2
					textureSE.bottomLeft.streamBuffer(primitiveType, outputStream);					// vec2
					textureSW.bottomLeft.streamBuffer(primitiveType, outputStream);					// vec2
					backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4
				}
				break;
				default:
					simpleException::show("Unhandled primitive type for GLQuad:  {}", primitiveType);
					break;
			}
		}

		size_t getHash() const override
		{
			// Wouldn't worry about hashing the whole data structure
			//
			return hashGenerator::generateHash(backgroundColor, vertexCurrent, textureCurrent, textureS, textureSE, textureSW);
		}

		const char* toString() const override
		{
			return textureCurrent.toString();
		}

	private:

		void copyImpl(const brogueAdjacencyColorQuad& copy)
		{
			vertexCurrent = copy.vertexCurrent;
			textureCurrent = copy.textureCurrent;
			textureS = copy.textureS;
			textureSE = copy.textureSE;
			textureSW = copy.textureSW;
			backgroundColor = copy.backgroundColor;
		}

	public:

		simpleQuad vertexCurrent;
		simpleQuad textureCurrent;
		simpleQuad textureS;
		simpleQuad textureSE;
		simpleQuad textureSW;

		vec4 backgroundColor;
	};
}