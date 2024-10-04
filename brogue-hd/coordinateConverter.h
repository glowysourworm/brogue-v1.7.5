#pragma once

#include "brogueGlobal.h"
#include "simpleGlData.h"
#include "gl.h"

namespace brogueHd::frontend::opengl
{
	class coordinateConverter
	{
    public:

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