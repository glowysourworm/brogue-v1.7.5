#pragma once

#include <simple.h>
#include <simpleException.h>
#include <simpleHash.h>
#include <simpleLine.h>
#include <simpleOrderedList.h>

#include "grid.h"
#include "gridRegion.h"
#include "gridDefinitions.h"
#include "gridCellOutline.h"
#include "gridCellOutlineSegment.h"
#include "gridCellOutlineSegmentHorizontalComparer.h"
#include "gridCellOutlineSegmentVerticalComparer.h"
#include "layoutCoordinateConverter.h"

namespace brogueHd::component
{
    using namespace simple;
    using namespace simple::math;

    template<isGridLocator T>
	class regionOutlineGrid
	{
    public:
        regionOutlineGrid(gridRegion<T>* region, layoutCoordinateConverter* coordinateConverter);
        ~regionOutlineGrid();

        gridCellOutlineSegment getNorthmostSegment() const;

        int getSegmentCount(brogueCompass segmentFace) const;
        gridCellOutlineSegment getAnySegment(brogueCompass segmentFace) const;

        /// <summary>
        /// Returns true if the grid has a segment at the specified location, for the specified face
        /// </summary>
        bool hasSegment(int column, int row, brogueCompass segmentFace);

        gridCellOutlineSegment getSegment(int column, int row, brogueCompass segmentFace);

        void removeSegment(const gridCellOutlineSegment& segment);

    private:

        void initialize();

        /// <summary>
		/// Reduces the horizontal and vertical segments where they're connected
		/// </summary>
		/// <param name="collection">N, S, E, or W segment collection - ordered ascendingly</param>
        void reduceSegments(simpleOrderedList<gridCellOutlineSegment>* collection);

	private:

        layoutCoordinateConverter* _coordinateConverter;

        grid<gridCellOutline*>* _grid;

        gridRegion<T>* _region;

        int _zoomLevel;

        // This should be the first segment called to begin the algorithm
        gridCellOutlineSegment _northMostSegment;

        simpleHash<gridCellOutlineSegment, gridCellOutlineSegment>* _northSegments;
        simpleHash<gridCellOutlineSegment, gridCellOutlineSegment>* _southSegments;
        simpleHash<gridCellOutlineSegment, gridCellOutlineSegment>* _eastSegments;
        simpleHash<gridCellOutlineSegment, gridCellOutlineSegment>* _westSegments;
	};

    template<isGridLocator T>
    regionOutlineGrid<T>::regionOutlineGrid(gridRegion<T>* region, layoutCoordinateConverter* coordinateConverter)
    {
        _coordinateConverter = coordinateConverter;
        _grid = new grid<gridCellOutline*>(region->getParentBoundary(), region->getBoundary());
        _region = region;
        _zoomFactor = zoomFactor;

        _northSegments = new simpleHash<simpleLine<int>, simpleLine<int>>();
        _southSegments = new simpleHash<simpleLine<int>, simpleLine<int>>();
        _eastSegments = new simpleHash<simpleLine<int>, simpleLine<int>>();
        _westSegments = new simpleHash<simpleLine<int>, simpleLine<int>>();

        initialize();
    }

    template<isGridLocator T>
    regionOutlineGrid<T>::~regionOutlineGrid()
    {
        _grid->iterate([] (int column, int row, gridCellOutline* item)
        {
            delete item;
            return iterationCallback::iterate;
        });

        delete _grid;
        delete _northSegments;
        delete _southSegments;
        delete _eastSegments;
        delete _westSegments;
    }

    template<isGridLocator T>
    gridCellOutlineSegment regionOutlineGrid<T>::getNorthmostSegment() const
    {
        return _northMostSegment;
    }

    template<isGridLocator T>
    int regionOutlineGrid<T>::getSegmentCount(brogueCompass segmentFace) const
    {
	    switch (segmentFace)
	    {
	    case brogueCompass::N:
            return _northSegments->count();
	    case brogueCompass::S:
            return _southSegments->count();
	    case brogueCompass::E:
            return _eastSegments->count();
	    case brogueCompass::W:
            return _westSegments->count();
	    default:
            throw simpleException("Mishandled compass direction:  regionOutlineGrid.h");
	    }
    }

    template<isGridLocator T>
    bool regionOutlineGrid<T>::hasSegment(int column, int row, brogueCompass segmentFace)
    {
        if (!_grid->isDefined(column, row))
            return false;

        gridCellOutlineSegment segment = _grid->get(column, row)->getSegment(segmentFace);

        if (segment == default_value::value<gridCellOutlineSegment>())
            return false;

        if (_northSegments->contains(segment))
            return true;

        if (_southSegments->contains(segment))
            return true;

        if (_eastSegments->contains(segment))
            return true;

        if (_westSegments->contains(segment))
            return true;

        return false;
    }

    template<isGridLocator T>
    gridCellOutlineSegment regionOutlineGrid<T>::getSegment(int column, int row, brogueCompass segmentFace)
    {
        return _grid->get(column, row)->getSegment(segmentFace);
    }

