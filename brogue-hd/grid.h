#pragma once

#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleArray.h"
#include "simpleException.h"
#include "simpleList.h"
#include "simpleMath.h"
#include "simpleOrderedList.h"
#include "simpleRange.h"
#include <limits>

using namespace brogueHd::simple;

namespace brogueHd::component
{
	template<isHashable T>
	class grid
	{
	public:

		grid(const gridRect& parentBoundary, const gridRect& relativeBoundary);
		~grid();

		/// <summary>
		/// Returns value from the grid - throws exception for out of bounds indices
		/// </summary>
		T get(int column, int row) const;

		/// <summary>
		/// Returns value from the grid - or default value if out of bounds.
		/// </summary>
		T getUnsafe(int column, int row) const;

		/// <summary>
		/// Gets adjacent element from the grid
		/// </summary>
		T getAdjacent(int column, int row, brogueCompass direction) const;

		/// <summary>
		/// Gets adjacent element from the grid (OR default_value::value)
		/// </summary>
		T getAdjacentUnsafe(int column, int row, brogueCompass direction) const;

		/// <summary>
		/// Gets adjacent as cell's locator - throws exception when out of bounds
		/// </summary>
		gridLocator getAdjacentLocator(int column, int row, brogueCompass direction) const;

		/// <summary>
		/// Gets adjacent as cell's locator (or default_value::value if out of bounds)
		/// </summary>
		gridLocator getAdjacentLocatorUnsafe(int column, int row, brogueCompass direction) const;

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
		bool isDefined(int column, int row) const;

		/// <summary>
		/// Returns true if the (column, row) are in bounds
		/// </summary>
		bool isInBounds(int column, int row) const;

		/// <summary>
		/// Sets the value to the grid, clipping it to the max and min values, and handling exceptions
		/// </summary>
		void set(int column, int row, T setValue, bool overwrite = false);

		/// <summary>
		/// Searches grid for requested value based on comparison. Each truthy aggregateComparator result will store
		/// the next value as the going compare-ee. This compare-ee will be passed as the first argument of the comparator.
		/// The second argument will be the current grid value.
		/// </summary>
		/// <returns>The aggregated search value (not summed, aggregated!)</returns>
		T search(gridAggregateComparer<T> aggregateComparator) const;

		/// <summary>
		/// Returns true if the location is at the edge of the grid (using default_value::value comparison).
		/// </summary>
		bool isEdge(int column, int row) const;

		/// <summary>
		/// Returns true if the location is at the edge of the grid (using default_value::value comparison), or 
		/// the provided predicate.
		/// </summary>
		bool isEdgeWhere(int column, int row, gridPredicate<T> predicate) const;

		/// <summary>
		/// Returns true if the adjacent element is positive with respect to the provided predicate OR is
		/// out of bounds OR is default_value::value FOR the provided direction.
		/// </summary>
		/// <param name="direction">Compass direction treated with DIRECT EQUALITY! (DOESN'T USE FLAGS)</param>
		bool isExposedEdge(int column, int row, brogueCompass direction, gridPredicate<T> predicate) const;

		/// <summary>
		/// Returns true if the adjacent element is positive with respect to the provided predicate OR is
		/// out of bounds OR is default_value::value FOR the provided NON-CARDINAL direction.
		/// </summary>
		/// <param name="direction">Compass direction treated with DIRECT EQUALITY! (DOESN'T USE FLAGS)</param>
		bool isExposedCorner(int column, int row, brogueCompass direction, gridPredicate<T> predicate) const;

		/// <summary>
		/// MODIFIES LAYOUT!  This will translate the grid's sub-grid by the specified amount. The result
		/// must lie within the parent boundary.
		/// </summary>
		void translate(int column, int row);

		/// <summary>
		/// Iterates entire grid and calls user callback
		/// </summary>
		void iterate(gridCallback<T> callback) const;

		/// <summary>
		/// Iterates only where there are cells defined.
		/// </summary>
		void iterateWhereDefined(gridCallback<T> callback) const;

		/// <summary>
		/// Iterates the grid in the same manner as "iterate", column first, then row. So, the start and end
		/// locations will be honored as such.
		/// </summary>
		void iterateFrom(const gridLocator& start, const gridLocator& end, gridCallback<T> callback) const;

		/// <summary>
		/// Iterates around a specific point by one-cell in the 4 cardinal directions
		/// </summary>
		void iterateAroundCardinal(int column, int row, bool withinBounds, gridCallback<T> callback) const;

		/// <summary>
		/// Iterates around a specific point by one-cell in all 8 directions
		/// </summary>
		void iterateAdjacent(int column, int row, bool withinBounds, gridCallbackAdjacent<T> callback) const;

		/// <summary>
		/// Iterates grid within specific boundary constraint
		/// </summary>
		void iterateIn(gridRect boundary, gridCallback<T> callback) const;

		/// <summary>
		/// Iterates outward from center location to specified distance
		/// </summary>
		void iterateOutward(int centerColumn, int centerRow, int distance, gridCallback<T> callback) const;

		/// <summary>
		/// Calculates the largest sub-rectangle in the grid.  The predicate is used to determine what to include 
		/// in the search. The getUnsafe method is used - which has a potential to return nullptr's or default data.
		/// </summary>
		gridRect calculateLargestRectangle(const gridRect& minSize, gridPredicate<T> predicate) const;

		/// <summary>
		/// Calculates the largest sub-rectangle in the grid. This will use the grid::isDefined as its decision
		/// maker.
		/// </summary>
		gridRect calculateLargestRectangle(const gridRect& minSize) const;

		/// <summary>
		/// Returns a list of elements that satisy the given predicate
		/// </summary>
		simpleList<T> collectWhere(gridPredicate<T> predicate) const;

		/// <summary>
		/// Returns list of elements adjacent to the specified location (using safe iterators)
		/// </summary>
		simpleList<T> getAdjacentElements(int column, int row) const;

		/// <summary>
		/// Returns list of elements cardinally adjacent to the specified location (using safe iterators)
		/// </summary>
		simpleList<T> getCardinallyAdjacentElements(int column, int row) const;

