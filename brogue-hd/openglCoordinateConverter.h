#pragma once

#include "brogueGlobal.h"
#include "simpleGlData.h"
#include "brogueImageQuad.h"
#include "brogueCellQuad.h"
#include "brogueColorQuad.h"
#include "brogueGlyphMap.h"
#include "gl.h"

using namespace brogueHd::frontend::ui;

namespace brogueHd::frontend::opengl
{
	class openglCoordinateConverter
	{
    public:

        /// <summary>
        /// Creates an instance of the coordinate converter with the specified parameters. 
        /// </summary>
        /// <param name="quadWidthUI">Width of a quad in UI coordinates</param>
        /// <param name="quadHeightUI">Height of a quad in UI coordinates</param>
        /// <param name="glyphQuadWidthUI">Width of a glyph in UI coordinates</param>
        /// <param name="glyphQuadHeightUI">Height of a glyph in UI coordinates</param>
        /// <param name="sceneWidth">Width of the scene (or view) in UI coordinates</param>
        /// <param name="sceneHeight">Height of the scene (or view) in UI coordinates</param>
        /// <param name="glyphSheetWidth">Width of the glyph sheet in UI coordinates</param>
        /// <param name="glyphSheetHeight">Height of the glyph sheet in UI coordinates</param>
        openglCoordinateConverter(float cellQuadWidthUI, float cellQuadHeightUI, float glyphQuadWidthUI, float glyphQuadHeightUI, int sceneWidth, int sceneHeight, int glyphSheetWidth, int glyphSheetHeight);
        ~openglCoordinateConverter();

        brogueImageQuad createBrogueImageQuadScene(const brogueCellDisplay& cell, int column, int row);
        brogueCellQuad createBrogueCellQuadScene(const brogueCellDisplay& cell, int column, int row, openglBrogueCellOutputSelector outputSelector);
        brogueColorQuad createBrogueColorQuadScene(const brogueCellDisplay& cell, int column, int row);

        brogueImageQuad createBrogueImageQuadFrame(int offsetColumn, int offsetRow);
        brogueColorQuad createBrogueColorQuadFrame(color theColor, int offsetColumn, int offsetRow);

        /// <summary>
        /// Converts from UI coordinates to XY-Normalized
        /// </summary>
        /// <param name="pixelX">UI-coordinate X</param>
        /// <param name="pixelY">UI-coordinate Y</param>
        vec2 convertToNormalizedXYScene(float pixelX, float pixelY);

        /// <summary>
        /// Creates Quad from UI coordinates converting to XY-Normalized
        /// </summary>
        /// <param name="primitiveType">Eventual draw type for each primitive. e.g. GL_TRIANGLES</param>
        /// <param name="pixelX">UI-coordinate X</param>
        /// <param name="pixelY">UI-coordinate Y</param>
        /// <param name="pixelWidth">UI-coordinate Width</param>
        /// <param name="pixelHeight">UI-coordinate Height</param>
        simpleQuad createQuadNormalizedXYScene(float pixelX, float pixelY, float pixelWidth, float pixelHeight);

        /// <summary>
        /// Converts from UI coordinates to UV-Normalized scene coordinates - used for texture overlays or post-processing
        /// </summary>
        /// <param name="pixelX">UI Pixel X</param>
        /// <param name="pixelY">UI Pixel Y</param>
        /// <returns>UV coordinates with respect to the scene</returns>
        vec2 convertToNormalizedUVScene(float pixelX, float pixelY);

        /// <summary>
        /// Converts from UI coordinates to UV coordinates (NON-NORMALIZED). This is used for texture rectangles / texelFetch. The
        /// result will be in UV coordinates; but scaled to the scene UI size.
        /// </summary>
        /// <param name="pixelX">UI Pixel X</param>
        /// <param name="pixelY">UI Pixel Y</param>
        ivec2 convertToUVScene(float pixelX, float pixelY);

        /// <summary>
        /// Creates GLQuad from UI coordinates converting to UV-Normalized scene coordinates - used for texture overlays or post-processing
        /// </summary>
        /// <param name="pixelX">UI Pixel X</param>
        /// <param name="pixelY">UI Pixel Y</param>
        /// <returns>UV coordinate GLQuad with respect to the scene</returns>
        simpleQuad createQuadNormalizedUVScene(float pixelX, float pixelY, float pixelWidth, float pixelHeight);

