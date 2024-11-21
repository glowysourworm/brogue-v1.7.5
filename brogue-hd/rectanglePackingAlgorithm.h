#pragma once
#include "grid.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "randomGenerator.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleHash.h"
#include "simpleList.h"
#include "simpleMath.h"
#include "simpleVector.h"
#include <limits>

using namespace brogueHd::backend;

namespace brogueHd::component
{
	/// <summary>
	/// Simple rectangle packing algorithm that packs rectangles using a distance heuristic
	/// comparing the centers of each rectangle with the first.
	/// </summary>
	class rectanglePackingAlgorithm
	{
	public:

		rectanglePackingAlgorithm(randomGenerator* randomGenerator, const gridRect& boundary);
		~rectanglePackingAlgorithm();

		void initialize(const gridRect& firstRect);

		bool addRectangle(gridRect& rect);

		gridRect getLargestUnusedRectangle() const;

		simpleList<gridRect> getResult() const;

	private:

		randomGenerator* _randomGenerator;

		grid<gridLocator>* _tilingGrid;

		simpleHash<gridRect, gridRect>* _rectangles;

		bool _initialized;
	};

	rectanglePackingAlgorithm::rectanglePackingAlgorithm(randomGenerator* randomGenerator, const gridRect& boundary)
	{
		_randomGenerator = randomGenerator;
		_rectangles = new simpleHash<gridRect, gridRect>();
		_tilingGrid = new grid<gridLocator>(boundary, boundary);
	}
	rectanglePackingAlgorithm::~rectanglePackingAlgorithm()
	{
		delete _rectangles;
		delete _tilingGrid;
	}

	void rectanglePackingAlgorithm::initialize(const gridRect& firstRect)
	{
		_rectangles->clear();
		_rectangles->add(firstRect, firstRect);

		grid<gridLocator>* grid = _tilingGrid;

		_tilingGrid->iterate([&grid, &firstRect] (short column, short row, const gridLocator& item)
		{
			if (firstRect.contains(column, row))
				grid->set(column, row, gridLocator(column, row), true);

			else
				grid->set(column, row, default_value::value<gridLocator>(), true);

			return iterationCallback::iterate;
		});
	}

	gridRect rectanglePackingAlgorithm::getLargestUnusedRectangle() const
	{
		return _tilingGrid->calculateLargestRectangle([] (short column, short row, const gridLocator& item)
		{
			// Use the predicate to look for negative space
			return item == default_value::value<gridLocator>();
		});
	}

	simpleList<gridRect> rectanglePackingAlgorithm::getResult() const
	{
		return _rectangles->getKeys();
	}

