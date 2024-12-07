#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridRect.h"

#include <simple.h>
#include <simpleArray.h>
#include <simpleException.h>
#include <simpleOrderedList.h>

namespace brogueHd::component
{
	using namespace simple;

	/// <summary>
	/// Grid component that creates a subgrid, using the same coordinate space, from a parent grid. This
	/// means the result will have the same width and height; but will be a subgrid on a specified gridRect
	/// of the parent grid. Each function and call will be protected to utilize only this region. No grid
	/// offsets are required for use.
	/// </summary>
	class gridRegion : public simpleObject
	{
	public:
		gridRegion(const simpleArray<gridLocator>& locations,
		           const simpleArray<gridLocator>& edgeLocations,
		           const simpleArray<gridLocator>& northExposedLocations,
		           const simpleArray<gridLocator>& southExposedLocations,
		           const simpleArray<gridLocator>& eastExposedLocations,
		           const simpleArray<gridLocator>& westExposedLocations,
		           const simpleArray<gridLocator>& northEastCornerLocations,
		           const simpleArray<gridLocator>& northWestCornerLocations,
		           const simpleArray<gridLocator>& southEastCornerLocations,
		           const simpleArray<gridLocator>& southWestCornerLocations,
		           const gridRect& parentBoundary,
		           const gridRect& relativeBoundary,
		           const gridRect& largestRectangularSubRegion);


		~gridRegion();

		gridRect getBoundary() const;
		gridRect getParentBoundary() const;
		gridRect getLargestSubRectangle() const;
		simpleArray<gridLocator> getLocations() const;
		simpleArray<gridLocator> getEdgeLocations() const;

		int getLocationCount() const;
		int getEdgeLocationCount() const;

		/// <summary>
		/// MODIFIES LAYOUT! Translates the grid region data by the provided amount without
		/// re-allocating memory. (Assumes copied data)
		/// </summary>
		void translate(int columnOffset, int rowOffset);

		/// <summary>
		/// Checks for location overlap
		/// </summary>
		bool overlaps(gridRegion* region) const;

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
		gridLocator get(int column, int row) const;

		gridLocator getLocation(int index) const;
		gridLocator getEdgeLocation(int index) const;

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
		/// Returns true if the edge is exposed in the direction specified.
		/// </summary>
		bool isExposedEdge(int column, int row, brogueCompass direction) const;

		/// <summary>
		/// Iterates the locations of the region and calls the user method
		/// </summary>
		void iterateLocations(gridCallbackConst<gridLocator> callback) const;

		/// <summary>
		/// Iterates the edge locations of the region and calls the user method
		/// </summary>
		void iterateEdges(gridCallbackConst<gridLocator> callback) const;

		/// <summary>
		/// Gets locations with an exposed edge in the specified direction
		/// </summary>
		simpleArray<gridLocator> getEdges(brogueCompass direction) const;

		/// <summary>
		/// Gets the locations ordered by their distance to the boundary (closest first)
		/// </summary>
		simpleOrderedList<gridLocator> getBestEdges(brogueCompass direction) const;

		/// <summary>
		/// Gets locations with exposed corner
		/// </summary>
		simpleArray<gridLocator> getCorners(brogueCompass nonCardinalDirection) const;

	private:
		grid<gridLocator>* _grid;
		grid<gridLocator>* _edgeGrid;

		gridRect _largestRectangularSubRegion;

		simpleArray<gridLocator>* _locations;
		simpleArray<gridLocator>* _edgeLocations;

		// Exposed Edge Faces
		simpleArray<gridLocator>* _northExposedLocations;
		simpleArray<gridLocator>* _southExposedLocations;
		simpleArray<gridLocator>* _westExposedLocations;
		simpleArray<gridLocator>* _eastExposedLocations;

		// Corners
		simpleArray<gridLocator>* _northEastCornerLocations;
		simpleArray<gridLocator>* _northWestCornerLocations;
		simpleArray<gridLocator>* _southEastCornerLocations;
		simpleArray<gridLocator>* _southWestCornerLocations;
	};

