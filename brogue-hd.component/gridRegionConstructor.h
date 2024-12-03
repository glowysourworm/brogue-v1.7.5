#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "gridRegion.h"

#include <simple.h>
#include <simpleArray.h>
#include <simpleException.h>
#include <simpleExt.h>
#include <simpleHash.h>
#include <simpleMath.h>
#include <limits>

namespace brogueHd::component
{
	using namespace simple;

	template <isGridLocator T>
	class gridRegionConstructor
	{
	public:
		/// <summary>
		/// Grid Region Constructor:  Flood fill mode ensures that a SINGLE region of connected cells is produced. This is
		/// the suggested usage. Otherwise, the location data is filled in during construction; and a potentially invalid
		/// region is created.
		/// </summary>
		gridRegionConstructor(gridRect parentBoundary, gridPredicate<T> inclusionPredicate, bool unsafeMode);
		gridRegionConstructor(gridRect parentBoundary, bool unsafeMode);
		~gridRegionConstructor();

		/// <summary>
		/// Completes the region collections, finalizes boundary, and validates connectedness.
		/// </summary>
		gridRegion<T>* complete();

		/// <summary>
		/// Adds new cell to the region, and updates collections.
		/// </summary>
		void add(int column, int row, T item);

		/// <summary>
		/// Checks to see whether or not the region constructor contains the specified item
		/// </summary>
		bool contains(T item) const;

		/// <summary>
		/// Checks to see whether the constructor is defined at the specified coordinates
		/// </summary>
		bool isDefined(int column, int row) const;

	private:
		void completeImpl();
		bool isConnected(int column, int row, T item);
		void addEdges(int column, int row, T item);
		void addBoundary(int column, int row, T item);
		void validate();
		void validateRegionCollection(const char* name, simpleHash<T, T>* collection);

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

		int _top;
		int _left;
		int _right;
		int _bottom;

		bool _completed;
		bool _unsafeMode;

		gridPredicate<T>* _predicate;
	};

	template <isGridLocator T>
	gridRegionConstructor<T>::gridRegionConstructor(gridRect parentBoundary, bool unsafeMode) :
		gridRegionConstructor(parentBoundary, [](int column, int row, const T& item)
		{
			return true; // Default inclusion predicate
		}, unsafeMode)
	{
	}

	template <isGridLocator T>
	gridRegionConstructor<T>::gridRegionConstructor(gridRect parentBoundary, gridPredicate<T> inclusionPredicate,
	                                                bool unsafeMode)
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

		_top = std::numeric_limits<int>::max();
		_left = std::numeric_limits<int>::max();
		_right = std::numeric_limits<int>::min();
		_bottom = std::numeric_limits<int>::min();

