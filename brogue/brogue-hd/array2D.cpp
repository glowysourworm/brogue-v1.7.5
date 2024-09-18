#include "array2D.h"
#include "griddef.h"

namespace brogueHd::backend::model::layout
{
	template<typename T>
	array2D<T>::array2D(gridRect subBoundary)
	{
		_array = new T * [subBoundary.width()];

		for (int index = 0; index < subBoundary.width(); index++)
			_array[index] = new T[subBoundary.height()];

		_parentColumn = subBoundary.left();
		_parentRow = subBoundary.right();
		_columns = subBoundary.width;
		_rows = subBoundary.height;
	}

	template<typename T>
	array2D<T>::~array2D()
	{
		for (int index = 0; index < DCOLS; index++)
			delete[] _array[index];

		delete[] _array;
	}

	template<typename T>
	T array2D<T>::get(short column, short row)
	{
		return _array[column - _parentColumn][row - _parentRow];
	}
}