#include "gridRegionLocator.h"
#include "gridRegionConstructor.h"
#include "grid.h"
#include "exceptionHandler.h"
#include "gridDefinitions.h"
#include "simpleList.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model::construction
{
    template<isGridLocator T>
    gridRegionLocator<T>::gridRegionLocator()
    {
    }

    template<isGridLocator T>
    gridRegionLocator<T>::~gridRegionLocator()
    {
        
    }

    template<isGridLocator T>
    simpleList<gridRegion<T>*> gridRegionLocator<T>::locateRegions(const grid<T>& grid)
    {
        return locateRegions(grid, [](short column, short row, T value)
        {
            return value != NULL;
        });
    }

    template<isGridLocator T>
    simpleList<gridRegion<T>*> gridRegionLocator<T>::locateRegions(const grid<T>& grid, gridDelegates<T>::predicate inclusionPredicate)
    {
        simpleList<gridRegion<T>*> result;

        // Procedure
        //
        // 1) Iterate the entire grid
        //      -> Callback (over history) to see if this location is in predicate
        //      -> Be sure we have not already visited this (new) region
        //
        grid.iterate([&inclusionPredicate, &result, &grid](short column, short row, T item)
        {
            // Check history (to see if we're still inside the previous region)
            for (int index = 0; index < result.size(); index++)
            {
                // Conditions:
                //
                // 1) Previously identified this location with another region
                // 2) This location is not included by the user's predicate
                //
                if (result[index]->isDefined(column, row) ||
                   !inclusionPredicate(column, row, grid.get(column, row)))
                    return iterationCallback::iterate;
            }

            // Check to see if we're at a valid location (that has not been processed)
            if (inclusionPredicate(column, row, grid.get(column, row)))
            {
                // Recurse to fill out the grid
                gridRegionConstructor<T> constructor = runFloodFill(grid, column, row, inclusionPredicate);

                // Validate / Complete the region
                gridRegion<T>* region = constructor->complete();

                // Set result
                result.add(region);
            }

            return iterationCallback::iterate;
        });

        return result;
    }

    template<isGridLocator T>
    gridRegion<T>* gridRegionLocator<T>::identifyRegion(const grid<T>& grid, short column, short row, gridDelegates<T>::predicate inclusionPredicate)
    {
        if (!grid.isDefined(column, row))
            return NULL;

        // Create the constructor
        gridRegionConstructor<T> constructor = this->runFloodFill(grid, column, row, inclusionPredicate);

        // (MEMORY!) Validate -> Setup Region
        gridRegion<T>* finalRegion = constructor.complete();

        return finalRegion;
    }

    template<isGridLocator T>
    gridRegionConstructor<T> gridRegionLocator<T>::runFloodFill(const grid<T>& grid, short column, short row, gridDelegates<T>::predicate inclusionPredicate)
    {
        if (!grid.isDefined(column, row) || !inclusionPredicate(column, row, grid.get(column, row)))
            brogueException::show("Trying to start FloodFill in non-region location");

        // Collect the region data in a constructor
        gridRegionConstructor<T> regionConstructor(gridRect(column, row, 0, 0), inclusionPredicate);

        // Use queue to know what locations have been verified. Starting with test location - continue 
        // until all connected cells have been added to the resulting region using the predicate.
        simpleList<T> resultQueue;

        // Process the first location
        T firstElement = grid.get(column, row);

        regionConstructor.add(column, row, firstElement);
        resultQueue.add(firstElement);

        while (resultQueue.count() > 0)
        {
            // Centered Location
            T regionLocation = resultQueue.removeAt(0);

            if (regionConstructor.contains(regionLocation))
                brogueException::show("Trying to add location duplicate:  gridRegionLocator.runFloodFill");

            // Add to region constructor (also prevents requeueing)
            regionConstructor.add(regionLocation);

            // Search cardinally adjacent cells (N,S,E,W)
            T north = grid.get(regionLocation.column, regionLocation.row - 1);
            T south = grid.get(regionLocation.column, regionLocation.row + 1);
            T east = grid.get(regionLocation.column + 1, regionLocation.row);
            T west = grid.get(regionLocation.column - 1, regionLocation.row);

            // Procedure: DON'T SET RESULT ARRAYS HERE
            //
            // 1) Add location to the region constructor to prevent re-queueing
            // 2) Expand the region boundary
            // 3) Queue this adjacent location
            // 
            // Result Arrays are built by the region constructor afterwards
            //

            // Find Connected Cells

            // N
            if (north != NULL &&
                !regionConstructor.isDefined(north.column,north.row) &&
                inclusionPredicate(north.column, north.row, north))
            {
                // Push cell onto the queue to be iterated
                resultQueue.add(north);
            }

            // S
            if (south != NULL &&
                !regionConstructor.isDefined(south.column, south.row) &&
                inclusionPredicate(south.column, south.row, south))
            {
                // Push cell onto the queue to be iterated
                resultQueue.add(south);
            }

            // E
            if (east != NULL &&
                !regionConstructor.isDefined(east.column, east.row) &&
                inclusionPredicate(east.column, east.row, east))
            {
                // Push cell onto the queue to be iterated
                resultQueue.add(east);
            }

            // W
            if (west != NULL &&
                !regionConstructor.isDefined(west.column, west.row) &&
                inclusionPredicate(west.column, west.row, west))
            {
                // Push cell onto the queue to be iterated
                resultQueue.add(west);
            }
        }

        return regionConstructor;
    }
}