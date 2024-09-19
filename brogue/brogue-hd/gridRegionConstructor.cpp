#pragma once

#include "mapextension.h"
#include "gridRect.h"
#include "gridDefinitions.h"
#include "gridRegionConstructor.h"
#include "exceptionHandler.h"

using namespace brogueHd::backend::extension;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model::construction
{
    template<gridCellConstraint T>
    gridRegionConstructor<T>::gridRegionConstructor(gridRect parentBoundary, gridDelegates::gridPredicate inclusionPredicate)
    {
        // This component is pretty much free-standing
        // _regionCentroidCalculator = IocContainer.Get<IRegionCentroidCalculator>();

        // Create a grid that is the size of the parent boundary - then make it smaller afterwards
        _grid = new grid<T>(parentBoundary.left(), parentBoundary.top());

        _predicate = inclusionPredicate;

        _top = SHRT_MAX;
        _left = SHRT_MAX;
        _right = SHRT_MIN;
        _bottom = SHRT_MIN;

        _completed = false;
    }

    template<gridCellConstraint T>
    gridRegionConstructor<T>::~gridRegionConstructor()
    {
        delete _grid;
    }

    template<gridCellConstraint T>
    gridRegion<T>* gridRegionConstructor<T>::complete()
    {
        complete();
        validate();

        //var centroids = _regionCentroidCalculator.CalculateCentroidLocations(regionInfo);

        gridRect largestSubRegionRect = calculateLargestRectangle();

        return new gridRegion<TResult>(getKeys(_locations),
                                        getKeys(_edgeLocations),
                                        getKeys(_westEdges),
                                        getKeys(_northEdges),
                                        getKeys(_eastEdges),
                                        getKeys(_southEdges),
                                        getKeys(_nwCorners),
                                        getKeys(_neCorners),
                                        getKeys(_seCorners),
                                        getKeys(_swCorners),
                                        centroids.ToArray(),
                                        _grid->getBoundary(),
                                        _calculatedBoundary,
                                        largestSubRectangle);
    }

    template<gridCellConstraint T>
    void gridRegionConstructor<T>::addCell(T cell)
    {
        if (_completed)
            brogueException::show("Trying to add location to a completed region constructor:  gridRegionConstructor.addCell");

        else if (_locations.find(location) != NULL)
            return;

        else
        {
            if (_locations.count() > 0 && !isConnected(location))
                brogueException::show("Trying to add un-connected cell to the region:  gridRegionConstructor.addCell");

            // Keep locations up to date
            _grid[location] = location;
            _locations.insert(location, location);

            // Expand the boundary
            addBoundary(location);
        }
    }

    template<gridCellConstraint T>
    array2D<T>* gridRegionConstructor<T>::complete()
    {
        if (_completed)
            return;

        _calculatedBoundary = gridRect(_left, _top, _right, _bottom);

        // Create the final grid and edge grid according to the final boundary
        array2D<T> resultGrid(_parentBoundary, _calculatedBoundary);

        forEach(_locations, [](T key, T value) 
        {
            resultGrid[key.column, key.row] = key;
        });

        // Iterate twice to add the proper edges
        forEach(_locations, [](T key, T value)
        {
            addEdges(key);
        });

        _completed = true;

        return resultGrid;
    }

    template<gridCellConstraint T>
    bool gridRegionConstructor<T>::isConnected(T location)
    {
        // ONE of the adjacent elements MUST be in the grid - using the predicate to verify
        T north = _grid.getAdjacent(location.column, location.row, brogueCompass::N);
        T south = _grid.getAdjacent(location.column, location.row, brogueCompass::S);
        T east = _grid.getAdjacent(location.column, location.row, brogueCompass::E);
        T west = _grid.getAdjacent(location.column, location.row, brogueCompass::W);

        if (north != NULL && _predicate(north.column, north.row, north))
            return true;

        if (south != NULL && _predicate(south.column, south.row, south))
            return true;

        if (east != NULL && _predicate(east.column, east.row, east))
            return true;

        if (west != NULL && _predicate(west.column, west.row, west))
            return true;

        return false;
    }

    template<gridCellConstraint T>
    void gridRegionConstructor<T>::addEdges(T location)
    {
        // Edges and Corners
        if (_grid.isEdge(location.column, location.row, _predicate))
        {
            _edgeLocations.insert(location, location);

            // Check for specificity:  Storing addition information about exposed edges, corners, and geometry

            // N
            if (_grid->isExposedEdge(location.column, location.row, brogueCompass::N, _predicate))
                _northEdges.insert(location, location);

            // S
            if (_grid->isExposedEdge(location.column, location.row, brogueCompass::S, _predicate))
                _southEdges.insert(location, location);

            // E
            if (_grid->isExposedEdge(location.column, location.row, brogueCompass::E, _predicate))
                _eastEdges.insert(location, location);

            // W
            if (_grid->isExposedEdge(location.column, location.row, brogueCompass::W, _predicate))
                _westEdges.insert(location, location);

            // NW Corner
            if (_grid->isExposedCorner(location.column, location.row, brogueCompass::NW, _predicate))
                _nwCorners.insert(location, location);

            // NE Corner
            if (_grid->isExposedCorner(location.column, location.row, brogueCompass::NE, _predicate))
                _neCorners.insert(location, location);

            // SE Corner
            if (_grid->isExposedCorner(location.column, location.row, brogueCompass::SE, _predicate))
                _seCorners.insert(location, location);

            // SW Corner
            if (_grid->isExposedCorner(location.column, location.row, brogueCompass::SW, _predicate))
                _swCorners.insert(location, location);
        }
    }

    template<gridCellConstraint T>
    void gridRegionConstructor<T>::addBoundary(T location)
    {
        if (location.Column < _left)
            _left = location.Column;

        if (location.Column > _right)
            _right = location.Column;

        if (location.Row < _top)
            _top = location.Row;

        if (location.Row > _bottom)
            _bottom = location.Row;
    }

    template<gridCellConstraint T>
    void gridRegionConstructor<T>::validate()
    {
        forEach(_locations, [](T key, T value)
        {
            if (_grid->get(key.column, key.row) != key)
                brogueException::show("RegionConstructor grid was not valid:  " + location.getString());
        });

        // VALIDATE COLLECTIONS
        validateRegionCollection("Locations", _locations /*, collection = > collection.Count() > 0*/);
        validateRegionCollection("Edge Locations", _edgeLocations);
        validateRegionCollection("N Edge Locations", _northEdges);
        validateRegionCollection("S Edge Locations", _southEdges);
        validateRegionCollection("E Edge Locations", _eastEdges);
        validateRegionCollection("W Edge Locations", _westEdges);
        validateRegionCollection("NE Corner Locations", _neCorners);
        validateRegionCollection("NW Corner Locations", _nwCorners);
        validateRegionCollection("SE Corner Locations", _seCorners);
        validateRegionCollection("SW Corner Locations", _swCorners);
    }

    template<gridCellConstraint T>
    void gridRegionConstructor<T>::validateRegionCollection(std::map<T, T> collection)
    {
        if (collection.count() <= 0)
            brogueException::show("Collection for building regions is not valid:  gridRegionConstructor.validate");
    }

    template<gridCellConstraint T>
    gridRect gridRegionConstructor<T>::calculateLargestRectangle()
    {
        // Procedure
        // 
        // 1) For each row:  - Count across and add to row counters foreach non-null cell
        //                   - For each NULL cell -> reset the counters
        //                   - Then, take contiguous sums of counters with the Max(Min(counter value))
        //                     This represents a rectangle from the previous rows. Update this best fit.
        // 
        //
        // TODO: Use a stack-based approach to cut down on iterating

        gridRect regionBoundary = _calculatedBoundary;
        short rowCountersLength = regionBoundary.width;
        short rowCounters[rowCountersLength];
        short bestStartColumn = -1;
        short bestEndColumn = -1;
        short bestStartRow = -1;
        short bestEndRow = -1;
        short bestArea = 0;

        for (short row = regionBoundary.top(); row <= regionBoundary.bottom(); row++)
        {
            for (short column = regionBoundary.left(); column <= regionBoundary.right(); column++)
            {
                short index = column - regionBoundary.left();

                // FIRST, CHECK ROW INDEX TRACKING
                if (!_grid->isZeroValue(column, row))
                    rowCounters[index]++;
                else
                    rowCounters[index] = 0;
            }



            for (short index1 = 0; index1 < rowCountersLength; index1++)
            {
                // Initialize min-height for the next sweep
                short minHeight = rowCounters[index1];

                for (short index2 = index1; index2 < rowCountersLength && minHeight > 0; index2++)
                {
                    minHeight = min(minHeight, rowCounters[index1], rowCounters[index2]);

                    // Current column against previous
                    if (rowCounters[index1] > bestArea)
                    {
                        bestStartColumn = index1 + regionBoundary.left();
                        bestEndColumn = index1 + regionBoundary.left();
                        bestStartRow = row - rowCounters[index1] + 1;
                        bestEndRow = row;

                        bestArea = rowCounters[index1];
                    }

                    // Current column check
                    if (rowCounters[index2] > bestArea)
                    {
                        bestStartColumn = index2 + regionBoundary.left();
                        bestEndColumn = index2 + regionBoundary.left();
                        bestStartRow = row - rowCounters[index2] + 1;
                        bestEndRow = row;

                        bestArea = rowCounters[index2];
                    }

                    // Current min-block check
                    if (minHeight * ((index2 - index1) + 1) > bestArea)
                    {
                        bestStartColumn = index1 + regionBoundary.left();
                        bestEndColumn = index2 + regionBoundary.left();
                        bestStartRow = row - minHeight + 1;
                        bestEndRow = row;

                        bestArea = minHeight * ((index2 - index1) + 1);
                    }
                }
            }
        }

        return gridRect(bestStartColumn, bestStartRow, (bestEndColumn - bestStartColumn) + 1, (bestEndRow - bestStartRow) + 1);
    }
}