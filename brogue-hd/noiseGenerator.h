#pragma once

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

		void cellularAutomata(const gridRect& parentBoundary, const gridRect& relativeBoundary, const cellularAutomataParameters& parameters, gridCallback<bool> callback);

	private:

		void cellularAutomataIteration(grid<bool>& resultGrid, const cellularAutomataRule& rule);

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

	void noiseGenerator::cellularAutomata(const gridRect& parentBoundary, const gridRect& relativeBoundary, const cellularAutomataParameters& parameters, gridCallback<bool> callback)
	{
		// Procedure
		//
		// 1) Generate white noise in the specified region
		// 2) Run the cellularAutomataSmoothing function the specified number of times
		//      -> While iterating:  try to check original Brogue constraints (see cellularAutomataParameters)
		// 3) Run the callback
		//        

		// Array for generating the result
		grid<bool> resultGrid(parentBoundary, relativeBoundary);

		randomGenerator* rand = _randomGenerator;

		// Generate white noise inside the chosen rectangle
		//
		relativeBoundary.iterate([&rand, &parameters, &resultGrid] (short x, short y)
		{
			if (rand->next() <= parameters.getFillRatio())
				resultGrid.set(x, y, true);

			return iterationCallback::iterate;
		});

		// Run iterations
		//
		for (int index = 0; index < parameters.getIterations(); index++)
		{
			for (int ruleIndex = 0; ruleIndex < parameters.getRules()->count(); ruleIndex++)
			{
				this->cellularAutomataIteration(resultGrid, parameters.getRules()->get(ruleIndex));
			}
		}

		// Callback
		relativeBoundary.iterate([&rand, &parameters, &resultGrid, &callback] (short x, short y)
		{
			return callback(x, y, resultGrid.get(x, y));
		});
	}

	void noiseGenerator::cellularAutomataIteration(grid<bool>& resultGrid, const cellularAutomataRule& rule)
	{
		short count = 0;

		resultGrid.iterate([&resultGrid, &rule, &count] (short columnRect, short rowRect, bool item)
		{
			count = 0;

			resultGrid.iterateAdjacent(columnRect, rowRect, false, [&resultGrid, &count, &rule] (short i, short j, brogueCompass direction, bool item)
			{
				// Use out-of-bounds as negative polarity
				if (!resultGrid.isInBounds(i, j))
				{
					if (!rule.countPolarity)
						count++;
				}
				else
				{
					// Count number of cells that match the count polarity
					//
					if (rule.countPolarity && resultGrid.isDefined(i, j))
						count++;

					else if (!rule.countPolarity && !resultGrid.isDefined(i, j))
						count++;
				}

				return iterationCallback::iterate;
			});

			// Apply the rule
			if (rule.polarity)
			{
				// Positive polarity
				if (resultGrid.isDefined(columnRect, rowRect) && count >= rule.count)
					resultGrid.set(columnRect, rowRect, rule.resultPolarity, true);
			}
			else
			{
				// Negative polarity
				if (!resultGrid.isDefined(columnRect, rowRect) && count >= rule.count)
					resultGrid.set(columnRect, rowRect, rule.resultPolarity, true);
			}

			return iterationCallback::iterate;
		});
	}
}