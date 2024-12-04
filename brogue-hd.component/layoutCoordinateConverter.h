#pragma once

#include "brogueCellDisplay.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRect.h"

#include <simpleLine.h>
#include <simpleMath.h>
#include <simplePoint.h>
#include <simpleRectangle.h>

namespace brogueHd::component
{
	using namespace simple;

	class layoutCoordinateConverter
	{
	public:
		layoutCoordinateConverter(const gridRect& parentBoundary, int zoomLevel);

		layoutCoordinateConverter(const layoutCoordinateConverter& copy)
		{
			_zoomLevel = copy.getZoomLevel();
		}

		~layoutCoordinateConverter();

		gridRect getLayoutBoundary() const;
		simpleRectangle<int> getLayoutBoundaryUI() const;
		simpleRectangle<float> getLayoutBoundaryUIReal() const;

		gridRect convertToGridRect(const simpleRectangle<int> rectangle) const;

		simpleRectangle<int> getOutlineUI(const gridLocator& location) const;

		simpleRectangle<int> convertToUI(const gridRect& rect, bool moveToCellCenter = false) const;
		simpleLine<int> convertToUI(const gridLocatorEdge& edge, bool moveToCellCenter = false) const;
		simplePoint<int> convertToUI(const gridLocator& location, bool moveToCellCenter = false) const;

		simpleRectangle<float> convertToUIReal(const gridRect& edge, bool moveToCellCenter = false) const;
		simpleLine<float> convertToUIReal(const gridLocatorEdge& edge, bool moveToCellCenter = false) const;
		simplePoint<float> convertToUIReal(const gridLocator& location, bool moveToCellCenter = false) const;

		gridLocator convertUIToGrid(const simplePoint<int> pointUI, bool centerOffsetUsed) const;
		gridLocatorEdge convertUIToGrid(const simpleLine<int> lineUI, bool centerOffsetUsed) const;

		gridLocator convertUIRealToGrid(const simplePoint<float> pointUIReal, bool centerOffsetUsed) const;
		gridLocatorEdge convertUIRealToGrid(const simpleLine<float> lineUIReal, bool centerOffsetUsed) const;

	public:
		int getZoomLevel() const
		{
			return _zoomLevel;
		}

	private:
		gridRect* _parentBoundary;
		int _zoomLevel;
	};

	layoutCoordinateConverter::layoutCoordinateConverter(const gridRect& parentBoundary, int zoomLevel)
	{
		_parentBoundary = new gridRect(parentBoundary);
		_zoomLevel = zoomLevel;
	}

	layoutCoordinateConverter::~layoutCoordinateConverter()
	{
		delete _parentBoundary;
	}

	gridRect layoutCoordinateConverter::getLayoutBoundary() const
	{
		return *_parentBoundary;
	}

	simpleRectangle<int> layoutCoordinateConverter::getLayoutBoundaryUI() const
	{
		return this->convertToUI(*_parentBoundary, false);
	}

	simpleRectangle<float> layoutCoordinateConverter::getLayoutBoundaryUIReal() const
	{
		return this->convertToUIReal(*_parentBoundary, false);
	}

	gridRect layoutCoordinateConverter::convertToGridRect(const simpleRectangle<int> rectangle) const
	{
		simpleRect rect = rectangle.getSimpleRect();

		return gridRect(rect);
	}

	simpleRectangle<int> layoutCoordinateConverter::getOutlineUI(const gridLocator& location) const
	{
		// Use SE cell to get the bottom right UI coordinate for this cell
		gridLocator southEast(location.column + 1, location.row + 1);

		simplePoint<int> topLeftUI = convertToUI(location, false);
		simplePoint<int> bottomRightUI = convertToUI(southEast, false);

		return simpleRectangle<int>(topLeftUI, bottomRightUI);
	}

	simplePoint<int> layoutCoordinateConverter::convertToUI(const gridLocator& location, bool moveToCellCenter) const
	{
		int centerOffsetX = moveToCellCenter ? brogueCellDisplay::CellWidth(_zoomLevel) / 2.0f : 0;
		int centerOffsetY = moveToCellCenter ? brogueCellDisplay::CellHeight(_zoomLevel) / 2.0f : 0;

		simplePoint<int> point(location.column * brogueCellDisplay::CellWidth(_zoomLevel) + centerOffsetX,
		                       location.row * brogueCellDisplay::CellHeight(_zoomLevel) + centerOffsetY);

		return point;
	}

