#pragma once

#include "simpleGlData.h"

namespace brogueHd::frontend::opengl
{
	struct brogueImageQuad : simpleGlData
	{
    public:

        brogueImageQuad(const simpleQuad& vertexCoords, const simpleQuad& textureCoords)
        {
            vertexCoordinates = vertexCoords;
            textureCoordinates = textureCoords;
        }

        int getElementSize(GLenum primitiveType) override
        {
            // These should probably be tested! :)
            switch (primitiveType)
            {
            case GL_TRIANGLES:
                return 6;
            default:
                simpleException::show("Unhandled primitive type for brogueImageQuad:  {}", primitiveType);
            }
        }

        int getStreamSize(GLenum primitiveType) override
        {
            // 24 TOTAL FLOATS
            return 24;
        }

        void streamBuffer(GLenum primitiveType, simpleDataStream<float>& outputStream) override
        {
            // Top Left
            outputStream.write(vertexCoordinates.left);
            outputStream.write(vertexCoordinates.top);
            outputStream.write(textureCoordinates.left);
            outputStream.write(textureCoordinates.top);

            // Top Right
            outputStream.write(vertexCoordinates.right);
            outputStream.write(vertexCoordinates.top);
            outputStream.write(textureCoordinates.right);
            outputStream.write(textureCoordinates.top);

            // Bottom Left
            outputStream.write(vertexCoordinates.left);
            outputStream.write(vertexCoordinates.bottom);
            outputStream.write(textureCoordinates.left);
            outputStream.write(textureCoordinates.bottom);

            // Bottom Left
            outputStream.write(vertexCoordinates.left);
            outputStream.write(vertexCoordinates.bottom);
            outputStream.write(textureCoordinates.left);
            outputStream.write(textureCoordinates.bottom);

            // Top Right
            outputStream.write(vertexCoordinates.right);
            outputStream.write(vertexCoordinates.top);
            outputStream.write(textureCoordinates.right);
            outputStream.write(textureCoordinates.top);

            // Bottom Right
            outputStream.write(vertexCoordinates.right);
            outputStream.write(vertexCoordinates.bottom);
            outputStream.write(textureCoordinates.right);
            outputStream.write(textureCoordinates.bottom);
        }

    public:

        simpleQuad vertexCoordinates;
        simpleQuad textureCoordinates;
	};
}