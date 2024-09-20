#pragma once

#include "terrainGenerator.h"
#include "brogueMath.h"
#include "array2DExtension.h"

using namespace brogueHd::backend::math;
using namespace brogueHd::backend::model;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::generator
{
	terrainGenerator::terrainGenerator(randomGenerator* randomGenerator)
	{
        _randomGenerator = randomGenerator;
	}

	array2D<bool>& terrainGenerator::createCellularAutomata(grid<brogueCell*>* grid, const cellularAutomataParameters& parameters)
	{
        // Procedure
        //
        // 1) Generate white noise in the specified region
        // 2) Run the cellularAutomataSmoothing function the specified number of times
        //      -> While iterating:  try to check original Brogue constraints
        //        

        // Parent boundary with 1 padding
        gridRect parentBoundary = gridRect(1, 1, DCOLS - 1, DROWS - 1);

        // Array for generating the result
        array2D<bool> resultGrid(parentBoundary, parameters.boundary);

        randomGenerator* rand = _randomGenerator;

        // Generate white noise inside the chosen rectangle
        //
        array2DExtension<bool>::iterate(parameters.boundary, [&rand, &parameters, &resultGrid](short x, short y)
        {
            if (rand->next() <= parameters.fillRatio)
                resultGrid.set(x, y, true);
        });

        // Run smoothing iterations
        //
        for (int index = 0; index < parameters.smoothingIterations; index++)
        {
            this->cellularAutomataIteration(resultGrid, parameters);
        }
        
        return resultGrid;
	}

    void terrainGenerator::cellularAutomataIteration(array2D<bool>& resultGrid, const cellularAutomataParameters& parameters)
    {
        short count = 0;

        array2DExtension<bool>::iterate(&resultGrid, [&resultGrid, &parameters, &count](short columnRect, short rowRect)
        {
            count = 0;

            array2DExtension<bool>::iterateAround(&resultGrid, columnRect, rowRect, true, [&resultGrid, &count](short i, short j)
            {
                // Count number of alive cells
                //
                if (resultGrid.isDefined(i, j))
                    count++;
            });

            // Birth
            if (!resultGrid.isDefined(columnRect, rowRect) && count >= parameters.birthCount)
            {
                resultGrid.set(columnRect, rowRect, true);
            }

            // Survival
            else if (resultGrid.isDefined(columnRect, rowRect) && count >= parameters.survivalCount)
            {
                // keep alive
            }

            // Death
            else
            {
                resultGrid.set(columnRect, rowRect, false);
            }
        });
    }
}