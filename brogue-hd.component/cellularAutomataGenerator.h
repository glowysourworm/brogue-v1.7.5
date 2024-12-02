#pragma once

#include "noiseGenerator.h"
#include "cellularAutomataParameters.h"
#include "grid.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "randomGenerator.h"
#include "simple.h"
#include "simpleList.h"

namespace brogueHd::component
{
	class cellularAutomataGenerator : public noiseGenerator
	{
	public:
		cellularAutomataGenerator(randomGenerator* randomGenerator);
		~cellularAutomataGenerator();

		void initialize(cellularAutomataParameters* parameters);
		void run(const gridRect& parentBoundary, const gridRect& relativeBoundary,
		         gridCallback<bool> callback) override;

	private:
		void runImpl(const gridRect& parentBoundary,
		             const gridRect& relativeBoundary,
		             gridCallback<bool> callback);

		void runIteration(grid<bool>& resultGrid,
		                  const simpleList<cellularAutomataRule>* rules,
		                  bool resultPolarity);

	private:
		randomGenerator* _randomGenerator;
		cellularAutomataParameters* _parameters;
	};

	cellularAutomataGenerator::cellularAutomataGenerator(randomGenerator* randomGenerator)
		: noiseGenerator(randomGenerator)
	{
		_randomGenerator = randomGenerator;
		_parameters = nullptr;
	}

	cellularAutomataGenerator::~cellularAutomataGenerator()
	{
	}

	void cellularAutomataGenerator::initialize(cellularAutomataParameters* parameters)
	{
		_parameters = parameters;
	}

	void cellularAutomataGenerator::run(const gridRect& parentBoundary, const gridRect& relativeBoundary,
	                                    gridCallback<bool> callback)
	{
		runImpl(parentBoundary, relativeBoundary, callback);
	}

	void cellularAutomataGenerator::runImpl(const gridRect& parentBoundary,
	                                        const gridRect& relativeBoundary,
	                                        gridCallback<bool> callback)
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
		cellularAutomataParameters* parameters = _parameters;
		gridRect paddedBoundary = relativeBoundary.createPadded(_parameters->getPadding());

		// Generate white noise inside the chosen rectangle
		//
		paddedBoundary.iterate([&rand, &parameters, &resultGrid](int x, int y)
		{
			if (rand->next() <= parameters->getFillRatio())
				resultGrid.set(x, y, parameters->getResultPolarity());

			return iterationCallback::iterate;
		});

		// Run iterations
		//
		for (int index = 0; index < _parameters->getIterations(); index++)
		{
			runIteration(resultGrid, _parameters->getRules(), _parameters->getResultPolarity());
		}

		// Callback
		relativeBoundary.iterate([&resultGrid, &callback](int x, int y)
		{
			return callback(x, y, resultGrid.get(x, y));
		});
	}

	void cellularAutomataGenerator::runIteration(grid<bool>& resultGrid, const simpleList<cellularAutomataRule>* rules,
	                                             bool resultPolarity)
	{
		int positiveCount = 0;
		int negativeCount = 0;

		resultGrid.iterate(
			[&resultGrid, &rules, &positiveCount, &negativeCount, &resultPolarity](
			int columnRect, int rowRect, bool item)
			{
				positiveCount = 0;
				negativeCount = 0;

				resultGrid.iterateAdjacent(columnRect, rowRect, false,
				                           [&resultGrid, &positiveCount, &negativeCount](
				                           int i, int j, brogueCompass direction, bool item)
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
