#include "grid.h"
#include "broguedef.h"
#include "color.h"
#include "gridRegion.h"
#include "gridMethods.h"
#include "griddef.h"
#include "randomGenerator.h"
#include <functional>
#include <exception>

using namespace std;

namespace brogueHd::backend::model::layout
{
    template<comparable T>
    grid<T>::grid(short columns, short rows, T zeroValue, T maxValue)
    {
        _grid = new T[columns][rows];
        _rows = rows;
        _columns = columns;
        _zeroValue = zeroValue;
        _maxValue = maxValue;
    }

    template<comparable T>
    grid<T>::~grid()
    {
        delete[][] _grid;

        delete _zeroValue;
        delete _maxValue;
    }

    template<comparable T>
    T grid<T>::get(short column, short row) const
    {
        return _grid[column][row];
    }

    template<comparable T>
    gridRect grid<T>::getBoundary() const
    {
        return gridRect(0, 0, _columns, _rows);
    }

    template<comparable T>
    short grid<T>::columnCount() const
    {
        return _columns;
    }

    template<comparable T>
    short grid<T>::rowCount() const
    {
        return _rows;
    }

    template<comparable T>
    bool grid<T>::isZeroValue(short column, short row) const
    {
        return _grid[column][row] == _zeroValue;
    }

    template<comparable T>
    bool grid<T>::isInBounds(short column, short row) const
    {
        if (column < 0 ||
            row < 0 ||
            column > _columns - 1 ||
            row > _rows - 1)
            return false;

        return true;
    }

    template<comparable T>
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

    template<comparable T>
    void grid<T>::fill(T fillValue)
    {
        iterate(this, [](short column, short row) 
        { 
            this->set(column, row, fillValue); 
        });
    }

    template<comparable T>
    short grid<T>::floodFill(short x, short y, T eligibleValueMin, T eligibleValueMax, T fillValue) 
    {
        enum directions dir;
        short newX, newY, fillCount = 1;

        brogueAssert(fillValue < eligibleValueMin || fillValue > eligibleValueMax);

        // Fill the current cell
        //
        this->set(x, y, fillValue);

        // Recurse over the cardinally adjacent cells and fill eligible ones
        //
        iterateAroundCardinal(this, x, y, true, [](short column, short row)
        {
            if (_grid[column, row] >= eligibleValueMin &&
                _grid[newX][newY] <= eligibleValueMax)
            {
                fillCount += floodFill(newX, newY, eligibleValueMin, eligibleValueMax, fillValue);
            }
        });

        return fillCount;
    }

    template<comparable T>
    void grid<T>::setRectangle(short x, short y, short width, short height, T value) 
    {
        short i, j;

        for (i = x; i < x + width; i++) {
            for (j = y; j < y + height; j++) {
                this->set(i, j, value);
            }
        }
    }

    template<comparable T>
    void grid<T>::setCircle(short x, short y, short radius, T value) 
    {
        short i, j;

        for (i = max(0, x - radius - 1); i < max(_columns, x + radius); i++) {
            for (j = max(0, y - radius - 1); j < max(_rows, y + radius); j++) {
                if ((i - x) * (i - x) + (j - y) * (j - y) < radius * radius + radius) {
                    this->set(i, j, value);
                }
            }
        }
    }

    template<comparable T>
    void grid<T>::setIntersection(const grid<T>* otherGrid, T setValue) 
    {
        short i, j;
        for (i = 0; i < _columns; i++)
        {
            for (j = 0; j < _rows; j++)
            {
                // Set intersection based on comparison
                //
                if (!this->isZeroValue() && !otherGrid->isZeroValue())
                    this->set(i, j, setValue);
            }
        }
    }

    template<comparable T>
    void grid<T>::setUnion(const grid<T>* otherGrid, T setValue)
    {
        short i, j;
        for (i = 0; i < _columns; i++)
        {
            for (j = 0; j < _rows; j++)
            {
                // Set union based on comparison
                //
                if (!this->isZeroValue() || !otherGrid->isZeroValue())
                    this->set(i, j, setValue);
            }
        }
    }

