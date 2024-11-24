#pragma once

#include "cellularAutomataParameters.h"
#include "grid.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "randomGenerator.h"
#include "simple.h"
#include "simpleList.h"

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

		void cellularAutomataIteration(grid<bool>& resultGrid, const simpleList<cellularAutomataRule>* rules, bool resultPolarity);

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
				resultGrid.set(x, y, parameters.getResultPolarity());

			return iterationCallback::iterate;
		});

		// Run iterations
		//
		for (int index = 0; index < parameters.getIterations(); index++)
		{
			this->cellularAutomataIteration(resultGrid, parameters.getRules(), parameters.getResultPolarity());
		}

		// Callback
		relativeBoundary.iterate([&rand, &parameters, &resultGrid, &callback] (short x, short y)
		{
			return callback(x, y, resultGrid.get(x, y));
		});
	}

	void noiseGenerator::cellularAutomataIteration(grid<bool>& resultGrid, const simpleList<cellularAutomataRule>* rules, bool resultPolarity)
	{
		short positiveCount = 0;
		short negativeCount = 0;

		resultGrid.iterate([&resultGrid, &rules, &positiveCount, &negativeCount, &resultPolarity] (short columnRect, short rowRect, bool item)
		{
			positiveCount = 0;
			negativeCount = 0;

			resultGrid.iterateAdjacent(columnRect, rowRect, false, [&resultGrid, &positiveCount, &negativeCount] (short i, short j, brogueCompass direction, bool item)
			{
				// Use out-of-bounds as negative polarity
				if (!resultGrid.isInBounds(i, j))
				{
					negativeCount++;
				}
				else
				{
					if (resultGrid.isDefined(i, j))
						positiveCount++;

					else
						negativeCount++;
				}

				return iterationCallback::iterate;
			});

			bool result = false;

			for (int index = 0; index < rules->count(); index++)
			{
				cellularAutomataRule rule = rules->get(index);

				// Must match the polarity of the rule
				if (rule.polarity != resultGrid.isDefined(columnRect, rowRect))
					continue;

				// Positive polarity
				if (rule.countPolarity && positiveCount >= rule.count)
					result |= true;

				// Negative polarity
				if (!rule.countPolarity && negativeCount >= rule.count)
					result |= true;
			}

			// Apply the rule(s)
			if (result)
				resultGrid.set(columnRect, rowRect, resultPolarity, true);

			return iterationCallback::iterate;
		});
	}
}