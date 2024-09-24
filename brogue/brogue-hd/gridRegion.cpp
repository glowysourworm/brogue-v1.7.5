#include "gridRegion.h"
#include "gridDefinitions.h"
#include "gridRegionConstructor.h"
#include "grid.h"
#include "gridRect.h"
#include "vectorExtension.h"
#include "arrayExtension.h"
#include "exceptionHandler.h"

using namespace std;

using namespace brogueHd::backend::extension;
using namespace brogueHd::backend::math;
using namespace brogueHd::backend::model::construction;

namespace brogueHd::backend::model::layout
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

		_locations = new std::vector<T>(SIZEOF(locations));
		_edgeLocations = new std::vector<T>(SIZEOF(edgeLocations));

		_northExposedLocations = new std::vector<T>(SIZEOF(northExposedLocations));
		_southExposedLocations = new std::vector<T>(SIZEOF(southExposedLocations));
		_eastExposedLocations = new std::vector<T>(SIZEOF(eastExposedLocations));
		_westExposedLocations = new std::vector<T>(SIZEOF(westExposedLocations));

		_northEastCornerLocations = new std::vector<T>(SIZEOF(northEastCornerLocations));
		_northWestCornerLocations = new std::vector<T>(SIZEOF(northWestCornerLocations));
		_southEastCornerLocations = new std::vector<T>(SIZEOF(southEastCornerLocations));
		_southWestCornerLocations = new std::vector<T>(SIZEOF(southWestCornerLocations));

		// Copy collections
		arrayExtension<T>::forEach(locations, [&_locations, &_grid](T item)
		{
			_grid->set(item.column, item.row, item);
			_locations->push_back(item);
		});
		arrayExtension<T>::forEach(edgeLocations, [&_edgeLocations, &_edgeGrid](T item)
		{
			_edgeGrid->set(item.column, item.row, item);
			_edgeLocations->push_back(item);
		});

		arrayExtension<T>::forEach(northExposedLocations, [&_northExposedLocations](T item) { _northExposedLocations->push_back(item); });
		arrayExtension<T>::forEach(southExposedLocations, [&_southExposedLocations](T item) { _southExposedLocations->push_back(item); });
		arrayExtension<T>::forEach(eastExposedLocations, [&_eastExposedLocations](T item) { _eastExposedLocations->push_back(item); });
		arrayExtension<T>::forEach(westExposedLocations, [&_westExposedLocations](T item) { _westExposedLocations->push_back(item); });

		arrayExtension<T>::forEach(northEastCornerLocations, [&_northEastCornerLocations](T item) { _northEastCornerLocations->push_back(item); });
		arrayExtension<T>::forEach(northWestCornerLocations, [&_northWestCornerLocations](T item) { _northWestCornerLocations->push_back(item); });
		arrayExtension<T>::forEach(southEastCornerLocations, [&_southEastCornerLocations](T item) { _southEastCornerLocations->push_back(item); });
		arrayExtension<T>::forEach(southWestCornerLocations, [&_southWestCornerLocations](T item) { _southWestCornerLocations->push_back(item); });
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
	std::vector<T>* gridRegion<T>::getLocations()
	{
		return _locations;
	}

	template<isGridLocator T>
	std::vector<T>* gridRegion<T>::getEdgeLocations()
	{
		return _edgeLocations;
	}

	template<isGridLocator T>
	std::vector<T>* gridRegion<T>::getEdges(brogueCompass direction)
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
	std::vector<T>& gridRegion<T>::getBoundaryEdges(brogueCompass direction)
	{
		gridRect boundary = this->getBoundary();

		switch (direction)
		{
		case brogueCompass::N:
			return vectorExtension<T>::where(*_northExposedLocations, [&boundary](T item) { item.row == boundary.top()});
		case brogueCompass::S:
			return vectorExtension<T>::where(*_southExposedLocations, [&boundary](T item) { item.row == boundary.bottom()});
		case brogueCompass::E:
			return vectorExtension<T>::where(*_eastExposedLocations, [&boundary](T item) { item.row == boundary.right()});
		case brogueCompass::W:
			return vectorExtension<T>::where(*_westExposedLocations, [&boundary](T item) { item.row == boundary.left()});
		default:
			brogueException::show("Must use cardinal direction for gridRegion<>::getEdges");
		}
	}

	template<isGridLocator T>
	std::vector<T>* gridRegion<T>::getCorners(brogueCompass nonCardinalDirection)
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
		vectorExtension<T>::forEach(_locations, [](T item)
		{
			callback(item.column, item.row, item);
		});
	}

	template<isGridLocator T>
	void gridRegion<T>::iterateEdges(gridDelegates<T>::callback callback)
	{
		vectorExtension<T>::forEach(_edgeLocations, [](T item)
		{
			callback(item.column, item.row, item);
		});
	}

	template<isGridLocator T>
	void gridRegion<T>::translate(gridLocator translation)
	{
		// Create a new region constructor
		gridRegionConstructor<T> constructor(_grid->getParentBoundary(), [](short column, short row, T item)
		{
			// Inclusion predicate
			return true;
		});

		vectorExtension<T>::forEach(_locations, [&constructor, &translation, &isLocator](short column, short row, T item)
		{
			short newColumn = column + translation.column;
			short newRow = row + translation.row;

			// Locators
			if (isLocator)
			{
				gridLocator locator = (gridLocator)(item);

				locator.column = newColumn;
				locator.row = newRow;
			}

			constructor.add(newColumn, newRow, locator);
		});

		// Commit the translated region to memory - calculating the rest of the data
		gridRegion<T>* newRegion = constructor.complete();

		// Copy the data over to this one
		this->initialize(newRegion->getLocations(), 
						 newRegion->getEdgeLocations(), 
						 newRegion->getEdges(brogueCompass::N),
 						 newRegion->getEdges(brogueCompass::S),
						 newRegion->getEdges(brogueCompass::E),
						 newRegion->getEdges(brogueCompass::W),
						 newRegion->getCorners(brogueCompass::NE),
						 newRegion->getCorners(brogueCompass::NW),
						 newRegion->getCorners(brogueCompass::SE),
						 newRegion->getCorners(brogueCompass::SW),
						 newRegion->getParentBoundary(),
						 newRegion->getBoundary(),
						 newRegion->getLargestSubRectangle());

		// Cleanup memory
		delete newRegion;
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
}
