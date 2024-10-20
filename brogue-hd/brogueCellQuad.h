#pragma once

#include "simpleGlData.h"
#include "brogueCellDisplay.h"

using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::opengl
{
	struct brogueCellQuad : simpleGlData
	{

	public:
		brogueCellQuad()
		{
			vertexCoordinates = default_value::value<simpleQuad>();
			textureCoordinates = default_value::value<simpleQuad>();
			backgroundColor = default_value::value<vec4>();

			textureNCoordinates = default_value::value<simpleQuad3>();
			textureSCoordinates = default_value::value<simpleQuad3>();
			textureECoordinates = default_value::value<simpleQuad3>();
			textureWCoordinates = default_value::value<simpleQuad3>();

			textureNECoordinates = default_value::value<simpleQuad3>();
			textureNWCoordinates = default_value::value<simpleQuad3>();
			textureSECoordinates = default_value::value<simpleQuad3>();
			textureSWCoordinates = default_value::value<simpleQuad3>();
		}
		brogueCellQuad(const brogueCellQuad& copy)
		{
			copyImpl(copy);
		}
		brogueCellQuad(const brogueCellDisplay& cell, 
					   const simpleQuad& vertices, 
					   const simpleQuad& texture,
					   const simpleQuad3& textureN,
					   const simpleQuad3& textureS,
					   const simpleQuad3& textureE,
					   const simpleQuad3& textureW,
					   const simpleQuad3& textureNE,
					   const simpleQuad3& textureNW,
					   const simpleQuad3& textureSE,
					   const simpleQuad3& textureSW)
		{
			// Consider translating these perhaps INTO the cell display - the view bounds.
			//
			backgroundColor = vec4(cell.backColor.red, cell.backColor.green, cell.backColor.blue, cell.backColor.alpha);
			
			vertexCoordinates = vertices;
			textureCoordinates = texture;

			textureNCoordinates = textureN;
			textureSCoordinates = textureS;
			textureECoordinates = textureE;
			textureWCoordinates = textureW;

			textureNECoordinates = textureNE;
			textureNWCoordinates = textureNW;
			textureSECoordinates = textureSE;
			textureSWCoordinates = textureSW;
		}
		void operator=(const brogueCellQuad& copy)
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
				return 192;
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
				// Data:		vertex, texture, color, textureN, ... , textureSW
				//
				// Total Data:  32 floats per vertex * 6 vertices = 192 (floats)

				// Triangle 1: Top Left (32 floats total)
				vertexCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec2
				textureCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				textureNCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3
				textureSCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3
				textureECoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3
				textureWCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3

				textureNECoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3
				textureNWCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3
				textureSECoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3
				textureSWCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3

				// Triangle 1:  Top Right
				vertexCoordinates.topRight.streamBuffer(primitiveType, outputStream);			// vec2
				textureCoordinates.topRight.streamBuffer(primitiveType, outputStream);			// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				textureNCoordinates.topRight.streamBuffer(primitiveType, outputStream);			// vec3
				textureSCoordinates.topRight.streamBuffer(primitiveType, outputStream);			// vec3
				textureECoordinates.topRight.streamBuffer(primitiveType, outputStream);			// vec3
				textureWCoordinates.topRight.streamBuffer(primitiveType, outputStream);			// vec3

				textureNECoordinates.topRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureNWCoordinates.topRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureSECoordinates.topRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureSWCoordinates.topRight.streamBuffer(primitiveType, outputStream);		// vec3

				// Triangle 1:  Bottom Right
				vertexCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec2
				textureCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				textureNCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureSCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureECoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureWCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3

				textureNECoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureNWCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureSECoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureSWCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3

				// Triangle 2: Top Left
				vertexCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec2
				textureCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				textureNCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3
				textureSCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3
				textureECoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3
				textureWCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3

				textureNECoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3
				textureNWCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3
				textureSECoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3
				textureSWCoordinates.topLeft.streamBuffer(primitiveType, outputStream);			// vec3

				// Triangle 2:  Bottom Right
				vertexCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec2
				textureCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				textureNCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureSCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureECoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureWCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3

				textureNECoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureNWCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureSECoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3
				textureSWCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);		// vec3

				// Triangle 2:  Bottom Left
				vertexCoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);			// vec2
				textureCoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);		// vec2
				backgroundColor.streamBuffer(primitiveType, outputStream);						// vec4

				textureNCoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);		// vec3
				textureSCoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);		// vec3
				textureECoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);		// vec3
				textureWCoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);		// vec3

				textureNECoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);		// vec3
				textureNWCoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);		// vec3
				textureSECoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);		// vec3
				textureSWCoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);		// vec3
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

			textureNCoordinates = copy.textureNCoordinates;
			textureSCoordinates = copy.textureSCoordinates;
			textureECoordinates = copy.textureECoordinates;
			textureWCoordinates = copy.textureWCoordinates;

			textureNECoordinates = copy.textureNECoordinates;
			textureNWCoordinates = copy.textureNWCoordinates;
			textureSECoordinates = copy.textureSECoordinates;
			textureSWCoordinates = copy.textureSWCoordinates;
		}

	public:

		simpleQuad vertexCoordinates;
		simpleQuad textureCoordinates;

		vec4 backgroundColor;

		simpleQuad3 textureNCoordinates;
		simpleQuad3 textureSCoordinates;
		simpleQuad3 textureECoordinates;
		simpleQuad3 textureWCoordinates;

		simpleQuad3 textureNECoordinates;
		simpleQuad3 textureNWCoordinates;
		simpleQuad3 textureSECoordinates;
		simpleQuad3 textureSWCoordinates;
	};
}