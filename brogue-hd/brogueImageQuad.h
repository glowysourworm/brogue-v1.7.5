#pragma once

#include "simpleGlData.h"

namespace brogueHd::frontend::opengl
{
	struct brogueImageQuad : simpleGlData
	{
        brogueImageQuad(){}

        brogueImageQuad(const simpleQuad& vertexCoords, const simpleQuad& textureCoords)
        {
            vertexCoordinates = vertexCoords;
            textureCoordinates = textureCoords;
        }

        int getElementSize(GLenum primitiveType) const override
        {
            // Total # of calls to the shader
            switch (primitiveType)
            {
            case GL_TRIANGLES:
                return 6;
            default:
                simpleException::show("Unhandled primitive type for brogueImageQuad:  {}", primitiveType);
            }
        }

        int getStreamSize(GLenum primitiveType) const override
        {
            // 24 TOTAL FLOATS
            return 24;
        }

        void streamBuffer(GLenum primitiveType, simpleDataStream<float>* outputStream) const override
        {
            // (Triangle 1) topLeft, topRight, bottomRight

            // Top Left
            vertexCoordinates.topLeft.streamBuffer(primitiveType, outputStream);        // vec2
            textureCoordinates.topLeft.streamBuffer(primitiveType, outputStream);       // vec2

            // Top Right
            vertexCoordinates.topRight.streamBuffer(primitiveType, outputStream);
            textureCoordinates.topRight.streamBuffer(primitiveType, outputStream);

            // Bottom Left
            vertexCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);
            textureCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);

            // (Triangle 2) topLeft, bottomRight, bottomLeft

            // Bottom Left
            vertexCoordinates.topLeft.streamBuffer(primitiveType, outputStream);
            textureCoordinates.topLeft.streamBuffer(primitiveType, outputStream);

            // Top Right
            vertexCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);
            textureCoordinates.bottomRight.streamBuffer(primitiveType, outputStream);

            // Bottom Right
            vertexCoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);
            textureCoordinates.bottomLeft.streamBuffer(primitiveType, outputStream);
        }

    public:

        simpleQuad vertexCoordinates;
        simpleQuad textureCoordinates;
	};
}