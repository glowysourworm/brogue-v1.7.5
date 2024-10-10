#pragma once

#include "brogueGlobal.h"
#include "simpleArray.h"
#include "coordinateConverter.h"
#include "simpleGlData.h"
#include "gl.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
    /// <summary>
    /// Rectangle that stores coordinates for a bounding box in all coordinate systems: UI-coordinates, UV-coordinates,
    /// and XY-coordinates relative to the 2D scene pixel dimensions.
    /// </summary>
	class boundingBox2D
	{
    public:
		boundingBox2D();
		boundingBox2D(int pixelX, int pixelY, int pixelWidth, int pixelHeight, int scenePixelWidth, int scenePixelHeight);

        /// <summary>
        /// Prepares a data vector for the GLImage2D with { vertex-x, vertex-y, texture-x, texture-y } to create the vertex map
        /// for a texture.
        /// </summary>
        simpleArray<vec4> interleveImage2DCoordinates();

        /// <summary>
        /// Returns coordinate vector in terms of XY-coordinates that are normalized [-1, 1]
        /// </summary>
        simpleArray<vec2> getXYCoordinatesNormalized();

        /// <summary>
        /// Returns coordinate vector in terms of UI-coordinates that are non-normalized
        /// </summary>
        simpleArray<vec2> getUICoordinates();

    private:
        int _pixelX;
        int _pixelY;
        int _pixelWidth;
        int _pixelHeight;
        int _scenePixelWidth;
        int _scenePixelHeight;
	};

    boundingBox2D::boundingBox2D()
    {
        _pixelX = 0;
        _pixelY = 0;
        _pixelWidth = 0;
        _pixelHeight = 0;
        _scenePixelHeight = 0;
        _scenePixelWidth = 0;
    }
    boundingBox2D::boundingBox2D(int pixelX, int pixelY, int pixelWidth, int pixelHeight, int scenePixelWidth, int scenePixelHeight)
    {
        _pixelX = pixelX;
        _pixelY = pixelY;
        _pixelWidth = pixelWidth;
        _pixelHeight = pixelHeight;
        _scenePixelHeight = scenePixelHeight;
        _scenePixelWidth = scenePixelWidth;
    }

    /// <summary>
    /// Returns coordinate vector in terms of UI-coordinates that are non-normalized
    /// </summary>
    simpleArray<vec2> boundingBox2D::getUICoordinates()
    {
        simpleArray<vec2> result(4);

        result.set(0, vec2(_pixelX, _pixelY));                              // Top-left
        result.set(1, vec2(_pixelX + _pixelWidth, _pixelY));                // Top-right
        result.set(2, vec2(_pixelX + _pixelWidth, _pixelY + _pixelHeight)); // Bottom-right
        result.set(3, vec2(_pixelX, _pixelY + _pixelHeight));               // Bottom-left

        return result;
    }

    /// <summary>
    /// Returns coordinate vector in terms of XY-coordinates that are normalized [-1, 1]
    /// </summary>
    simpleArray<vec2> boundingBox2D::getXYCoordinatesNormalized()
    {
        vec2 topLeft = coordinateConverter::convertToNormalizedXYScene(_pixelX, _pixelY, _scenePixelWidth, _scenePixelHeight);
        vec2 topRight = coordinateConverter::convertToNormalizedXYScene(_pixelX + _pixelWidth, _pixelY, _scenePixelWidth, _scenePixelHeight);
        vec2 bottomRight = coordinateConverter::convertToNormalizedXYScene(_pixelX + _pixelWidth, _pixelY + _pixelHeight, _scenePixelWidth, _scenePixelHeight);
        vec2 bottomLeft = coordinateConverter::convertToNormalizedXYScene(_pixelX, _pixelY + _pixelHeight, _scenePixelWidth, _scenePixelHeight);

        simpleArray<vec2> result(4);
        result.set(0, topLeft);
        result.set(1, topRight);
        result.set(2, bottomRight);
        result.set(3, bottomLeft);
    }


    simpleArray<vec4> boundingBox2D::interleveImage2DCoordinates()
    {
        simpleArray<vec2> xyCoords = this->getXYCoordinatesNormalized();

        simpleArray<vec4> result(4);

        result.set(0, vec4(xyCoords.get(0).x, xyCoords.get(0).y, 0.0f, 1.0f)); // Top-left
        result.set(1, vec4(xyCoords.get(1).x, xyCoords.get(1).y, 1.0f, 1.0f)); // Top-right
        result.set(2, vec4(xyCoords.get(2).x, xyCoords.get(2).y, 1.0f, 0.0f)); // Bottom-right
        result.set(3, vec4(xyCoords.get(3).x, xyCoords.get(3).y, 0.0f, 0.0f)); // Bottom-left

        return result;
    }
}