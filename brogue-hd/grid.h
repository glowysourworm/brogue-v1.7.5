#pragma once

#include "simpleException.h"
#include "simpleMath.h"
#include "gridRect.h"
#include "gridDefinitions.h"

using namespace brogueHd::simple;

namespace brogueHd::component
{
	template<typename T>
	class grid
	{
	public:

		grid(){};
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
		/// Sets the value to the grid, clipping it to the max and min values, and handling exceptions
		/// </summary>
		void set(short column, short row, T setValue);

		/// <summary>
		/// Searches grid for requested value based on comparison. Each truthy aggregateComparator result will store
		/// the next value as the going compare-ee. This compare-ee will be passed as the first argument of the comparator.
		/// The second argument will be the current grid value.
		/// </summary>
		/// <returns>The aggregated search value (not summed, aggregated!)</returns>
		T search(gridAggregateComparer<T> aggregateComparator) const;

		/// <summary>
		/// Returns true if the location is at the edge of the grid (using NULL comparison).
		/// </summary>
		bool isEdge(short column, short row) const;

		/// <summary>
		/// Returns true if the location is at the edge of the grid (using NULL comparison), or 
		/// the provided predicate.
		/// </summary>
		bool isEdgeWhere(short column, short row, gridPredicate<T> predicate) const;

		/// <summary>
		/// Returns true if the adjacent element is positive with respect to the provided predicate OR is
		/// out of bounds OR is null FOR the provided direction.
		/// </summary>
		/// <param name="direction">Compass direction treated with DIRECT EQUALITY! (DOESN'T USE FLAGS)</param>
		bool isExposedEdge(int column, int row, brogueCompass direction, gridPredicate<T> predicate) const;

		/// <summary>
		/// Returns true if the adjacent element is positive with respect to the provided predicate OR is
		/// out of bounds OR is null FOR the provided NON-CARDINAL direction.
		/// </summary>
		/// <param name="direction">Compass direction treated with DIRECT EQUALITY! (DOESN'T USE FLAGS)</param>
		bool isExposedCorner(int column, int row, brogueCompass direction, gridPredicate<T> predicate) const;

		/// <summary>
		/// Iterates entire grid and calls user callback
		/// </summary>
		void iterate(gridCallback<T> callback) const;

		/// <summary>
		/// Iterates around a specific point by one-cell in the 4 cardinal directions
		/// </summary>
		void iterateAroundCardinal(short column, short row, bool withinBounds, gridCallback<T> callback) const;

		/// <summary>
		/// Iterates around a specific point by one-cell in all 8 directions
		/// </summary>
		void iterateAround(short column, short row, bool withinBounds, gridCallback<T> callback) const;

		/// <summary>
		/// Iterates grid within specific boundary constraint
		/// </summary>
		void iterateIn(gridRect boundary, gridCallback<T> callback) const;

		/// <summary>
		/// Iterates outward from center location to specified distance
		/// </summary>
		void iterateOutward(short centerColumn, short centerRow, short distance, gridCallback<T> callback) const;

	private:

		T** _grid;

		gridRect _parentBoundary;
		gridRect _relativeBoundary;
	};

    template<typename T>
    grid<T>::grid(gridRect parentBoundary, gridRect relativeBoundary)
    {
        _grid = new T * [relativeBoundary.width];

        for (int index = 0; index < relativeBoundary.width; index++)
            _grid[index] = new T[relativeBoundary.height];

        _relativeBoundary = relativeBoundary;
        _parentBoundary = parentBoundary;
    }

    template<typename T>
    grid<T>::~grid()
    {
        // Added grid cells from this class
        for (int index = 0; index < _relativeBoundary.height; index++)
            delete[] _grid[index];

        delete[] _grid;
    }

    template<typename T>
    T grid<T>::get(short column, short row) const
    {
        return _grid[column][row];
    }