	bool rectanglePackingAlgorithm::addRectangle(gridRect& rect)
	{
		if (_rectangles->count() == 0)
			throw simpleException("Must first initialize the algorithm:  call rectanglePackingAlgorithm::initialize()");

		// Validate that the caller is not trying to add rectangle that overlaps
		// any existing rectangle
		//
		_rectangles->iterate([&rect] (const gridRect& tileKey, const gridRect& tile)
		{
			if (rect.overlaps(tile))
				throw simpleException("Trying to add gridRect over the top of an existing tile:  rectanglePackingAlgorithm.h");

			return iterationCallback::iterate;
		});

		// Procedure:  Pseudo-Rectangular Packing Problem (w/ random positioning and distance heuristic)
		// 
		// 0) Check that there is either a random, or sub-rectangle left for the new rectangle. Use distance
		//    heuristic to minimize the distance from the first rectangle.
		// 1) Utilize the heuristic vector to "move towards" the first rectangle.
		// 2) Check against the vector for edge adjacency to another tile. 
		//		-> True: Slide laterally (in the opposite dimension) to see if it minimizes the 
		//				 heuristic.
		//		-> False: Move against the vector (in both dimensions) using the slope to determine
		//				  the amount in each dimension until there is adjacency established. 
		// 3) Set the result and break the loop
		//

		gridRect constraint = _tilingGrid->getRelativeBoundary();
		gridRect firstRect = _rectangles->getAt(0)->key;
		simpleVector<float> heuristic;
		simpleVector<float> nextHeuristic;

		// Mark the distance
		heuristic.set(firstRect.centerX() - rect.centerX(), firstRect.centerY() - rect.centerY());
		nextHeuristic.set(heuristic);

		bool randomPlacementSuccessful = false;
		int counter = 0;

		// Try randomly a few times to minimize iteration work in the following loops
		while (counter++ < 10)
		{
			gridRect randomBoundary(_randomGenerator->randomIndex(constraint.left(), constraint.right() - rect.width + 1),
									_randomGenerator->randomIndex(constraint.top(), constraint.bottom() - rect.height + 1),
									rect.width, rect.height);

			bool overlaps = false;

			// Check the tile overlap
			_rectangles->iterate([&randomBoundary, &overlaps] (const gridRect& tileKey, const gridRect& tile)
			{
				if (tile.overlaps(randomBoundary))
				{
					overlaps = true;
					return iterationCallback::breakAndReturn;
				}
				return iterationCallback::iterate;
			});

			// Check boundary of parent
			if (!constraint.contains(randomBoundary))
				continue;

			// Tile overlap found for random try for a better heuristic
			if (overlaps)
				continue;

			nextHeuristic.set(firstRect.centerX() - randomBoundary.centerX(),
							  firstRect.centerY() - randomBoundary.centerY());

			// Valid location found - update boundary and heuristic
			if (nextHeuristic.magnitude() < heuristic.magnitude())
			{
				rect = randomBoundary;
				heuristic = nextHeuristic;
				randomPlacementSuccessful = true;
			}
		}

		// Random Placement Failed:  Try calculating the largest sub-rectangle
		//
		if (!randomPlacementSuccessful)
		{
			// Look for largest sub-rectangle in the constraint area
			//
			gridRect nextLargestBoundary = _tilingGrid->calculateLargestRectangle([] (short column, short row, const gridLocator& item)
			{
				// Use the predicate to look for negative space
				return item == default_value::value<gridLocator>();
			});

			rect = gridRect(nextLargestBoundary.column, nextLargestBoundary.row, rect.width, rect.height);

			// Invalid:  No space left. Return false.
			if (!constraint.contains(rect))
				return false;
		}

		bool movement = true;

		// Slide into position (the boundary may take a bump or two with other rectangles on its way towards the 
		// first entrance rectangle)
		while (movement)
		{
			bool stepOneValid = false;
			bool stepTwoValid = false;
			bool stepThreeValid = false;

			// Procedure
			//
			// 1) Move - both dimensions at once
			//		-> Check heuristic
			// 2) Check Tile Overlap
			//		-> Valid: Goto (#1)
			// 3) Move - one dimension at a time
			//		Subloop:
			//			-> Check heuristic
			//			-> Check tile overlap
			//		Endloop: (iterate while there is valid movement)
			//

			// Mark Distance Heuristic (check against this one during iteration)
			heuristic.set(firstRect.centerX() - rect.centerX(),
						  firstRect.centerY() - rect.centerY());

			int yMoves = 0;
			int xMoves = 0;

			if (heuristic.slope() == 0)
			{
				yMoves = 0;
				xMoves = simpleMath::sign(heuristic.x);
			}
			else if (heuristic.slope() == std::numeric_limits<float>::infinity() ||
					-1 * heuristic.slope() == std::numeric_limits<float>::infinity())
			{
				yMoves = simpleMath::sign(heuristic.y);
				xMoves = 0;
			}
			else
			{
				// The heuristic vector ends up being the right iterators. But, these will be tried one at
				// a time.
				yMoves = heuristic.y;
				xMoves = heuristic.x;
			}

			// First, check overlapping tiles (before iterating positions independently)
			rect.translate(xMoves, yMoves);

			nextHeuristic.set(firstRect.centerX() - rect.centerX(),
							  firstRect.centerY() - rect.centerY());

			// Step 1:  Invalid
			if (nextHeuristic.magnitude() >= heuristic.magnitude())
			{
				// This movement didn't produce a better heuristic, so back it out.
				rect.translate(-1 * xMoves, -1 * yMoves);
			}
			else
				stepOneValid = true;

			// Step 2:  Check tile and grid boundaries
			if (stepOneValid)
			{
				bool overlaps = false;

				// Check for overlapping tiles
				_rectangles->iterate([&rect, &overlaps] (const gridRect& tileKey, const gridRect& tile)
				{
					if (rect.overlaps(tile))
					{
						overlaps = true;
						return iterationCallback::breakAndReturn;
					}
					return iterationCallback::iterate;
				});

				// Check the parent boundary
				if (!constraint.contains(rect))
					overlaps = true;

				// Step 2: Invalid
				if (overlaps)
				{
					// This violates tile overlap - back out the translation
					rect.translate(-1 * xMoves, -1 * yMoves);

					stepTwoValid = false;
				}
			}

			// Step 3: This could occur when there was an iteration that produced lateral movement; or
			//		   when there was a tile overlap; and there could be a way to move laterally to try
			//		   getting a better heuristic.
			//
			if (!stepTwoValid)
			{
				// Move one-dimension, one-cell at a time; and check using the tile overlap.
				//
				for (int index = 0; index < simpleMath::abs(xMoves) + simpleMath::abs(yMoves); index++)
				{
					// Use this flag in the subloop for step 3
					bool subloopMovement = true;

					bool yMove = (index >= simpleMath::abs(xMoves));

					if (!yMove)
						rect.translate(simpleMath::sign(xMoves), 0);
					else
						rect.translate(0, simpleMath::sign(yMoves));

					// Check Heuristic
					nextHeuristic.set(firstRect.centerX() - rect.centerX(),
									  firstRect.centerY() - rect.centerY());

					// Step 3: Invalid
					if (nextHeuristic.magnitude() >= heuristic.magnitude())
					{
						subloopMovement = false;
					}

					// (Valid) -> (Check tile overlap)
					if (subloopMovement)
					{
						// Iterate just the tiles to save processing time (checking for invalid movement)
						_rectangles->iterate([&rect, &subloopMovement] (const gridRect& tileKey, const gridRect& tile)
						{
							if (rect.overlaps(tile))
							{
								subloopMovement = false;
								return iterationCallback::breakAndReturn;
							}
							return iterationCallback::iterate;
						});

						if (!constraint.contains(rect))
							subloopMovement = false;
					}

					// Step 3: Valid
					if (subloopMovement)
					{
						// Finally, mark step 3
						stepThreeValid = true;

						// Need heuristic update for this sub-loop
						heuristic.set(nextHeuristic);
						continue;
					}

					// Step 3: Invalid: Back out the translation and move onto the y - dimension
					else
					{
						// X-Dimension:  Can still check the y-dimension
						if (!yMove)
						{
							rect.translate(-1 * simpleMath::sign(xMoves), 0);

							// Skip to the y-dimension
							index += (simpleMath::abs(xMoves) - index);

							// Check if there is a y-dimension to iterate
							if (index >= simpleMath::abs(xMoves) + simpleMath::abs(yMoves))
								break;
						}

						// Y-Dimension:  Must break the loop
						else
						{
							rect.translate(0, -1 * simpleMath::sign(yMoves));
							break;
						}
					}
				}
			}

			if ((stepOneValid || stepTwoValid) && !stepThreeValid ||
				(!stepOneValid && !stepTwoValid && !stepThreeValid))
			{
				movement = false;
				break;
			}
		}

		// Success!  Keep Rectangle and Tiling Grid up to date
		_rectangles->add(rect, rect);
		
		grid<gridLocator>* tilingGrid = _tilingGrid;

		rect.iterate([&tilingGrid] (short column, short row)
		{
			tilingGrid->set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		return true;
	}
}