	gridRegion::gridRegion(const simpleArray<gridLocator>& locations,
	                          const simpleArray<gridLocator>& edgeLocations,
	                          const simpleArray<gridLocator>& northExposedLocations,
	                          const simpleArray<gridLocator>& southExposedLocations,
	                          const simpleArray<gridLocator>& eastExposedLocations,
	                          const simpleArray<gridLocator>& westExposedLocations,
	                          const simpleArray<gridLocator>& northEastCornerLocations,
	                          const simpleArray<gridLocator>& northWestCornerLocations,
	                          const simpleArray<gridLocator>& southEastCornerLocations,
	                          const simpleArray<gridLocator>& southWestCornerLocations,
	                          const gridRect& parentBoundary,
	                          const gridRect& relativeBoundary,
	                          const gridRect& largestRectangularSubRegion)
	{
		_grid = new grid<gridLocator>(parentBoundary, relativeBoundary);
		_edgeGrid = new grid<gridLocator>(parentBoundary, relativeBoundary);

		_largestRectangularSubRegion = largestRectangularSubRegion;

		_locations = new simpleArray<gridLocator>(locations);
		_edgeLocations = new simpleArray<gridLocator>(edgeLocations);

		_northExposedLocations = new simpleArray<gridLocator>(northExposedLocations);
		_southExposedLocations = new simpleArray<gridLocator>(southExposedLocations);
		_eastExposedLocations = new simpleArray<gridLocator>(eastExposedLocations);
		_westExposedLocations = new simpleArray<gridLocator>(westExposedLocations);

		_northEastCornerLocations = new simpleArray<gridLocator>(northEastCornerLocations);
		_northWestCornerLocations = new simpleArray<gridLocator>(northWestCornerLocations);
		_southEastCornerLocations = new simpleArray<gridLocator>(southEastCornerLocations);
		_southWestCornerLocations = new simpleArray<gridLocator>(southWestCornerLocations);

		// Initialize Grids
		for (int index = 0; index < locations.count(); index++)
		{
			gridLocator item = locations.get(index);

			// Validates that each cell is at a unique location
			_grid->set(item.column, item.row, item);
		}

		for (int index = 0; index < edgeLocations.count(); index++)
		{
			gridLocator item = locations.get(index);

			_edgeGrid->set(item.column, item.row, item);
		}
	}

	gridRegion::~gridRegion()
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

	gridLocator gridRegion::get(int column, int row) const
	{
		return _grid->get(column, row);
	}

	gridLocator gridRegion::getLocation(int index) const
	{
		return _locations->get(index);
	}

	gridLocator gridRegion::getEdgeLocation(int index) const
	{
		return _edgeLocations->get(index);
	}

	int gridRegion::getLocationCount() const
	{
		return _locations->count();
	}

	int gridRegion::getEdgeLocationCount() const
	{
		return _edgeLocations->count();
	}

	bool gridRegion::isDefined(int column, int row) const
	{
		// Checking "bounds" for a region isn't really an appropriate meaning for
		// an amorphous region. The grid needs to be protected from out of bounds
		// indexing.
		//
		if (!_grid->isInBounds(column, row))
			return false;

		return _grid->isDefined(column, row);
	}

	bool gridRegion::isDefined(const gridLocator& locator) const
	{
		return isDefined(locator.column, locator.row);
	}

	bool gridRegion::isEdge(int column, int row) const
	{
		return _edgeGrid->isDefined(column, row);
	}

	bool gridRegion::isExposedEdge(int column, int row, brogueCompass direction) const
	{
		return _edgeGrid->isExposedEdge(column, row, direction);
	}