	simpleLine<int> layoutCoordinateConverter::convertToUI(const gridLocatorEdge& edge, bool moveToCellCenter) const
	{
		simplePoint<int> point1 = convertToUI(edge.node1, moveToCellCenter);
		simplePoint<int> point2 = convertToUI(edge.node2, moveToCellCenter);

		return simpleLine<int>(point1, point2);
	}

	simpleRectangle<int> layoutCoordinateConverter::convertToUI(const gridRect& rect, bool moveToCellCenter) const
	{
		gridLocator topLeft = rect.topLeft();
		gridLocator bottomRight = rect.bottomRight();

		simplePoint<int> topLeftUI = convertToUI(topLeft, moveToCellCenter);
		simplePoint<int> bottomRightUI = convertToUI(bottomRight, moveToCellCenter);

		return simpleRectangle<int>(topLeftUI, bottomRightUI);
	}

	simpleRectangle<float> layoutCoordinateConverter::convertToUIReal(const gridRect& rect, bool moveToCellCenter) const
	{
		gridLocator topLeft = rect.topLeft();
		gridLocator bottomRight = rect.bottomRight();

		simplePoint<float> topLeftUIReal = convertToUIReal(topLeft, moveToCellCenter);
		simplePoint<float> bottomRightUIReal = convertToUIReal(bottomRight, moveToCellCenter);

		return simpleRectangle<float>(topLeftUIReal, bottomRightUIReal);
	}

	simpleLine<float> layoutCoordinateConverter::convertToUIReal(const gridLocatorEdge& edge,
	                                                             bool moveToCellCenter) const
	{
		simpleLine<int> line = convertToUI(edge, moveToCellCenter);

		// Flip y-coordinate to go from UI -> Real Numbers
		line.node1.y = line.node1.y * -1;
		line.node2.y = line.node2.y * -1;

		simplePoint<float> point1(line.node1.x, line.node1.y);
		simplePoint<float> point2(line.node2.x, line.node2.y);

		return simpleLine<float>(point1, point2);
	}

	simplePoint<float> layoutCoordinateConverter::convertToUIReal(const gridLocator& location,
	                                                              bool moveToCellCenter) const
	{
		simplePoint<int> point = convertToUI(location, moveToCellCenter);

		// Flip y-coordinate to go from UI -> Real Numbers
		point.y = point.y * -1;

		return simplePoint<float>(point.x, point.y);
	}

	gridLocator layoutCoordinateConverter::convertUIToGrid(const simplePoint<int> pointUI, bool centerOffsetUsed) const
	{
		int offsetX = centerOffsetUsed ? -1 * brogueCellDisplay::CellWidth(_zoomLevel) : 0;
		int offsetY = centerOffsetUsed ? -1 * brogueCellDisplay::CellHeight(_zoomLevel) : 0;

		gridLocator locator(simpleMath::floor(pointUI.x / brogueCellDisplay::CellWidth(_zoomLevel)),
		                    simpleMath::floor(pointUI.y / brogueCellDisplay::CellHeight(_zoomLevel)));

		return locator;
	}

	gridLocatorEdge layoutCoordinateConverter::convertUIToGrid(const simpleLine<int> lineUI,
	                                                           bool centerOffsetUsed) const
	{
		gridLocator location1 = convertUIToGrid(lineUI.node1, centerOffsetUsed);
		gridLocator location2 = convertUIToGrid(lineUI.node2, centerOffsetUsed);

		return gridLocatorEdge(location1, location2);
	}

	gridLocator layoutCoordinateConverter::convertUIRealToGrid(const simplePoint<float> pointUIReal,
	                                                           bool centerOffsetUsed) const
	{
		// Flip y-coordinate to go from Real Numbers -> UI coordinates
		simplePoint<int> pointUI(pointUIReal.x, -1 * pointUIReal.y);

		return convertUIToGrid(pointUI, centerOffsetUsed);
	}

	gridLocatorEdge layoutCoordinateConverter::convertUIRealToGrid(const simpleLine<float> lineUIReal,
	                                                               bool centerOffsetUsed) const
	{
		// Flip y-coordinate to go from Real Numbers -> UI coordinates
		simplePoint<int> point1UI(lineUIReal.node1.x, lineUIReal.node1.y * -1);
		simplePoint<int> point2UI(lineUIReal.node2.x, lineUIReal.node2.y * -1);

		simpleLine<int> lineUI(point1UI, point2UI);

		return convertUIToGrid(lineUI, centerOffsetUsed);
	}
}
