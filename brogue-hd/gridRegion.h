#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleArray.h"
#include "simpleException.h"
#include "simpleOrderedList.h"

using namespace brogueHd::simple;

namespace brogueHd::component
{
	/// <summary>
	/// Grid component that creates a subgrid, using the same coordinate space, from a parent grid. This
	/// means the result will have the same width and height; but will be a subgrid on a specified gridRect
	/// of the parent grid. Each function and call will be protected to utilize only this region. No grid
	/// offsets are required for use.
	/// </summary>
	template<isGridLocator T>
	class gridRegion : public hashableObject
	{
	public:
		gridRegion(const simpleArray<T>& locations,
					const simpleArray<T>& edgeLocations,
					const simpleArray<T>& northExposedLocations,
					const simpleArray<T>& southExposedLocations,
					const simpleArray<T>& eastExposedLocations,
					const simpleArray<T>& westExposedLocations,
					const simpleArray<T>& northEastCornerLocations,
					const simpleArray<T>& northWestCornerLocations,
					const simpleArray<T>& southEastCornerLocations,
					const simpleArray<T>& southWestCornerLocations,
					const gridRect& parentBoundary,
					const gridRect& relativeBoundary,
					const gridRect& largestRectangularSubRegion);


		~gridRegion();

		gridRect getBoundary() const;
		gridRect getParentBoundary() const;
		gridRect getLargestSubRectangle() const;
		simpleArray<T> getLocations() const;
		simpleArray<T> getEdgeLocations() const;

		int getLocationCount() const;

		/// <summary>
		/// MODIFIES LAYOUT! Translates the grid region data by the provided amount without
		/// re-allocating memory. (Assumes copied data)
		/// </summary>
		void translate_StackLike(int column, int row);

		/// <summary>
		/// MODIFIES LAYOUT! Translates the grid region data by the provided amount without
		/// re-allocating memory. (Assumes pointers)
		/// </summary>
		void translate_HeapLike(int column, int row);

		/// <summary>
		/// Checks for location overlap
		/// </summary>
		bool overlaps(gridRegion<T>* region) const;

	public:

		size_t getHash() const override
		{
			// TODO: Look for more efficient method. Probably, we want to avoid hashing values
			//		 when there is an available pointer. (SEE SIMPLE.H) (isPointer<T> overloads...)
			//
			//		 Actually, lets just use the locations pointer for now and see what other
			//		 methods make sense.
			//
			//		 (value hash is expensive) return _locations->getHash();	// TOO EXPENSIVE
			//

			return hashGenerator::generateHash(_locations);
		}

	public:

		/// <summary>
		/// Returns value from the underlying grid. This is the index space of the parent grid.
		/// </summary>
		T get(int column, int row) const;

		/// <summary>
		/// Returns true if a cell is defined for the region
		/// </summary>
		bool isDefined(int column, int row) const;

		/// <summary>
		/// Returns true if a cell is defined for the region
		/// </summary>
		bool isDefined(const gridLocator& locator) const;

		/// <summary>
		/// Returns true if an edge cell is defined
		/// </summary>
		bool isEdge(int column, int row) const;

		/// <summary>
		/// Iterates the locations of the region and calls the user method
		/// </summary>
		void iterateLocations(gridCallback<T> callback) const;

		/// <summary>
		/// Iterates the edge locations of the region and calls the user method
		/// </summary>
		void iterateEdges(gridCallback<T> callback) const;

		/// <summary>
		/// Gets locations with an exposed edge in the specified direction
		/// </summary>
		simpleArray<T> getEdges(brogueCompass direction) const;

		/// <summary>
		/// Gets the locations ordered by their distance to the boundary (closest first)
		/// </summary>
		simpleOrderedList<T> getBestEdges(brogueCompass direction) const;

		/// <summary>
		/// Gets locations with exposed corner
		/// </summary>
		simpleArray<T> getCorners(brogueCompass nonCardinalDirection) const;

	private:

		grid<T>* _grid;
		grid<T>* _edgeGrid;

		gridRect _largestRectangularSubRegion;

		simpleArray<T>* _locations;
		simpleArray<T>* _edgeLocations;

		// Exposed Edge Faces
		simpleArray<T>* _northExposedLocations;
		simpleArray<T>* _southExposedLocations;
		simpleArray<T>* _westExposedLocations;
		simpleArray<T>* _eastExposedLocations;

		// Corners
		simpleArray<T>* _northEastCornerLocations;
		simpleArray<T>* _northWestCornerLocations;
		simpleArray<T>* _southEastCornerLocations;
		simpleArray<T>* _southWestCornerLocations;
	};