		_completed = false;
		_unsafeMode = unsafeMode;
		_calculatedBoundary = nullptr;
	}

	template <isGridLocator T>
	gridRegionConstructor<T>::~gridRegionConstructor()
	{
		delete _grid;
		delete _predicate;

		delete _locations;
		delete _edgeLocations;

		delete _northEdges;
		delete _southEdges;
		delete _eastEdges;
		delete _westEdges;

		delete _nwCorners;
		delete _neCorners;
		delete _seCorners;
		delete _swCorners;
	}

	template <isGridLocator T>
	gridRegion<T>* gridRegionConstructor<T>::complete()
	{
		completeImpl();
		validate();

		gridRect largestSubRegionRect = _grid->calculateLargestRectangle(gridRect(0, 0, 1, 1));

		simpleArray<T> locations = _locations->getKeys().toArray();
		simpleArray<T> edgeLocations = _edgeLocations->getKeys().toArray();

		simpleArray<T> edgesN = _northEdges->getKeys().toArray();
		simpleArray<T> edgesS = _southEdges->getKeys().toArray();
		simpleArray<T> edgesE = _eastEdges->getKeys().toArray();
		simpleArray<T> edgesW = _westEdges->getKeys().toArray();

		simpleArray<T> cornersNE = _neCorners->getKeys().toArray();
		simpleArray<T> cornersNW = _nwCorners->getKeys().toArray();
		simpleArray<T> cornersSE = _seCorners->getKeys().toArray();
		simpleArray<T> cornersSW = _swCorners->getKeys().toArray();

		gridRect parentBoundary = _grid->getParentBoundary();
		gridRect boundary = *_calculatedBoundary;

		// (MEMORY!)
		gridRegion<T>* result = new gridRegion<T>(locations, edgeLocations,
		                                          edgesN, edgesS, edgesE, edgesW,
		                                          cornersNE, cornersNW, cornersSE, cornersSW,
		                                          parentBoundary, boundary, largestSubRegionRect);

		return result;
	}

	template <isGridLocator T>
	void gridRegionConstructor<T>::add(int column, int row, T location)
	{
		if (_completed)
			throw simpleException(
				"Trying to add location to a completed region constructor:  gridRegionConstructor.add");

		else if (_locations->contains(location))
			throw simpleException(
				"Trying to add duplicate location to a region constructor:  gridRegionConstructor.add");

		else
		{
			// Unsafe Mode:  Doesn't check adjacency while adding locations
			if (!_unsafeMode)
			{
				if (_locations->count() > 0 && !isConnected(column, row, location))
					throw simpleException(
						"Trying to add un-connected cell to the region:  gridRegionConstructor.add (try unsafe non-flood fill mode?)");
			}

			// Keep locations up to date
			_grid->set(column, row, location);
			_locations->add(location, location);

			// Expand the boundary
			addBoundary(column, row, location);
		}
	}

	template <isGridLocator T>
	void gridRegionConstructor<T>::completeImpl()
	{
		if (_completed)
			return;

		gridRegionConstructor<T>* that = this;

		_calculatedBoundary = new gridRect(_left, _top, _right - _left + 1, _bottom - _top + 1);

		// Complete the edge collections
		_locations->iterate([&that](T key, T value)
		{
			gridLocator locator = (gridLocator)key;
			that->addEdges(locator.column, locator.row, key);

			return iterationCallback::iterate;
		});

		_completed = true;
	}

	template <isGridLocator T>
	bool gridRegionConstructor<T>::isDefined(int column, int row) const
	{
		return _grid->isDefined(column, row);
	}

	template <isGridLocator T>
	bool gridRegionConstructor<T>::contains(T item) const
	{
		return _grid->isDefined(item.column, item.row) && (_grid->get(item.column, item.row) == item);
	}

	template <isGridLocator T>
	bool gridRegionConstructor<T>::isConnected(int column, int row, T item)
	{
		if (!(*_predicate)(item.column, item.row, item))
			return false;

		// ONE of the adjacent elements MUST be in the grid - using the predicate to verify
		T north = _grid->getAdjacentUnsafe(column, row, brogueCompass::N);
		T south = _grid->getAdjacentUnsafe(column, row, brogueCompass::S);
		T east = _grid->getAdjacentUnsafe(column, row, brogueCompass::E);
		T west = _grid->getAdjacentUnsafe(column, row, brogueCompass::W);

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

	template <isGridLocator T>
	void gridRegionConstructor<T>::addEdges(int column, int row, T location)
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

	template <isGridLocator T>
	void gridRegionConstructor<T>::addBoundary(int column, int row, T location)
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

	template <isGridLocator T>
	void gridRegionConstructor<T>::validate()
	{
		grid<T>* grid = _grid;

		_locations->forEach([&grid](T key, T value)
		{
			if (grid->get(key.column, key.row) != key)
				throw simpleException(simpleExt::format("RegionConstructor grid was not valid:  Column={} Row={}",
				                                        key.column, key.row));

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

	template <isGridLocator T>
	void gridRegionConstructor<T>::validateRegionCollection(const char* name, simpleHash<T, T>* collection)
	{
		if (collection->count() <= 0)
			throw simpleException("Collection for building regions is not valid:  gridRegionConstructor.validate");
	}
}
