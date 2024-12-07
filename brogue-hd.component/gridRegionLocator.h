#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "gridRegion.h"
#include "gridRegionConstructor.h"
#include <simple.h>
#include <simpleException.h>
#include <simpleList.h>

namespace brogueHd::component
{
	using namespace simple;

	template<typename T>
	class gridRegionLocator
	{
	public:
		gridRegionLocator();
		~gridRegionLocator();

		/// <summary>
		/// Runs flood fill on the grid; and creates grid region instances.
		/// </summary>
		/// <returns>Completed region instanc vector for the grid</returns>
		
		simpleList<gridRegion*> locateRegions(const grid<T>& grid);

		/// <summary>
		/// Creates regions from the grid using flood fill iteration. The region predicate is used to select
		/// whether a particular cell is in / out of the region.
		/// </summary>
		simpleList<gridRegion*> locateRegions(const grid<T>& grid, gridPredicate<T> inclusionPredicate);

		/// <summary>
		/// Creates region at the specified point (or NULL)
		/// </summary>
		gridRegion* identifyRegion(const grid<T>& grid, int column, int row, gridPredicate<T> inclusionPredicate);

	private:
		gridRegionConstructor* runFloodFill(const grid<T>& grid, int column, int row,
		                                       gridPredicate<T> inclusionPredicate);
	};

	template <typename T>
	gridRegionLocator<T>::gridRegionLocator()
	{
	}

	template <typename T>
	gridRegionLocator<T>::~gridRegionLocator()
	{
	}

	template <typename T>
	simpleList<gridRegion*> gridRegionLocator<T>::locateRegions(const grid<T>& grid)
	{
		return locateRegions(grid, [](int column, int row, const T& value)
		{
			return value != default_value::value<T>();
		});
	}

	template <typename T>
	simpleList<gridRegion*> gridRegionLocator<T>::locateRegions(const grid<T>& grid, gridPredicate<T> inclusionPredicate)
	{
		simpleList<gridRegion*> result;

		// Procedure
		//
		// 1) Iterate the entire grid
		//      -> Callback (over history) to see if this location is in predicate
		//      -> Be sure we have not already visited this (new) region
		//

		gridRegionLocator<T>* that = this;

		grid.iterate([&inclusionPredicate, &result, &grid, &that](int column, int row, T item)
		{
			// Check history (to see if we're still inside the previous region)
			for (int index = 0; index < result.count(); index++)
			{
				// Conditions:
				//
				// 1) Previously identified this location with another region
				// 2) This location is not included by the user's predicate
				//
				if (result.get(index)->isDefined(column, row) ||
					!inclusionPredicate(column, row, grid.get(column, row)))
					return iterationCallback::iterate;
			}

			// Check to see if we're at a valid location (that has not been processed)
			if (inclusionPredicate(column, row, grid.get(column, row)))
			{
				// Recurse to fill out the grid
				gridRegionConstructor* constructor = that->runFloodFill(grid, column, row, inclusionPredicate);

				// (MEMORY!) Validate / Complete the region
				gridRegion* region = constructor->complete();

				delete constructor;

				// Set result
				result.add(region);
			}

			return iterationCallback::iterate;
		});

		return result;
	}

	template <typename T>
	gridRegion* gridRegionLocator<T>::identifyRegion(const grid<T>& grid, int column, int row, gridPredicate<T> inclusionPredicate)
	{
		if (!grid.isDefined(column, row))
			return nullptr;

		// (MEMORY!) Create the constructor
		gridRegionConstructor* constructor = this->runFloodFill(grid, column, row, inclusionPredicate);

		// (MEMORY!) Validate -> Setup Region
		gridRegion* finalRegion = constructor->complete();

		delete constructor;

		return finalRegion;
	}

