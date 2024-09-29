#pragma once

#include "gridRegionConstructor.h"
#include "simpleList.h"
#include <functional>

namespace brogueHd::component
{
	template<isGridLocator T>
	class gridRegionLocator
	{
	public:

		gridRegionLocator();
		~gridRegionLocator();

		/// <summary>
		/// Runs flood fill on the grid; and creates grid region instances.
		/// </summary>
		/// <returns>Completed region instanc vector for the grid</returns>
		simpleList<gridRegion<T>*> locateRegions(const grid<T>& grid);

		/// <summary>
		/// Creates regions from the grid using flood fill iteration. The region predicate is used to select
		/// whether a particular cell is in / out of the region.
		/// </summary>
		simpleList<gridRegion<T>*> locateRegions(const grid<T>& grid, gridPredicate<T> inclusionPredicate);

		/// <summary>
		/// Creates region at the specified point (or NULL)
		/// </summary>
		gridRegion<T>* identifyRegion(const grid<T>& grid, short column, short row, gridPredicate<T> inclusionPredicate);

	private:

		gridRegionConstructor<T> runFloodFill(const grid<T>& grid, short column, short row, gridPredicate<T> inclusionPredicate);
	};
}

