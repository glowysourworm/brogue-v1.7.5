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
		brogueCoordinateConverter(resourceController* resourceController, int sceneWidth, int sceneHeight, int zoomLevel);
		brogueCoordinateConverter(const brogueCoordinateConverter& copy)
		{
			_viewConverter = copy.getViewConverter();
			_glyphConverter = copy.getGlyphConverter();
			_glyphMap = copy.getGlyphMap();
			_zoomLevel = copy.getZoomLevel();
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
		int getZoomLevel() const
		{
			return _zoomLevel;
		}

	private:

		openglQuadConverter _glyphConverter;
		openglQuadConverter _viewConverter;

		brogueGlyphMap* _glyphMap;
		int _zoomLevel;
	};

	brogueCoordinateConverter::brogueCoordinateConverter(resourceController* resourceController, int sceneWidth, int sceneHeight, int zoomLevel)
	{
		int glyphSheetWidth = resourceController->getFontGlyphs(zoomLevel)->pixelWidth();
		int glyphSheetHeight = resourceController->getFontGlyphs(zoomLevel)->pixelHeight();

		float glyphWidth = glyphSheetWidth / (float)_glyphMap->GlyphSheetColumns;
		float glyphHeight = glyphSheetHeight / (float)(_glyphMap->GlyphSheetRows + _glyphMap->GlyphSheetRowOffset);

		openglQuadConverter glyphMap(glyphWidth, glyphHeight, glyphSheetWidth, glyphSheetHeight);
		openglQuadConverter viewMap(brogueCellDisplay::CellWidth(zoomLevel), 
									brogueCellDisplay::CellHeight(zoomLevel), 
									brogueCellDisplay::CellWidth(zoomLevel) * sceneWidth,
									brogueCellDisplay::CellHeight(zoomLevel) * sceneHeight);

		_glyphConverter = glyphMap;
		_viewConverter = viewMap;
		_zoomLevel = zoomLevel;
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