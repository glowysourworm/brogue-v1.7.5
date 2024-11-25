#pragma once

#include "brogueCellDisplay.h"
#include "brogueUIConstants.h"
#include "brogueQuad.h"
#include "gl.h"
#include "gridLocator.h"
#include "simple.h"
#include "simpleDataStream.h"
#include "simpleException.h"
#include "simpleGlData.h"

using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	struct brogueCellQuad : public brogueQuad
	{
		brogueCellQuad()
		{
			location = default_value::value<gridLocator>();
			vertexXY = default_value::value<simpleQuad>();
			textureUV = default_value::value<simpleQuad>();
			glyphUV = default_value::value<simpleQuad>();
			foregroundColor = default_value::value<vec4>();
			backgroundColor = default_value::value<vec4>();
		}
		brogueCellQuad(const brogueCellQuad& copy)
		{
			copyImpl(copy);
		}
		brogueCellQuad(const brogueCellDisplay& cell,
						const simpleQuad& averticesXY,
						const simpleQuad& atextureUV,
						const simpleQuad& aglyphUV,
						openglBrogueCellOutputSelector aoutputSelector)
		{
			// Cache the location data
			location.column = cell.column;
			location.row = cell.row;

			// Consider translating these perhaps INTO the cell display - the view bounds.
			//
			backgroundColor = vec4(cell.backColor.red, cell.backColor.green, cell.backColor.blue, cell.backColor.alpha);
			foregroundColor = vec4(cell.foreColor.red, cell.foreColor.green, cell.foreColor.blue, cell.foreColor.alpha);

			vertexXY = averticesXY;
			textureUV = atextureUV;
			glyphUV = aglyphUV;
			outputSelector = aoutputSelector;
		}
		void operator=(const brogueCellQuad& copy)
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
			// Total # of float / int values. (not bytes!) (assert(sizeof(int) == sizeof(float)))
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 90 * sizeof(float);
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
					// Data:		vertexXY, textureUV, textureUI, color, outputSelector
					//
					// Total Data:  15 floats per vertex * 6 vertices = 90 (floats)

					// Triangle 1: Top Left (32 floats total)
					vertexXY.topLeft.streamBuffer(primitiveType, outputStream);				// vec2
					textureUV.topLeft.streamBuffer(primitiveType, outputStream);			// vec2
					glyphUV.topLeft.streamBuffer(primitiveType, outputStream);				// vec2
					foregroundColor.streamBuffer(primitiveType, outputStream);				// vec4
					backgroundColor.streamBuffer(primitiveType, outputStream);				// vec4
					outputStream->writeFloat((float)outputSelector);						// float

					// Triangle 1:  Top Right
					vertexXY.topRight.streamBuffer(primitiveType, outputStream);			// vec2
					textureUV.topRight.streamBuffer(primitiveType, outputStream);			// vec2
					glyphUV.topRight.streamBuffer(primitiveType, outputStream);				// vec2
					foregroundColor.streamBuffer(primitiveType, outputStream);				// vec4
					backgroundColor.streamBuffer(primitiveType, outputStream);				// vec4
					outputStream->writeFloat((float)outputSelector);						// float

					// Triangle 1:  Bottom Right
					vertexXY.bottomRight.streamBuffer(primitiveType, outputStream);			// vec2
					textureUV.bottomRight.streamBuffer(primitiveType, outputStream);		// vec2
					glyphUV.bottomRight.streamBuffer(primitiveType, outputStream);			// vec2
					foregroundColor.streamBuffer(primitiveType, outputStream);				// vec4
					backgroundColor.streamBuffer(primitiveType, outputStream);				// vec4
					outputStream->writeFloat((float)outputSelector);						// float

					// Triangle 2: Top Left
					vertexXY.topLeft.streamBuffer(primitiveType, outputStream);				// vec2
					textureUV.topLeft.streamBuffer(primitiveType, outputStream);			// vec2
					glyphUV.topLeft.streamBuffer(primitiveType, outputStream);				// vec2
					foregroundColor.streamBuffer(primitiveType, outputStream);				// vec4
					backgroundColor.streamBuffer(primitiveType, outputStream);				// vec4
					outputStream->writeFloat((float)outputSelector);						// float

					// Triangle 2:  Bottom Right
					vertexXY.bottomRight.streamBuffer(primitiveType, outputStream);			// vec2
					textureUV.bottomRight.streamBuffer(primitiveType, outputStream);		// vec2
					glyphUV.bottomRight.streamBuffer(primitiveType, outputStream);			// vec2
					foregroundColor.streamBuffer(primitiveType, outputStream);				// vec4
					backgroundColor.streamBuffer(primitiveType, outputStream);				// vec4
					outputStream->writeFloat((float)outputSelector);						// float

					// Triangle 2:  Bottom Left
					vertexXY.bottomLeft.streamBuffer(primitiveType, outputStream);			// vec2
					textureUV.bottomLeft.streamBuffer(primitiveType, outputStream);			// vec2
					glyphUV.bottomLeft.streamBuffer(primitiveType, outputStream);			// vec2
					foregroundColor.streamBuffer(primitiveType, outputStream);				// vec4
					backgroundColor.streamBuffer(primitiveType, outputStream);				// vec4
					outputStream->writeFloat((float)outputSelector);						// float
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
			return hashGenerator::generateHash(backgroundColor, vertexXY, textureUV);
		}

		const char* toString() const override
		{
			return vertexXY.toString();
		}

	private:

		void copyImpl(const brogueCellQuad& copy)
		{
			location = copy.location;

			backgroundColor = copy.backgroundColor;
			foregroundColor = copy.foregroundColor;
			glyphUV = copy.glyphUV;

			vertexXY = copy.vertexXY;
			textureUV = copy.textureUV;
			outputSelector = copy.outputSelector;
		}

	public:

		simpleQuad vertexXY;
		simpleQuad textureUV;

		simpleQuad glyphUV;

		vec4 backgroundColor;
		vec4 foregroundColor;
		openglBrogueCellOutputSelector outputSelector;
	};
}