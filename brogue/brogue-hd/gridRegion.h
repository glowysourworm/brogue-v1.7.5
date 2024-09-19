#pragma once

#include "array2D.h"
#include "grid.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "dijkstra.h"
#include <functional>

using namespace std;

using namespace brogueHd::backend::math;

namespace brogueHd::backend::model::layout
{
	/// <summary>
	/// Grid component that creates a subgrid, using the same coordinate space, from a parent grid. This
	/// means the result will have the same width and height; but will be a subgrid on a specified gridRect
	/// of the parent grid. Each function and call will be protected to utilize only this region. No grid
	/// offsets are required for use.
	/// </summary>
	template<gridCellConstraint T>
	class gridRegion
	{
	public:

		gridRegion(T* locations,
					T* edgeLocations,
					T* westExposedLocations,
					T* northExposedLocations,
					T* eastExposedLocations,
					T* southExposedLocations,
					T* northWestCornerLocations,
					T* northEastCornerLocations,
					T* southEastCornerLocations,
					T* southWestCornerLocations,
					gridRect parentBoundary,
					gridRect relativeBoundary,
					gridRect largestRectangularSubRegion);


		~gridRegion();

		gridRect getBoundary() const;

	public:

		/// <summary>
		/// Returns value from the underlying grid. This is the index space of the parent grid.
		/// </summary>
		T get(short column, short row) const;

	private:

		array2D<T>* _array2D;

		gridRect _boundary;
	};
}