		/// <summary>
		/// Returns list of elements adjacent to the specified location (using unsafe iterators may return null / default)
		/// </summary>
		simpleList<T> getAdjacentElementsUnsafe(int column, int row) const;

		/// <summary>
		/// Returns list of elements cardinally adjacent to the specified location (using unsafe iterators may return null / default)
		/// </summary>
		simpleList<T> getCardinallyAdjacentElementsUnsafe(int column, int row) const;

		/// <summary>
		/// Returns list of elements adjacent to the specified location (using safe iterators) that satisfy
		/// the given predicate.
		/// </summary>
		simpleList<T> getAdjacentElements(int column, int row, gridPredicate<T> predicate) const;

		/// <summary>
		/// Returns list of elements cardinally adjacent to the specified location (using safe iterators) that
		/// satisfy the given predicate.
		/// </summary>
		simpleList<T> getCardinallyAdjacentElements(int column, int row, gridPredicate<T> predicate) const;

	private:

		T** _grid;

		gridRect* _parentBoundary;
		gridRect* _relativeBoundary;
	};

	template<isHashable T>
	grid<T>::grid(const gridRect& parentBoundary, const gridRect& relativeBoundary)
	{
		if (!parentBoundary.contains(relativeBoundary))
			throw simpleException("Parent boundary does not contain relative boundary: grid.h");

		_grid = new T * [relativeBoundary.width];
		_parentBoundary = new gridRect(parentBoundary);
		_relativeBoundary = new gridRect(relativeBoundary);

		for (int index = 0; index < relativeBoundary.width; index++)
			_grid[index] = new T[relativeBoundary.height]{ default_value::value<T>() };
	}

	template<isHashable T>
	grid<T>::~grid()
	{
		// Added grid cells from this class
		for (int index = 0; index < _relativeBoundary->width; index++)
			delete[] _grid[index];

		delete[] _grid;

		delete _parentBoundary;
		delete _relativeBoundary;
	}

	template<isHashable T>
	T grid<T>::get(int column, int row) const
	{
		if (column < _relativeBoundary->column ||
			row < _relativeBoundary->row ||
			column > _relativeBoundary->right() ||
			row > _relativeBoundary->bottom())
			throw simpleException("Index outside the bounds of the array:  grid.h");

		// Must subtract off the offset to address the primary grid
		return _grid[column - _relativeBoundary->column][row - _relativeBoundary->row];
	}

	template<isHashable T>
	T grid<T>::getUnsafe(int column, int row) const
	{
		if (column < _relativeBoundary->column ||
			row < _relativeBoundary->row ||
			column > _relativeBoundary->right() ||
			row > _relativeBoundary->bottom())
			return default_value::value<T>();

		// Must subtract off the offset to address the primary grid
		return _grid[column - _relativeBoundary->column][row - _relativeBoundary->row];
	}

	template<isHashable T>
	T grid<T>::getAdjacentUnsafe(int column, int row, brogueCompass direction) const
	{
		switch (direction)
		{
			case brogueCompass::N:
				return this->isInBounds(column, row - 1) ? this->get(column, row - 1) : default_value::value<T>();

			case brogueCompass::S:
				return this->isInBounds(column, row + 1) ? this->get(column, row + 1) : default_value::value<T>();

			case brogueCompass::E:
				return this->isInBounds(column + 1, row) ? this->get(column + 1, row) : default_value::value<T>();

			case brogueCompass::W:
				return this->isInBounds(column - 1, row) ? this->get(column - 1, row) : default_value::value<T>();

			case brogueCompass::NW:
				return this->isInBounds(column - 1, row - 1) ? this->get(column - 1, row - 1) : default_value::value<T>();

			case brogueCompass::NE:
				return this->isInBounds(column + 1, row - 1) ? this->get(column + 1, row - 1) : default_value::value<T>();

			case brogueCompass::SW:
				return this->isInBounds(column - 1, row + 1) ? this->get(column - 1, row + 1) : default_value::value<T>();

			case brogueCompass::SE:
				return this->isInBounds(column + 1, row + 1) ? this->get(column + 1, row + 1) : default_value::value<T>();
			default:
				return default_value::value<T>();
		}
	}

	template<isHashable T>
	T grid<T>::getAdjacent(int column, int row, brogueCompass direction) const
	{
		switch (direction)
		{
			case brogueCompass::None:
				return default_value::value<T>();

			case brogueCompass::N:
				if (!this->isInBounds(column, row - 1))
					throw simpleException("Out of bounds exception:  grid::getAdjacent");

				return this->get(column, row - 1);
			case brogueCompass::S:
				if (!this->isInBounds(column, row + 1))
					throw simpleException("Out of bounds exception:  grid::getAdjacent");

				return this->get(column, row + 1);
			case brogueCompass::E:
				if (!this->isInBounds(column + 1, row))
					throw simpleException("Out of bounds exception:  grid::getAdjacent");

				return this->get(column + 1, row);
			case brogueCompass::W:
				if (!this->isInBounds(column - 1, row))
					throw simpleException("Out of bounds exception:  grid::getAdjacent");

				return this->get(column - 1, row);
			case brogueCompass::NW:
				if (!this->isInBounds(column - 1, row - 1))
					throw simpleException("Out of bounds exception:  grid::getAdjacent");

				return this->get(column - 1, row - 1);
			case brogueCompass::NE:
				if (!this->isInBounds(column + 1, row - 1))
					throw simpleException("Out of bounds exception:  grid::getAdjacent");

				return this->get(column + 1, row - 1);
			case brogueCompass::SW:
				if (!this->isInBounds(column - 1, row + 1))
					throw simpleException("Out of bounds exception:  grid::getAdjacent");

				return this->get(column - 1, row + 1);
			case brogueCompass::SE:
				if (!this->isInBounds(column + 1, row + 1))
					throw simpleException("Out of bounds exception:  grid::getAdjacent");

				return this->get(column + 1, row + 1);
			default:
				throw simpleException("Unhandled brogueCompass type:  grid::getAdjacent");
		}
	}