	void gridRegion::translate(int columnOffset, int rowOffset)
	{
		// Translates the coordinate boundary (relative boundary)
		_grid->translate(columnOffset, rowOffset);

		// Data must all be copied over (each collection)
		_grid->iterateModify([&columnOffset, &rowOffset](int column, int row, gridLocator& item)
		{
			item.translate(columnOffset, rowOffset);
			return iterationCallback::iterate;
		});

		_edgeGrid->iterateModify([&columnOffset, &rowOffset](int column, int row, gridLocator& item)
		{
			item.translate(columnOffset, rowOffset);

			return iterationCallback::iterate;
		});

		// Locations
		_locations->iterateModify([&columnOffset, &rowOffset](gridLocator& item)
		{
			item.translate(columnOffset, rowOffset);
			return iterationCallback::iterate;
		});

		// Edges
		_edgeLocations->iterateModify([&columnOffset, &rowOffset](gridLocator& item)
		{
			item.translate(columnOffset, rowOffset);
			return iterationCallback::iterate;
		});

		// N Edges
		_northExposedLocations->iterateModify([&columnOffset, &rowOffset](gridLocator& item)
		{
			item.translate(columnOffset, rowOffset);
			return iterationCallback::iterate;
		});

		// S Edges
		_southExposedLocations->iterateModify([&columnOffset, &rowOffset](gridLocator& item)
		{
			item.translate(columnOffset, rowOffset);
			return iterationCallback::iterate;
		});

		// E Edges
		_eastExposedLocations->iterateModify([&columnOffset, &rowOffset](gridLocator& item)
		{
			item.translate(columnOffset, rowOffset);
			return iterationCallback::iterate;
		});

		// W Edges
		_westExposedLocations->iterateModify([&columnOffset, &rowOffset](gridLocator& item)
		{
			item.translate(columnOffset, rowOffset);
			return iterationCallback::iterate;
		});

		// NE Corners
		_northEastCornerLocations->iterateModify([&columnOffset, &rowOffset](gridLocator& item)
		{
			item.translate(columnOffset, rowOffset);
			return iterationCallback::iterate;
		});

		// NW Corners
		_northWestCornerLocations->iterateModify([&columnOffset, &rowOffset](gridLocator& item)
		{
			item.translate(columnOffset, rowOffset);
			return iterationCallback::iterate;
		});

		// SE Corners
		_southEastCornerLocations->iterateModify([&columnOffset, &rowOffset](gridLocator& item)
		{
			item.translate(columnOffset, rowOffset);
			return iterationCallback::iterate;
		});

		// SW Corners
		_southWestCornerLocations->iterateModify([&columnOffset, &rowOffset](gridLocator& item)
		{
			item.translate(columnOffset, rowOffset);
			return iterationCallback::iterate;
		});
	}

	simpleArray<gridLocator> gridRegion::getLocations() const
	{
		return *_locations;
	}

	simpleArray<gridLocator> gridRegion::getEdgeLocations() const
	{
		return *_edgeLocations;
	}

	simpleArray<gridLocator> gridRegion::getEdges(brogueCompass direction) const
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

	simpleOrderedList<gridLocator> gridRegion::getBestEdges(brogueCompass direction) const
	{
		gridRect boundary = this->getBoundary();

		switch (direction)
		{
		case brogueCompass::N:
			{
				simpleOrderedList<gridLocator> result([&boundary](const gridLocator& item1, const gridLocator& item2)
				{
					return (item1.row - boundary.top()) - (item2.row - boundary.top());
				});

				result.addRange(*_northExposedLocations);

				return result;
			}
		case brogueCompass::S:
			{
				simpleOrderedList<gridLocator> result([&boundary](const gridLocator& item1, const gridLocator& item2)
				{
					return (boundary.bottom() - item1.row) - (boundary.top() - item2.row);
				});

				result.addRange(*_southExposedLocations);

				return result;
			}
		case brogueCompass::E:
			{
				simpleOrderedList<gridLocator> result([&boundary](const gridLocator& item1, const gridLocator& item2)
				{
					return (boundary.right() - item1.column) - (boundary.right() - item2.column);
				});

				result.addRange(*_eastExposedLocations);

				return result;
			}
		case brogueCompass::W:
			{
				simpleOrderedList<gridLocator> result([&boundary](const gridLocator& item1, const gridLocator& item2)
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

	simpleArray<gridLocator> gridRegion::getCorners(brogueCompass nonCardinalDirection) const
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

	gridRect gridRegion::getBoundary() const
	{
		return _grid->getRelativeBoundary();
	}

	gridRect gridRegion::getParentBoundary() const
	{
		return _grid->getParentBoundary();
	}

	gridRect gridRegion::getLargestSubRectangle() const
	{
		return _largestRectangularSubRegion;
	}

	void gridRegion::iterateLocations(gridCallbackConst<gridLocator> callback) const
	{
		_locations->forEach([&callback](const gridLocator& item)
		{
			return callback(item.column, item.row, item);
		});
	}

	void gridRegion::iterateEdges(gridCallbackConst<gridLocator> callback) const
	{
		_edgeLocations->forEach([&callback](const gridLocator& item)
		{
			return callback(item.column, item.row, item);
		});
	}

	bool gridRegion::overlaps(gridRegion* region) const
	{
		bool overlap = false;

		this->iterateLocations([&region, &overlap](int column, int row, const gridLocator& item)
		{
			if (region->isDefined(column, row))
				return iterationCallback::breakAndReturn;

			return iterationCallback::iterate;
		});

		return overlap;
	}
}
