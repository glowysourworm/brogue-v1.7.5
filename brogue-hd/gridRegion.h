#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleArray.h"
#include "simpleException.h"

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
		gridRegion() {}
		gridRegion(const gridRegion<T>& copy);
		gridRegion(const simpleArray<T>& locations,
					const simpleArray<T>& edgeLocations,
					const simpleArray<T>& westExposedLocations,
					const simpleArray<T>& northExposedLocations,
					const simpleArray<T>& eastExposedLocations,
					const simpleArray<T>& southExposedLocations,
					const simpleArray<T>& northWestCornerLocations,
					const simpleArray<T>& northEastCornerLocations,
					const simpleArray<T>& southEastCornerLocations,
					const simpleArray<T>& southWestCornerLocations,
					gridRect parentBoundary,
					gridRect relativeBoundary,
					gridRect largestRectangularSubRegion);


		~gridRegion();

		gridRect getBoundary() const;
		gridRect getParentBoundary() const;
		gridRect getLargestSubRectangle() const;
		simpleArray<T> getLocations() const;
		simpleArray<T> getEdgeLocations() const;

		/// <summary>
		/// MODIFIES LAYOUT! Translates the grid region data by the provided amount without
		/// re-allocating memory. (Assumes copied data)
		/// </summary>
		void translate_StackLike(short column, short row);

		/// <summary>
		/// MODIFIES LAYOUT! Translates the grid region data by the provided amount without
		/// re-allocating memory. (Assumes pointers)
		/// </summary>
		void translate_HeapLike(short column, short row);

		/// <summary>
		/// Checks for location overlap
		/// </summary>
		bool overlaps(const gridRegion<T>& region) const;

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
		T get(short column, short row) const;

		/// <summary>
		/// Returns true if a cell is defined for the region
		/// </summary>
		bool isDefined(short column, short row) const;

		/// <summary>
		/// Returns true if an edge cell is defined
		/// </summary>
		bool isEdge(short column, short row) const;

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
		/// Gets the locations that are also along the bounding rectangle
		/// </summary>
		simpleArray<T> getBoundaryEdges(brogueCompass direction) const;

		/// <summary>
		/// Gets locations with exposed corner
		/// </summary>
		simpleArray<T> getCorners(brogueCompass nonCardinalDirection) const;

	private:

		// Needed a mutator for the translate() function
		void initialize(const simpleArray<T>& locations,
			const simpleArray<T>& edgeLocations,
			const simpleArray<T>& westExposedLocations,
			const simpleArray<T>& northExposedLocations,
			const simpleArray<T>& eastExposedLocations,
			const simpleArray<T>& southExposedLocations,
			const simpleArray<T>& northWestCornerLocations,
			const simpleArray<T>& northEastCornerLocations,
			const simpleArray<T>& southEastCornerLocations,
			const simpleArray<T>& southWestCornerLocations,
			gridRect parentBoundary,
			gridRect relativeBoundary,
			gridRect largestRectangularSubRegion);

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
		const simpleArray<T>& westExposedLocations,
		const simpleArray<T>& northExposedLocations,
		const simpleArray<T>& eastExposedLocations,
		const simpleArray<T>& southExposedLocations,
		const simpleArray<T>& northWestCornerLocations,
		const simpleArray<T>& northEastCornerLocations,
		const simpleArray<T>& southEastCornerLocations,
		const simpleArray<T>& southWestCornerLocations,
		gridRect parentBoundary,
		gridRect relativeBoundary,
		gridRect largestRectangularSubRegion)
	{
		this->initialize(locations, edgeLocations,
			northExposedLocations,
			southExposedLocations,
			eastExposedLocations,
			westExposedLocations,
			northEastCornerLocations,
			northWestCornerLocations,
			southEastCornerLocations,
			southWestCornerLocations,
			parentBoundary,
			relativeBoundary,
			largestRectangularSubRegion);
	}
	template<isGridLocator T>
	gridRegion<T>::gridRegion(const gridRegion<T>& copy)
	{
		this->initialize(copy.getLocations(),
						 copy.getEdgeLocations(),
						 copy.getEdges(brogueCompass::N),
						 copy.getEdges(brogueCompass::S),
						 copy.getEdges(brogueCompass::E),
						 copy.getEdges(brogueCompass::W),
						 copy.getCorners(brogueCompass::NE),
						 copy.getCorners(brogueCompass::NW),
						 copy.getCorners(brogueCompass::SE),
						 copy.getCorners(brogueCompass::SW),
						 copy.getParentBoundary(),
						 copy.getBoundary(),
						 copy.getLargestSubRectangle());
	}

	template<isGridLocator T>
	void gridRegion<T>::initialize(const simpleArray<T>& locations,
									const simpleArray<T>& edgeLocations,
									const simpleArray<T>& westExposedLocations,
									const simpleArray<T>& northExposedLocations,
									const simpleArray<T>& eastExposedLocations,
									const simpleArray<T>& southExposedLocations,
									const simpleArray<T>& northWestCornerLocations,
									const simpleArray<T>& northEastCornerLocations,
									const simpleArray<T>& southEastCornerLocations,
									const simpleArray<T>& southWestCornerLocations,
									gridRect parentBoundary,
									gridRect relativeBoundary,
									gridRect largestRectangularSubRegion)
	{
		if (_grid != nullptr) delete _grid;
		if (_edgeGrid != nullptr) delete _edgeGrid;

		if (_northExposedLocations != nullptr) delete _northExposedLocations;
		if (_southExposedLocations != nullptr) delete _southExposedLocations;
		if (_eastExposedLocations != nullptr) delete _eastExposedLocations;
		if (_westExposedLocations != nullptr) delete _westExposedLocations;

		if (_northEastCornerLocations != nullptr) delete _northEastCornerLocations;
		if (_northWestCornerLocations != nullptr) delete _northWestCornerLocations;
		if (_southEastCornerLocations != nullptr) delete _southEastCornerLocations;
		if (_southWestCornerLocations != nullptr) delete _southWestCornerLocations;

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

		grid<T>* grid = _grid;

		// Initialize Grid
		_locations->iterate([&grid] (T item)
		{
			// Validates that each cell is at a unique location
			grid->set(item.column, item.row, item);

			return iterationCallback::iterate;
		});
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
	T gridRegion<T>::get(short column, short row) const
	{
		return _grid->get(column, row);
	}

	template<isGridLocator T>
	bool gridRegion<T>::isDefined(short column, short row) const
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
	bool gridRegion<T>::isEdge(short column, short row) const
	{
		return _edgeGrid->isDefined(column, row);
	}

	template<isGridLocator T>
	void gridRegion<T>::translate_HeapLike(short column, short row)
	{
		// NOT SURE HOW THIS METHOD MIGHT WORK:  The template must understand
		// that T is a pointer; but that will take some more understanding of
		// C++ concepts.
		//

		// Translates the coordinate boundary (relative boundary)
		_grid->translate(column, row);

		_grid->iterate([] (short column, short row, T item)
		{
			item.translate(column, row);

			return iterationCallback::iterate;
		});
	}

	template<isGridLocator T>
	void gridRegion<T>::translate_StackLike(short column, short row)
	{
		// Translates the coordinate boundary (relative boundary)
		_grid->translate(column, row);

		// Data must all be copied over (each collection)
		_grid->iterate([] (short column, short row, T item)
		{
			item.translate(column, row);

			return iterationCallback::iterate;
		});

		_edgeGrid->iterate([] (short column, short row, T item)
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
	simpleArray<T> gridRegion<T>::getBoundaryEdges(brogueCompass direction) const
	{
		gridRect boundary = this->getBoundary();

		switch (direction)
		{
			case brogueCompass::N:
				return _northExposedLocations->whereArray([&boundary] (T item)
				{
					return item.row == boundary.top();
				});
			case brogueCompass::S:
				return _southExposedLocations->whereArray([&boundary] (T item)
				{
					return item.row == boundary.bottom();
				});
			case brogueCompass::E:
				return _eastExposedLocations->whereArray([&boundary] (T item)
				{
					return item.column == boundary.right();
				});
			case brogueCompass::W:
				return _westExposedLocations->whereArray([&boundary] (T item)
				{
					return item.column == boundary.left();
				});
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
				simpleException::showCstr("Must use non cardinal direction for gridRegion<>::getCorners");
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
	bool gridRegion<T>::overlaps(const gridRegion<T>& region) const
	{
		bool overlap = false;

		this->iterateLocations([&region, &overlap] (short column, short row, T item)
		{
			if (region.isDefined(column, row))
				return iterationCallback::breakAndReturn;

			return iterationCallback::iterate;
		});

		return overlap;
	}
}

