#pragma once

#include "randomGenerator.h"
#include "broguedef.h"
#include "griddef.h"
#include "gridRegion.h"
#include <functional>

using namespace std;

namespace brogueHd::backend::model::layout
{
	// Constraint that the type T must inherit from gridOperator<T>
	template<typename T>
	concept comparable = requires(T a, T b)
	{
		{ a <= b } -> std::convertible_to<bool>;
		{ a < b }  -> std::convertible_to<bool>;
		{ a > b }  -> std::convertible_to<bool>;
		{ a >= b } -> std::convertible_to<bool>;
		{ a == b } -> std::convertible_to<bool>;
	};

	template<comparable T>
	class grid
	{
	public:

		grid(short columns, short rows, T zeroValue, T maxValue);
		~grid();

		T get(short column, short row) const;

		gridRect getBoundary() const;
		short columnCount() const;
		short rowCount() const;

		/// <summary>
		/// Returns true if the (column, row) correspond to the grid's zero value
		/// </summary>
		bool isZeroValue(short column, short row) const;

		/// <summary>
		/// Returns true if the (column, row) are in bounds
		/// </summary>
		bool isInBounds(short column, short row) const;

		/// <summary>
		/// Completely fills grid with the specified value
		/// </summary>
		void fill(T value);
		
		/// <summary>
		/// Flood-fills the grid from (x, y) along cells that are within the eligible range.
		/// Returns the total count of filled cells.
		/// </summary>
		short floodFill(short x, short y, T eligibleValueMin, T eligibleValueMax, T fillValue);

		/// <summary>
		/// Sets the value to the grid, clipping it to the max and min values, and handling exceptions
		/// </summary>
		void set(short column, short row, T setValue);

		/// <summary>
		/// Sets value for rectangle on the grid
		/// </summary>
		void setRectangle(short x, short y, short width, short height, T value);

		/// <summary>
		/// Sets value for a circle on the grid
		/// </summary>
		void setCircle(short x, short y, short radius, T value);

		/// <summary>
		/// Sets the intersection of the grid with the other grid
		/// </summary>
		void setIntersection(const grid<T>* otherGrid, T value);

		/// <summary>
		/// Sets the union of the grid with the other grid
		/// </summary>
		void setUnion(const grid<T>* otherGrid, T value);

		/// <summary>
		/// Sets value of grid based on provided predicate
		/// </summary>
		/// <param name="predicate">function that gets set based on the current grid (column, row, and value)</param>
		void setFor(function<bool(short, short, T)> predicate, T setValue);

		/// <summary>
		/// Returns count of search value from the grid
		/// </summary>
		/// <param name="value">value to search for in the grid</param>
		short count(T value);

		/// <summary>
		/// Searches grid for requested value based on comparison. Each truthy aggregateComparator result will store
		/// the next value as the going compare-ee. This compare-ee will be passed as the first argument of the comparator.
		/// The second argument will be the current grid value.
		/// </summary>
		/// <returns>The aggregated search value (not summed, aggregated!)</returns>
		T search(function<bool(T, T)> aggregateComparator) const;

		/// <summary>
		/// Fills contiguous, 4-way cardinal, region with the supplied value. Checking is done against the grid's zero
		/// value, supplied by the constructor.
		/// </summary>
		short fillContiguousRegion(short column, short row, T fillValue);

		/// <summary>
		/// Generates cellular automata with the specified parameters. NOTE*** Smoothing Passes needs to be less than or equal to 10.
		/// </summary>
		void generateCellularAutomata(randomGenerator* randomGenerator, cellularAutomataParameters parameters);

		/// <summary>
		/// Creates regions from the grid using flood fill iteration. The region predicate is used to select
		/// whether a particular cell is in / out of the region.
		/// </summary>
		std::vector<gridRegion*> locateRegions(function<bool(T)> predicate, T fillValue);

	private:

		/// <summary>
		/// Single pass of CA algorithm. This will apply the supplied birth / death parameters.
		/// </summary>
		void cellularAutomataIteration(cellularAutomataParameters parameters);

		void locateRegionRecurse(grid<T>* regionGrid, gridRect& boundary, short currentColumn, short currentRow, T fillValue, function<bool(T)> predicate);

	private:

		T** _grid;
		short _rows;
		short _columns;

		T _zeroValue;
		T _maxValue;
	};
}