        /// <summary>
        /// Creates a GL quad that is in UV coordinates; but scaled to the UI size. This is used with texture rectangle / texelFetch.
        /// </summary>
        /// <param name="pixelX">UI Pixel X</param>
        /// <param name="pixelY">UI Pixel Y</param>
        /// <param name="pixelWidth">UI Pixel Width</param>
        /// <param name="pixelHeight">UI Pixel Height</param>
        simpleiQuad createQuadUVScene(float pixelX, float pixelY, float pixelWidth, float pixelHeight);

    private:

        float _cellQuadWidthUI;
        float _cellQuadHeightUI;
        float _glyphQuadWidthUI;
        float _glyphQuadHeightUI;
        int _sceneWidth;
        int _sceneHeight;
        int _glyphSheetWidth;
        int _glyphSheetHeight;

        brogueGlyphMap* _glyphMap;
	};

    openglCoordinateConverter::openglCoordinateConverter(float cellQuadWidthUI, float cellQuadHeightUI, float glyphQuadWidthUI, float glyphQuadHeightUI, int sceneWidth, int sceneHeight, int glyphSheetWidth, int glyphSheetHeight)
    {
        _cellQuadWidthUI = cellQuadWidthUI;
        _cellQuadHeightUI = cellQuadHeightUI;
        _glyphQuadWidthUI = glyphQuadWidthUI;
        _glyphQuadHeightUI = glyphQuadHeightUI;
        _sceneWidth = sceneWidth;
        _sceneHeight = sceneHeight;
        _glyphSheetWidth = glyphSheetWidth;
        _glyphSheetHeight = glyphSheetHeight;

        _glyphMap = new brogueGlyphMap();
    }

    openglCoordinateConverter::~openglCoordinateConverter()
    {
        delete _glyphMap;
    }

    brogueImageQuad openglCoordinateConverter::createBrogueImageQuadScene(const brogueCellDisplay& cell, int column, int row)
    {
        float pixelX = column * _cellQuadWidthUI;
        float pixelY = row * _cellQuadHeightUI;

        simpleQuad quadXY = createQuadNormalizedXYScene(pixelX, pixelY, _cellQuadWidthUI, _cellQuadHeightUI);
        simpleQuad quadUV = createQuadNormalizedUVScene(pixelX, pixelY, _cellQuadWidthUI, _cellQuadHeightUI);

        return brogueImageQuad(quadXY, quadUV);
    }

    brogueCellQuad openglCoordinateConverter::createBrogueCellQuadScene(const brogueCellDisplay& cell, int column, int row, openglBrogueCellOutputSelector outputSelector)
    {
        float pixelX = column * _cellQuadWidthUI;
        float pixelY = row * _cellQuadHeightUI;

        gridLocator glyphLocation = _glyphMap->getGlyphLocation(cell.character);

        simpleQuad quadXY = createQuadNormalizedXYScene(pixelX, pixelY, _cellQuadWidthUI, _cellQuadHeightUI);
        simpleQuad quadUV = createQuadNormalizedUVScene(pixelX, pixelY, _cellQuadWidthUI, _cellQuadHeightUI);       
        simpleQuad glyphUV = createQuadNormalizedUVScene(glyphLocation.column * _glyphQuadWidthUI, glyphLocation.row * _glyphQuadHeightUI, _glyphQuadWidthUI, _glyphQuadHeightUI);

        return brogueCellQuad(cell, quadXY, quadUV, glyphUV, outputSelector);
    }

    brogueColorQuad openglCoordinateConverter::createBrogueColorQuadScene(const brogueCellDisplay& cell, int column, int row)
    {
        float pixelX = column * _cellQuadWidthUI;
        float pixelY = row * _cellQuadHeightUI;

        simpleQuad quadXY = createQuadNormalizedXYScene(pixelX, pixelY, _cellQuadWidthUI, _cellQuadHeightUI);

        return brogueColorQuad(cell, quadXY);
    }

    brogueImageQuad openglCoordinateConverter::createBrogueImageQuadFrame(int offsetColumn, int offsetRow)
    {
        float pixelX = offsetColumn * _cellQuadWidthUI;
        float pixelY = offsetRow * _cellQuadHeightUI;

        simpleQuad quadXY = createQuadNormalizedXYScene(pixelX, pixelY, _sceneWidth, _sceneHeight);
        simpleQuad quadUV = createQuadNormalizedUVScene(pixelX, pixelY, _sceneWidth, _sceneHeight);

        return brogueImageQuad(quadXY, quadUV);
    }

