#pragma once

#include "brogueCellDisplay.h"
#include "brogueGlyphMap.h"
#include "gridLocatorEdge.h"
#include "gridRect.h"
#include "openglQuadConverter.h"
#include "resourceController.h"
#include "simpleGlData.h"
#include "simpleLine.h"
#include "simplePoint.h"

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

		simpleLine<int> convertToUI(const gridLocatorEdge& edge, bool moveToCellCenter = false);

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
	simpleLine<int> brogueCoordinateConverter::convertToUI(const gridLocatorEdge& edge, bool moveToCellCenter)
	{
		int centerOffsetX = moveToCellCenter ? brogueCellDisplay::CellWidth(_zoomLevel) / 2.0f : 0;
		int centerOffsetY = moveToCellCenter ? brogueCellDisplay::CellHeight(_zoomLevel) / 2.0f : 0;

		simplePoint<int> point1(edge.node1.column * brogueCellDisplay::CellWidth(_zoomLevel) + centerOffsetX,
								edge.node1.row * brogueCellDisplay::CellHeight(_zoomLevel) + centerOffsetY);

		simplePoint<int> point2(edge.node2.column * brogueCellDisplay::CellWidth(_zoomLevel) + centerOffsetX,
								edge.node2.row * brogueCellDisplay::CellHeight(_zoomLevel) + centerOffsetY);

		return simpleLine<int>(point1, point2);
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