    template<typename T>
    T grid<T>::getAdjacentUnsafe(short column, short row, brogueCompass direction) const
    {
        switch (brogueCompass)
        {
        case brogueCompass::None:
            return NULL;

        case brogueCompass::N:
            return this->get(column, row - 1);

        case brogueCompass::S:
            return this->get(column, row + 1);

        case brogueCompass::E:
            return this->get(column + 1, row);

        case brogueCompass::W:
            return this->get(column - 1, row);

        case brogueCompass::NW:
            return this->get(column - 1, row - 1);

        case brogueCompass::NE:
            return this->get(column + 1, row - 1);

        case brogueCompass::SW:
            return this->get(column - 1, row + 1);

        case brogueCompass::SE:
            return this->get(column + 1, row + 1);
        default:
            return NULL;
        }
    }

    template<typename T>
    T grid<T>::getAdjacent(short column, short row, brogueCompass direction) const
    {
        switch (direction)
        {
        case brogueCompass::None:
            return default_value<T>::value;

        case brogueCompass::N:
            if (!this->isInBounds(column, row - 1))
                simpleException::showCstr("Out of bounds exception:  grid::getAdjacent");

            return this->get(column, row - 1);
        case brogueCompass::S:
            if (!this->isInBounds(column, row + 1))
                simpleException::showCstr("Out of bounds exception:  grid::getAdjacent");

            return this->get(column, row + 1);
        case brogueCompass::E:
            if (!this->isInBounds(column + 1, row))
                simpleException::showCstr("Out of bounds exception:  grid::getAdjacent");

            return this->get(column + 1, row);
        case brogueCompass::W:
            if (!this->isInBounds(column - 1, row))
                simpleException::showCstr("Out of bounds exception:  grid::getAdjacent");

            return this->get(column - 1, row);
        case brogueCompass::NW:
            if (!this->isInBounds(column - 1, row - 1))
                simpleException::showCstr("Out of bounds exception:  grid::getAdjacent");

            return this->get(column - 1, row - 1);
        case brogueCompass::NE:
            if (!this->isInBounds(column + 1, row - 1))
                simpleException::showCstr("Out of bounds exception:  grid::getAdjacent");

            return this->get(column + 1, row - 1);
        case brogueCompass::SW:
            if (!this->isInBounds(column - 1, row + 1))
                simpleException::showCstr("Out of bounds exception:  grid::getAdjacent");

            return this->get(column - 1, row + 1);
        case brogueCompass::SE:
            if (!this->isInBounds(column + 1, row + 1))
                simpleException::showCstr("Out of bounds exception:  grid::getAdjacent");

            return this->get(column + 1, row + 1);
        default:
            return default_value<T>::value;
        }
    }

    template<typename T>
    gridLocator grid<T>::getAdjacentLocator(short column, short row, brogueCompass direction) const
    {
        switch (brogueCompass)
        {
        case brogueCompass::None:
            return NULL;

        case brogueCompass::N:
            if (!this->isInBounds(column, row - 1))
                return NULL;

            return gridLocator(column, row - 1);

        case brogueCompass::S:
            if (!this->isInBounds(column, row + 1))
                return NULL;

            return gridLocator(column, row + 1);

        case brogueCompass::E:
            if (!this->isInBounds(column + 1, row))
                return NULL;

            return gridLocator(column + 1, row);

        case brogueCompass::W:
            if (!this->isInBounds(column - 1, row))
                return NULL;

            return gridLocator(column - 1, row);

        case brogueCompass::NW:
            if (!this->isInBounds(column - 1, row - 1))
                return NULL;

            return gridLocator(column - 1, row - 1);

        case brogueCompass::NE:
            if (!this->isInBounds(column + 1, row - 1))
                return NULL;

            return gridLocator(column + 1, row - 1);

        case brogueCompass::SW:
            if (!this->isInBounds(column - 1, row + 1))
                return NULL;

            return gridLocator(column - 1, row + 1);

        case brogueCompass::SE:
            if (!this->isInBounds(column + 1, row + 1))
                return NULL;

            return gridLocator(column + 1, row + 1);
        default:
            return NULL;
        }
    }

