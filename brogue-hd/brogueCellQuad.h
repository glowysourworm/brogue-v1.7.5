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
		brogueCellQuad(const brogueCellDisplay& cell)
		{
			// Consider translating these perhaps INTO the cell display - the view bounds.
			//
			backgroundColor = vec3(cell.backColor.red, cell.backColor.green, cell.backColor.blue);
			topLeft = vec2(cell.column * brogueCellDisplay::CellWidth, cell.row * brogueCellDisplay::CellHeight);
			topRight = vec2((cell.column + 1) * brogueCellDisplay::CellWidth, cell.row * brogueCellDisplay::CellHeight);
			bottomLeft = vec2(cell.column * brogueCellDisplay::CellWidth, (cell.row + 1) * brogueCellDisplay::CellHeight);
			bottomRight = vec2((cell.column + 1) * brogueCellDisplay::CellWidth, (cell.row + 1) * brogueCellDisplay::CellHeight);
		}

		int getElementSize(GLenum primitiveType) override
		{
			// These should probably be tested! :)
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
				return 30;
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
				backgroundColor.streamBuffer(primitiveType, outputStream);

				topRight.streamBuffer(primitiveType, outputStream);
				backgroundColor.streamBuffer(primitiveType, outputStream);

				bottomRight.streamBuffer(primitiveType, outputStream);
				backgroundColor.streamBuffer(primitiveType, outputStream);

				// (Triangle 2) topLeft, color, bottomRight, color, bottomLeft color

				topLeft.streamBuffer(primitiveType, outputStream);
				backgroundColor.streamBuffer(primitiveType, outputStream);

				bottomRight.streamBuffer(primitiveType, outputStream);
				backgroundColor.streamBuffer(primitiveType, outputStream);

				bottomLeft.streamBuffer(primitiveType, outputStream);
				backgroundColor.streamBuffer(primitiveType, outputStream);
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

	public:

		vec3 backgroundColor;
		vec2 topLeft;
		vec2 topRight;
		vec2 bottomLeft;
		vec2 bottomRight;
	};
}