	template<isGridLocator T>
	gridRegion<T>::gridRegion(const simpleArray<T>& locations,
								const simpleArray<T>& edgeLocations,
								const simpleArray<T>& northExposedLocations,
								const simpleArray<T>& southExposedLocations,
								const simpleArray<T>& eastExposedLocations,
								const simpleArray<T>& westExposedLocations,
								const simpleArray<T>& northEastCornerLocations,
								const simpleArray<T>& northWestCornerLocations,
								const simpleArray<T>& southEastCornerLocations,
								const simpleArray<T>& southWestCornerLocations,
								const gridRect& parentBoundary,
								const gridRect& relativeBoundary,
								const gridRect& largestRectangularSubRegion)
	{
		_grid = new grid<T>(parentBoundary, relativeBoundary);
		_edgeGrid = new grid<T>(parentBoundary, relativeBoundary);

		_largestRectangularSubRegion = largestRectangularSubRegion;

		_locations = new simpleArray<T>(locations);
		_edgeLocations = new simpleArray<T>(edgeLocations);

		_northExposedLocations = new simpleArray<T>(northExposedLocations);
		_southExposedLocations = new simpleArray<T>(southExposedLocations);
		_eastExposedLocations = new simpleArray<T>(eastExposedLocations);
		_westExposedLocations = new simpleArray<T>(westExposedLocations);

		_northEastCornerLocations = new simpleArray<T>(northEastCornerLocations);
		_northWestCornerLocations = new simpleArray<T>(northWestCornerLocations);
		_southEastCornerLocations = new simpleArray<T>(southEastCornerLocations);
		_southWestCornerLocations = new simpleArray<T>(southWestCornerLocations);

		// Initialize Grids
		for (int index = 0; index < locations.count(); index++)
		{
			T item = locations.get(index);

			// Validates that each cell is at a unique location
			_grid->set(item.column, item.row, item);
		}

		for (int index = 0; index < edgeLocations.count(); index++)
		{
			T item = edgeLocations.get(index);

			_edgeGrid->set(item.column, item.row, item);
		}
	}

	template<isGridLocator T>
	gridRegion<T>::~gridRegion()
	{
		// MEMORY!  gridRegionConstructor (new T) -> gridRegion(...)
		//			gridRegion.translate  (new T)

		// MEMORY!  HAVE TO DELETE gridCell instances...
		//for (int index = 0; index < _locations->size(); index++)
		//{
		//	delete (gridCell)_locations->get(index);  // ~gridCell, ~brogueCell
		//}

		delete _grid;
		delete _edgeGrid;

		delete _locations;
		delete _edgeLocations;

		delete _northExposedLocations;
		delete _southExposedLocations;
		delete _eastExposedLocations;
		delete _westExposedLocations;

		delete _northEastCornerLocations;
		delete _northWestCornerLocations;
		delete _southEastCornerLocations;
		delete _southWestCornerLocations;
	}

	template<isGridLocator T>
	T gridRegion<T>::get(int column, int row) const
	{
		return _grid->get(column, row);
	}

	template<isGridLocator T>
	int gridRegion<T>::getLocationCount() const
	{
		return _locations->count();
	}

	template<isGridLocator T>
	bool gridRegion<T>::isDefined(int column, int row) const
	{
		// Checking "bounds" for a region isn't really an appropriate meaning for
		// an amorphous region. The grid needs to be protected from out of bounds
		// indexing.
		//
		if (!_grid->isInBounds(column, row))
			return false;

		return _grid->isDefined(column, row);
	}

	template<isGridLocator T>
	bool gridRegion<T>::isDefined(const gridLocator& locator) const
	{
		return isDefined(locator.column, locator.row);
	}

	template<isGridLocator T>
	bool gridRegion<T>::isEdge(int column, int row) const
	{
		return _edgeGrid->isDefined(column, row);
	}

	template<isGridLocator T>
	void gridRegion<T>::translate_HeapLike(int column, int row)
	{
		// NOT SURE HOW THIS METHOD MIGHT WORK:  The template must understand
		// that T is a pointer; but that will take some more understanding of
		// C++ concepts.
		//

		// Translates the coordinate boundary (relative boundary)
		_grid->translate(column, row);

		_grid->iterate([] (int column, int row, T item)
		{
			item.translate(column, row);

			return iterationCallback::iterate;
		});
	}

	template<isGridLocator T>
	void gridRegion<T>::translate_StackLike(int column, int row)
	{
		// Translates the coordinate boundary (relative boundary)
		_grid->translate(column, row);

		// Data must all be copied over (each collection)
		_grid->iterate([] (int column, int row, T item)
		{
			item.translate(column, row);

			return iterationCallback::iterate;
		});

		_edgeGrid->iterate([] (int column, int row, T item)
		{
			item.translate(column, row);

			return iterationCallback::iterate;
		});

		// Locations
		_locations->iterate([] (T item)
		{
			item.translate(item.column, item.row);

			return iterationCallback::iterate;
		});

		// Edges
		_edgeLocations->iterate([] (T item)
		{
			item.translate(item.column, item.row);

			return iterationCallback::iterate;
		});

		// N Edges
		_northExposedLocations->iterate([] (T item)
		{
			item.translate(item.column, item.row);

			return iterationCallback::iterate;
		});

		// S Edges
		_southExposedLocations->iterate([] (T item)
		{
			item.translate(item.column, item.row);

			return iterationCallback::iterate;
		});

		// E Edges
		_eastExposedLocations->iterate([] (T item)
		{
			item.translate(item.column, item.row);

			return iterationCallback::iterate;
		});

		// W Edges
		_westExposedLocations->iterate([] (T item)
		{
			item.translate(item.column, item.row);

			return iterationCallback::iterate;
		});

		// NE Corners
		_northEastCornerLocations->iterate([] (T item)
		{
			item.translate(item.column, item.row);

			return iterationCallback::iterate;
		});

		// NW Corners
		_northWestCornerLocations->iterate([] (T item)
		{
			item.translate(item.column, item.row);

			return iterationCallback::iterate;
		});

		// SE Corners
		_southEastCornerLocations->iterate([] (T item)
		{
			item.translate(item.column, item.row);

			return iterationCallback::iterate;
		});

		// SW Corners
		_southWestCornerLocations->iterate([] (T item)
		{
			item.translate(item.column, item.row);

			return iterationCallback::iterate;
		});
	}

