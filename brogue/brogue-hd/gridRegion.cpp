#include "gridRegion.h"
#include "gridDefinitions.h"
#include "grid.h"
#include "gridRect.h"

using namespace std;

using namespace brogueHd::backend::math;

namespace brogueHd::backend::model::layout
{
	template<gridCellConstraint T>
	gridRegion<T>::gridRegion(T* locations,
								T* edgeLocations,
								T* westExposedLocations,
								T* northExposedLocations,
								T* eastExposedLocations,
								T* southExposedLocations,
								T* northWestCornerLocations,
								T* northEastCornerLocations,
								T* southEastCornerLocations,
								T* southWestCornerLocations,
								gridRect relativeBoundary)
	{
		_grid = parentGrid;
		_boundary = boundary;
	}

	template<gridCellConstraint T>
	gridRegion<T>::~gridRegion()
	{
		if (_grid != NULL)
		{
			delete _grid;

			_grid = NULL;
		}
	}

	template<gridCellConstraint T>
	T gridRegion<T>::get(short column, short row) const
	{
		return _array2D[column][row];
	}

	template<gridCellConstraint T>
	gridRect gridRegion<T>::getBoundary() const
	{
		return _boundary;
	}
}
