#pragma once

#include "brogueGlobal.h"
#include "cellularAutomataParameters.h"
#include "grid.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "randomGenerator.h"
#include "simple.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model;

namespace brogueHd::backend
{
	class noiseGenerator
	{
	public:

		noiseGenerator(randomGenerator* randomGenerator);
		~noiseGenerator();

		void cellularAutomata(const cellularAutomataParameters& parameters, gridCallback<bool> callback);

	private:

		void cellularAutomataIteration(grid<bool>& resultGrid, const cellularAutomataParameters& parameters);

	private:

		randomGenerator* _randomGenerator;
	};

	noiseGenerator::noiseGenerator(randomGenerator* randomGenerator)
	{
		_randomGenerator = randomGenerator;
	}

	noiseGenerator::~noiseGenerator()
	{

	}

	void noiseGenerator::cellularAutomata(const cellularAutomataParameters& parameters, gridCallback<bool> callback)
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
		parameters.boundary.iterate([&rand, &parameters, &resultGrid] (short x, short y)
		{
			if (rand->next() <= parameters.fillRatio)
				resultGrid.set(x, y, true);

			return iterationCallback::iterate;
		});

		// Run smoothing iterations
		//
		for (int index = 0; index < parameters.smoothingIterations; index++)
		{
			this->cellularAutomataIteration(resultGrid, parameters);
		}

		// Callback
		parameters.boundary.iterate([&rand, &parameters, &resultGrid, &callback] (short x, short y)
		{
			return callback(x, y, resultGrid.get(x, y));
		});
	}

	void noiseGenerator::cellularAutomataIteration(grid<bool>& resultGrid, const cellularAutomataParameters& parameters)
	{
		short count = 0;

		resultGrid.iterate([&resultGrid, &parameters, &count] (short columnRect, short rowRect, bool item)
		{
			count = 0;

			resultGrid.iterateAdjacent(columnRect, rowRect, true, [&resultGrid, &count] (short i, short j, brogueCompass direction, bool item)
			{
				// Count number of alive cells
				//
				if (resultGrid.isDefined(i, j))
					count++;

				return iterationCallback::iterate;
			});

			// Birth
			if (!resultGrid.isDefined(columnRect, rowRect) && count >= parameters.birthCount)
			{
				resultGrid.set(columnRect, rowRect, true, true);
			}

			// Survival
			else if (resultGrid.isDefined(columnRect, rowRect) && count >= parameters.survivalCount)
			{
				// keep alive
			}

			// Death
			else
			{
				resultGrid.set(columnRect, rowRect, false, true);
			}

			return iterationCallback::iterate;
		});
	}
}