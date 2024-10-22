#pragma once

#include "simpleGlData.h"
#include "brogueCellDisplay.h"

using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::opengl
{
	struct brogueCellQuad : simpleGlData
	{
		brogueCellQuad()
		{
			vertexCoordinates = default_value::value<simpleQuad>();
			textureCoordinates = default_value::value<simpleQuad>();
			backgroundColor = default_value::value<vec4>();
		}
		brogueCellQuad(const brogueCellQuad& copy)
		{
			copyImpl(copy);
		}
		brogueCellQuad(const brogueCellDisplay& cell, 
					   const simpleQuad& vertices, 
					   const simpleQuad& texture)
		{
			// Consider translating these perhaps INTO the cell display - the view bounds.
			//
			backgroundColor = vec4(cell.backColor.red, cell.backColor.green, cell.backColor.blue, cell.backColor.alpha);
			
			vertexCoordinates = vertices;
			textureCoordinates = texture;
		}
		void operator=(const brogueCellQuad& copy)
		{
			copyImpl(copy);
		}

		int getElementSize(GLenum primitiveType) const override
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
		void streamBuffer(GLenum primitiveType, simpleDataStream<float>* outputStream) const override
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
				// Data:		vertex, texture, color
				//
				// Total Data:  8 floats per vertex * 6 vertices = 48 (floats)

				// Triangle 1: Top Left (32 floats total)
				vertexCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec2
				textureCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				// Triangle 1:  Top Right
				vertexCoordinates.topRight.streamBuffer(primitiveType, outputStream);			// vec2
				textureCoordinates.topRight.streamBuffer(primitiveType, outputStream);			// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				// Triangle 1:  Bottom Right
				vertexCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec2
				textureCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				// Triangle 2: Top Left
				vertexCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec2
				textureCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				// Triangle 2:  Bottom Right
				vertexCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec2
				textureCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				// Triangle 2:  Bottom Left
				vertexCoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);			// vec2
				textureCoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);		// vec2
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
			return hashGenerator::generateHash(backgroundColor, vertexCoordinates, textureCoordinates);
		}

	private:

		void copyImpl(const brogueCellQuad& copy)
		{
			backgroundColor = copy.backgroundColor;
			
			vertexCoordinates = copy.vertexCoordinates;
			textureCoordinates = copy.textureCoordinates;
		}

	public:

		simpleQuad vertexCoordinates;
		simpleQuad textureCoordinates;

		vec4 backgroundColor;
	};
}