#include "gridRegion.h"
#include "gridDefinitions.h"
#include "grid.h"
#include "gridRect.h"
#include "exceptionHandler.h"

using namespace brogueHd::component::math;

namespace brogueHd::component
{
	template<isGridLocator T>
	gridRegion<T>::gridRegion(T* locations,
								T* edgeLocations,
								T* northExposedLocations,
								T* southExposedLocations,
								T* eastExposedLocations,	
								T* westExposedLocations,
								T* northEastCornerLocations,
								T* northWestCornerLocations,
								T* southEastCornerLocations,
								T* southWestCornerLocations,
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
	gridRegion<T>::gridRegion(const gridRegion& copy)
	{
		this->initialize(copy.getLocations(), copy.getEdgeLocations(),
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
	void gridRegion<T>::initialize(T* locations,
									T* edgeLocations,
									T* northExposedLocations,
									T* southExposedLocations,
									T* eastExposedLocations,
									T* westExposedLocations,
									T* northEastCornerLocations,
									T* northWestCornerLocations,
									T* southEastCornerLocations,
									T* southWestCornerLocations,
									gridRect parentBoundary,
									gridRect relativeBoundary,
									gridRect largestRectangularSubRegion)
	{
		if (_grid != NULL) delete _grid;
		if (_edgeGrid != NULL) delete _edgeGrid;

		if (_northExposedLocations != NULL) delete _northExposedLocations;
		if (_southExposedLocations != NULL) delete _southExposedLocations;
		if (_eastExposedLocations != NULL) delete _eastExposedLocations;
		if (_westExposedLocations != NULL) delete _westExposedLocations;

		if (_northEastCornerLocations != NULL) delete _northEastCornerLocations;
		if (_northWestCornerLocations != NULL) delete _northWestCornerLocations;
		if (_southEastCornerLocations != NULL) delete _southEastCornerLocations;
		if (_southWestCornerLocations != NULL) delete _southWestCornerLocations;

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
	}

	template<isGridLocator T>
	gridRegion<T>::~gridRegion()
	{
		// MEMORY!  gridRegionConstructor (new T) -> gridRegion(...)
		//			gridRegion.translate  (new T)

		for (int index = 0; index < _locations.size(); index++)
		{
			delete _locations[index];  // ~gridCell, ~brogueCell
		}

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
		return _grid->get(column,row);
	}

	template<isGridLocator T>
	bool gridRegion<T>::isDefined(short column, short row) const
	{
		return _grid->isDefined(column, row);
	}

	template<isGridLocator T>
	bool gridRegion<T>::isEdge(short column, short row) const
	{
		return _edgeGrid->isDefined(column, row);
	}

	template<isGridLocator T>
	simpleArray<T>* gridRegion<T>::getLocations()
	{
		return _locations;
	}

	template<isGridLocator T>
	simpleArray<T>* gridRegion<T>::getEdgeLocations()
	{
		return _edgeLocations;
	}

	template<isGridLocator T>
	simpleArray<T>* gridRegion<T>::getEdges(brogueCompass direction)
	{
		switch (direction)
		{
		case brogueCompass::N:
			return _northExposedLocations;
		case brogueCompass::S:
			return _southExposedLocations;
		case brogueCompass::E:
			return _eastExposedLocations;
		case brogueCompass::W:
			return _westExposedLocations;
		default:
			brogueException::show("Must use cardinal direction for gridRegion<>::getEdges");
		}
	}

	template<isGridLocator T>
	simpleArray<T>* gridRegion<T>::getBoundaryEdges(brogueCompass direction)
	{
		gridRect boundary = this->getBoundary();

		switch (direction)
		{
		case brogueCompass::N:
			return _northExposedLocations->where([&boundary](T item) { return item.row == boundary.top(); });
		case brogueCompass::S:
			return _southExposedLocations->where([&boundary](T item) { return item.row == boundary.bottom(); });
		case brogueCompass::E:
			return _eastExposedLocations->where([&boundary](T item) { return item.row == boundary.right(); });
		case brogueCompass::W:
			return _westExposedLocations->where([&boundary](T item) { return item.row == boundary.left(); });
		default:
			brogueException::show("Must use cardinal direction for gridRegion<>::getEdges");
		}
	}

	template<isGridLocator T>
	simpleArray<T>* gridRegion<T>::getCorners(brogueCompass nonCardinalDirection)
	{
		switch (nonCardinalDirection)
		{
		case brogueCompass::NE:
			return _northEastCornerLocations;
		case brogueCompass::NW:
			return _northWestCornerLocations;
		case brogueCompass::SE:
			return _southEastCornerLocations;
		case brogueCompass::SW:
			return _southWestCornerLocations;
		default:
			brogueException::show("Must use non cardinal direction for gridRegion<>::getCorners");
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
	void gridRegion<T>::iterateLocations(gridDelegates<T>::callback callback)
	{
		_locations->forEach([](T item)
		{
			return callback(item.column, item.row, item);
		});
	}

	template<isGridLocator T>
	void gridRegion<T>::iterateEdges(gridDelegates<T>::callback callback)
	{
		_edgeLocations->forEach([](T item)
		{
			return callback(item.column, item.row, item);
		});
	}

	template<isGridLocator T>
	bool gridRegion<T>::overlaps(const gridRegion<T>& region)
	{
		bool overlap = false;

		this->iterateLocations([&region, &overlap](short column, short row, T item)
		{
			if (region.isDefined(column, row))
				return iterationCallback::breakAndReturn;
		});

		return overlap;
	}

	template<isGridLocator T>
	void gridRegion<T>::copyTo(grid<T>& grid)
	{
		this->iterateLocations([&grid](short column, short row, T item)
		{
			grid.set(column, row, item);
		});
	}
}