	template <typename T>
	gridRegionConstructor* gridRegionLocator<T>::runFloodFill(const grid<T>& grid, int column, int row, gridPredicate<T> inclusionPredicate)
	{
		if (!grid.isDefined(column, row) || !inclusionPredicate(column, row, grid.get(column, row)))
			throw simpleException("Trying to start FloodFill in non-region location");

		// (MEMORY!) Collect the region data in a constructor
		gridRegionConstructor* regionConstructor = new gridRegionConstructor(grid.getParentBoundary(), inclusionPredicate, false);

		// Use queue to know what locations have been verified. Starting with test location - continue 
		// until all connected cells have been added to the resulting region using the predicate.
		simpleList<gridLocator> locatorQueue;

		// Process the first location
		T firstElement = grid.get(column, row);

		regionConstructor->add(column, row, firstElement);
		locatorQueue.add(gridLocator(column, row));

		while (locatorQueue.count() > 0)
		{
			// Centered Location
			gridLocator regionLocation = locatorQueue.removeAt(0);

			//if (regionConstructor.contains(regionLocation))
			//	throw simpleException("Trying to add location duplicate:  gridRegionLocator.runFloodFill");

			// Add to region constructor (also prevents requeueing)
			//regionConstructor.add(regionLocation.column, regionLocation.row, regionLocation);

			// Search cardinally adjacent cells (N,S,E,W)
			T north = grid.isInBounds(regionLocation.column, regionLocation.row - 1)
				          ? grid.get(regionLocation.column, regionLocation.row - 1)
				          : default_value::value<T>();
			T south = grid.isInBounds(regionLocation.column, regionLocation.row + 1)
				          ? grid.get(regionLocation.column, regionLocation.row + 1)
				          : default_value::value<T>();
			T east = grid.isInBounds(regionLocation.column + 1, regionLocation.row)
				         ? grid.get(regionLocation.column + 1, regionLocation.row)
				         : default_value::value<T>();
			T west = grid.isInBounds(regionLocation.column - 1, regionLocation.row)
				         ? grid.get(regionLocation.column - 1, regionLocation.row)
				         : default_value::value<T>();

			gridLocator northLocation(regionLocation.column, regionLocation.row - 1);
			gridLocator southLocation(regionLocation.column, regionLocation.row + 1);
			gridLocator eastLocation(regionLocation.column + 1, regionLocation.row);
			gridLocator westLocation(regionLocation.column - 1, regionLocation.row);

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
			if (north != default_value::value<T>() &&
				!regionConstructor->isDefined(northLocation.column, northLocation.row) &&
				inclusionPredicate(northLocation.column, northLocation.row, north))
			{
				// Add to region constructor (also prevents requeueing)
				regionConstructor->add(northLocation.column, northLocation.row, north);

				// Push cell onto the queue to be iterated
				locatorQueue.add(northLocation);
			}

			// S
			if (south != default_value::value<T>() &&
				!regionConstructor->isDefined(southLocation.column, southLocation.row) &&
				inclusionPredicate(southLocation.column, southLocation.row, south))
			{
				// Add to region constructor (also prevents requeueing)
				regionConstructor->add(southLocation.column, southLocation.row, south);

				// Push cell onto the queue to be iterated
				locatorQueue.add(southLocation);
			}

			// E
			if (east != default_value::value<T>() &&
				!regionConstructor->isDefined(eastLocation.column, eastLocation.row) &&
				inclusionPredicate(eastLocation.column, eastLocation.row, east))
			{
				// Add to region constructor (also prevents requeueing)
				regionConstructor->add(eastLocation.column, eastLocation.row, east);

				// Push cell onto the queue to be iterated
				locatorQueue.add(eastLocation);
			}

			// W
			if (west != default_value::value<T>() &&
				!regionConstructor->isDefined(westLocation.column, westLocation.row) &&
				inclusionPredicate(westLocation.column, westLocation.row, west))
			{
				// Add to region constructor (also prevents requeueing)
				regionConstructor->add(westLocation.column, westLocation.row, west);

				// Push cell onto the queue to be iterated
				locatorQueue.add(westLocation);
			}
		}

		return regionConstructor;
	}
}
