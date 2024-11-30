#pragma once

#include "brogueCellDisplay.h"
#include "brogueGlyphMap.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRect.h"
#include "openglQuadConverter.h"
#include "resourceController.h"
#include "simpleGlData.h"
#include "simpleLine.h"
#include "simpleMath.h"
#include "simplePoint.h"
#include "simpleRectangle.h"

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
	simplePoint<int> brogueCoordinateConverter::convertToUI(const gridLocator& location, bool moveToCellCenter)
	{
		int centerOffsetX = moveToCellCenter ? brogueCellDisplay::CellWidth(_zoomLevel) / 2.0f : 0;
		int centerOffsetY = moveToCellCenter ? brogueCellDisplay::CellHeight(_zoomLevel) / 2.0f : 0;

		simplePoint<int> point(location.column * brogueCellDisplay::CellWidth(_zoomLevel) + centerOffsetX,
							   location.row * brogueCellDisplay::CellHeight(_zoomLevel) + centerOffsetY);

		return point;
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
	simpleRectangle<int> brogueCoordinateConverter::convertToUI(const gridRect& rect, bool moveToCellCenter)
	{
		gridLocator topLeft = rect.topLeft();
		gridLocator bottomRight = rect.bottomRight();

		simplePoint<int> topLeftUI = convertToUI(topLeft, moveToCellCenter);
		simplePoint<int> bottomRightUI = convertToUI(bottomRight, moveToCellCenter);

		return simpleRectangle<int>(topLeftUI, bottomRightUI);
	}
	simpleRectangle<float> brogueCoordinateConverter::convertToUIReal(const gridRect& rect, bool moveToCellCenter)
	{
		gridLocator topLeft = rect.topLeft();
		gridLocator bottomRight = rect.bottomRight();

		simplePoint<float> topLeftUIReal = convertToUIReal(topLeft, moveToCellCenter);
		simplePoint<float> bottomRightUIReal = convertToUIReal(bottomRight, moveToCellCenter);

		return simpleRectangle<float>(topLeftUIReal, bottomRightUIReal);
	}
	simpleLine<float> brogueCoordinateConverter::convertToUIReal(const gridLocatorEdge& edge, bool moveToCellCenter)
	{
		simpleLine<int> line = convertToUI(edge, moveToCellCenter);

		// Flip y-coordinate to go from UI -> Real Numbers
		line.node1.y = line.node1.y * -1;
		line.node2.y = line.node2.y * -1;

		simplePoint<float> point1(line.node1.x, line.node1.y);
		simplePoint<float> point2(line.node2.x, line.node2.y);

		return simpleLine<float>(point1, point2);
	}
	simplePoint<float> brogueCoordinateConverter::convertToUIReal(const gridLocator& location, bool moveToCellCenter)
	{
		simplePoint<int> point = convertToUI(location, moveToCellCenter);

		// Flip y-coordinate to go from UI -> Real Numbers
		point.y = point.y * -1;

		return simplePoint<float>(point.x, point.y);
	}
	gridLocator brogueCoordinateConverter::convertUIToGrid(const simplePoint<int> pointUI, bool centerOffsetUsed)
	{
		int offsetX = centerOffsetUsed ? -1 * brogueCellDisplay::CellWidth(_zoomLevel) : 0;
		int offsetY = centerOffsetUsed ? -1 * brogueCellDisplay::CellHeight(_zoomLevel) : 0;

		gridLocator locator(simpleMath::floor(pointUI.x / brogueCellDisplay::CellWidth(_zoomLevel)), 
							simpleMath::floor(pointUI.y / brogueCellDisplay::CellHeight(_zoomLevel)));

		return locator;
	}
	gridLocatorEdge brogueCoordinateConverter::convertUIToGrid(const simpleLine<int> lineUI, bool centerOffsetUsed)
	{
		gridLocator location1 = convertUIToGrid(lineUI.node1, centerOffsetUsed);
		gridLocator location2 = convertUIToGrid(lineUI.node2, centerOffsetUsed);

		return gridLocatorEdge(location1, location2);
	}
	gridLocator brogueCoordinateConverter::convertUIRealToGrid(const simplePoint<float> pointUIReal, bool centerOffsetUsed)
	{
		// Flip y-coordinate to go from Real Numbers -> UI coordinates
		simplePoint<int> pointUI(pointUIReal.x, -1 * pointUIReal.y);

		return convertUIToGrid(pointUI, centerOffsetUsed);
	}
	gridLocatorEdge brogueCoordinateConverter::convertUIRealToGrid(const simpleLine<float> lineUIReal, bool centerOffsetUsed)
	{
		// Flip y-coordinate to go from Real Numbers -> UI coordinates
		simplePoint<int> point1UI(lineUIReal.node1.x, lineUIReal.node1.y * -1);
		simplePoint<int> point2UI(lineUIReal.node2.x, lineUIReal.node2.y * -1);

		simpleLine<int> lineUI(point1UI, point2UI);

		return convertUIToGrid(lineUI, centerOffsetUsed);
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