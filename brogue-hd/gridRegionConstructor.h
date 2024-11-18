#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "gridRegion.h"
#include "simple.h"
#include "simpleArray.h"
#include "simpleException.h"
#include "simpleHash.h"
#include "simpleMath.h"
#include <limits>

using namespace brogueHd::simple;

namespace brogueHd::component
{
	template<isGridLocator T>
	class gridRegionConstructor
	{
	public:

		/// <summary>
		/// Grid Region Constructor:  Flood fill mode ensures that a SINGLE region of connected cells is produced. This is
		/// the suggested usage. Otherwise, the location data is filled in during construction; and a potentially invalid
		/// region is created.
		/// </summary>
		gridRegionConstructor(gridRect parentBoundary, gridPredicate<T> inclusionPredicate);
		~gridRegionConstructor();

		/// <summary>
		/// Completes the region collections, finalizes boundary, and validates connectedness.
		/// </summary>
		gridRegion<T>* complete();

		/// <summary>
		/// Adds new cell to the region, and updates collections.
		/// </summary>
		void add(short column, short row, T item);

		/// <summary>
		/// Sets new cells in the grid using the parameterless constructor
		/// </summary>
		void add(const grid<T>& grid);

		/// <summary>
		/// Sets new cells in the grid using the parameterless constructor
		/// </summary>
		void add(T* itemArray);

		/// <summary>
		/// Checks to see whether or not the region constructor contains the specified item
		/// </summary>
		bool contains(T item);

		/// <summary>
		/// Checks to see whether the constructor is defined at the specified coordinates
		/// </summary>
		bool isDefined(short column, short row);

		///// <summary>
		///// Translates a gridRegion using the specified offset - creating a new gridRegion instance
		///// </summary>
		///// <param name="region"></param>
		///// <returns></returns>
		//static gridRegion<T>* translate(const gridRegion<T>& region, gridLocator translation);

	private:

		void completeImpl();
		bool isConnected(short column, short row, T item);
		void addEdges(short column, short row, T item);
		void addBoundary(short column, short row, T item);
		void validate();
		void validateRegionCollection(const char* name, simpleHash<T, T>* collection);
		gridRect calculateLargestRectangle();

	private:

		grid<T>* _grid;

		simpleHash<T, T>* _locations;
		simpleHash<T, T>* _edgeLocations;

		simpleHash<T, T>* _northEdges;
		simpleHash<T, T>* _southEdges;
		simpleHash<T, T>* _eastEdges;
		simpleHash<T, T>* _westEdges;

		simpleHash<T, T>* _nwCorners;
		simpleHash<T, T>* _neCorners;
		simpleHash<T, T>* _seCorners;
		simpleHash<T, T>* _swCorners;

		gridRect* _calculatedBoundary;

		short _top;
		short _left;
		short _right;
		short _bottom;

		bool _completed;

		gridPredicate<T>* _predicate;
	};

	template<isGridLocator T>
	gridRegionConstructor<T>::gridRegionConstructor(gridRect parentBoundary, gridPredicate<T> inclusionPredicate)
	{
		// This component is pretty much free-standing
		// _regionCentroidCalculator = IocContainer.Get<IRegionCentroidCalculator>();

		// Create a grid that is the size of the parent boundary - then make it smaller afterwards
		_grid = new grid<T>(parentBoundary, parentBoundary);

		_predicate = new gridPredicate<T>(inclusionPredicate);

		_locations = new simpleHash<T, T>();
		_edgeLocations = new simpleHash<T, T>();

		_northEdges = new simpleHash<T, T>();
		_southEdges = new simpleHash<T, T>();
		_eastEdges = new simpleHash<T, T>();
		_westEdges = new simpleHash<T, T>();

		_nwCorners = new simpleHash<T, T>();
		_neCorners = new simpleHash<T, T>();
		_seCorners = new simpleHash<T, T>();
		_swCorners = new simpleHash<T, T>();

		_top = std::numeric_limits<short>::max();
		_left = std::numeric_limits<short>::max();
		_right = std::numeric_limits<short>::min();
		_bottom = std::numeric_limits<short>::min();

		_completed = false;
		_calculatedBoundary = nullptr;
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

		return new gridRegion<T>(_locations->getKeys().toArray(),
								_edgeLocations->getKeys().toArray(),
								_westEdges->getKeys().toArray(),
								_northEdges->getKeys().toArray(),
								_eastEdges->getKeys().toArray(),
								_southEdges->getKeys().toArray(),
								_nwCorners->getKeys().toArray(),
								_neCorners->getKeys().toArray(),
								_seCorners->getKeys().toArray(),
								_swCorners->getKeys().toArray(),
								_grid->getParentBoundary(),
								*_calculatedBoundary,
								largestSubRegionRect);
	}

