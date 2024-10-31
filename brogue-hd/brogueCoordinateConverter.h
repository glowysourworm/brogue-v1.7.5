#pragma once

#include "brogueGlobal.h"
#include "simpleGlData.h"
#include "brogueImageQuad.h"
#include "brogueCellQuad.h"
#include "brogueColorQuad.h"
#include "brogueGlyphMap.h"
#include "openglQuadConverter.h"
#include "gl.h"

using namespace brogueHd::frontend::ui;

namespace brogueHd::frontend::opengl
{
	class brogueCoordinateConverter
	{
    public:

        /// <summary>
        /// Creates an instance of the coordinate converter with the specified parameters quad maps. There should be
        /// one for the glyphs for a view; and one for the view itself.
        /// </summary>
        brogueCoordinateConverter(const openglQuadConverter& glyphConverter, const openglQuadConverter& viewConverter);
        brogueCoordinateConverter(const brogueCoordinateConverter& copy)
        {
            _viewConverter = copy.getViewConverter();
            _glyphConverter = copy.getGlyphConverter();
            _glyphMap = copy.getGlyphMap();
        }
        ~brogueCoordinateConverter();

        brogueImageQuad createBrogueImageQuadScene(const brogueCellDisplay& cell, int column, int row);
        brogueCellQuad createBrogueCellQuadScene(const brogueCellDisplay& cell, int column, int row, openglBrogueCellOutputSelector outputSelector);
        brogueColorQuad createBrogueColorQuadScene(const brogueCellDisplay& cell, int column, int row);

        brogueImageQuad createBrogueImageQuadFrame();
        brogueColorQuad createBrogueColorQuadFrame(color theColor);

    public:

        openglQuadConverter getViewConverter() const
        {
            return _viewConverter;
        }
        openglQuadConverter getGlyphConverter() const
        {
            return _glyphConverter;
        }
        brogueGlyphMap* getGlyphMap() const
        {
            return _glyphMap;
        }

    private:

        openglQuadConverter _glyphConverter;
        openglQuadConverter _viewConverter;

        brogueGlyphMap* _glyphMap;
	};

    brogueCoordinateConverter::brogueCoordinateConverter(const openglQuadConverter& glyphMap, const openglQuadConverter& viewMap)
    {
        _glyphConverter = glyphMap;
        _viewConverter = viewMap;
        _glyphMap = new brogueGlyphMap();
    }

    brogueCoordinateConverter::~brogueCoordinateConverter()
    {
        delete _glyphMap;
    }

    brogueImageQuad brogueCoordinateConverter::createBrogueImageQuadScene(const brogueCellDisplay& cell, int column, int row)
    {
        simpleQuad quadXY = _viewConverter.createQuadNormalizedXY_FromLocator(column, row);
        simpleQuad quadUV = _viewConverter.createQuadNormalizedUV_FromLocator(column, row);

        return brogueImageQuad(quadXY, quadUV);
    }

    brogueCellQuad brogueCoordinateConverter::createBrogueCellQuadScene(const brogueCellDisplay& cell, int column, int row, openglBrogueCellOutputSelector outputSelector)
    {
        gridLocator glyphLocation = _glyphMap->getGlyphLocation(cell.character);

        simpleQuad quadXY = _viewConverter.createQuadNormalizedXY_FromLocator(column, row);
        simpleQuad quadUV = _viewConverter.createQuadNormalizedUV_FromLocator(column, row);
        simpleQuad glyphUV = _glyphConverter.createQuadNormalizedUV_FromLocator(glyphLocation);

        return brogueCellQuad(cell, quadXY, quadUV, glyphUV, outputSelector);
    }

    brogueColorQuad brogueCoordinateConverter::createBrogueColorQuadScene(const brogueCellDisplay& cell, int column, int row)
    {
        simpleQuad quadXY = _viewConverter.createQuadNormalizedXY_FromLocator(column, row);

        return brogueColorQuad(cell, quadXY);
    }

    brogueImageQuad brogueCoordinateConverter::createBrogueImageQuadFrame()
    {
        simpleQuad quadXY = _viewConverter.createQuadNormalizedXY(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());
        simpleQuad quadUV = _viewConverter.createQuadNormalizedUV(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());

        return brogueImageQuad(quadXY, quadUV);
    }

    brogueColorQuad brogueCoordinateConverter::createBrogueColorQuadFrame(color theColor)
    {
        simpleQuad quadXY = _viewConverter.createQuadNormalizedXY(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());
        simpleQuad quadUV = _viewConverter.createQuadNormalizedUV(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());

        return brogueColorQuad(theColor, quadXY);
    }
}