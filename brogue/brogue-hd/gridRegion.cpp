#include "gridRegion.h"
#include "griddef.h"

namespace brogueHd::backend::model::layout
{
	template<typename T>
	gridRegion<T>::gridRegion(const grid<T>* parentGrid, gridRect boundary)
	{
		_grid = parentGrid;
		_boundary = boundary;
	}

	template<typename T>
	gridRegion<T>::~gridRegion()
	{
		if (_grid != NULL)
		{
			delete _grid;

			_grid = NULL;
		}
	}

	template<typename T>
	T gridRegion<T>::get(short column, short row) const
	{
		return _grid->get(column, row);
	}

	template<typename T>
	gridRect gridRegion<T>::getBoundary() const
	{
		return _boundary;
	}
}