	template<isHashable T>
	gridLocator grid<T>::getAdjacentLocator(int column, int row, brogueCompass direction) const
	{
		switch (direction)
		{
			case brogueCompass::None:
				return default_value::value<T>();

			case brogueCompass::N:
				if (!this->isInBounds(column, row - 1))
					throw simpleException("Out of bounds exception:  grid::getAdjacentLocator");

				return gridLocator(column, row - 1);

			case brogueCompass::S:
				if (!this->isInBounds(column, row + 1))
					throw simpleException("Out of bounds exception:  grid::getAdjacentLocator");

				return gridLocator(column, row + 1);

			case brogueCompass::E:
				if (!this->isInBounds(column + 1, row))
					throw simpleException("Out of bounds exception:  grid::getAdjacentLocator");

				return gridLocator(column + 1, row);

			case brogueCompass::W:
				if (!this->isInBounds(column - 1, row))
					throw simpleException("Out of bounds exception:  grid::getAdjacentLocator");

				return gridLocator(column - 1, row);

			case brogueCompass::NW:
				if (!this->isInBounds(column - 1, row - 1))
					throw simpleException("Out of bounds exception:  grid::getAdjacentLocator");

				return gridLocator(column - 1, row - 1);

			case brogueCompass::NE:
				if (!this->isInBounds(column + 1, row - 1))
					throw simpleException("Out of bounds exception:  grid::getAdjacentLocator");

				return gridLocator(column + 1, row - 1);

			case brogueCompass::SW:
				if (!this->isInBounds(column - 1, row + 1))
					throw simpleException("Out of bounds exception:  grid::getAdjacentLocator");

				return gridLocator(column - 1, row + 1);

			case brogueCompass::SE:
				if (!this->isInBounds(column + 1, row + 1))
					throw simpleException("Out of bounds exception:  grid::getAdjacentLocator");

				return gridLocator(column + 1, row + 1);
			default:
				throw simpleException("Unhandled brogueCompass type:  grid::getAdjacentLocator");
		}
	}

	template<isHashable T>
	gridLocator grid<T>::getAdjacentLocatorUnsafe(int column, int row, brogueCompass direction) const
	{
		switch (direction)
		{
			case brogueCompass::None:
				return default_value::value<T>();

			case brogueCompass::N:
				if (!this->isInBounds(column, row - 1))
					return default_value::value<gridLocator>();

				return gridLocator(column, row - 1);

			case brogueCompass::S:
				if (!this->isInBounds(column, row + 1))
					return default_value::value<gridLocator>();

				return gridLocator(column, row + 1);

			case brogueCompass::E:
				if (!this->isInBounds(column + 1, row))
					return default_value::value<gridLocator>();

				return gridLocator(column + 1, row);

			case brogueCompass::W:
				if (!this->isInBounds(column - 1, row))
					return default_value::value<gridLocator>();

				return gridLocator(column - 1, row);

			case brogueCompass::NW:
				if (!this->isInBounds(column - 1, row - 1))
					return default_value::value<gridLocator>();

				return gridLocator(column - 1, row - 1);

			case brogueCompass::NE:
				if (!this->isInBounds(column + 1, row - 1))
					return default_value::value<gridLocator>();

				return gridLocator(column + 1, row - 1);

			case brogueCompass::SW:
				if (!this->isInBounds(column - 1, row + 1))
					return default_value::value<gridLocator>();

				return gridLocator(column - 1, row + 1);

			case brogueCompass::SE:
				if (!this->isInBounds(column + 1, row + 1))
					return default_value::value<gridLocator>();

				return gridLocator(column + 1, row + 1);
			default:
				return default_value::value<gridLocator>();
		}
	}

	template<isHashable T>
	gridRect grid<T>::getRelativeBoundary() const
	{
		return *_relativeBoundary;
	}

	template<isHashable T>
	gridRect grid<T>::getParentBoundary() const
	{
		return *_parentBoundary;
	}

	template<isHashable T>
	bool grid<T>::isDefined(int column, int row) const
	{
		return  this->get(column, row) != default_value::value<T>();
	}

	template<isHashable T>
	bool grid<T>::isInBounds(int column, int row) const
	{
		return _relativeBoundary->contains(column, row);
	}

	template<isHashable T>
	void grid<T>::set(int column, int row, T value, bool overwrite)
	{
		if (!this->isInBounds(column, row))
			throw simpleException("Grid out of bounds:  grid.cpp");

		if (this->isDefined(column, row) && !overwrite)
			throw simpleException("Trying to overwrite grid value:  grid.cpp (use remove first)");

		// Must subtract off the relative boundary offset to address the primary grid
		_grid[column - _relativeBoundary->column][row - _relativeBoundary->row] = value;
	}

	template<isHashable T>
	T grid<T>::search(gridAggregateComparer<T> aggregateComparator) const
	{
		T searchValue = default_value::value<T>();

		grid<T>* grid = this;

		iterate(this, [&grid] (int column, int row)
		{
			if (aggregateComparator(searchValue, grid->get(column, row)))
				searchValue = grid->get(column, row);
		});

		return searchValue;
	}

	template<isHashable T>
	bool grid<T>::isEdge(int column, int row) const
	{
		return this->isEdgeWhere(column, row, [] (int acolumn, int arow, T item)
		{
			return item != default_value::value<T>();
		});
	}

	template<isHashable T>
	bool grid<T>::isEdgeWhere(int column, int row, gridPredicate<T> predicate) const
	{
		T north = this->getUnsafe(column, row - 1);
		T south = this->getUnsafe(column, row + 1);
		T east = this->getUnsafe(column + 1, row);
		T west = this->getUnsafe(column - 1, row);
		T northEast = this->getUnsafe(column + 1, row - 1);
		T northWest = this->getUnsafe(column - 1, row - 1);
		T southEast = this->getUnsafe(column + 1, row + 1);
		T southWest = this->getUnsafe(column - 1, row + 1);

		return (north == default_value::value<T>() || (north != default_value::value<T>() && !predicate(column, row - 1, north))) ||
			(south == default_value::value<T>() || (south != default_value::value<T>() && !predicate(column, row + 1, south))) ||
			(east == default_value::value<T>() || (east != default_value::value<T>() && !predicate(column + 1, row, east))) ||
			(west == default_value::value<T>() || (west != default_value::value<T>() && !predicate(column - 1, row, west))) ||
			(northEast == default_value::value<T>() || (northEast != default_value::value<T>() && !predicate(column + 1, row - 1, northEast))) ||
			(northWest == default_value::value<T>() || (northWest != default_value::value<T>() && !predicate(column - 1, row - 1, northWest))) ||
			(southEast == default_value::value<T>() || (southEast != default_value::value<T>() && !predicate(column + 1, row + 1, southEast))) ||
			(southWest == default_value::value<T>() || (southWest != default_value::value<T>() && !predicate(column - 1, row + 1, southWest)));
	}

