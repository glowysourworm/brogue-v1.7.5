#pragma once

#include "grid.h"
#include "gridRegion.h"
#include "gridDefinitions.h"
#include "gridRegionConstructor.h"
#include <functional>

using namespace std;

using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model::construction
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
		std::vector<gridRegion<T>*> locateRegions(const grid<T>& grid);

		/// <summary>
		/// Creates regions from the grid using flood fill iteration. The region predicate is used to select
		/// whether a particular cell is in / out of the region.
		/// </summary>
		std::vector<gridRegion<T>*> locateRegions(const grid<T>& grid, extensionDelegates<T>::simplePredicate inclusionPredicate);

		/// <summary>
		/// Creates region at the specified point (or NULL)
		/// </summary>
		gridRegion<T>* identifyRegion(const grid<T>& grid, short column, short row, extensionDelegates<T>::simplePredicate inclusionPredicate);

	private:

		gridRegionConstructor<T>* runFloodFill(const grid<T>& grid, short column, short row, extensionDelegates<T>::simplePredicate inclusionPredicate);
	};
}

