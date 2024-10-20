#pragma once

#include "simpleGlData.h"
#include "brogueCellDisplay.h"

using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::opengl
{
	struct brogueColorQuad : simpleGlData
	{

	public:
		brogueColorQuad()
		{
			vertexCoordinates = default_value::value<simpleQuad>();
			backgroundColor = default_value::value<vec4>();
		}
		brogueColorQuad(const brogueColorQuad& copy)
		{
			copyImpl(copy);
		}
		brogueColorQuad(const brogueCellDisplay& cell,
						const simpleQuad& vertices)
		{
			// Consider translating these perhaps INTO the cell display - the view bounds.
			//
			backgroundColor = vec4(cell.backColor.red, cell.backColor.green, cell.backColor.blue, cell.backColor.alpha);

			vertexCoordinates = vertices;
		}
		brogueColorQuad(const color& theColor,
						const simpleQuad& vertices)
		{
			// Consider translating these perhaps INTO the cell display - the view bounds.
			//
			backgroundColor = vec4(theColor.red, theColor.green, theColor.blue, theColor.alpha);

			vertexCoordinates = vertices;
		}
		void operator=(const brogueColorQuad& copy)
		{
			copyImpl(copy);
		}

		int getElementSize(GLenum primitiveType) override
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
		int getStreamSize(GLenum primitiveType) override
		{
			// Total # of floats
			switch (primitiveType)
			{
			case GL_TRIANGLES:
				return 48;
			default:
				simpleException::show("Unhandled primitive type for GLQuad:  {}", primitiveType);
				break;
			}
		}
		void streamBuffer(GLenum primitiveType, simpleDataStream<float>& outputStream) override
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
				// Total Data:  6 floats per vertex * 6 vertices = 36 (floats)

				// Triangle 1: Top Left (32 floats total)
				vertexCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				// Triangle 1:  Top Right
				vertexCoordinates.topRight.streamBuffer(primitiveType, outputStream);			// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				// Triangle 1:  Bottom Right
				vertexCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				// Triangle 2: Top Left
				vertexCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				// Triangle 2:  Bottom Right
				vertexCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				// Triangle 2:  Bottom Left
				vertexCoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);			// vec2
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
			return hashGenerator::generateHash(backgroundColor, vertexCoordinates);
		}

	private:

		void copyImpl(const brogueColorQuad& copy)
		{
			backgroundColor = copy.backgroundColor;

			vertexCoordinates = copy.vertexCoordinates;
		}

	public:

		simpleQuad vertexCoordinates;

		vec4 backgroundColor;
	};
}