	template<isHashable T>
	bool grid<T>::isExposedEdge(int column, int row, brogueCompass direction, gridPredicate<T> predicate) const
	{
		T north = this->getUnsafe(column, row - 1);
		T south = this->getUnsafe(column, row + 1);
		T east = this->getUnsafe(column + 1, row);
		T west = this->getUnsafe(column - 1, row);

		if (direction == brogueCompass::N)
			return north == default_value::value<T>() || (north != default_value::value<T>() && !predicate(column, row - 1, north));

		else if (direction == brogueCompass::S)
			return south == default_value::value<T>() || (south != default_value::value<T>() && !predicate(column, row + 1, south));

		else if (direction == brogueCompass::E)
			return east == default_value::value<T>() || (east != default_value::value<T>() && !predicate(column + 1, row, east));

		else if (direction == brogueCompass::W)
			return west == default_value::value<T>() || (west != default_value::value<T>() && !predicate(column - 1, row, west));

		else
			throw simpleException("Invalid use of direction parameter:  grid.isExposedEdge");
	}

	template<isHashable T>
	bool grid<T>::isExposedCorner(int column, int row, brogueCompass direction, gridPredicate<T> predicate) const
	{
		if (direction == brogueCompass::NW)
			return isExposedEdge(column, row, brogueCompass::N, predicate) &&
			isExposedEdge(column, row, brogueCompass::W, predicate);

		else if (direction == brogueCompass::NE)
			return isExposedEdge(column, row, brogueCompass::N, predicate) &&
			isExposedEdge(column, row, brogueCompass::E, predicate);

		else if (direction == brogueCompass::SE)
			return isExposedEdge(column, row, brogueCompass::S, predicate) &&
			isExposedEdge(column, row, brogueCompass::E, predicate);

		else if (direction == brogueCompass::SW)
			return isExposedEdge(column, row, brogueCompass::S, predicate) &&
			isExposedEdge(column, row, brogueCompass::W, predicate);

		else
			throw simpleException("Invalid use of direction parameter:  grid.isExposedCorner");
	}

	template<isHashable T>
	bool grid<T>::areAdjacent(T location, T otherLocation) const
	{
		if (!this->isDefined(location.column, location.row))
			return false;

		if (!this->isDefined(otherLocation.column, otherLocation.row))
			return false;

		if (simpleMath::abs(otherLocation.column - location.column) > 1)
			return false;

		if (simpleMath::abs(otherLocation.row - location.row) > 1)
			return false;

		return true;
	}

	template<isHashable T>
	void grid<T>::translate(int column, int row)
	{
		gridLocator translation(column, row);

		if (!_parentBoundary->contains((*_relativeBoundary) + translation))
			throw simpleException("Grid translation falls outside of parent bounds:  grid.h");

		_relativeBoundary->translate(translation);
	}

	template<isHashable T>
	void grid<T>::iterate(gridCallback<T> callback) const
	{
		bool userBreak = false;

		gridRect boundary = this->getRelativeBoundary();

		for (int i = boundary.left(); i <= boundary.right() && !userBreak; i++)
		{
			for (int j = boundary.top(); j <= boundary.bottom() && !userBreak; j++)
			{
				if (callback(i, j, this->get(i, j)) == iterationCallback::breakAndReturn)
					userBreak = true;
			}
		}
	}

	template<isHashable T>
	void grid<T>::iterateWhereDefined(gridCallback<T> callback) const
	{
		bool userBreak = false;

		gridRect boundary = this->getRelativeBoundary();

		for (int i = boundary.left(); i <= boundary.right() && !userBreak; i++)
		{
			for (int j = boundary.top(); j <= boundary.bottom() && !userBreak; j++)
			{
				// Block non-defined location callbacks
				if (!this->isDefined(i, j))
					continue;

				if (callback(i, j, this->get(i, j)) == iterationCallback::breakAndReturn)
					userBreak = true;
			}
		}
	}

	template<isHashable T>
	void grid<T>::iterateFrom(const gridLocator& start, const gridLocator& end, gridCallback<T> callback) const
	{
		if (start.row > end.row ||
		   (start.row == end.row && start.column > end.column))
			throw simpleException("Start location is greater than end location in order of iteration:  grid<>::iterateFrom");

		gridRect boundary = this->getRelativeBoundary();

		if (!boundary.contains(start) ||
			!boundary.contains(end))
			throw simpleException("Start or end location is not inside the relative boundary of the grid:  grid<>::iterateFrom");

		bool userBreak = false;

		int column = start.column;
		int row = start.row;

		while (column <= end.column && row <= end.row && !userBreak)
		{
			if (callback(column, row, this->get(column, row)) == iterationCallback::breakAndReturn)
				userBreak = true;

			column++;

			if (column == boundary.right())
			{
				column = boundary.left();
				row++;
			}
		}
	}

	template<isHashable T>
	void grid<T>::iterateOutward(int centerColumn,
		int centerRow,
		int distance,
		gridCallback<T> callback) const
	{
		bool userBreak = false;

		gridRect boundary = this->getRelativeBoundary();

		int left = simpleMath::clamp(centerColumn - distance, boundary.left(), boundary.right());
		int right = simpleMath::clamp(centerColumn + distance, boundary.left(), boundary.right());
		int top = simpleMath::clamp(centerRow - distance, boundary.top(), boundary.bottom());
		int bottom = simpleMath::clamp(centerColumn + distance, boundary.top(), boundary.bottom());

		for (int i = left; i <= right && !userBreak; i++)
		{
			for (int j = top; j <= bottom && !userBreak; j++)
			{
				if (callback(i, j, this->get(i, j)) == iterationCallback::breakAndReturn)
					userBreak = true;
			}
		}
	}

