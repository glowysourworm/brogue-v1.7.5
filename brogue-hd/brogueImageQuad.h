#pragma once

#include "brogueQuad.h"
#include "gl.h"
#include "gridLocator.h"
#include "simpleDataStream.h"
#include "simpleException.h"
#include "simpleExt.h"
#include "simpleGlData.h"

namespace brogueHd::frontend
{
	struct brogueImageQuad : public brogueQuad
	{
		brogueImageQuad() {}
		brogueImageQuad(const brogueImageQuad& copy)
		{
			location = copy.location;
			vertexXY = copy.vertexXY;
			textureUV = copy.textureUV;
		}
		brogueImageQuad(const gridLocator& locator, const simpleQuad& avertexXY, const simpleQuad& atextureUV)
		{
			location = locator;
			vertexXY = avertexXY;
			textureUV = atextureUV;
		}

		int getElementVertexSize(GLenum primitiveType) const override
		{
			// Total # of calls to the shader
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 6;
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for brogueImageQuad:  {}", primitiveType));
			}
		}

		int getStreamSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return (24 * sizeof(float));
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for GLQuad:  {}", primitiveType));
					break;
			}
		}

		void streamBuffer(GLenum primitiveType, simpleDataStream* outputStream) const override
		{
			// (Triangle 1) topLeft, topRight, bottomRight

			// Top Left
			vertexXY.topLeft.streamBuffer(primitiveType, outputStream);        // vec2
			textureUV.topLeft.streamBuffer(primitiveType, outputStream);       // vec2

			// Top Right
			vertexXY.topRight.streamBuffer(primitiveType, outputStream);
			textureUV.topRight.streamBuffer(primitiveType, outputStream);

			// Bottom Left
			vertexXY.bottomRight.streamBuffer(primitiveType, outputStream);
			textureUV.bottomRight.streamBuffer(primitiveType, outputStream);

			// (Triangle 2) topLeft, bottomRight, bottomLeft

			// Bottom Left
			vertexXY.topLeft.streamBuffer(primitiveType, outputStream);
			textureUV.topLeft.streamBuffer(primitiveType, outputStream);

			// Top Right
			vertexXY.bottomRight.streamBuffer(primitiveType, outputStream);
			textureUV.bottomRight.streamBuffer(primitiveType, outputStream);

			// Bottom Right
			vertexXY.bottomLeft.streamBuffer(primitiveType, outputStream);
			textureUV.bottomLeft.streamBuffer(primitiveType, outputStream);
		}

		const char* toString() const override
		{
			return vertexXY.toString();
		}

	public:

		simpleQuad vertexXY;
		simpleQuad textureUV;
	};
}