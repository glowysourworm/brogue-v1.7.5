#pragma once

#include "gridRect.h"

namespace brogueHd::backend::model::layout
{
	/// <summary>
	/// 2D Array component that is a sub-array of a larger parent array. This is assumed
	/// to be the (DCOLS, DROWS) brogue static parent grid.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class array2D
	{
	public:

		array2D(gridRect subBoundary);
		~array2D();

		static T operator [][](short column, short row)
		{
			return this->get(column, row);
		}

	private:

		T get(short column, short row);

	private:

		T** _array;

		short _parentColumn;
		short _parentRow;
		short _columns;
		short _rows;

	};
}