	template<isHashable T>
	void grid<T>::iterateIn(gridRect boundary, gridCallback<T> callback) const
	{
		bool userBreak = false;

		gridRect gridBoundary = this->getRelativeBoundary();

		int safeRight = simpleMath::clamp(boundary.right(), gridBoundary.left(), gridBoundary.right());
		int safeBottom = simpleMath::clamp(boundary.bottom(), gridBoundary.top(), gridBoundary.bottom());

		for (int i = boundary.left(); i <= safeRight && !userBreak; i++)
		{
			for (int j = boundary.top(); j <= safeBottom && !userBreak; j++)
			{
				if (callback(i, j, this->get(i, j)) == iterationCallback::breakAndReturn)
					userBreak = true;
			}
		}
	}

	template<isHashable T>
	void grid<T>::iterateAdjacent(int column, int row, bool withinBounds, gridCallbackAdjacent<T> callback) const
	{
		bool userBreak = false;

		// N
		if (!userBreak && (this->isInBounds(column, row - 1) || !withinBounds))
			userBreak = callback(column, row - 1, brogueCompass::N, this->getAdjacentUnsafe(column, row, brogueCompass::N));

		// S
		if (!userBreak && (this->isInBounds(column, row + 1) || !withinBounds))
			userBreak = callback(column, row + 1, brogueCompass::S, this->getAdjacentUnsafe(column, row, brogueCompass::S));

		// E
		if (!userBreak && (this->isInBounds(column + 1, row) || !withinBounds))
			userBreak = callback(column + 1, row, brogueCompass::E, this->getAdjacentUnsafe(column, row, brogueCompass::E));

		// W
		if (!userBreak && (this->isInBounds(column - 1, row) || !withinBounds))
			userBreak = callback(column - 1, row, brogueCompass::W, this->getAdjacentUnsafe(column, row, brogueCompass::W));

		// NE
		if (!userBreak && (this->isInBounds(column + 1, row - 1) || !withinBounds))
			userBreak = callback(column + 1, row - 1, brogueCompass::NE, this->getAdjacentUnsafe(column, row, brogueCompass::NE));

		// NW
		if (!userBreak && (this->isInBounds(column - 1, row - 1) || !withinBounds))
			userBreak = callback(column - 1, row - 1, brogueCompass::NW, this->getAdjacentUnsafe(column, row, brogueCompass::NW));

		// SE
		if (!userBreak && (this->isInBounds(column + 1, row + 1) || !withinBounds))
			userBreak = callback(column + 1, row + 1, brogueCompass::SE, this->getAdjacentUnsafe(column, row, brogueCompass::SE));

		// SW
		if (!userBreak && (this->isInBounds(column - 1, row + 1) || !withinBounds))
			userBreak = callback(column - 1, row + 1, brogueCompass::SW, this->getAdjacentUnsafe(column, row, brogueCompass::SW));
	}

	template<isHashable T>
	void grid<T>::iterateAroundCardinal(int column, int row, bool withinBounds, gridCallback<T> callback) const
	{
		iterationCallback response = iterationCallback::iterate;

		// North
		T north = this->getAdjacentUnsafe(column, row, brogueCompass::N);

		if (this->isInBounds(column, row - 1))
			response = callback(column, row - 1, north);

		else if (!withinBounds)
			response = callback(column, row - 1, north);

		if (response == iterationCallback::breakAndReturn)
			return;

		// South
		T south = this->getAdjacentUnsafe(column, row, brogueCompass::S);

		if (this->isInBounds(column, row + 1))
			response = callback(column, row + 1, south);

		else if (!withinBounds)
			response = callback(column, row + 1, south);

		if (response == iterationCallback::breakAndReturn)
			return;

		// East
		T east = this->getAdjacentUnsafe(column, row, brogueCompass::E);

		if (this->isInBounds(column + 1, row))
			response = callback(column + 1, row, east);

		else if (!withinBounds)
			response = callback(column + 1, row, east);

		if (response == iterationCallback::breakAndReturn)
			return;

		// West
		T west = this->getAdjacentUnsafe(column, row, brogueCompass::W);

		if (this->isInBounds(column - 1, row))
			response = callback(column - 1, row, west);

		else if (!withinBounds)
			response = callback(column - 1, row, west);
	}

	template<isHashable T>
	simpleList<T> grid<T>::collectWhere(gridPredicate<T> predicate) const
	{
		simpleList<T> result;

		this->iterate([&result, &predicate] (int column, int row, const T& item)
		{
			if (predicate(column, row, item))
				result.add(item);

			return iterationCallback::iterate;
		});

		return result;
	}

	template<isHashable T>
	simpleList<T> grid<T>::getAdjacentElements(int column, int row) const
	{
		simpleList<T> result;

		this->iterateAdjacent(column, row, true, [&result] (int column, int row, brogueCompass direction, const T& item)
		{
			result.add(item);
			return iterationCallback::iterate;
		});

		return result;
	}

	template<isHashable T>
	simpleList<T> grid<T>::getCardinallyAdjacentElements(int column, int row) const
	{
		simpleList<T> result;

		this->iterateAroundCardinal(column, row, true, [&result] (int column, int row, const T& item)
		{
			result.add(item);
			return iterationCallback::iterate;
		});

		return result;
	}

	template<isHashable T>
	simpleList<T> grid<T>::getAdjacentElementsUnsafe(int column, int row) const
	{
		simpleList<T> result;

		this->iterateAdjacent(column, row, false, [&result] (int column, int row, brogueCompass direction, const T& item)
		{
			result.add(item);
			return iterationCallback::iterate;
		});

		return result;
	}

	template<isHashable T>
	simpleList<T> grid<T>::getCardinallyAdjacentElementsUnsafe(int column, int row) const
	{
		simpleList<T> result;

		this->iterateAroundCardinal(column, row, false, [&result] (int column, int row, const T& item)
		{
			result.add(item);
			return iterationCallback::iterate;
		});

		return result;
	}

