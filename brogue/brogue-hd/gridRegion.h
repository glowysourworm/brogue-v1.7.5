#pragma once

#include "grid.h"
#include "griddef.h"
#include <functional>

using namespace std;

namespace brogueHd::backend::model::layout
{
	/// <summary>
	/// Grid component that creates a subgrid, using the same coordinate space, from a parent grid. This
	/// means the result will have the same width and height; but will be a subgrid on a specified gridRect
	/// of the parent grid. Each function and call will be protected to utilize only this region. No grid
	/// offsets are required for use.
	/// </summary>
	template<typename T>
	class gridRegion
	{
	public:

		gridRegion(const grid<T>* parentGrid, gridRect boundary);
		~gridRegion();

		gridRect getBoundary() const;

		/// <summary>
		/// Returns value from the underlying grid. This is the index space of the parent grid.
		/// </summary>
		T get(short parentColumn, short parentRow) const;

	private:

		grid<T>* _grid;
		gridRect _boundary;
	};
}