    brogueColorQuad openglCoordinateConverter::createBrogueColorQuadFrame(color theColor, int offsetColumn, int offsetRow)
    {
        float pixelX = offsetColumn * _cellQuadWidthUI;
        float pixelY = offsetRow * _cellQuadHeightUI;

        simpleQuad quadXY = createQuadNormalizedXYScene(pixelX, pixelY, _sceneWidth, _sceneHeight);
        simpleQuad quadUV = createQuadNormalizedUVScene(pixelX, pixelY, _sceneWidth, _sceneHeight);

        return brogueColorQuad(theColor, quadXY);
    }

    /// <summary>
    /// Converts from UI coordinates to XY-Normalized
    /// </summary>
    /// <param name="pixelX">UI-coordinate X</param>
    /// <param name="pixelY">UI-coordinate Y</param>
    /// <param name="sceneWidth">UI Width</param>
    /// <param name="sceneHeight">UI Height</param>
    vec2 openglCoordinateConverter::convertToNormalizedXYScene(float pixelX, float pixelY)
    {
        float normalizedY = ((pixelY / (float)_sceneHeight) * -2.0f) + 1.0f;
        float normalizedX = ((pixelX / (float)_sceneWidth) * 2.0f) - 1.0f;

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
    simpleQuad openglCoordinateConverter::createQuadNormalizedXYScene(float pixelX, float pixelY, float pixelWidth, float pixelHeight)
    {
        vec2 topLeft = convertToNormalizedXYScene(pixelX, pixelY);
        vec2 bottomRight = convertToNormalizedXYScene(pixelX + pixelWidth, pixelY + pixelHeight);

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
    vec2 openglCoordinateConverter::convertToNormalizedUVScene(float pixelX, float pixelY)
    {
        // Have to invert the Y coordinate to work with the texture
        float normalizedU = pixelX / (float)_sceneWidth;
        float normalizedV = 1 - (pixelY / (float)_sceneHeight);

        return vec2(normalizedU, normalizedV);
    }

    /// <summary>
    /// Converts from UI coordinates to UV coordinates (NON-NORMALIZED). This is used for texture rectangles / texelFetch. The
    /// result will be in UV coordinates; but scaled to the scene UI size.
    /// </summary>
    /// <param name="pixelX">UI Pixel X</param>
    /// <param name="pixelY">UI Pixel Y</param>
    /// <param name="sceneWidth">UI scene width</param>
    /// <param name="sceneHeight">UI scene height</param>
    ivec2 openglCoordinateConverter::convertToUVScene(float pixelX, float pixelY)
    {
        return ivec2(pixelX, _sceneHeight - pixelY);
    }

    /// <summary>
    /// Creates GLQuad from UI coordinates converting to UV-Normalized scene coordinates - used for texture overlays or post-processing
    /// </summary>
    /// <param name="pixelX">UI Pixel X</param>
    /// <param name="pixelY">UI Pixel Y</param>
    /// <param name="sceneWidth">UI scene width</param>
    /// <param name="sceneHeight">UI scene height</param>
    /// <returns>UV coordinate GLQuad with respect to the scene</returns>
    simpleQuad openglCoordinateConverter::createQuadNormalizedUVScene(float pixelX, float pixelY, float pixelWidth, float pixelHeight)
    {
        vec2 topLeft = convertToNormalizedUVScene(pixelX, pixelY);
        vec2 bottomRight = convertToNormalizedUVScene(pixelX + pixelWidth, pixelY + pixelHeight);

        return simpleQuad(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
    }

    /// <summary>
    /// Creates a GL quad that is in UV coordinates; but scaled to the UI size. This is used with texture rectangle / texelFetch.
    /// </summary>
    /// <param name="pixelX">UI Pixel X</param>
    /// <param name="pixelY">UI Pixel Y</param>
    /// <param name="pixelWidth">UI Pixel Width</param>
    /// <param name="pixelHeight">UI Pixel Height</param>
    /// <param name="sceneWidth">UI Pixel scene width</param>
    /// <param name="sceneHeight">UI Pixel Scene Height</param>
    simpleiQuad openglCoordinateConverter::createQuadUVScene(float pixelX, float pixelY, float pixelWidth, float pixelHeight)
    {
        ivec2 topLeft = convertToUVScene(pixelX, pixelY);
        ivec2 bottomRight = convertToUVScene(pixelX + pixelWidth, pixelY + pixelHeight);

        return simpleiQuad(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
    }
}