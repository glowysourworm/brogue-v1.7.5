#include "gridRegionLocator.h"
#include "floodFillData.h"
#include "mathdef.h"
#include "grid.h"
#include "exceptionHandler.h"
#include "gridDefinitions.h"

using namespace std;

using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model::construction
{
    template<gridCellConstraint T>
    gridRegionLocator<T>::gridRegionLocator(const grid<T>*& grid)
    {
        _grid = grid;
    }

    template<gridCellConstraint T>
    gridRegionLocator<T>::~gridRegionLocator()
    {
        
    }

    template<gridCellConstraint T>
    std::vector<gridRegion<T>*> gridRegionLocator<T>::locateRegions()
    {
        
    }

    template<gridCellConstraint T>
    std::vector<gridRegion<T>*> gridRegionLocator<T>::locateRegions(gridDelegates::gridPredicate predicate)
    {

    }

    template<gridCellConstraint T>
    floodFillData<T> runFloodFill(short column, short row, gridDelegates::gridPredicate predicate)
    {
        if (!_grid->isDefined(column, row) || !predicate(column, row, _grid->get(column, row)))
            brogueException::show("Trying to start FloodFill in non-region location");

        // Region Data (use array2D to help calculate the region. The relative boundary must be calculated)
        array2D<T> regionGrid(parentBoundary, relativeBoundary);

        std::map<T, T> regionLocations;

        // Use queue to know what locations have been verified. Starting with test location - continue 
        // until all connected cells have been added to the resulting region using the predicate.
        std::list<T> resultQueue;

        // Process the first location
        T firstElement = _grid->get(column, row);

        resultQueue.push_back(firstElement);
        regionGrid[column, row] = firstElement;

        // Collect the region data in a constructor
        var regionConstructor = new RegionConstructor<T, TResult>(parentBoundary, predicate, selector);

        while (resultQueue.count() > 0)
        {
            // Centered Location
            T regionLocation = resultQueue.pop_front();

            if (regionLocations.find(regionLocation))
                brogueException::show("Trying to add location duplicate:  gridRegionLocator.runFloodFill");

            // Locations
            regionLocations.insert(regionLocation, regionLocation);

            // Add to region constructor
            regionConstructor.insert(regionLocation);

            // Search cardinally adjacent cells (N,S,E,W)
            T north = _grid->get(regionLocation.column, regionLocation.row - 1);
            T south = _grid->get(regionLocation.column, regionLocation.row + 1);
            T east = _grid->get(regionLocation.column + 1, regionLocation.row);
            T west = _grid->get(regionLocation.column - 1, regionLocation.row);

            // Procedure: DON'T SET RESULT ARRAYS HERE
            //
            // 1) Add location to the region grid to prevent re-queueing
            // 2) Expand the region boundary
            // 3) Queue this adjacent location
            //

            // Find Connected Cells

            // N
            if (north != null &&
                regionGrid[north.column][north.row] == NULL &&
                predicate(north.column, north.row, north))
            {
                // Add location to region grid
                regionGrid[north.column][north.row] = north;

                // Push cell onto the queue to be iterated
                resultQueue.push_back(north);
            }

            // S
            if (south != null &&
                regionGrid[south.column][south.row] == NULL &&
                predicate(south.column, south.row, south))
            {
                // Add location to region grid
                regionGrid[south.column][south.row] = south;

                // Push cell onto the queue to be iterated
                resultQueue.push_back(south);
            }

            // E
            if (east != null &&
                regionGrid[east.column][east.row] == NULL &&
                predicate(east.column, east.row, east))
            {
                // Add location to region grid
                regionGrid[east.column][east.row] = east;

                // Push cell onto the queue to be iterated
                resultQueue.push_back(east);
            }

            // W
            if (west != null &&
                regionGrid[west.column][west.row] == NULL &&
                predicate(west.column, west.row, west))
            {
                // Add location to region grid
                regionGrid[west.column][west.row] = west;

                // Push cell onto the queue to be iterated
                resultQueue.push_back(west);
            }
        }

        return regionConstructor;
    }

    template<gridCellConstraint T>
    bool gridRegionLocator<T>::validateFloodFill(const floodFillData<T>& data)
    {

    }

    template<gridCellConstraint T>
    void gridRegionLocator<T>::locateRegionRecurse(grid<T>* regionGrid, gridRect& boundary, short currentColumn, short currentRow, gridDelegates::gridPredicate predicate)
    {

    }
}