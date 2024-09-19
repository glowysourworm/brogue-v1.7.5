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
	template<gridCellConstraint T>
	class gridRegionLocator
	{
	public:

		gridRegionLocator(const grid<T>*& grid);
		~gridRegionLocator();

		/// <summary>
		/// Runs flood fill on the grid; and creates grid region instances.
		/// </summary>
		/// <returns>Completed region instanc vector for the grid</returns>
		std::vector<gridRegion<T>*> locateRegions();

		/// <summary>
		/// Creates regions from the grid using flood fill iteration. The region predicate is used to select
		/// whether a particular cell is in / out of the region.
		/// </summary>
		std::vector<gridRegion<T>*> locateRegions(gridDelegates::gridPredicate inclusionPredicate);

	private:

		gridRegionConstructor<T>* runFloodFill(short column, short row, gridDelegates::gridPredicate inclusionPredicate);

	private:

		grid<T>* _grid;
	};
}