	template<isHashable T>
	simpleList<T> grid<T>::getAdjacentElements(int column, int row, gridPredicate<T> predicate) const
	{
		simpleList<T> result;

		this->iterateAdjacent(column, row, true, [&result, &predicate] (int acolumn, int arow, brogueCompass direction, const T& item)
		{
			if (predicate(acolumn, arow, item))
				result.add(item);

			return iterationCallback::iterate;
		});

		return result;
	}

	template<isHashable T>
	simpleList<T> grid<T>::getCardinallyAdjacentElements(int column, int row, gridPredicate<T> predicate) const
	{
		simpleList<T> result;

		this->iterateAroundCardinal(column, row, true, [&result, &predicate] (int acolumn, int arow, const T& item)
		{
			if (predicate(acolumn, arow, item))
				result.add(item);

			return iterationCallback::iterate;
		});

		return result;
	}


	template<isHashable T>
	gridRect grid<T>::calculateLargestRectangle(const gridRect& minSize) const
	{
		const grid<T>* that = this;

		return calculateLargestRectangle(minSize, [&that] (int column, int row, const T& item)
		{
			return that->isDefined(column, row);
		});
	}

	template<isHashable T>
	gridRect grid<T>::calculateLargestRectangle(const gridRect& minSize, gridPredicate<T> predicate) const
	{
		const grid<T>* that = this;
		gridRect boundary = this->getRelativeBoundary();

		int maxSum = std::numeric_limits<int>::min();

		simpleArray<int> counters(boundary.height);
		simpleRange<int> contiguousRange(-1, -1, -1, counters.count());
		int sum = -1;
		bool found = false;

		// Keep an ordered list of best rectangle
		simpleOrderedList<gridRect*> subRectangles([] (gridRect* rect1, gridRect* rect2)
		{
			return rect1->area() - rect2->area();
		});

		// Set the counters to carry the row index
		for (int index = 0; index < counters.count(); index++)
		{
			counters.set(index, index + boundary.top());
		}

		for (int column = boundary.left(); column <= boundary.right(); column++)
		{
			// Reset range
			contiguousRange.set(-1, -1);

			// Get best sub-array
			sum = counters.kadanesAlgorithm<int>(contiguousRange, [&column, &predicate, &that] (int rowIndex)
			{
				return predicate(column, rowIndex, that->get(column, rowIndex)) ? 1 : 0; // Return positive if the cell predicate passes
			});

			found = false;

			if (contiguousRange.getLow() < 0 ||
				contiguousRange.getHigh() < 0)
				continue;

			// Add this to the list unless there is currently one of the same size (in rows). Otherwise,
			// update any rect instances that overlap this rect.
			//
			gridRect newRect(column, contiguousRange.getLow() + boundary.top(), 1, contiguousRange.size());

			// Update rectangles
			subRectangles.forEach([&found, &column, &newRect, &boundary] (gridRect* rect)
			{
				// Equals
				if (rect->top() == newRect.top() &&
					rect->bottom() == newRect.bottom() &&
					rect->right() == column - 1)
				{
					rect->width = rect->width + 1;
					found = true;
				}

				// Overlaps (The rect could be LARGER in area if you add this overlap)
				else if (((rect->top() >= newRect.top() && rect->top() <= newRect.bottom()) ||
						 (rect->bottom() >= newRect.top() && rect->bottom() <= newRect.bottom())) &&
						  (rect->right() == column - 1))
				{
					int top = simpleMath::maxOf(rect->top(), newRect.top());
					int bottom = simpleMath::minOf(rect->bottom(), newRect.bottom());

					gridRect testRect(rect->column, top, rect->width + 1, bottom - top + 1);

					// Better fit: Take new dimensions
					if (testRect.area() > rect->area())
					{
						rect->row = testRect.row;
						rect->column = testRect.column;
						rect->width = testRect.width;
						rect->height = testRect.height;

						// DON'T set found = true. The next rectangle from now on could end up
						// being greater than this one.
					}
				}

				return iterationCallback::iterate;
			});

			// Start a new rectangle for this iteration (check constraint)
			if (!found && (sum > 0) && (contiguousRange.size() >= minSize.height))
			{
				gridRect nextRect(column, contiguousRange.getLow() + boundary.top(), 1, contiguousRange.size());

				subRectangles.add(new gridRect(nextRect));
			}
		}

		if (subRectangles.count() == 0)
			return default_value::value<gridRect>();

		else
		{
			// Get result data (checked the height, now we must check the width)
			simpleList<gridRect*> finalRects = subRectangles.where([&minSize] (gridRect* rect)
			{
				return rect->width >= minSize.width &&
					rect->height >= minSize.height;

			});

			gridRect result = finalRects.count() == 0 ? default_value::value<gridRect>() : *(finalRects.get(finalRects.count() - 1));

			// Clean up memory
			for (int index = 0; index < subRectangles.count(); index++)
			{
				delete subRectangles.get(index);
			}

			return result;
		}
	}
}