	template<isGridLocator T>
	void gridRegionConstructor<T>::add(short column, short row, T location)
	{
		if (_completed)
			throw simpleException("Trying to add location to a completed region constructor:  gridRegionConstructor.add");

		else if (_locations->contains(location))
			throw simpleException("Trying to add duplicate location to a region constructor:  gridRegionConstructor.add");

		else
		{
			if (_locations->count() > 0 && !isConnected(column, row, location))
				throw simpleException("Trying to add un-connected cell to the region:  gridRegionConstructor.add (try unsafe non-flood fill mode?)");

			// Keep locations up to date
			_grid->set(column, row, location);
			_locations->add(location, location);

			// Expand the boundary
			addBoundary(column, row, location);
		}
	}

	template<isGridLocator T>
	void gridRegionConstructor<T>::add(const grid<T>& grid)
	{
		gridRegionConstructor<T>* that = this;

		grid.iterate([&that] (short column, short row, T item)
		{
			if (item != NULL)
				that->add(column, row, item);

			return iterationCallback::iterate;
		});
	}

	template<isGridLocator T>
	void gridRegionConstructor<T>::add(T* locators)
	{
		for (int index = 0; index < sizeof(locators); index++)
		{
			this->add(locators[index].column, locators[index].row, locators[index]);
		}
	}