    template<typename T>
    gridRect grid<T>::getRelativeBoundary() const
    {
        return _relativeBoundary;
    }

    template<typename T>
    gridRect grid<T>::getParentBoundary() const
    {
        return _parentBoundary;
    }

    template<typename T>
    bool grid<T>::isDefined(short column, short row) const
    {
        return this->get(column, row) == default_value<T>::value;
    }

    template<typename T>
    bool grid<T>::isInBounds(short column, short row) const
    {
        return _relativeBoundary.contains(column, row);
    }

    template<typename T>
    void grid<T>::set(short column, short row, T value)
    {
        if (!_relativeBoundary.contains(column, row))
            simpleException::showCstr("Grid out of bounds:  grid.cpp");

        if (this->isDefined(column, row))
            simpleException::showCstr("Trying to overwrite grid value:  grid.cpp (use remove first)");

        _grid[column][row] = value;
    }

    // Fills grid locations with the given value if they match any terrain flags or map flags.
    // Otherwise does not change the grid location.

    //template<typename T>
    //void grid<T>::getTerrainGrid(short** grid, short value, unsigned long terrainFlags, unsigned long mapFlags) 
    //{
    //    short i, j;
    //    for (i = 0; i < DCOLS; i++) {
    //        for (j = 0; j < DROWS; j++) {
    //            if (grid[i][j] != value && cellHasTerrainFlag(i, j, terrainFlags) || (pmap[i][j].flags & mapFlags)) {
    //                grid[i][j] = value;
    //            }
    //        }
    //    }
    //}

    //template<typename T>
    //void grid<T>::getTMGrid(short** grid, short value, unsigned long TMflags) {
    //    short i, j;
    //    for (i = 0; i < DCOLS; i++) {
    //        for (j = 0; j < DROWS; j++) {
    //            if (grid[i][j] != value && cellHasTMFlag(i, j, TMflags)) {
    //                grid[i][j] = value;
    //            }
    //        }
    //    }
    //}

    //template<typename T>
    //void grid<T>::getPassableArcGrid(short** grid, short minPassableArc, short maxPassableArc, short value) {
    //    short i, j, count;
    //    for (i = 0; i < DCOLS; i++) {
    //        for (j = 0; j < DROWS; j++) {
    //            if (grid[i][j] != value) {
    //                count = passableArcCount(i, j);
    //                if (count >= minPassableArc && count <= maxPassableArc) {
    //                    grid[i][j] = value;
    //                }
    //            }
    //        }
    //    }
    //}

    template<typename T>
    T grid<T>::search(gridAggregateComparer<T> aggregateComparator) const
    {
        T searchValue;

        iterate(this, [](short column, short row)
        {
            if (aggregateComparator(searchValue, _grid[column, row]))
                searchValue = _grid[column][row];
        });

        return searchValue;
    }

    template<typename T>
    bool grid<T>::isEdge(short column, short row) const
    {
        return this->isEdgeWhere(column, row, [](short acolumn, short arow, T item)
        {
            return item != default_value<T>::value;
        });
    }

    /// <summary>
    /// Returns true if the location is at the edge of the grid (using NULL comparison), or 
    /// the provided predicate.
    /// </summary>
    template<typename T>
    bool grid<T>::isEdgeWhere(short column, short row, gridPredicate<T> predicate) const
    {
        T north = this->get(column, row - 1);
        T south = this->get(column, row + 1);
        T east = this->get(column + 1, row);
        T west = this->get(column - 1, row);
        T northEast = this->get(column + 1, row - 1);
        T northWest = this->get(column - 1, row - 1);
        T southEast = this->get(column + 1, row + 1);
        T southWest = this->get(column - 1, row + 1);

        return (north == default_value<T>::value || (north != default_value<T>::value && !predicate(column, row - 1, north))) ||
                (south == default_value<T>::value || (south != default_value<T>::value && !predicate(column, row + 1, south))) ||
                (east == default_value<T>::value || (east != default_value<T>::value && !predicate(column + 1, row, east))) ||
                (west == default_value<T>::value || (west != default_value<T>::value && !predicate(column - 1, row, west))) ||
                (northEast == default_value<T>::value || (northEast != default_value<T>::value && !predicate(column + 1, row - 1, northEast))) ||
                (northWest == default_value<T>::value || (northWest != default_value<T>::value && !predicate(column - 1, row - 1, northWest))) ||
                (southEast == default_value<T>::value || (southEast != default_value<T>::value && !predicate(column + 1, row + 1, southEast))) ||
                (southWest == default_value<T>::value || (southWest != default_value<T>::value && !predicate(column - 1, row + 1, southWest)));
    }