    template<isGridLocator T>
    gridCellOutlineSegment regionOutlineGrid<T>::getAnySegment(brogueCompass segmentFace) const
    {
        switch (segmentFace)
        {
            case brogueCompass::N:
                return _northSegments->firstKey();
            case brogueCompass::S:
                return _southSegments->firstKey();
            case brogueCompass::E:
                return _eastSegments->firstKey();
            case brogueCompass::W:
                return _westSegments->firstKey();
            default:
                throw simpleException("Mishandled compass direction:  regionOutlineGrid.h");
        }
    }

    template<isGridLocator T>
    void regionOutlineGrid<T>::removeSegment(const gridCellOutlineSegment& segment)
    {
        if (_northSegments->contains(segment))
            _northSegments->remove(segment);

        else if (_southSegments->contains(segment))
            _southSegments->remove(segment);

        else if (_eastSegments->contains(segment))
            _eastSegments->remove(segment);

        else if (_westSegments->contains(segment))
            _westSegments->remove(segment);

        else
            throw simpleException("Segment not found in regionOutlineGrid:  regionOutlineGrid::removeSegment");
    }

    template<isGridLocator T>
    void regionOutlineGrid<T>::initialize()
    {
        // Sorting:
        //
        // 1) Create ordered lists of vertical / horizontal line segments
        //    ordered up the axes.
        //
        // 2) Hash these lists by the opposing coordinate on their vertices
        //
        // 3) Assemble the polygon outline by iterating the axes and connecting
        //    the vertices from these line segments.
        //

        simpleHash<int, simpleOrderedList<gridCellOutlineSegment>*> northSegments;
        simpleHash<int, simpleOrderedList<gridCellOutlineSegment>*> southSegments;
        simpleHash<int, simpleOrderedList<gridCellOutlineSegment>*> eastSegments;
        simpleHash<int, simpleOrderedList<gridCellOutlineSegment>*> westSegments;

        // Edges:  Ordered by angle with the X-axis. 
        for (int index = 0; index < _region->getEdgeLocationCount(); index++)
        {
            T location = _region->getEdgeLocation(index);

            // Get outline for this location's cell
            simpleRectangle<int> rectUI = _coordinateConverter->getOutlineUI(location);

            gridCellOutlineSegment north;
            gridCellOutlineSegment south;
            gridCellOutlineSegment east;
            gridCellOutlineSegment west;

            // ASSEMBLE VERTICALLY OR HORIZONTALLY (IN UI COORDINATES):  This will look for flags for exposed
            // faces. Each face will contribute a line segment to the outline. These will
            // be reduced afterwards by aggregating over each axis; and connecting endpoints to form
            // maximal segments per axis index.

            // N
            if (_region->isExposedEdge(location.column, location.row, brogueCompass::N))
            {
                // N FACE:  W -> E:  Top-Left, Top-Right
                north = gridCellOutlineSegment(rectUI.getTopLeft(), rectUI.getTopRight(), location, location);
            }

            // S
            if (_region->isExposedEdge(location.column, location.row, brogueCompass::S))
            {
                // S FACE: W -> E: Bottom-Left, Bottom-Right
                south = gridCellOutlineSegment(rectUI.getBottomLeft(), rectUI.getBottomRight(), location, location);
            }

            // E
            if (_region->isExposedEdge(location.column, location.row, brogueCompass::E))
            {
                // E FACE:  N -> S:  Top-Right, Bottom-Right
                east = gridCellOutlineSegment(rectUI.getTopRight(), rectUI.getBottomRight(), location, location);
            }

            // W
            if (_region->isExposedEdge(location.column, location.row, brogueCompass::W))
            {
                // W FACE:  N -> S: Top-Left, Bottom-Left
                west = gridCellOutlineSegment(rectUI.getTopLeft(), rectUI.getBottomLeft(), location, location);
            }

            // Initialize the data structures
            if (north != default_value::value<gridCellOutlineSegment>())
            {
                if (!northSegments.contains(location.row))
                    northSegments.add(location.row, new simpleOrderedList<gridCellOutlineSegment>(gridCellOutlineSegmentHorizontalComparer(true)));

                northSegments.get(location.row).add(north);
            }

            if (south != default_value::value<gridCellOutlineSegment>())
            {
                if (!southSegments.contains(location.row))
                    southSegments.add(location.row, new simpleOrderedList<gridCellOutlineSegment>(gridCellOutlineSegmentHorizontalComparer(true)));

                southSegments.get(location.row).add(south);
            }

            if (west != default_value::value<gridCellOutlineSegment>())
            {
                if (!westSegments.contains(location.row))
                    westSegments.add(location.row, new simpleOrderedList<gridCellOutlineSegment>(gridCellOutlineSegmentVerticalComparer(true)));

                westSegments.get(location.row).add(west);
            }

            if (east != default_value::value<gridCellOutlineSegment>())
            {
                if (!eastSegments.contains(location.row))
                    eastSegments.add(location.row, new simpleOrderedList<gridCellOutlineSegment>(gridCellOutlineSegmentVerticalComparer(true)));

                eastSegments.get(location.row).add(east);
            }
        }

        // Take ordered list of segments and connect them down the axis
        for (int column = _region.getBoundary().left(); column <= _region.getBoundary().right(); column++)
        {
            // E
            if (eastSegments.contains(column))
                reduceSegments(eastSegments.get(column));

            // W
            if (westSegments.contains(column))
                reduceSegments(westSegments.get(column));

            // Apply segments to the private collections
            if (eastSegments.contains(column))
            {
                for (int index = 0; index < eastSegments.count(); index++)
                    _eastSegments->add(eastSegments.get(index), eastSegments.get(index));
            }

            if (westSegments.contains(column))
            {
                for (int index = 0; index < westSegments.count(); index++)
                    _westSegments->add(westSegments.get(index), westSegments.get(index));
            }
        }

        for (int row = _region->getBoundary().top(); row <= _region->getBoundary().bottom(); row++)
        {
            // N
            if (northSegments.contains(row))
                reduceSegments(northSegments.get(row));

            // S
            if (southSegments.contains(row))
                reduceSegments(southSegments.get(row));

            // Apply segments to the private collections
            if (northSegments.contains(row))
            {
                for (int index = 0; index < northSegments.count(); index++)
                    _northSegments->add(northSegments.get(index), northSegments.get(index));
            }
            if (southSegments.contains(row))
            {
                for (int index = 0; index < southSegments.count(); index++)
                    _southSegments->add(southSegments.get(index), southSegments.get(index));
            }
        }

        grid<gridCellOutline*>* grid = _grid;

        // Add segments to the grid
        _northSegments->iterate([&grid] (const gridCellOutlineSegment& segment, const gridCellOutlineSegment& value)
        {
            for (index = segment.location1.column; index <= segment.location2.column; index++)
                grid->set(index, segment.location1.row, new gridCellOutline(segment, nullptr, nullptr, nullptr));
        });

        _southSegments->iterate([&grid] (const gridCellOutlineSegment& segment, const gridCellOutlineSegment& value)
        {
            for (int index = segment.location1.column; index <= segment.location2.column; index++)
            {
                if (_grid->get(index, segment.location1.row) == nullptr)
                    _grid->get(index, segment.location1.row) = new gridCellOutline(nullptr, segment, nullptr, nullptr);

                else
                    _grid->get(index, segment.location1.row)->setSegment(segment, brogueCompass::S);
            }
        });

        _eastSegments->iterate([&grid] (const gridCellOutlineSegment& segment, const gridCellOutlineSegment& value)
        {
            for (int index = segment.location1.row; index <= segment.location2.row; index++)
            {
                if (_grid->get(segment.location1.column, index) == nullptr)
                    _grid->get(segment.location1.column, index) = new gridCellOutline(nullptr, nullptr, segment, nullptr);

                else
                    _grid->get(segment.location1.column, index)->setSegment(segment, brogueCompass::E);
            }
        });

        _westSegments->iterate([&grid] (const gridCellOutlineSegment& segment, const gridCellOutlineSegment& value)
        {
            for (int index = segment.location1.row; index <= segment.location2.row; index++)
            {
                if (_grid->get(segment.location1.column, index) == nullptr)
                    _grid->get(segment.location1.column, index) = new gridCellOutline(nullptr, nullptr, nullptr, segment);

                else
                    _grid->get(segment.location1.column, index)->setSegment(segment, brogueCompass::W);
            }
        });

        // Set the north-most segment
        _northMostSegment = northSegments.get(_region->getBoundary().top())->get(0);

        // (Memory!) These lists can be easily deleted. The necessary data has been heap-allocated.
        northSegments.getValues().forEach([] (simpleOrderedList<gridCellOutlineSegment>* list)
        {
            delete list;
            return iterationCallback::iterate;
        });
        southSegments.getValues().forEach([] (simpleOrderedList<gridCellOutlineSegment>* list)
        {
            delete list;
            return iterationCallback::iterate;
        });
        eastSegments.getValues().forEach([] (simpleOrderedList<gridCellOutlineSegment>* list)
        {
            delete list;
            return iterationCallback::iterate;
        });
        westSegments.getValues().forEach([] (simpleOrderedList<gridCellOutlineSegment>* list)
        {
            delete list;
            return iterationCallback::iterate;
        });
    }

    /// <summary>
	/// Reduces the horizontal and vertical segments where they're connected
	/// </summary>
	/// <param name="collection">N, S, E, or W segment collection - ordered ascendingly</param>
	template<isGridLocator T>
    void regionOutlineGrid<T>::reduceSegments(simpleOrderedList<gridCellOutlineSegment>* collection)
    {
        for (int index = collection->count() - 1; index > 0; index--)
        {
            // Descending order (segment1 is the lower in UI coordinates)
            gridCellOutlineSegment segment1 = collection.get(index - 1);
            gridCellOutlineSegment segment2 = collection.get(index);

            if (segment1.vertex2 == segment2.vertex1))
            {
                // Remove UPPER segment (BEFORE MODIFYING!)
                collection->removeAt(index);

                // Update segment-1 (Which now spans the total length)
                segment1.vertex2 = segment2.vertex2;

                // (MODIFIED! This was not in the original!)
                segment1.location2 = segment2.location2;
            }
        }
    }
}