/*
// Procedure
//
// 1) For each row:  - Count across and add to row counters foreach non-null cell
//                   - For each NULL cell -> reset the counters
//                   - Then, take contiguous sums of counters with the Max(Min(counter value))
//                     This represents a rectangle from the previous rows. Update this best fit.
//
//
// TODO: Use a stack-based approach to cut down on iterating

gridRect boundary = this->getRelativeBoundary();
int rowCountersLength = boundary.width;
simpleArray<int> rowCounters(rowCountersLength);

int bestStartColumn = -1;
int bestEndColumn = -1;
int bestStartRow = -1;
int bestEndRow = -1;
int bestArea = 0;

for (int row = boundary.top(); row <= boundary.bottom(); row++)
{
	for (int column = boundary.left(); column <= boundary.right(); column++)
	{
		int index = column - boundary.left();

		// FIRST, INCREMENT ROW COUNTERS
		if (predicate(column, row, this->getUnsafe(column, row)))
			rowCounters.set(index, rowCounters.get(index) + 1);
		else
			rowCounters.set(index, 0);
	}

	// From the Left
	for (int index1 = 0; index1 < rowCountersLength; index1++)
	{
		// Initialize min-height for the next sweep (from the left)
		int minHeight = rowCounters.get(index1);

		// From the left
		for (int index2 = index1; index2 < rowCountersLength && minHeight > 0; index2++)
		{
			minHeight = simpleMath::minOf(minHeight, rowCounters.get(index1), rowCounters.get(index2));

			// Current column against previous
			if (rowCounters.get(index1) > bestArea)
			{
				bestStartColumn = index1 + boundary.left();
				bestEndColumn = index1 + boundary.left();
				bestStartRow = row - rowCounters.get(index1) + 1;
				bestEndRow = row;

				bestArea = rowCounters.get(index1);
			}

			// Current column check
			if (rowCounters.get(index2) > bestArea)
			{
				bestStartColumn = index2 + boundary.left();
				bestEndColumn = index2 + boundary.left();
				bestStartRow = row - rowCounters.get(index2) + 1;
				bestEndRow = row;

				bestArea = rowCounters.get(index2);
			}

			// Current min-block check
			if (minHeight * ((index2 - index1) + 1) > bestArea)
			{
				bestStartColumn = index1 + boundary.left();
				bestEndColumn = index2 + boundary.left();
				bestStartRow = row - minHeight + 1;
				bestEndRow = row;

				bestArea = minHeight * ((index2 - index1) + 1);
			}
		}
	}

	// From the Right
	for (int index1 = rowCountersLength - 1; index1 >= 0; index1--)
	{
		// Initialize min-height for the next sweep (from the right)
		int minHeight = rowCounters.get(index1);

		// From the right
		for (int index2 = index1; index2 >= 0 && minHeight > 0; index2--)
		{
			minHeight = simpleMath::minOf(minHeight, rowCounters.get(index1), rowCounters.get(index2));

			// Current column against previous
			if (rowCounters.get(index1) > bestArea)
			{
				bestStartColumn = index1 + boundary.left();
				bestEndColumn = index1 + boundary.left();
				bestStartRow = row - rowCounters.get(index1) + 1;
				bestEndRow = row;

				bestArea = rowCounters.get(index1);
			}

			// Current column check
			if (rowCounters.get(index2) > bestArea)
			{
				bestStartColumn = index2 + boundary.left();
				bestEndColumn = index2 + boundary.left();
				bestStartRow = row - rowCounters.get(index2) + 1;
				bestEndRow = row;

				bestArea = rowCounters.get(index2);
			}

			// Current min-block check
			if (minHeight * ((index1 - index2) + 1) > bestArea)
			{
				// Columns are reversed
				bestStartColumn = index2 + boundary.left();
				bestEndColumn = index1 + boundary.left();
				bestStartRow = row - minHeight + 1;
				bestEndRow = row;

				bestArea = minHeight * ((index1 - index2) + 1);
			}
		}
	}
}

if (bestArea == 0)
	return default_value::value<gridRect>();

// Validation: Check boundaries; check that the predicate passes
//
const grid<T>* that = this;
gridRect result(bestStartColumn, bestStartRow, (bestEndColumn - bestStartColumn) + 1, (bestEndRow - bestStartRow) + 1);

if (!boundary.contains(result))
	throw simpleException("Invalid sub-region rectangle calculation:  gridRegionConstructor::calculateLargestRectangle");

result.iterate([&that, &predicate] (int column, int row)
{
	if (!predicate(column, row, that->getUnsafe(column, row)))
		throw simpleException("Invalid sub-region rectangle calculation:  gridRegionConstructor::calculateLargestRectangle");

	return iterationCallback::iterate;
});

return result;
*/
//}

///// <summary>
///// Checks for grid adjacency using an AND mask with the provided compass constrained direction.
///// </summary>
//public static CompassConstrained GetAdjacency<T>(this Grid<T> grid, int column, int row)
//{
//    CompassConstrained result = CompassConstrained.Null;

//    if (grid.IsDefined(column, row - 1))
//        result |= CompassConstrained.N;

//    if (grid.IsDefined(column, row + 1))
//        result |= CompassConstrained.S;

//    if (grid.IsDefined(column + 1, row))
//        result |= CompassConstrained.E;

//    if (grid.IsDefined(column - 1, row))
//        result |= CompassConstrained.W;

//    if (grid.IsDefined(column + 1, row - 1))
//        result |= CompassConstrained.NE;

//    if (grid.IsDefined(column - 1, row - 1))
//        result |= CompassConstrained.NW;

//    if (grid.IsDefined(column + 1, row + 1))
//        result |= CompassConstrained.SE;

//    if (grid.IsDefined(column - 1, row + 1))
//        result |= CompassConstrained.SW;

//    return result;
//}


//// Takes a grid as a mask of valid locations, chooses one randomly and returns it as (x, y).
//// If there are no valid locations, returns (-1, -1).

//template<isHashable T>
//void grid<T>::randomLocationInGrid(int* x, int* y, int validValue) 
//{
//    const int locationCount = validLocationCount(grid, validValue);
//    int i, j;

//    if (locationCount <= 0) {
//        *x = *y = -1;
//        return;
//    }
//    int index = rand_range(0, locationCount - 1);
//    for (i = 0; i < DCOLS && index >= 0; i++) {
//        for (j = 0; j < DROWS && index >= 0; j++) {
//            if (grid[i][j] == validValue) {
//                if (index == 0) {
//                    *x = i;
//                    *y = j;
//                }
//                index--;
//            }
//        }
//    }
//    return;
//}

//// Finds the lowest positive number in a grid, chooses one location with that number randomly and returns it as (x, y).
//// If there are no valid locations, returns (-1, -1).

//template<isHashable T>
//void grid<T>::randomLeastPositiveLocationInGrid(int** grid, int* x, int* y, boolean deterministic) {
//    const int targetValue = leastPositiveValueInGrid(grid);
//    int locationCount;
//    int i, j, index;

//    if (targetValue == 0) {
//        *x = *y = -1;
//        return;
//    }

//    locationCount = 0;
//    for (i = 0; i < DCOLS; i++) {
//        for (j = 0; j < DROWS; j++) {
//            if (grid[i][j] == targetValue) {
//                locationCount++;
//            }
//        }
//    }

