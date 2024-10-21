#pragma once

#include "brogueGlobal.h"
#include "simpleGlData.h"
#include "brogueImageQuad.h"
#include "brogueCellQuad.h"
#include "brogueColorQuad.h"
#include "gl.h"

namespace brogueHd::frontend::opengl
{
	class coordinateConverter
	{
    protected:

        // Not sure what to do with these yet
        //
        static const int QuadWidth = brogueCellDisplay::CellWidth;
        static const int QuadHeight = brogueCellDisplay::CellHeight;

    public:

        static brogueImageQuad createBrogueImageQuadScene(const brogueCellDisplay& cell, int column, int row, int sceneWidth, int sceneHeight)
        {
            int pixelX = column * coordinateConverter::QuadWidth;
            int pixelY = row * coordinateConverter::QuadHeight;

            simpleQuad quadXY = coordinateConverter::createQuadNormalizedXYScene(pixelX, pixelY, coordinateConverter::QuadWidth, coordinateConverter::QuadHeight, sceneWidth, sceneHeight);
            simpleQuad quadUV = coordinateConverter::createQuadNormalizedUVScene(pixelX, pixelY, coordinateConverter::QuadWidth, coordinateConverter::QuadHeight, sceneWidth, sceneHeight);

            return brogueImageQuad(quadXY, quadUV);
        }

        static brogueCellQuad createBrogueCellQuadScene(const brogueCellDisplay& cell, int column, int row, int sceneWidth, int sceneHeight)
        {
            int pixelX = column * coordinateConverter::QuadWidth;
            int pixelY = row * coordinateConverter::QuadHeight;

            simpleQuad quadXY = coordinateConverter::createQuadNormalizedXYScene(pixelX, pixelY, coordinateConverter::QuadWidth, coordinateConverter::QuadHeight, sceneWidth, sceneHeight);
            simpleQuad quadUV = coordinateConverter::createQuadNormalizedUVScene(pixelX, pixelY, coordinateConverter::QuadWidth, coordinateConverter::QuadHeight, sceneWidth, sceneHeight);

            return brogueCellQuad(cell, quadXY, quadUV);
        }

        static brogueColorQuad createBrogueColorQuadScene(const brogueCellDisplay& cell, int column, int row, int sceneWidth, int sceneHeight)
        {
            int pixelX = column * coordinateConverter::QuadWidth;
            int pixelY = row * coordinateConverter::QuadHeight;

            simpleQuad quadXY = coordinateConverter::createQuadNormalizedXYScene(pixelX, pixelY, coordinateConverter::QuadWidth, coordinateConverter::QuadHeight, sceneWidth, sceneHeight);
            simpleQuad quadUV = coordinateConverter::createQuadNormalizedUVScene(pixelX, pixelY, coordinateConverter::QuadWidth, coordinateConverter::QuadHeight, sceneWidth, sceneHeight);

            return brogueColorQuad(cell, quadXY);
        }

        static brogueImageQuad createBrogueImageQuadFrame(int offsetColumn, int offsetRow, int sceneWidth, int sceneHeight)
        {
            int pixelX = offsetColumn * coordinateConverter::QuadWidth;
            int pixelY = offsetRow * coordinateConverter::QuadHeight;

            simpleQuad quadXY = coordinateConverter::createQuadNormalizedXYScene(pixelX, pixelY, sceneWidth, sceneHeight, sceneWidth, sceneHeight);
            simpleQuad quadUV = coordinateConverter::createQuadNormalizedUVScene(pixelX, pixelY, sceneWidth, sceneHeight, sceneWidth, sceneHeight);

            return brogueImageQuad(quadXY, quadUV);
        }