	template<isGridLocator T>
	simpleArray<T> gridRegion<T>::getLocations() const
	{
		return *_locations;
	}

	template<isGridLocator T>
	simpleArray<T> gridRegion<T>::getEdgeLocations() const
	{
		return *_edgeLocations;
	}

	template<isGridLocator T>
	simpleArray<T> gridRegion<T>::getEdges(brogueCompass direction) const
	{
		switch (direction)
		{
			case brogueCompass::N:
				return *_northExposedLocations;
			case brogueCompass::S:
				return *_southExposedLocations;
			case brogueCompass::E:
				return *_eastExposedLocations;
			case brogueCompass::W:
				return *_westExposedLocations;
			default:
				throw simpleException("Must use cardinal direction for gridRegion<>::getEdges");
		}
	}

	template<isGridLocator T>
	simpleOrderedList<T> gridRegion<T>::getBestEdges(brogueCompass direction) const
	{
		gridRect boundary = this->getBoundary();

		switch (direction)
		{
			case brogueCompass::N:
			{
				simpleOrderedList<T> result([&boundary] (const T& item1, const T& item2)
				{
					return (item1.row - boundary.top()) - (item2.row - boundary.top());
				});

				result.addRange(*_northExposedLocations);

				return result;
			}
			case brogueCompass::S:
			{
				simpleOrderedList<T> result([&boundary] (const T& item1, const T& item2)
				{
					return (boundary.bottom() - item1.row) - (boundary.top() - item2.row);
				});

				result.addRange(*_southExposedLocations);

				return result;
			}
			case brogueCompass::E:
			{
				simpleOrderedList<T> result([&boundary] (const T& item1, const T& item2)
				{
					return (boundary.right() - item1.column) - (boundary.right() - item2.column);
				});

				result.addRange(*_eastExposedLocations);

				return result;
			}
			case brogueCompass::W:
			{
				simpleOrderedList<T> result([&boundary] (const T& item1, const T& item2)
				{
					return (item1.column - boundary.left()) - (item2.column - boundary.left());
				});

				result.addRange(*_westExposedLocations);

				return result;
			}
			default:
				throw simpleException("Must use cardinal direction for gridRegion<>::getEdges");
		}
	}

	template<isGridLocator T>
	simpleArray<T> gridRegion<T>::getCorners(brogueCompass nonCardinalDirection) const
	{
		switch (nonCardinalDirection)
		{
			case brogueCompass::NE:
				return *_northEastCornerLocations;
			case brogueCompass::NW:
				return *_northWestCornerLocations;
			case brogueCompass::SE:
				return *_southEastCornerLocations;
			case brogueCompass::SW:
				return *_southWestCornerLocations;
			default:
				throw simpleException("Must use non cardinal direction for gridRegion<>::getCorners");
		}
	}

	template<isGridLocator T>
	gridRect gridRegion<T>::getBoundary() const
	{
		return _grid->getRelativeBoundary();
	}

	template<isGridLocator T>
	gridRect gridRegion<T>::getParentBoundary() const
	{
		return _grid->getParentBoundary();
	}

	template<isGridLocator T>
	gridRect gridRegion<T>::getLargestSubRectangle() const
	{
		return _largestRectangularSubRegion;
	}

	template<isGridLocator T>
	void gridRegion<T>::iterateLocations(gridCallback<T> callback) const
	{
		_locations->forEach([&callback] (T item)
		{
			return callback(item.column, item.row, item);
		});
	}

	template<isGridLocator T>
	void gridRegion<T>::iterateEdges(gridCallback<T> callback) const
	{
		_edgeLocations->forEach([&callback] (T item)
		{
			return callback(item.column, item.row, item);
		});
	}

	template<isGridLocator T>
	bool gridRegion<T>::overlaps(gridRegion<T>* region) const
	{
		bool overlap = false;

		this->iterateLocations([&region, &overlap] (int column, int row, T item)
		{
			if (region->isDefined(column, row))
				return iterationCallback::breakAndReturn;

			return iterationCallback::iterate;
		});

		return overlap;
	}
}

