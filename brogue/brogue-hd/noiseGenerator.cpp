#pragma once

#include "noiseGenerator.h"
#include "brogueMath.h"
#include "gridExtension.h"

using namespace brogueHd::backend::math;
using namespace brogueHd::backend::model;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::generator
{
    noiseGenerator::noiseGenerator(randomGenerator* randomGenerator)
    {
        _randomGenerator = randomGenerator;
    }

    noiseGenerator::~noiseGenerator()
    {
        
    }

    void noiseGenerator::cellularAutomata(const cellularAutomataParameters& parameters, gridDelegates<bool>::callback userCallback)
    {
        // Procedure
        //
        // 1) Generate white noise in the specified region
        // 2) Run the cellularAutomataSmoothing function the specified number of times
        //      -> While iterating:  try to check original Brogue constraints (see cellularAutomataParameters)
        // 3) Run the callback
        //        

        // Parent boundary with 1 padding
        gridRect parentBoundary = gridRect(1, 1, DCOLS - 1, DROWS - 1);

        // Array for generating the result
        grid<bool> resultGrid(parentBoundary, parameters.boundary);

        randomGenerator* rand = _randomGenerator;

        // Generate white noise inside the chosen rectangle
        //
        gridRectExtension::iterate(parameters.boundary, [&rand, &parameters, &resultGrid](short x, short y)
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

        // Callback
        gridRectExtension::iterate(parameters.boundary, [&rand, &parameters, &resultGrid, &userCallback](short x, short y)
        {
            userCallback(x, y, resultGrid.get(x, y));
        });
    }

    void noiseGenerator::cellularAutomataIteration(grid<bool>& resultGrid, const cellularAutomataParameters& parameters)
    {
        short count = 0;

        gridExtension<bool>::iterate(&resultGrid, [&resultGrid, &parameters, &count](short columnRect, short rowRect, bool item)
        {
            count = 0;

            gridExtension<bool>::iterateAround(&resultGrid, columnRect, rowRect, true, [&resultGrid, &count](short i, short j, bool item)
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