#pragma once

#include "brogueCellDisplay.h"
#include "brogueGlyphMap.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRect.h"
#include "openglQuadConverter.h"
#include "resourceController.h"
#include "layoutCoordinateConverter.h"
#include "simpleGlData.h"
#include "simpleLine.h"
#include "simpleMath.h"
#include "simplePoint.h"
#include "simpleRectangle.h"

namespace brogueHd::frontend
{
	using namespace brogueHd::backend;
	using namespace brogueHd::component;

	class brogueCoordinateConverter
	{
	public:
		/// <summary>
		/// Creates an instance of the coordinate converter with the specified parameters quad maps. There should be
		/// one for the glyphs for a view; and one for the view itself.
		/// </summary>
		brogueCoordinateConverter(resourceController* resourceController,
		                          const gridRect& sceneBoundary,
		                          const gridRect& levelParentBoundary,
		                          int zoomLevel);

		brogueCoordinateConverter(const brogueCoordinateConverter& copy)
		{
			_viewConverter = copy.getViewConverter();
			_glyphConverter = copy.getGlyphConverter();
			_glyphMap = copy.getGlyphMap();
			_zoomLevel = copy.getZoomLevel();
		}

		~brogueCoordinateConverter();

		gridRect calculateSceneBoundaryUI() const;

		simpleRectangle<int> convertToUI(const gridRect& rect, bool moveToCellCenter = false);
		simpleLine<int> convertToUI(const gridLocatorEdge& edge, bool moveToCellCenter = false);
		simplePoint<int> convertToUI(const gridLocator& location, bool moveToCellCenter = false);

		simpleRectangle<float> convertToUIReal(const gridRect& edge, bool moveToCellCenter = false);
		simpleLine<float> convertToUIReal(const gridLocatorEdge& edge, bool moveToCellCenter = false);
		simplePoint<float> convertToUIReal(const gridLocator& location, bool moveToCellCenter = false);

		gridLocator convertUIToGrid(const simplePoint<int> pointUI, bool centerOffsetUsed);
		gridLocatorEdge convertUIToGrid(const simpleLine<int> lineUI, bool centerOffsetUsed);

		gridLocator convertUIRealToGrid(const simplePoint<float> pointUIReal, bool centerOffsetUsed);
		gridLocatorEdge convertUIRealToGrid(const simpleLine<float> lineUIReal, bool centerOffsetUsed);

		simpleQuad createFrameQuadXY();
		simpleQuad createFrameQuadUV();

	public:
		openglQuadConverter* getViewConverter() const
		{
			return _viewConverter;
		}

		openglQuadConverter* getGlyphConverter() const
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
		layoutCoordinateConverter* _layoutConverter;

		openglQuadConverter* _glyphConverter;
		openglQuadConverter* _viewConverter;

		brogueGlyphMap* _glyphMap;
		int _zoomLevel;
	};

	brogueCoordinateConverter::brogueCoordinateConverter(resourceController* resourceController,
	                                                     const gridRect& sceneBoundary,
	                                                     const gridRect& levelParentBoundary,
	                                                     int zoomLevel)
	{
		int glyphSheetWidth = resourceController->getFontGlyphs(zoomLevel)->pixelWidth();
		int glyphSheetHeight = resourceController->getFontGlyphs(zoomLevel)->pixelHeight();

		float glyphWidth = glyphSheetWidth / (float)_glyphMap->GlyphSheetColumns;
		float glyphHeight = glyphSheetHeight / (float)(_glyphMap->GlyphSheetRows + _glyphMap->GlyphSheetRowOffset);

		_zoomLevel = zoomLevel;

		_glyphConverter = new openglQuadConverter(glyphWidth, glyphHeight, glyphSheetWidth,
		                                          glyphSheetHeight);
		_viewConverter = new openglQuadConverter(brogueCellDisplay::CellWidth(zoomLevel),
		                                         brogueCellDisplay::CellHeight(zoomLevel),
		                                         brogueCellDisplay::CellWidth(zoomLevel) * sceneBoundary.
		                                         width,
		                                         brogueCellDisplay::CellHeight(zoomLevel) * sceneBoundary.
		                                         height);

		_glyphMap = new brogueGlyphMap();

		_layoutConverter = new layoutCoordinateConverter(levelParentBoundary, zoomLevel);
	}

	brogueCoordinateConverter::~brogueCoordinateConverter()
	{
		delete _glyphMap;
		delete _layoutConverter;
		delete _viewConverter;
		delete _glyphConverter;
	}

	gridRect brogueCoordinateConverter::calculateSceneBoundaryUI() const
	{
		return gridRect(0, 0, _viewConverter->getViewWidth(), _viewConverter->getViewHeight());
	}

	simplePoint<int> brogueCoordinateConverter::convertToUI(const gridLocator& location, bool moveToCellCenter)
	{
		return _layoutConverter->convertToUI(location, moveToCellCenter);
	}

	simpleLine<int> brogueCoordinateConverter::convertToUI(const gridLocatorEdge& edge, bool moveToCellCenter)
	{
		return _layoutConverter->convertToUI(edge, moveToCellCenter);
	}

	simpleRectangle<int> brogueCoordinateConverter::convertToUI(const gridRect& rect, bool moveToCellCenter)
	{
		return _layoutConverter->convertToUI(rect, moveToCellCenter);
	}

	simpleRectangle<float> brogueCoordinateConverter::convertToUIReal(const gridRect& rect, bool moveToCellCenter)
	{
		return _layoutConverter->convertToUIReal(rect, moveToCellCenter);
	}

	simpleLine<float> brogueCoordinateConverter::convertToUIReal(const gridLocatorEdge& edge, bool moveToCellCenter)
	{
		return _layoutConverter->convertToUIReal(edge, moveToCellCenter);
	}

	simplePoint<float> brogueCoordinateConverter::convertToUIReal(const gridLocator& location, bool moveToCellCenter)
	{
		return _layoutConverter->convertToUIReal(location, moveToCellCenter);
	}

	gridLocator brogueCoordinateConverter::convertUIToGrid(const simplePoint<int> pointUI, bool centerOffsetUsed)
	{
		return _layoutConverter->convertUIToGrid(pointUI, centerOffsetUsed);
	}

	gridLocatorEdge brogueCoordinateConverter::convertUIToGrid(const simpleLine<int> lineUI, bool centerOffsetUsed)
	{
		return _layoutConverter->convertUIToGrid(lineUI, centerOffsetUsed);
	}

	gridLocator brogueCoordinateConverter::convertUIRealToGrid(const simplePoint<float> pointUIReal,
	                                                           bool centerOffsetUsed)
	{
		return _layoutConverter->convertUIRealToGrid(pointUIReal, centerOffsetUsed);
	}

	gridLocatorEdge brogueCoordinateConverter::convertUIRealToGrid(const simpleLine<float> lineUIReal,
	                                                               bool centerOffsetUsed)
	{
		return _layoutConverter->convertUIRealToGrid(lineUIReal, centerOffsetUsed);
	}

	simpleQuad brogueCoordinateConverter::createFrameQuadXY()
	{
		return _viewConverter->createQuadNormalizedXY(0, 0, _viewConverter->getViewWidth(),
		                                              _viewConverter->getViewHeight());
	}

	simpleQuad brogueCoordinateConverter::createFrameQuadUV()
	{
		return _viewConverter->createQuadNormalizedUV(0, 0, _viewConverter->getViewWidth(),
		                                              _viewConverter->getViewHeight());
	}
}
