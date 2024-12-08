#pragma once

#include "brogueCellDisplay.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRect.h"

#include <simpleLine.h>
#include <simpleMath.h>
#include <simplePoint.h>
#include <simpleRectangle.h>
#include <simplePolygon.h>

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;

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
		simpleLine<int> convertToUI(const gridLocator& location1, const gridLocator& location2, bool moveToCellCenter = false) const;
		simplePoint<int> convertToUI(const gridLocator& location, bool moveToCellCenter = false) const;

		simplePolygon<float>* convertToUIReal(const simplePolygon<int>*& polygonUI) const;
		simpleLine<float> convertToUIReal(const simpleLine<int>& lineUI) const;
		simplePoint<float> convertToUIReal(const simplePoint<int>& pointUI) const;
		simpleRectangle<float> convertToUIReal(const gridRect& edge, bool moveToCellCenter = false) const;
		simpleLine<float> convertToUIReal(const gridLocator& location1, const gridLocator& location2, bool moveToCellCenter = false) const;
		simplePoint<float> convertToUIReal(const gridLocator& location, bool moveToCellCenter = false) const;

		gridLocator convertUIToGrid(const simplePoint<int> pointUI, bool centerOffsetUsed) const;
		gridLocator convertUIRealToGrid(const simplePoint<float> pointUIReal, bool centerOffsetUsed) const;

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

	simpleLine<int> layoutCoordinateConverter::convertToUI(const gridLocator& location1, const gridLocator& location2, bool moveToCellCenter) const
	{
		simplePoint<int> point1 = convertToUI(location1, moveToCellCenter);
		simplePoint<int> point2 = convertToUI(location2, moveToCellCenter);

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
	simplePolygon<float>* layoutCoordinateConverter::convertToUIReal(const simplePolygon<int>*& polygonUI) const
	{
		simpleList<simpleLine<float>> segments;

		for (int index = 0; index < polygonUI->segmentCount() ;index++)
		{
			simpleLine<float> segmentUIReal = convertToUIReal(polygonUI->getSegment(index));

			segments.add(segmentUIReal);
		}

		return new simplePolygon<float>(segments, polygonUI->isClosed());
	}
	simpleLine<float> layoutCoordinateConverter::convertToUIReal(const simpleLine<int>& lineUI) const
	{
		simplePoint<float> point1UIReal = convertToUIReal(lineUI.node1);
		simplePoint<float> point2UIReal = convertToUIReal(lineUI.node2);

		return simpleLine<float>(point1UIReal, point2UIReal);
	}
	simplePoint<float> layoutCoordinateConverter::convertToUIReal(const simplePoint<int>& pointUI) const
	{
		return simplePoint<float>(pointUI.x, pointUI.y * -1.0f);
	}
	simpleRectangle<float> layoutCoordinateConverter::convertToUIReal(const gridRect& rect, bool moveToCellCenter) const
	{
		gridLocator topLeft = rect.topLeft();
		gridLocator bottomRight = rect.bottomRight();

		simplePoint<float> topLeftUIReal = convertToUIReal(topLeft, moveToCellCenter);
		simplePoint<float> bottomRightUIReal = convertToUIReal(bottomRight, moveToCellCenter);

		return simpleRectangle<float>(topLeftUIReal, bottomRightUIReal);
	}

	simpleLine<float> layoutCoordinateConverter::convertToUIReal(const gridLocator& location1, const gridLocator& location2, bool moveToCellCenter) const
	{
		simpleLine<int> line = convertToUI(location1, location2, moveToCellCenter);

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

	gridLocator layoutCoordinateConverter::convertUIRealToGrid(const simplePoint<float> pointUIReal, bool centerOffsetUsed) const
	{
		// Flip y-coordinate to go from Real Numbers -> UI coordinates
		simplePoint<int> pointUI(pointUIReal.x, -1 * pointUIReal.y);

		return convertUIToGrid(pointUI, centerOffsetUsed);
	}
}