//    if (deterministic) {
//        index = locationCount / 2;
//    }
//    else {
//        index = rand_range(0, locationCount - 1);
//    }

//    for (i = 0; i < DCOLS && index >= 0; i++) {
//        for (j = 0; j < DROWS && index >= 0; j++) {
//            if (grid[i][j] == targetValue) {
//                if (index == 0) {
//                    *x = i;
//                    *y = j;
//                }
//                index--;
//            }
//        }
//    }
//    return;
//}

//template<isHashable T>
//boolean grid<T>::getQualifyingPathLocNear(int* retValX, int* retValY,
//    int x, int y,
//    boolean hallwaysAllowed,
//    unsigned long blockingTerrainFlags,
//    unsigned long blockingMapFlags,
//    unsigned long forbiddenTerrainFlags,
//    unsigned long forbiddenMapFlags,
//    boolean deterministic) {
//    int** grid, ** costMap;
//    int loc[2];

//    // First check the given location to see if it works, as an optimization.
//    if (!cellHasTerrainFlag(x, y, blockingTerrainFlags | forbiddenTerrainFlags)
//        && !(pmap[x][y].flags & (blockingMapFlags | forbiddenMapFlags))
//        && (hallwaysAllowed || passableArcCount(x, y) <= 1)) {

//        *retValX = x;
//        *retValY = y;
//        return true;
//    }

//    // Allocate the grids.
//    grid = allocGrid();
//    costMap = allocGrid();

//    // Start with a base of a high number everywhere.
//    fillGrid(grid, 30000);
//    fillGrid(costMap, 1);

//    // Block off the pathing blockers.
//    getTerrainGrid(costMap, PDS_FORBIDDEN, blockingTerrainFlags, blockingMapFlags);
//    if (blockingTerrainFlags & (T_OBSTRUCTS_DIAGONAL_MOVEMENT | T_OBSTRUCTS_PASSABILITY)) {
//        getTerrainGrid(costMap, PDS_OBSTRUCTION, T_OBSTRUCTS_DIAGONAL_MOVEMENT, 0);
//    }

//    // Run the distance scan.
//    grid[x][y] = 1;
//    costMap[x][y] = 1;
//    dijkstraScan(grid, costMap, true);
//    findReplaceGrid(grid, 30000, 30000, 0);

//    // Block off invalid targets that aren't pathing blockers.
//    getTerrainGrid(grid, 0, forbiddenTerrainFlags, forbiddenMapFlags);
//    if (!hallwaysAllowed) {
//        getPassableArcGrid(grid, 2, 10, 0);
//    }

//    // Get the solution.
//    randomLeastPositiveLocationInGrid(grid, retValX, retValY, deterministic);

//    //    dumpLevelToScreen();
//    //    displayGrid(grid);
//    //    if (coordinatesAreInMap(*retValX, *retValY)) {
//    //        hiliteCell(*retValX, *retValY, &yellow, 100, true);
//    //    }
//    //    temporaryMessage("Qualifying path selected:", true);

//    freeGrid(grid);
//    freeGrid(costMap);

//    // Fall back to a pathing-agnostic alternative if there are no solutions.
//    if (*retValX == -1 && *retValY == -1) {
//        if (getQualifyingLocNear(loc, x, y, hallwaysAllowed, NULL,
//            (blockingTerrainFlags | forbiddenTerrainFlags),
//            (blockingMapFlags | forbiddenMapFlags),
//            false, deterministic)) {
//            *retValX = loc[0];
//            *retValY = loc[1];
//            return true; // Found a fallback solution.
//        }
//        else {
//            return false; // No solutions.
//        }
//    }
//    else {
//        return true; // Found a primary solution.
//    }
//}



//template<gridCellConstraint T>
//int grid<T>::fillContiguousRegion(int column, int row, T fillValue) 
//{
//    int numberOfCells = 1;

//    // Fill current cell value
//    //
//    this->set(column, row, fillValue);

//    // Recurse through in a flood-fill fashion
//    //
//    iterateAroundCardinal(this, column, row, true, [](int x, int y)
//    {
//        // If the neighbor is an unmarked region cell
//        //
//        if (!this->isZeroValue(x,y))
//        { 
//            numberOfCells += this->fillContiguousRegion(x, y, fillValue); // then recurse.
//        }
//    });

//    return numberOfCells;
//}

//template<gridCellConstraint T>
//std::vector<gridRegion<T>*> grid<T>::locateRegions(function<bool(T)> predicate, T fillValue)
//{
//    std::vector<gridRegion<T>*> result;

//    iterate(this, [](int column, int row)
//    {
//        // Check history
//        for (int index = 0; index < result.size(); index++)
//        {
//            // Return from iterator (only)
//            if (predicate(result[i][column, row]))
//                return;
//        }

//        // Check to see if we're at a valid location
//        if (predicate(_grid[column, row]))
//        {
//            // Start a new region
//            grid<T>* regionGrid = new grid<T>(_columns, _rows, _zeroValue, _maxValue);

//            // Keep track of the boundary
//            gridRect boundary(column, row, 0, 0);

//            // Set initial value
//            regionGrid->set(column, row, fillValue);

//            // Recurse to fill out the grid
//            this->locateRegionRecurse(regionGrid, column, row, predicate, fillValue, predicate);

//            // Set result
//            result.push_back(new gridRegion<T>(regionGrid, boundary));
//        }
//    });

//    return result;
//}

//template<gridCellConstraint T>
//void grid<T>::locateRegionRecurse(grid<T>* regionGrid, gridRect& boundary, int currentColumn, int currentRow, T fillValue, function<bool(T)> predicate)
//{
//    // Recurse through in a flood-fill fashion
//    //
//    iterateAroundCardinal(this, currentColumn, currentRow, true, [](int x, int y)
//    {
//        // Neighbor Cell:  Protect from infinite recursion by checking region grid
//        //
//        if (predicate(this, x,y) && !predicate(regionGrid, x, y))
//        {
//            // Mark as region
//            regionGrid->set(x, y, fillValue);

//            // Expand boundary
//            boundary.expand(x, y);

//            // Recurse
//            locateRegionRecurse(regionGrid, boundary, x, y, fillValue, predicate);
//        }
//    });
//}