        static brogueColorQuad createBrogueColorQuadFrame(color theColor, int offsetColumn, int offsetRow, int sceneWidth, int sceneHeight)
        {
            int pixelX = offsetColumn * coordinateConverter::QuadWidth;
            int pixelY = offsetRow * coordinateConverter::QuadHeight;

            simpleQuad quadXY = coordinateConverter::createQuadNormalizedXYScene(pixelX, pixelY, sceneWidth, sceneHeight, sceneWidth, sceneHeight);
            simpleQuad quadUV = coordinateConverter::createQuadNormalizedUVScene(pixelX, pixelY, sceneWidth, sceneHeight, sceneWidth, sceneHeight);

            return brogueColorQuad(theColor, quadXY);
        }

        /// <summary>
        /// Converts from UI coordinates to XY-Normalized
        /// </summary>
        /// <param name="pixelX">UI-coordinate X</param>
        /// <param name="pixelY">UI-coordinate Y</param>
        /// <param name="sceneWidth">UI Width</param>
        /// <param name="sceneHeight">UI Height</param>
        static vec2 convertToNormalizedXYScene(int pixelX, int pixelY, int sceneWidth, int sceneHeight)
        {
            float normalizedY = ((pixelY / (float)sceneHeight) * -2.0f) + 1.0f;
            float normalizedX = ((pixelX / (float)sceneWidth) * 2.0f) - 1.0f;

            return vec2(normalizedX, normalizedY);
        }

        /// <summary>
        /// Creates Quad from UI coordinates converting to XY-Normalized
        /// </summary>
        /// <param name="primitiveType">Eventual draw type for each primitive. e.g. GL_TRIANGLES</param>
        /// <param name="pixelX">UI-coordinate X</param>
        /// <param name="pixelY">UI-coordinate Y</param>
        /// <param name="pixelWidth">UI-coordinate Width</param>
        /// <param name="pixelHeight">UI-coordinate Height</param>
        /// <param name="sceneWidth">UI Width</param>
        /// <param name="sceneHeight">UI Height</param>
        static simpleQuad createQuadNormalizedXYScene(int pixelX, int pixelY, int pixelWidth, int pixelHeight, int sceneWidth, int sceneHeight)
        {
            vec2 topLeft = convertToNormalizedXYScene(pixelX, pixelY, sceneWidth, sceneHeight);
            vec2 bottomRight = convertToNormalizedXYScene(pixelX + pixelWidth, pixelY + pixelHeight, sceneWidth, sceneHeight);

            return simpleQuad(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
        }

        /// <summary>
        /// Converts from UI coordinates to UV-Normalized scene coordinates - used for texture overlays or post-processing
        /// </summary>
        /// <param name="pixelX">UI Pixel X</param>
        /// <param name="pixelY">UI Pixel Y</param>
        /// <param name="sceneWidth">UI scene width</param>
        /// <param name="sceneHeight">UI scene height</param>
        /// <returns>UV coordinates with respect to the scene</returns>
        static vec2 convertToNormalizedUVScene(int pixelX, int pixelY, int sceneWidth, int sceneHeight)
        {
            // Have to invert the Y coordinate to work with the texture
            float normalizedU = pixelX / (float)sceneWidth;
            float normalizedV = 1 - (pixelY / (float)sceneHeight);

            return vec2(normalizedU, normalizedV);
        }

        /// <summary>
        /// Creates GLQuad from UI coordinates converting to UV-Normalized scene coordinates - used for texture overlays or post-processing
        /// </summary>
        /// <param name="pixelX">UI Pixel X</param>
        /// <param name="pixelY">UI Pixel Y</param>
        /// <param name="sceneWidth">UI scene width</param>
        /// <param name="sceneHeight">UI scene height</param>
        /// <returns>UV coordinate GLQuad with respect to the scene</returns>
        static simpleQuad createQuadNormalizedUVScene(int pixelX, int pixelY, int pixelWidth, int pixelHeight, int sceneWidth, int sceneHeight)
        {
            vec2 topLeft = convertToNormalizedUVScene(pixelX, pixelY, sceneWidth, sceneHeight);
            vec2 bottomRight = convertToNormalizedUVScene(pixelX + pixelWidth, pixelY + pixelHeight, sceneWidth, sceneHeight);

            return simpleQuad(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
        }
	};
}