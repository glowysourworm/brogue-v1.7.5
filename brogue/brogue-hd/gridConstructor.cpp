#pragma once

#include "grid.h"
#include "gridRect.h"
#include "gridConstructor.h"
#include "exceptionHandler.h"

namespace brogueHd::backend::model::construction
{
	template<gridCellConstraint T>
	gridConstructor<T>::gridConstructor(const gridRect& boundary, gridCellConstructor cellConstructor)
	{
		_grid = new grid<T>(boundary.width, boundary.height);
	}

	template<gridCellConstraint T>
	gridConstructor<T>::~gridConstructor()
	{
        // DO NOT DELETE GRID
	}

    template<gridCellConstraint T>
    void gridConstructor<T>::insertNew(short column, short row)
    {
        if (_grid->isDefined(column, row))
            brogueException::show("Trying to insert cell at already filled location:  gridConstructor::insertNew");

        _grid->set(column, row, _cellConstructor(column, row));
    }

    template<gridCellConstraint T>
    void gridConstructor<T>::remove(short column, short row)
    {
        if (!_grid->isDefined(column, row))
            brogueException::show("Trying to remove cell at empty location:  gridConstructor::remove");

        _grid->set(column, row, NULL);
    }

    //template<gridCellConstraint T>
    //short grid<T>::floodFill(short x, short y, T eligibleValueMin, T eligibleValueMax)
    //{
    //    enum directions dir;
    //    short newX, newY, fillCount = 1;

    //    brogueAssert(fillValue < eligibleValueMin || fillValue > eligibleValueMax);

    //    // Fill the current cell
    //    //
    //    this->set(x, y, fillValue);

    //    // Recurse over the cardinally adjacent cells and fill eligible ones
    //    //
    //    iterateAroundCardinal(this, x, y, true, [](short column, short row)
    //    {
    //        if (_grid[column, row] >= eligibleValueMin &&
    //            _grid[newX][newY] <= eligibleValueMax)
    //        {
    //            fillCount += floodFill(newX, newY, eligibleValueMin, eligibleValueMax, fillValue);
    //        }
    //    });

    //    return fillCount;
    //}

    template<gridCellConstraint T>
    void gridConstructor<T>::setRectangle(short x, short y, short width, short height)
    {
        short i, j;

        for (i = x; i < x + width; i++) {
            for (j = y; j < y + height; j++) {
                this->insertNew(i, j, value);
            }
        }
    }

    template<gridCellConstraint T>
    void gridConstructor<T>::setCircle(short x, short y, short radius)
    {
        short i, j;

        for (i = max(0, x - radius - 1); i < max(_columns, x + radius); i++) {
            for (j = max(0, y - radius - 1); j < max(_rows, y + radius); j++) {
                if ((i - x) * (i - x) + (j - y) * (j - y) < radius * radius + radius) {
                    this->insertNew(i, j, value);
                }
            }
        }
    }

    template<gridCellConstraint T>
    void gridConstructor<T>::setIntersection(const grid<T>* otherGrid)
    {
        short i, j;

        for (i = 0; i < _columns; i++)
        {
            for (j = 0; j < _rows; j++)
            {
                // Keep only the intersection
                if (this->isDefined() && !otherGrid->isDefined())
                    this->remove(i, j);
            }
        }
    }

    template<gridCellConstraint T>
    void gridConstructor<T>::setUnion(const grid<T>* otherGrid, bool copyCells)
    {
        short i, j;
        for (i = 0; i < _columns; i++)
        {
            for (j = 0; j < _rows; j++)
            {
                // Keep the union
                if (!this->isDefined() && otherGrid->isDefined())
                {
                    if (!copyCells)
                        this->insertNew(i, j);

                    // COPY! (Possible shared pointer)
                    else
                        _grid->set(i, j, otherGrid->get(i, j));
                }
            }
        }
    }

    template<gridCellConstraint T>
    void gridConstructor<T>::setFor(function<bool(short, short, T)> predicate, bool existingPermissive)
    {
        iterate(this, [](short column, short row)
        {
            if (predicate(column, row, _grid[i][j]))
            {
                if (_grid->isDefined(column, row) && !existingPermissive)
                    brogueException::show("Trying to copy over existing grid cell:  gridConstructor::setFor (can use permissive)");

                // SET DIRECTLY:  Already checked for exception
                _grid->set(column, row, setValue);
            }
        });
    }

	/// <summary>
	/// Generates cellular automata with the specified parameters. NOTE*** Smoothing Passes needs to be less than or equal to 10.
	/// </summary>
	template<gridCellConstraint T>
	void gridConstructor<T>::generateCellularAutomata(randomGenerator* randomGenerator, cellularAutomataParameters parameters)
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
            if (randomGenerator->rand_percent(random) && !_grid->isDefined(x, y))
                this->insertNew(x, y, fillValue);
        });

        // Run smoothing iterations
        //
        for (int index = 0; index < clamp(smoothingPasses, CELLULAR_AUTOMATA_MAX); index++)
        {
            this->cellularAutomataIteration(parameters);
        }
	}

    template<gridCellConstraint T>
    void gridConstructor<T>::cellularAutomataIteration(cellularAutomataParameters parameters)
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
                if (_grid->isDefined(i, j))
                    nbCount++;
            });

            if (!_grid->isDefined(columnRect, rowRect) && birthParameters[nbCount])
            {
                this->insertNew(columnRect, rowRect)	    // birth
            }
            else if (_grid->isDefined(columnRect, rowRect) && survivalParameters[nbCount])
            {
                // survival
            }
            else
            {
                this->remove(columnRect, rowRect);          // death
            }
        });
    }
}