    template<comparable T>
    void grid<T>::setFor(function<bool(short, short, T)> predicate, T setValue)
    {
        iterate(this, [](short column, short row)
        {
            if (predicate(column, row, _grid[i][j]))
                this->set(column, row, setValue);
        });
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

    template<comparable T>
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

    template<comparable T>
    T grid<T>::search(function<bool(T, T)> aggregateComparator) const
    {
        T searchValue;
        
        iterate(this, [](short column, short row)
        {
            if (aggregateComparator(searchValue, _grid[column, row]))
                searchValue = _grid[column][row];
        });

        return searchValue;
    }

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

    template<comparable T>
    void grid<T>::cellularAutomataIteration(cellularAutomataParameters parameters)
    {
        short nbCount;

        iterateIn(this, column, row, width, height, [](short columnRect, short rowRect)
        {
            // Count of "alive" cells at this location
            nbCount = 0;

            iterateAround(this, columnRect, rowRect, true, [](short i, short j)
            {
                // If our grid "is defined" here
                //
                if (!this->isZeroValue(i, j))
                    nbCount++;
            });

            if (this->isZeroValue(columnRect, rowRect) && birthParameters[nbCount])
            {
                this->set(columnRect, rowRect, birthValue)	    // birth
            }
            else if (!this->isZeroValue(columnRect, rowRect) && survivalParameters[nbCount])
            {
                // survival
            }
            else
            {
                this->set(columnRect, rowRect, _zeroValue);     // death
            }
        });
    }

    template<comparable T>
    short grid<T>::fillContiguousRegion(short column, short row, T fillValue) 
    {
        short numberOfCells = 1;

        // Fill current cell value
        //
        this->set(column, row, fillValue);

        // Recurse through in a flood-fill fashion
        //
        iterateAroundCardinal(this, column, row, true, [](short x, short y)
        {
            // If the neighbor is an unmarked region cell
            //
            if (!this->isZeroValue(x,y))
            { 
                numberOfCells += this->fillContiguousRegion(x, y, fillValue); // then recurse.
            }
        });

        return numberOfCells;
    }

    template<comparable T>
    void generateCellularAutomata(randomGenerator* randomGenerator, cellularAutomataParameters parameters)
    {
        // Procedure
        //
        // 1) Generate white noise in the specified region
        // 2) Run the cellularAutomataSmoothing function the specified number of times
        //      -> While iterating:  try to check original Brogue constraints
        //        

        short random, smoothingPassesMax;

        // Generate white noise inside the chosen rectangle
        //
        iterateIn(this, column, row, width, height, [](short x, short y)
        {
            if (randomGenerator->rand_percent(random))
                this->set(x, y, fillValue);
        });

        // Run smoothing iterations
        //
        for (int index = 0; index < clamp(smoothingPasses, CELLULAR_AUTOMATA_MAX); index++)
        {
            this->cellularAutomataIteration(parameters);
        }
    }

    template<comparable T>
    std::vector<gridRegion<T>*> grid<T>::locateRegions(function<bool(T)> predicate, T fillValue)
    {
        std::vector<gridRegion<T>*> result;

        iterate(this, [](short column, short row)
        {
            // Check history
            for (int index = 0; index < result.size(); index++)
            {
                // Return from iterator (only)
                if (predicate(result[i][column, row]))
                    return;
            }

            // Check to see if we're at a valid location
            if (predicate(_grid[column, row]))
            {
                // Start a new region
                grid<T>* regionGrid = new grid<T>(_columns, _rows, _zeroValue, _maxValue);

                // Keep track of the boundary
                gridRect boundary(column, row, 0, 0);

                // Set initial value
                regionGrid->set(column, row, fillValue);

                // Recurse to fill out the grid
                this->locateRegionRecurse(regionGrid, column, row, predicate, fillValue, predicate);

                // Set result
                result.push_back(new gridRegion<T>(regionGrid, boundary));
            }
        });

        return result;
    }

    template<comparable T>
    void grid<T>::locateRegionRecurse(grid<T>* regionGrid, gridRect& boundary, short currentColumn, short currentRow, T fillValue, function<bool(T)> predicate)
    {
        // Recurse through in a flood-fill fashion
        //
        iterateAroundCardinal(this, currentColumn, currentRow, true, [](short x, short y)
        {
            // Neighbor Cell:  Protect from infinite recursion by checking region grid
            //
            if (predicate(this, x,y) && !predicate(regionGrid, x, y))
            {
                // Mark as region
                regionGrid->set(x, y, fillValue);

                // Expand boundary
                boundary.expand(x, y);

                // Recurse
                locateRegionRecurse(regionGrid, boundary, x, y, fillValue, predicate);
            }
        });
    }
}