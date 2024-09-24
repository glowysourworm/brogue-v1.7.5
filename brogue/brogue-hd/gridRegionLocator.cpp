#include "gridRegionLocator.h"
#include "gridRegionConstructor.h"
#include "mathdef.h"
#include "grid.h"
#include "exceptionHandler.h"
#include "gridDefinitions.h"
#include "extensionDefinitions.h"
#include <vector>

using namespace std;

using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model::construction
{
    template<gridCellConstraint T>
    gridRegionLocator<T>::gridRegionLocator()
    {
    }

    template<gridCellConstraint T>
    gridRegionLocator<T>::~gridRegionLocator()
    {
        
    }

    template<gridCellConstraint T>
    std::vector<gridRegion<T>*> gridRegionLocator<T>::locateRegions(const grid<T>& grid)
    {
        return locateRegions(grid, [](short column, short row, T value)
        {
            return value != NULL;
        });
    }

    template<gridCellConstraint T>
    std::vector<gridRegion<T>*> gridRegionLocator<T>::locateRegions(const grid<T>& grid, extensionDelegates<T>::simplePredicate inclusionPredicate)
    {
        std::vector<gridRegion<T>> result;

        // Procedure
        //
        // 1) Iterate the entire grid
        //      -> Callback (over history) to see if this location is in predicate
        //      -> Be sure we have not already visited this (new) region
        //
        iterate(grid, [](short column, short row)
        {
            // Check history
            for (int index = 0; index < result.size(); index++)
            {
                // Return from iterator (only)
                if (inclusionPredicate(result[i]->get(column, row)))
                    return iterationCallback::iterate;
            }

            // Check to see if we're at a valid location
            if (inclusionPredicate(grid[column, row]))
            {
                // Start a new region
                grid<T>* regionGrid = new grid<T>(_columns, _rows);

                // Keep track of the boundary (starts empty and expands)
                gridRect boundary(column, row, 0, 0);

                // Set initial value
                regionGrid->set(column, row, new T(column, row));

                // Recurse to fill out the grid
                gridRegionConstructor<T>* constructor = runFloodFill(regionGrid, column, row, inclusionPredicate);

                // Validate / Complete the region
                gridRegion<T>* region = constructor->complete();

                // Set result
                result.push_back(region);
            }
        });

        return result;
    }

    template<gridCellConstraint T>
    gridRegion<T>* gridRegionLocator<T>::identifyRegion(const grid<T>& grid, short column, short row, extensionDelegates<T>::simplePredicate inclusionPredicate)
    {
        if (!grid->isDefined(column, row))
            return NULL;

        // Create the constructor
        gridRegionConstructor<T>* constructor = this->runFloodFill(column, row, inclusionPredicate);

        // Validate -> Setup Region
        gridRegion<T>* finalRegion = constructor->complete();

        // MEMORY!
        delete constructor;

        return finalRegion;
    }

    template<gridCellConstraint T>
    gridRegionConstructor<T>* gridRegionLocator<T>::runFloodFill(const grid<T>& grid, short column, short row, extensionDelegates<T>::simplePredicate inclusionPredicate)
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
        T firstElement = grid->get(column, row);

        resultQueue.push_back(firstElement);
        regionGrid[column, row] = firstElement;

        // Collect the region data in a constructor
        gridRegionConstructor<T>* regionConstructor = new gridRegionConstructor(gridRect(column, row, 0, 0), inclusionPredicate);

        while (resultQueue.count() > 0)
        {
            // Centered Location
            T regionLocation = resultQueue.pop_front();

            if (regionLocations.find(regionLocation))
                brogueException::show("Trying to add location duplicate:  gridRegionLocator.runFloodFill");

            // Locations
            regionLocations.insert(regionLocation, regionLocation);

            // Add to region constructor
            regionConstructor.addCell(regionLocation);

            // Search cardinally adjacent cells (N,S,E,W)
            T north = grid->get(regionLocation.column, regionLocation.row - 1);
            T south = grid->get(regionLocation.column, regionLocation.row + 1);
            T east = grid->get(regionLocation.column + 1, regionLocation.row);
            T west = grid->get(regionLocation.column - 1, regionLocation.row);

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
                inclusionPredicate(north.column, north.row, north))
            {
                // Add location to region grid
                regionGrid[north.column][north.row] = north;

                // Push cell onto the queue to be iterated
                resultQueue.push_back(north);
            }

            // S
            if (south != null &&
                regionGrid[south.column][south.row] == NULL &&
                inclusionPredicate(south.column, south.row, south))
            {
                // Add location to region grid
                regionGrid[south.column][south.row] = south;

                // Push cell onto the queue to be iterated
                resultQueue.push_back(south);
            }

            // E
            if (east != null &&
                regionGrid[east.column][east.row] == NULL &&
                inclusionPredicate(east.column, east.row, east))
            {
                // Add location to region grid
                regionGrid[east.column][east.row] = east;

                // Push cell onto the queue to be iterated
                resultQueue.push_back(east);
            }

            // W
            if (west != null &&
                regionGrid[west.column][west.row] == NULL &&
                inclusionPredicate(west.column, west.row, west))
            {
                // Add location to region grid
                regionGrid[west.column][west.row] = west;

                // Push cell onto the queue to be iterated
                resultQueue.push_back(west);
            }
        }

        return regionConstructor;
    }
}