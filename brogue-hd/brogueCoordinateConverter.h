#pragma once

#include "brogueCellDisplay.h"
#include "brogueGlyphMap.h"
#include "gridRect.h"
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

		gridRect calculateSceneBoundaryUI() const;

		simpleQuad createFrameQuadXY();
		simpleQuad createFrameQuadUV();

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
	gridRect brogueCoordinateConverter::calculateSceneBoundaryUI() const
	{
		return gridRect(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());
	}
	simpleQuad brogueCoordinateConverter::createFrameQuadXY()
	{
		return _viewConverter.createQuadNormalizedXY(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());
	}
	simpleQuad brogueCoordinateConverter::createFrameQuadUV()
	{
		return _viewConverter.createQuadNormalizedUV(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());
	}
}