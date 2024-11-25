#pragma once

#include "brogueCellDisplay.h"
#include "brogueColorQuad.h"
#include "brogueGlyphMap.h"
#include "brogueImageQuad.h"
#include "color.h"
#include "gridLocator.h"
#include "openglQuadConverter.h"
#include "resourceController.h"
#include "simpleGlData.h"

namespace brogueHd::frontend
{
	class brogueCoordinateConverter
	{
	public:

		/// <summary>
		/// Creates an instance of the coordinate converter with the specified parameters quad maps. There should be
		/// one for the glyphs for a view; and one for the view itself.
		/// </summary>
		brogueCoordinateConverter(resourceController* resourceController, int sceneWidthUI, int sceneHeightUI, int zoomLevel);
		brogueCoordinateConverter(const brogueCoordinateConverter& copy)
		{
			_viewConverter = copy.getViewConverter();
			_glyphConverter = copy.getGlyphConverter();
			_glyphMap = copy.getGlyphMap();
		}
		~brogueCoordinateConverter();

	protected:

		brogueImageQuad createBrogueImageQuadFrame();
		brogueColorQuad createBrogueColorQuadFrame(const color& theColor);

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

	brogueCoordinateConverter::brogueCoordinateConverter(resourceController* resourceController, int sceneWidthUI, int sceneHeightUI, int zoomLevel)
	{
		int glyphSheetWidth = resourceController->getFontGlyphs(zoomLevel)->pixelWidth();
		int glyphSheetHeight = resourceController->getFontGlyphs(zoomLevel)->pixelHeight();

		float glyphWidth = glyphSheetWidth / (float)_glyphMap->GlyphSheetColumns;
		float glyphHeight = glyphSheetHeight / (float)(_glyphMap->GlyphSheetRows + _glyphMap->GlyphSheetRowOffset);

		openglQuadConverter glyphMap(glyphWidth, glyphHeight, glyphSheetWidth, glyphSheetHeight);
		openglQuadConverter viewMap(brogueCellDisplay::CellWidth(zoomLevel), brogueCellDisplay::CellHeight(zoomLevel), sceneWidthUI, sceneHeightUI);

		_glyphConverter = glyphMap;
		_viewConverter = viewMap;
		_glyphMap = new brogueGlyphMap();
	}

	brogueCoordinateConverter::~brogueCoordinateConverter()
	{
		delete _glyphMap;
	}
	brogueImageQuad brogueCoordinateConverter::createBrogueImageQuadFrame()
	{
		simpleQuad quadXY = _viewConverter.createQuadNormalizedXY(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());
		simpleQuad quadUV = _viewConverter.createQuadNormalizedUV(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());

		return brogueImageQuad(gridLocator(0, 0), quadXY, quadUV);
	}

	brogueColorQuad brogueCoordinateConverter::createBrogueColorQuadFrame(const color& theColor)
	{
		simpleQuad quadXY = _viewConverter.createQuadNormalizedXY(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());
		simpleQuad quadUV = _viewConverter.createQuadNormalizedUV(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());

		return brogueColorQuad(gridLocator(0, 0), theColor, quadXY);
	}
}