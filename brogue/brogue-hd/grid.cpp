#include "grid.h"
#include "broguedef.h"
#include "brogueMath.h"
#include "color.h"
#include "gridRegion.h"
#include "gridMethods.h"
#include "gridDefinitions.h"
#include "randomGenerator.h"
#include "exceptionHandler.h"
#include <functional>
#include <exception>

using namespace std;

namespace brogueHd::backend::math;

namespace brogueHd::backend::model::layout
{
    template<gridCellConstraint T>
    grid<T>::grid(short columns, short rows)
    {
        _grid = new T[columns][rows];
        _rows = rows;
        _columns = columns;
        _zeroValue = zeroValue;
        _maxValue = maxValue;
    }

    template<gridCellConstraint T>
    grid<T>::~grid()
    {
        delete[][] _grid;

        delete _zeroValue;
        delete _maxValue;
    }

    template<gridCellConstraint T>
    T grid<T>::get(short column, short row) const
    {
        return _grid[column][row];
    }

    template<gridCellConstraint T>
    T grid<T>::getOrNull(short column, short row) const
    {
        if (!_grid->isDefined(column, row))
            return NULL;

        return _grid[column][row];
    }

    template<gridCellConstraint T>
    T grid<T>::getAdjacent(short column, short row, brogueCompass direction) const
    {
        switch (brogueCompass)
        {
        case brogueCompass::None:
            return NULL;

        case brogueCompass::N:
            return this->getOrNull(column, row - 1);

        case brogueCompass::S:
            return this->getOrNull(column, row + 1);

        case brogueCompass::E:
            return this->getOrNull(column + 1, row);

        case brogueCompass::W:
            return this->getOrNull(column - 1, row);

        case brogueCompass::NW:
            return this->getOrNull(column - 1, row - 1);

        case brogueCompass::NE:
            return this->getOrNull(column + 1, row - 1);

        case brogueCompass::SW:
            return this->getOrNull(column - 1, row + 1);

        case brogueCompass::SE:
            return this->getOrNull(column + 1, row + 1);
        default:
            return NULL;
        }
    }

    template<gridCellConstraint T>
    gridRect grid<T>::getBoundary() const
    {
        return gridRect(0, 0, _columns, _rows);
    }

    template<gridCellConstraint T>
    bool grid<T>::isDefined(short column, short row) const
    {
        return _grid[column][row] != NULL;
    }

    template<gridCellConstraint T>
    bool grid<T>::isInBounds(short column, short row) const
    {
        if (column < 0 ||
            row < 0 ||
            column > _columns - 1 ||
            row > _rows - 1)
            return false;

        return true;
    }