	template<isGridLocator T>
	void gridRegionConstructor<T>::completeImpl()
	{
		if (_completed)
			return;

		gridRegionConstructor<T>* that = this;

		_calculatedBoundary = new gridRect(_left, _top, _right - _left + 1, _bottom - _top + 1);

		// Complete the edge collections
		_locations->iterate([&that] (T key, T value)
		{
			gridLocator locator = (gridLocator)key;
			that->addEdges(locator.column, locator.row, key);

			return iterationCallback::iterate;
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
		if (!(*_predicate)(item.column, item.row, item))
			return false;

		// ONE of the adjacent elements MUST be in the grid - using the predicate to verify
		T north = _grid->getAdjacent(column, row, brogueCompass::N);
		T south = _grid->getAdjacent(column, row, brogueCompass::S);
		T east = _grid->getAdjacent(column, row, brogueCompass::E);
		T west = _grid->getAdjacent(column, row, brogueCompass::W);

		if (north != default_value::value<T>() && (*_predicate)(column, row - 1, north))
			return true;

		if (south != default_value::value<T>() && (*_predicate)(column, row + 1, south))
			return true;

		if (east != default_value::value<T>() && (*_predicate)(column + 1, row, east))
			return true;

		if (west != default_value::value<T>() && (*_predicate)(column - 1, row, west))
			return true;

		return false;
	}

	template<isGridLocator T>
	void gridRegionConstructor<T>::addEdges(short column, short row, T location)
	{
		// Edges and Corners
		if (_grid->isEdge(column, row) && (*_predicate)(column, row, location))
		{
			_edgeLocations->add(location, location);

			// Check for specificity:  Storing addition information about exposed edges, corners, and geometry

			// N
			if (_grid->isExposedEdge(column, row, brogueCompass::N, (*_predicate)))
				_northEdges->add(location, location);

			// S
			if (_grid->isExposedEdge(column, row, brogueCompass::S, (*_predicate)))
				_southEdges->add(location, location);

			// E
			if (_grid->isExposedEdge(column, row, brogueCompass::E, (*_predicate)))
				_eastEdges->add(location, location);

			// W
			if (_grid->isExposedEdge(column, row, brogueCompass::W, (*_predicate)))
				_westEdges->add(location, location);

			// NW Corner
			if (_grid->isExposedCorner(column, row, brogueCompass::NW, (*_predicate)))
				_nwCorners->add(location, location);

			// NE Corner
			if (_grid->isExposedCorner(column, row, brogueCompass::NE, (*_predicate)))
				_neCorners->add(location, location);

			// SE Corner
			if (_grid->isExposedCorner(column, row, brogueCompass::SE, (*_predicate)))
				_seCorners->add(location, location);

			// SW Corner
			if (_grid->isExposedCorner(column, row, brogueCompass::SW, (*_predicate)))
				_swCorners->add(location, location);
		}
	}

	template<isGridLocator T>
	void gridRegionConstructor<T>::addBoundary(short column, short row, T location)
	{
		if (column < _left)
			_left = location.column;

		if (column > _right)
			_right = location.column;

		if (row < _top)
			_top = location.row;

		if (row > _bottom)
			_bottom = location.row;
	}

	template<isGridLocator T>
	void gridRegionConstructor<T>::validate()
	{
		grid<T>* grid = _grid;

		_locations->forEach([&grid] (T key, T value)
		{
			if (grid->get(key.column, key.row) != key)
				simpleException::show("RegionConstructor grid was not valid:  Column={} Row={}", key.column, key.row);

			return iterationCallback::iterate;
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
	void gridRegionConstructor<T>::validateRegionCollection(const char* name, simpleHash<T, T>* collection)
	{
		if (collection->count() <= 0)
			simpleException::showCstr("Collection for building regions is not valid:  gridRegionConstructor.validate");
	}

	template<isGridLocator T>
	gridRect gridRegionConstructor<T>::calculateLargestRectangle()
	{
		if (!_completed)
			throw simpleException("Trying to run largest rectangle calculation before calling complete:  gridRegionConstructor.h");

		// Procedure
		// 
		// 1) For each row:  - Count across and add to row counters foreach non-null cell
		//                   - For each NULL cell -> reset the counters
		//                   - Then, take contiguous sums of counters with the Max(Min(counter value))
		//                     This represents a rectangle from the previous rows. Update this best fit.
		// 
		//
		// TODO: Use a stack-based approach to cut down on iterating

		gridRect regionBoundary = *_calculatedBoundary;
		short rowCountersLength = regionBoundary.width;
		simpleArray<short> rowCounters(rowCountersLength);
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
					rowCounters.set(index, rowCounters.get(index) + 1);
				else
					rowCounters.set(index, 0);
			}

			for (short index1 = 0; index1 < rowCountersLength; index1++)
			{
				// Initialize min-height for the next sweep
				short minHeight = rowCounters.get(index1);

				for (short index2 = index1; index2 < rowCountersLength && minHeight > 0; index2++)
				{
					minHeight = simpleMath::minOf(minHeight, rowCounters.get(index1), rowCounters.get(index2));

					// Current column against previous
					if (rowCounters.get(index1) > bestArea)
					{
						bestStartColumn = index1 + regionBoundary.left();
						bestEndColumn = index1 + regionBoundary.left();
						bestStartRow = row - rowCounters.get(index1) + 1;
						bestEndRow = row;

						bestArea = rowCounters.get(index1);
					}

					// Current column check
					if (rowCounters.get(index2) > bestArea)
					{
						bestStartColumn = index2 + regionBoundary.left();
						bestEndColumn = index2 + regionBoundary.left();
						bestStartRow = row - rowCounters.get(index2) + 1;
						bestEndRow = row;

						bestArea = rowCounters.get(index2);
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

		// Validation: Check boundaries; check that grid is defined
		//
		gridRegionConstructor<T>* that = this;
		gridRect result(bestStartColumn, bestStartRow, (bestEndColumn - bestStartColumn) + 1, (bestEndRow - bestStartRow) + 1);

		if (!_calculatedBoundary->contains(result))
			throw simpleException("Invalid sub-region rectangle calculation:  gridRegionConstructor::calculateLargestRectangle");

		result.iterate([&that] (short column, short row)
		{
			if (!that->isDefined(column, row))
				throw simpleException("Invalid sub-region rectangle calculation:  gridRegionConstructor::calculateLargestRectangle");

			return iterationCallback::iterate;
		});

		return result;
	}

	//template<isGridLocator T>
	//gridRegion<T>* gridRegionConstructor<T>::translate(const gridRegion<T>& region, gridLocator translation)
	//{
	//	gridRegionConstructor<T> constructor(region.getParentBoundary(), [] (short column, short row, T item)
	//	{
	//		return true;
	//	});

	//	// MODIFIES GRID LOCATORS!
	//	region.iterateLocations([&constructor, &translation] (short column, short row, T item)
	//	{
	//		short newColumn = item.column + translation.column;
	//		short newRow = item.row + translation.row;

	//		gridLocator locator = (gridLocator)(item);

	//		locator.column = newColumn;
	//		locator.row = newRow;

	//		constructor.add(newColumn, newRow, locator);

	//		return iterationCallback::iterate;
	//	});

	//	// Commit the translated region to memory - calculating the rest of the data
	//	return constructor.complete();
	//}
}

