#pragma once

#include "gridRect.h"
#include "gridDefinitions.h"
#include "gridRegionConstructor.h"
#include "exceptionHandler.h"
#include "brogueMath.h"

using namespace std;

using namespace brogueHd::backend::math;
using namespace brogueHd::component;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model::construction
{
    template<isGridLocator T>
    gridRegionConstructor<T>::gridRegionConstructor(gridRect parentBoundary, gridDelegates<T>::predicate inclusionPredicate)
    {
        // This component is pretty much free-standing
        // _regionCentroidCalculator = IocContainer.Get<IRegionCentroidCalculator>();

        // Create a grid that is the size of the parent boundary - then make it smaller afterwards
        _grid = new grid<T>(parentBoundary.left(), parentBoundary.top());

        _predicate = inclusionPredicate;

        _top = std::numeric_limits<short>::max();
        _left = std::numeric_limits<short>::max();
        _right = std::numeric_limits<short>::min();
        _bottom = std::numeric_limits<short>::min();

        _completed = false;
    }

    template<isGridLocator T>
    gridRegionConstructor<T>::~gridRegionConstructor()
    {
        delete _grid;
    }

    template<isGridLocator T>
    gridRegion<T>* gridRegionConstructor<T>::complete()
    {
        completeImpl();
        validate();

        //var centroids = _regionCentroidCalculator.CalculateCentroidLocations(regionInfo);

        gridRect largestSubRegionRect = calculateLargestRectangle();

        return new gridRegion<T>(getKeys(_locations),
                                    getKeys(_edgeLocations),
                                    getKeys(_westEdges),
                                    getKeys(_northEdges),
                                    getKeys(_eastEdges),
                                    getKeys(_southEdges),
                                    getKeys(_nwCorners),
                                    getKeys(_neCorners),
                                    getKeys(_seCorners),
                                    getKeys(_swCorners),
                                    _grid->getParentBoundary(),
                                    _calculatedBoundary,
                                    largestSubRectangle);
    }

    template<isGridLocator T>
    void gridRegionConstructor<T>::add(short column, short row, T location)
    {
        if (_completed)
            brogueException::show("Trying to add location to a completed region constructor:  gridRegionConstructor.addCell");

        else if (_locations->containsKey(location))
            return;

        else
        {
            if (_locations->count() > 0 && !isConnected(location))
                brogueException::show("Trying to add un-connected cell to the region:  gridRegionConstructor.addCell");

            // Keep locations up to date
            _grid->set(column, row, location);
            _locations->add(location, location);

            // Expand the boundary
            addBoundary(location);
        }
    }

    template<isGridLocator T>
    void gridRegionConstructor<T>::add(const grid<T>& grid)
    {
        gridRegionCosntructor<T>* that = this;

        grid.iterate([&that](short column, short row, T item)
        {
            if (item != NULL)
                that->add(column, row, item);

            return iterationCallback::iterate;
        });
    }

    template<isGridLocator T>
    void gridRegionConstructor<T>::add(T* locators)
    {
        for (int index = 0; index < SIZEOF(locators); index++)
        {
            this->add(locators[index].column, locators[index].row, locators[index]);
        }
    }

    template<isGridLocator T>
    void gridRegionConstructor<T>::completeImpl()
    {
        if (_completed)
            return;

        _calculatedBoundary = gridRect(_left, _top, _right, _bottom);

        // Complete the edge collections
        _locations->iterate([](T key, T value)
        {
            addEdges(key);
        });

        _completed = true;
    }

    template<isGridLocator T>
    bool gridRegionConstructor<T>::isDefined(short column, short row)
    {
        return _grid->isDefined(column, row);
    }

    template<isGridLocator T>
    bool gridRegionConstructor<T>::contains(T item)
    {
        return _grid->isDefined(item.column, item.row) && (_grid->get(item.column, item.row) == item);
    }

    template<isGridLocator T>
    bool gridRegionConstructor<T>::isConnected(short column, short row, T item)
    {
        // ONE of the adjacent elements MUST be in the grid - using the predicate to verify
        T north = _grid.getAdjacent(column, row, brogueCompass::N);
        T south = _grid.getAdjacent(column, row, brogueCompass::S);
        T east = _grid.getAdjacent(column, row, brogueCompass::E);
        T west = _grid.getAdjacent(column, row, brogueCompass::W);

        if (north != NULL && _predicate(column, row - 1, north))
            return true;

        if (south != NULL && _predicate(column, row + 1, south))
            return true;

        if (east != NULL && _predicate(column + 1, row, east))
            return true;

        if (west != NULL && _predicate(column - 1, row, west))
            return true;

        return false;
    }

    template<isGridLocator T>
    void gridRegionConstructor<T>::addEdges(short column, short row, T location)
    {
        // Edges and Corners
        if (_grid->isEdge(column, row, _predicate))
        {
            _edgeLocations->add(location, location);

            // Check for specificity:  Storing addition information about exposed edges, corners, and geometry

            // N
            if (_grid->isExposedEdge(column, row, brogueCompass::N, _predicate))
                _northEdges->add(location, location);

            // S
            if (_grid->isExposedEdge(column, row, brogueCompass::S, _predicate))
                _southEdges->add(location, location);

            // E
            if (_grid->isExposedEdge(column, row, brogueCompass::E, _predicate))
                _eastEdges->add(location, location);

            // W
            if (_grid->isExposedEdge(column, row, brogueCompass::W, _predicate))
                _westEdges->add(location, location);

            // NW Corner
            if (_grid->isExposedCorner(column, row, brogueCompass::NW, _predicate))
                _nwCorners->add(location, location);

            // NE Corner
            if (_grid->isExposedCorner(column, row, brogueCompass::NE, _predicate))
                _neCorners->add(location, location);

            // SE Corner
            if (_grid->isExposedCorner(column, row, brogueCompass::SE, _predicate))
                _seCorners->add(location, location);

            // SW Corner
            if (_grid->isExposedCorner(column, row, brogueCompass::SW, _predicate))
                _swCorners->add(location, location);
        }
    }

    template<isGridLocator T>
    void gridRegionConstructor<T>::addBoundary(short column, short row, T location)
    {
        if (column < _left)
            _left = location.Column;

        if (column > _right)
            _right = location.Column;

        if (row < _top)
            _top = location.Row;

        if (row > _bottom)
            _bottom = location.Row;
    }

    template<isGridLocator T>
    void gridRegionConstructor<T>::validate()
    {
        _locations->forEach([](T key, T value)
        {
            if (_grid->get(key.column, key.row) != key)
                brogueException::show("RegionConstructor grid was not valid:  " + key.getString());
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

    template<isGridLocator T>
    void gridRegionConstructor<T>::validateRegionCollection(simpleHash<T, T>* collection)
    {
        if (collection->count() <= 0)
            brogueException::show("Collection for building regions is not valid:  gridRegionConstructor.validate");
    }

    template<isGridLocator T>
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
                if (_grid->isDefined(column, row))
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
                    minHeight = brogueMath<short>::min(minHeight, rowCounters[index1], rowCounters[index2]);

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