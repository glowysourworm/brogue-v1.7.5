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
			backgroundColor = default_value::value<vec3>();
			topLeft = default_value::value<vec2>();
			topRight = default_value::value<vec2>();
			bottomLeft = default_value::value<vec2>();
			bottomRight = default_value::value<vec2>();
		}
		brogueCellQuad(const brogueCellQuad& copy)
		{
			copyImpl(copy);
		}
		brogueCellQuad(const brogueCellDisplay& cell, const simpleQuad& quad)
		{
			// Consider translating these perhaps INTO the cell display - the view bounds.
			//
			backgroundColor = vec3(cell.backColor.red, cell.backColor.green, cell.backColor.blue);
			topLeft = quad.topLeft;
			topRight = vec2(quad.bottomRight.x, quad.topLeft.y);
			bottomLeft = vec2(quad.topLeft.x, quad.bottomRight.y);
			bottomRight = quad.bottomRight;
		}
		void operator=(const brogueCellQuad& copy)
		{
			copyImpl(copy);
		}

		int getElementSize(GLenum primitiveType) override
		{
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
				return 12;
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
				// Interleve Point-Color values. Color is a vec3. Point is a vec2.

				// (Triangle 1) topLeft, color, topRight, color, bottomRight, color 

				topLeft.streamBuffer(primitiveType, outputStream);
				//backgroundColor.streamBuffer(primitiveType, outputStream);

				topRight.streamBuffer(primitiveType, outputStream);
				//backgroundColor.streamBuffer(primitiveType, outputStream);

				bottomRight.streamBuffer(primitiveType, outputStream);
				//backgroundColor.streamBuffer(primitiveType, outputStream);

				// (Triangle 2) topLeft, color, bottomRight, color, bottomLeft color

				topLeft.streamBuffer(primitiveType, outputStream);
				//backgroundColor.streamBuffer(primitiveType, outputStream);

				bottomRight.streamBuffer(primitiveType, outputStream);
				//backgroundColor.streamBuffer(primitiveType, outputStream);

				bottomLeft.streamBuffer(primitiveType, outputStream);
				//backgroundColor.streamBuffer(primitiveType, outputStream);
			}
			break;
			default:
				simpleException::show("Unhandled primitive type for GLQuad:  {}", primitiveType);
				break;
			}
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(backgroundColor, topLeft, topRight, bottomLeft, bottomRight);
		}

	private:

		void copyImpl(const brogueCellQuad& copy)
		{
			backgroundColor = copy.backgroundColor;
			topLeft = copy.topLeft;
			topRight = copy.topRight;
			bottomLeft = copy.bottomLeft;
			bottomRight = copy.bottomRight;
		}

	public:

		vec3 backgroundColor;
		vec2 topLeft;
		vec2 topRight;
		vec2 bottomLeft;
		vec2 bottomRight;
	};
}