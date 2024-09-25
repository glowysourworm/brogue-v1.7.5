#pragma once

#include "randomGenerator.h"
#include "broguedef.h"
#include "gridRect.h"
#include "gridDefinitions.h"
#include "gridRegion.h"
#include "extensionDefinitions.h"
#include <functional>

using namespace std;

using namespace brogueHd::backend::extension;

namespace brogueHd::backend::model::layout
{
	template<typename T>
	class grid
	{
	public:

		grid(gridRect parentBoundary, gridRect relativeBoundary);
		~grid();

		/// <summary>
		/// Returns value from the grid
		/// </summary>
		T get(short column, short row) const;

		/// <summary>
		/// Gets adjacent element from the grid
		/// </summary>
		T getAdjacent(short column, short row, brogueCompass direction) const;

		/// <summary>
		/// Gets adjacent element from the grid (OR NULL)
		/// </summary>
		T getAdjacentUnsafe(short column, short row, brogueCompass direction) const;

		/// <summary>
		/// Gets adjacent as cell's locator (or NULL if out of bounds)
		/// </summary>
		gridLocator getAdjacentLocator(short column, short row, brogueCompass direction) const;

		/// <summary>
		/// Returns true if grid cell locations are adjacent
		/// </summary>
		bool areAdjacent(T location, T otherLocation) const;

		/// <summary>
		/// Returns the relative boundary of the grid (parent boundary contains this boundary)
		/// </summary>
		gridRect getRelativeBoundary() const;

		/// <summary>
		/// Returns parent boundary of the grid
		/// </summary>
		gridRect getParentBoundary() const;

		/// <summary>
		/// Returns true if the (column, row) correspond to the grid's zero value
		/// </summary>
		bool isDefined(short column, short row) const;

		/// <summary>
		/// Returns true if the (column, row) are in bounds
		/// </summary>
		bool isInBounds(short column, short row) const;

		/// <summary>
		/// Sets instances from the supplied region to the grid
		/// </summary>
		void setFromRegion(gridRegion<T>* region);

		/// <summary>
		/// Sets the value to the grid, clipping it to the max and min values, and handling exceptions
		/// </summary>
		void set(short column, short row, T setValue);

		/// <summary>
		/// Searches grid for requested value based on comparison. Each truthy aggregateComparator result will store
		/// the next value as the going compare-ee. This compare-ee will be passed as the first argument of the comparator.
		/// The second argument will be the current grid value.
		/// </summary>
		/// <returns>The aggregated search value (not summed, aggregated!)</returns>
		T search(gridDelegates<T>::aggregateComparer aggregateComparator) const;

		/// <summary>
		/// Returns true if the location is at the edge of the grid (using NULL comparison).
		/// </summary>
		bool isEdge(short column, short row) const;

		/// <summary>
		/// Returns true if the location is at the edge of the grid (using NULL comparison), or 
		/// the provided predicate.
		/// </summary>
		bool isEdge(short column, short row, gridDelegates<T>::simplePredicate predicate) const;

		/// <summary>
		/// Returns true if the adjacent element is positive with respect to the provided predicate OR is
		/// out of bounds OR is null FOR the provided direction.
		/// </summary>
		/// <param name="direction">Compass direction treated with DIRECT EQUALITY! (DOESN'T USE FLAGS)</param>
		bool isExposedEdge(int column, int row, brogueCompass direction, gridDelegates<T>::simplePredicate predicate) const;

		/// <summary>
		/// Returns true if the adjacent element is positive with respect to the provided predicate OR is
		/// out of bounds OR is null FOR the provided NON-CARDINAL direction.
		/// </summary>
		/// <param name="direction">Compass direction treated with DIRECT EQUALITY! (DOESN'T USE FLAGS)</param>
		bool isExposedCorner(int column, int row, brogueCompass direction, gridDelegates<T>::simplePredicate predicate) const;

	private:

		T** _grid;

		gridRect _parentBoundary;
		gridRect _relativeBoundary;
	};
}