    template<typename T>
    bool grid<T>::isExposedEdge(int column, int row, brogueCompass direction, gridPredicate<T> predicate) const
    {
        T north = this->get(column, row - 1);
        T south = this->get(column, row + 1);
        T east = this->get(column + 1, row);
        T west = this->get(column - 1, row);

        if (direction == brogueCompass::N)
            return north == default_value<T>::value || (north != default_value<T>::value && !predicate(column, row - 1, north));

        else if (direction == brogueCompass::S)
            return south == default_value<T>::value || (south != default_value<T>::value && !predicate(column, row + 1, south));

        else if (direction == brogueCompass::E)
            return east == default_value<T>::value || (east != default_value<T>::value && !predicate(column + 1, row, east));

        else if (direction == brogueCompass::W)
            return west == default_value<T>::value || (west != default_value<T>::value && !predicate(column - 1, row, west));

        else
            simpleException::showCstr("Invalid use of direction parameter:  grid.isExposedEdge");
    }

    template<typename T>
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
            simpleException::showCstr("Invalid use of direction parameter:  grid.isExposedCorner");
    }

    template<typename T>
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

    template<typename T>
    void grid<T>::iterate(gridCallback<T> callback) const
    {
        bool userBreak = false;

        gridRect boundary = this->getRelativeBoundary();

        for (short i = boundary.left(); i <= boundary.right() && !userBreak; i++)
        {
            for (short j = boundary.top(); j <= boundary.bottom() && !userBreak; j++)
            {
                if (callback(i, j, this->get(i, j)) == iterationCallback::breakAndReturn)
                    userBreak = true;
            }
        }
    }

    template<typename T>
    void grid<T>::iterateOutward(short centerColumn,
        short centerRow,
        short distance,
        gridCallback<T> callback) const
    {
        bool userBreak = false;

        gridRect boundary = this->getRelativeBoundary();

        short left = simpleMath::clamp(centerColumn - distance, boundary.left(), boundary.right());
        short right = simpleMath::clamp(centerColumn + distance, boundary.left(), boundary.right());
        short top = simpleMath::clamp(centerRow - distance, boundary.top(), boundary.bottom());
        short bottom = simpleMath::clamp(centerColumn + distance, boundary.top(), boundary.bottom());

        for (short i = left; i <= right && !userBreak; i++)
        {
            for (short j = top; j <= bottom && !userBreak; j++)
            {
                if (callback(i, j, this->get(i, j)) == iterationCallback::breakAndReturn)
                    userBreak = true;
            }
        }
    }

    template<typename T>
    void grid<T>::iterateIn(gridRect boundary, gridCallback<T> callback) const
    {
        bool userBreak = false;

        gridRect gridBoundary = this->getRelativeBoundary();

        short safeRight = simpleMath::clamp(boundary.right(), gridBoundary.left(), gridBoundary.right());
        short safeBottom = simpleMath::clamp(boundary.bottom(), gridBoundary.top(), gridBoundary.bottom());

        for (short i = boundary.left(); i <= safeRight && !userBreak; i++)
        {
            for (short j = boundary.top(); j <= safeBottom && !userBreak; j++)
            {
                if (callback(i, j, this->get(i, j)) == iterationCallback::breakAndReturn)
                    userBreak = true;
            }
        }
    }

    template<typename T>
    void grid<T>::iterateAround(short column, short row, bool withinBounds, gridCallback<T> callback) const
    {
        short newX, newY;

        bool userBreak = false;

        for (short i = column - 1; i < column + 1 && !userBreak; i++)
        {
            for (short j = row - 1; j < row + 1 && !userBreak; j++)
            {
                if (withinBounds)
                {
                    if (this->isInBounds(i, j))
                        userBreak = callback(i, j, this->get(i, j));
                }
                else
                {
                    userBreak = callback(i, j, this->get(i, j));
                }
            }
        }
    }

    template<typename T>
    void grid<T>::iterateAroundCardinal(short column, short row, bool withinBounds, gridCallback<T> callback) const
    {
        iterationCallback response = iterationCallback::iterate;

        // North
        T north = grid->getAdjacentUnsafe(column, row - 1);

        if (this->isInBounds(column, row - 1))
            response = callback(column, row - 1, north);

        else if (!withinBounds)
            response = callback(column, row - 1, north);

        if (response == iterationCallback::breakAndReturn)
            return;

        // South
        T south = this->getAdjacentUnsafe(column, row + 1);

        if (this->isInBounds(column, row + 1))
            response = callback(column, row + 1, south);

        else if (!withinBounds)
            response = callback(column, row + 1, south);

        if (response == iterationCallback::breakAndReturn)
            return;

        // East
        T east = this->getAdjacentUnsafe(column + 1, row);

        if (this->isInBounds(column + 1, row))
            response = callback(column + 1, row, east);

        else if (!withinBounds)
            response = callback(column + 1, row, east);

        if (response == iterationCallback::breakAndReturn)
            return;

        // West
        T west = this->getAdjacentUnsafe(column - 1, row);

        if (this->isInBounds(column - 1, row))
            response = callback(column - 1, row, west);

        else if (!withinBounds)
            response = callback(column - 1, row, west);
    }

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

    //template<typename T>
    //void grid<T>::randomLocationInGrid(short* x, short* y, short validValue) 
    //{
    //    const short locationCount = validLocationCount(grid, validValue);
    //    short i, j;

    //    if (locationCount <= 0) {
    //        *x = *y = -1;
    //        return;
    //    }
    //    short index = rand_range(0, locationCount - 1);
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

    //template<typename T>
    //void grid<T>::randomLeastPositiveLocationInGrid(short** grid, short* x, short* y, boolean deterministic) {
    //    const short targetValue = leastPositiveValueInGrid(grid);
    //    short locationCount;
    //    short i, j, index;

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

    //template<typename T>
    //boolean grid<T>::getQualifyingPathLocNear(short* retValX, short* retValY,
    //    short x, short y,
    //    boolean hallwaysAllowed,
    //    unsigned long blockingTerrainFlags,
    //    unsigned long blockingMapFlags,
    //    unsigned long forbiddenTerrainFlags,
    //    unsigned long forbiddenMapFlags,
    //    boolean deterministic) {
    //    short** grid, ** costMap;
    //    short loc[2];

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
    //short grid<T>::fillContiguousRegion(short column, short row, T fillValue) 
    //{
    //    short numberOfCells = 1;

    //    // Fill current cell value
    //    //
    //    this->set(column, row, fillValue);

    //    // Recurse through in a flood-fill fashion
    //    //
    //    iterateAroundCardinal(this, column, row, true, [](short x, short y)
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

    //    iterate(this, [](short column, short row)
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
    //void grid<T>::locateRegionRecurse(grid<T>* regionGrid, gridRect& boundary, short currentColumn, short currentRow, T fillValue, function<bool(T)> predicate)
    //{
    //    // Recurse through in a flood-fill fashion
    //    //
    //    iterateAroundCardinal(this, currentColumn, currentRow, true, [](short x, short y)
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
}