    template<gridCellConstraint T>
    void grid<T>::set(short column, short row, T value)
    {
        if (column < 0 ||
            row < 0 ||
            column > _columns - 1 ||
            row > _rows - 1)
            throw std::runtime_exception(std::string("Grid out of bounds:  grid.cpp"));

        // Clips the value for the grid (not sure how to handle all template situations). This
        // seems to compile for the grid<short>.
        //
        if (value < _zeroValue)
            _grid[column][row] = _zeroValue;

        else if (value > _maxValue)
            _grid[column][row] = _maxValue;

        else
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

    template<gridCellConstraint T>
    short grid<T>::count(T value) 
    {
        short count = 0;

        iterate(this, [](short column, short row)
        {
            if (_grid[column, row] == value)
                count++;
        });

        return count;
    }

    template<gridCellConstraint T>
    T grid<T>::search(gridDelegates::gridAggregateComparer aggregateComparator) const
    {
        T searchValue;
        
        iterate(this, [](short column, short row)
        {
            if (aggregateComparator(searchValue, _grid[column, row]))
                searchValue = _grid[column][row];
        });

        return searchValue;
    }

    template<gridCellConstraint T>
    bool grid<T>::isEdge(short column, short row) const
    {
        return isEdge(column, row, [](short, short, T value) 
        {
            return value != NULL;
        });
    }

    /// <summary>
    /// Returns true if the location is at the edge of the grid (using NULL comparison), or 
    /// the provided predicate.
    /// </summary>
    template<gridCellConstraint T>
    bool grid<T>::isEdge(short column, short row, gridDelegates::gridPredicate predicate) const
    {
        T north = this->getOrNull(column, row - 1);
        T south = this->getOrNull((column, row + 1);
        T east = this->getOrNull((column + 1, row);
        T west = this->getOrNull((column - 1, row);
        T northEast = this->getOrNull((column + 1, row - 1);
        T northWest = this->getOrNull((column - 1, row - 1);
        T southEast = this->getOrNull((column + 1, row + 1);
        T southWest = this->getOrNull((column - 1, row + 1);

        return (north == NULL || (north != NULL && !predicate(column, row - 1, north))) ||
                (south == NULL || (south != NULL && !predicate(column, row + 1, south))) ||
                (east == NULL || (east != NULL && !predicate(column + 1, row, east))) ||
                (west == NULL || (west != NULL && !predicate(column - 1, row, west))) ||
                (northEast == NULL || (northEast != NULL && !predicate(column + 1, row - 1, northEast))) ||
                (northWest == NULL || (northWest != NULL && !predicate(column - 1, row - 1, northWest))) ||
                (southEast == NULL || (southEast != NULL && !predicate(column + 1, row + 1, southEast))) ||
                (southWest == NULL || (southWest != NULL && !predicate(column - 1, row + 1, southWest)));
    }

    template<gridCellConstraint T>
    bool grid<T>::isExposedEdge(int column, int row, brogueCompass direction, gridDelegates::gridPredicate predicate) const
    {
        T north = this->getOrNull(column, row - 1);
        T south = this->getOrNull((column, row + 1);
        T east = this->getOrNull((column + 1, row);
        T west = this->getOrNull((column - 1, row);

        if (direction == brogueCompass::N)
            return north == NULL || (north != NULL && !predicate(column, row - 1, north));

        else if (direction == brogueCompass::S)
            return south == NULL || (south != NULL && !predicate(column, row + 1, south));

        else if (direction == brogueCompass::E)
            return east == NULL || (east != NULL && !predicate(column + 1, row, east));

        else if (direction == brogueCompass::W)
            return west == NULL || (west != NULL && !predicate(column - 1, row, west));

        else
            brogueException::show("Invalid use of direction parameter:  grid.isExposedEdge");
    }

    template<gridCellConstraint T>
    bool grid<T>::isExposedCorner(int column, int row, brogueCompass direction, gridDelegates::gridPredicate predicate) const
    {
        if (direction == brogueCompass::NW)
            return isExposedEdge(grid, column, row, Compass.N, predicate) &&
                   isExposedEdge(grid, column, row, Compass.W, predicate);

        else if (direction == brogueCompass::NE)
            return isExposedEdge(grid, column, row, Compass.N, predicate) &&
                   isExposedEdge(grid, column, row, Compass.E, predicate);

        else if (direction == brogueCompass::SE)
            return isExposedEdge(grid, column, row, Compass.S, predicate) &&
                   isExposedEdge(grid, column, row, Compass.E, predicate);

        else if (direction == brogueCompass::SW)
            return isExposedEdge(grid, column, row, Compass.S, predicate) &&
                   isExposedEdge(grid, column, row, Compass.W, predicate);

        else
            brogueException::show("Invalid use of direction parameter:  grid.isExposedCorner");
    }

    template<gridCellConstraint T>
    bool grid<T>::areAdjacent(T location, T otherLocation) const
    {
        if (!this->isDefined(location.column, location.row))
            return false;

        if (!this->isDefined(otherLocation.column, otherLocation.row))
            return false;

        if (brogueMath::abs(otherLocation.column - location.column) > 1)
            return false;

        if (brogueMath::abs(otherLocation.row - location.row) > 1)
            return false;

        return true;
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
    //void grid<T>::cellularAutomataIteration(cellularAutomataParameters parameters)
    //{
    //    short nbCount;

    //    iterateIn(this, column, row, width, height, [](short columnRect, short rowRect)
    //    {
    //        // Count of "alive" cells at this location
    //        nbCount = 0;

    //        iterateAround(this, columnRect, rowRect, true, [](short i, short j)
    //        {
    //            // If our grid "is defined" here
    //            //
    //            if (!this->isZeroValue(i, j))
    //                nbCount++;
    //        });

    //        if (this->isZeroValue(columnRect, rowRect) && birthParameters[nbCount])
    //        {
    //            this->set(columnRect, rowRect, birthValue)	    // birth
    //        }
    //        else if (!this->isZeroValue(columnRect, rowRect) && survivalParameters[nbCount])
    //        {
    //            // survival
    //        }
    //        else
    //        {
    //            this->set(columnRect, rowRect, _zeroValue);     // death
    //        }
    //    });
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
    //void generateCellularAutomata(randomGenerator* randomGenerator, cellularAutomataParameters parameters)
    //{
    //    // Procedure
    //    //
    //    // 1) Generate white noise in the specified region
    //    // 2) Run the cellularAutomataSmoothing function the specified number of times
    //    //      -> While iterating:  try to check original Brogue constraints
    //    //        

    //    short random, smoothingPassesMax;

    //    // Generate white noise inside the chosen rectangle
    //    //
    //    iterateIn(this, column, row, width, height, [](short x, short y)
    //    {
    //        if (randomGenerator->rand_percent(random))
    //            this->set(x, y, fillValue);
    //    });

    //    // Run smoothing iterations
    //    //
    //    for (int index = 0; index < clamp(smoothingPasses, CELLULAR_AUTOMATA_MAX); index++)
    //    {
    //        this->cellularAutomataIteration(parameters);
    //    }
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