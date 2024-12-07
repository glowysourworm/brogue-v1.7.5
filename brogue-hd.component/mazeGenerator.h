#pragma once

#include "grid.h"
#include "gridRect.h"
#include "gridMethods.h"
#include "randomGenerator.h"

#include <dungeonConstants.h>

#include <simpleList.h>
#include <simpleStack.h>

namespace brogueHd::component
{
	using namespace simple;
	using namespace brogueHd::model;

	class mazeGenerator
	{
	public:
		mazeGenerator(randomGenerator* randomGenerator);
		~mazeGenerator();

		void initialize(const gridRect& parentBoundary, const gridRect& relativeBoundary);

		void run(double wallRemovalRatio,
		         double horizontalVerticalBias,
		         brogueMazeType mazeType,
		         gridPredicate<gridLocator> inclusionPredicate,
				 gridCallbackConst<bool> callback);

	private:
		// Scales [0, 1] fill ratio to a safe scale
		const double MAZE_WALL_REMOVAL_LOW = 0.0;
		const double MAZE_WALL_REMOVAL_HIGH = 0.5;

	private:
		/// <summary>
		/// Same as the FiveAdjacentWallsRule with the added condition that the 5 adjacent walls must be "in the direction of
		/// travel". Example:  Movement Direction = E. Then, the set { N, NE, E, SE, S } must ALL be walls. This tends to make
		/// the maze look a little more "bulky" with walls. (Filled feel)
		/// </summary>
		bool fiveAdjacentWallsDirectionalRule(const gridLocator& currentLocation,
		                                      const gridLocator& nextLocation,
		                                      const simpleList<gridLocator>& nextCell8WayNeighbors);

		/// <summary>
		/// Returns true if AT LEAST 5 adjacent cells to the NEXT cell are walls. This ALLOWS off-diagonal CORRIDORS. (Open feel)
		/// </summary>
		bool fiveAdjacentWallsRule(const gridLocator& currentLocation,
		                           const gridLocator& nextLocation,
		                           const simpleList<gridLocator>& nextCell8WayNeighbors);

		/// <summary>
		/// Returns true if all 4-way adjacent cells are walls (Except the current cell)
		/// </summary>
		bool cardinalAdjacentWallsRule(const gridLocator& currentLocation,
		                               const gridLocator& nextLocation,
		                               const simpleList<gridLocator>& nextCell4WayNeighbors);

		gridLocator getCardinalAdjacentCellWithBias(const simpleList<gridLocator>& cardinalAdjacentLocations,
		                                            const gridLocator& currentLocation,
		                                            double horizontalVerticalBias);

	private:
		randomGenerator* _randomGenerator;

		grid<gridLocator>* _mazeGrid;
	};

	mazeGenerator::mazeGenerator(randomGenerator* randomGenerator)
	{
		_randomGenerator = randomGenerator;
		_mazeGrid = nullptr;
	}

	mazeGenerator::~mazeGenerator()
	{
		if (_mazeGrid != nullptr)
			delete _mazeGrid;
	}


	void mazeGenerator::initialize(const gridRect& parentBoundary, const gridRect& relativeBoundary)
	{
		if (_mazeGrid != nullptr)
			delete _mazeGrid;

		_mazeGrid = new grid<gridLocator>(parentBoundary, relativeBoundary);

		grid<gridLocator>* mazeGrid = _mazeGrid;

		// Iterate - setting the entire maze to walls
		_mazeGrid->iterate([&mazeGrid](int column, int row, const gridLocator& location)
		{
			// Non-locations are considered NOT walls
			mazeGrid->set(column, row, gridLocator(column, row), true);
			return iterationCallback::iterate;
		});
	}

	void mazeGenerator::run(double wallRemovalRatio,
	                        double horizontalVerticalBias,
	                        brogueMazeType mazeType,
	                        gridPredicate<gridLocator> inclusionPredicate,
							gridCallbackConst<bool> callback)
	{
		// Recursive Backtracker
		//
		// Pre-Condition: All cells in the region (region = inclusionPredicate) must be filled with walls
		//
		// Procedure:  Region grid is a sub-grid of the whole grid. 
		//
		// 1) Choose random cell in the region
		// 2) Initialize the recursive back-tracking algorithm
		//      - Begin history with the chosen cell
		//      - While history > 0:
		//          - Choose N, S, E, W (randomly)
		//          - Chosen Current Cell:
		//              - Check to see if cell is viable (there's no surrounding cells that have been removed)
		//              - Remove the wall, push it on the stack, mark visited
		//
		//          - If all adjacent cells visited
		//              - Remove the most recent cell
		//              - Back-track one and continue
		//
		// 3) When the loop completes - the history will be empty
		// 4) Remove (at random) some of the walls to make it a little easier. A fully "walled" recursive back
		//    -tracker has, typically, only one path - which can be very long and windy.
		//

		// We've already initialized the grid. So, we're going to collect any viable locations first using
		// the inclusion predicate. (This allows the user to accept/reject cells based on what else might
		// be in the way)
		//
		simpleList<gridLocator> locations = _mazeGrid->collectWhere(inclusionPredicate);

		// Choose random starting location
		gridLocator startingLocation = _randomGenerator->getRandomElement(locations);

		// Keep track of visited locations
		grid<bool> visitedCells(_mazeGrid->getParentBoundary(), _mazeGrid->getRelativeBoundary());

		// Track from the starting location
		gridLocator currentLocation = startingLocation;

		// Initialize the history
		simpleStack<gridLocator> history;
		history.push(currentLocation);

		// Set the first cell ("knock-out" walls)
		_mazeGrid->set(currentLocation.column, currentLocation.row, default_value::value<gridLocator>(), true);
		visitedCells.set(currentLocation.column, currentLocation.row, true, true);

		//Main loop - create the maze!
		while (history.count() > 0)
		{
			// Get all unvisited neighbor cells in the region that pass the user's predicate (usually to avoid impassable terrain)
			//
			simpleList<gridLocator> adjacentLocations = _mazeGrid->getCardinallyAdjacentElements(
				                                                     currentLocation.column, currentLocation.row)
			                                                     .where([](const gridLocator& location)
			                                                     {
				                                                     return location != default_value::value<
					                                                     gridLocator>();
			                                                     }) // !visited
			                                                     .where([&inclusionPredicate](
				                                                     const gridLocator& location)
				                                                     {
					                                                     return inclusionPredicate(
						                                                     location.column, location.row, location);
				                                                     })
			                                                     .where([&visitedCells](const gridLocator& location)
			                                                     {
				                                                     return !visitedCells.get(
					                                                     location.column, location.row);
			                                                     });

			// Have tried all possibilities at this location - so back track
			if (adjacentLocations.count() == 0)
			{
				// Back at the beginning - time to terminate loop
				if (history.count() == 1)
					break;

					// Back-track one cell in the history and continue
				else
					currentLocation = history.pop();
			}

			// Try directions randomly to continue iteration
			else
			{
				// NOTE*** Be sure to use the method from the random sequence generator since
				//         we're building the scenario
				//
				gridLocator nextLocation = getCardinalAdjacentCellWithBias(
					adjacentLocations, currentLocation, horizontalVerticalBias);

				// Track that the cell has been visited
				//
				visitedCells.set(nextLocation.column, nextLocation.row, true, true);

				// Run query to see whether this cell can be used in the maze
				//
				bool viableLocation = (mazeType == brogueMazeType::Filled) ? 
									   fiveAdjacentWallsDirectionalRule(currentLocation, nextLocation, _mazeGrid->getAdjacentElementsUnsafe(nextLocation.column, nextLocation.row)) :
									   fiveAdjacentWallsRule(currentLocation, nextLocation, _mazeGrid->getAdjacentElementsUnsafe(nextLocation.column, nextLocation.row));

				// If any neighbor cells CAN be visited - then push the current one on the stack
				if (viableLocation)
				{
					// Remove the "wall" - THIS IS A VIABLE MAZE PATH
					_mazeGrid->set(nextLocation.column, nextLocation.row, default_value::value<gridLocator>(), true);

					// Push on the stack
					history.push(nextLocation);

					// Increment follower
					currentLocation = nextLocation;
				}
			}
		}

		// Scale [0, 1] wall removal ratio
		//float safeRemovalRatio = ((MAZE_WALL_REMOVAL_HIGH - MAZE_WALL_REMOVAL_LOW) * wallRemovalRatio) + MAZE_WALL_REMOVAL_LOW;

		randomGenerator* randomGenerator = _randomGenerator;
		grid<gridLocator>* mazeGrid = _mazeGrid;

		// Process wall removals -> and send results back to user
		_mazeGrid->iterate(
			[&inclusionPredicate, &wallRemovalRatio, &randomGenerator, &mazeGrid, &callback](
			int column, int row, const gridLocator& location)
			{
				// RETURN A 2D ARRAY OF BOOLEANS AS THE RESULT (NULL -> WALL REMOVED)

				// Wall was already removed (already checked predicate) (callback)
				if (location == default_value::value<gridLocator>())
					callback(column, row, true);

					// Check user inclusion predicate (not in result set)
				else if (!inclusionPredicate(column, row, location))
					return iterationCallback::iterate;

					// Check random wall removal ratio
				else if (randomGenerator->next() > wallRemovalRatio)
					return iterationCallback::iterate;

				// Remove the wall (maze grid is essentially finished here; but for record keeping and debugging...)
				mazeGrid->set(column, row, default_value::value<gridLocator>(), true);

				// Process callback
				callback(column, row, true);

				return iterationCallback::iterate;
			});
	}


	gridLocator mazeGenerator::getCardinalAdjacentCellWithBias(const simpleList<gridLocator>& cardinalAdjacentLocations,
	                                                           const gridLocator& currentLocation,
	                                                           double horizontalVerticalBias)
	{
		// Used to limit bias
		float biasDivisor = 1.0f;

		// Horizontal
		if (horizontalVerticalBias < 0.5)
		{
			simpleList<gridLocator> horizontalLocations = cardinalAdjacentLocations.where(
				[&currentLocation](const gridLocator& location)
				{
					brogueCompass direction = gridMethods::getAdjacentDirection(currentLocation, location);

					return direction == brogueCompass::E || direction == brogueCompass::W;
				});

			// Return biased cell
			if (_randomGenerator->next() < ((0.5f - horizontalVerticalBias) / biasDivisor) &&
				horizontalLocations.count() > 0)
				return _randomGenerator->getRandomElement(horizontalLocations);

				// Return default
			else
				return _randomGenerator->getRandomElement(cardinalAdjacentLocations);
		}
		else
		{
			simpleList<gridLocator> verticalLocations = cardinalAdjacentLocations.where(
				[&currentLocation](const gridLocator& location)
				{
					brogueCompass direction = gridMethods::getAdjacentDirection(currentLocation, location);

					return direction == brogueCompass::N || direction == brogueCompass::S;
				});

			// Return biased cell
			if (_randomGenerator->next() < ((horizontalVerticalBias - 0.5) / biasDivisor) &&
				verticalLocations.count() > 0)
				return _randomGenerator->getRandomElement(verticalLocations);

				// Return default
			else
				return _randomGenerator->getRandomElement(cardinalAdjacentLocations);
		}
	}

	bool mazeGenerator::cardinalAdjacentWallsRule(const gridLocator& currentLocation,
	                                              const gridLocator& nextLocation,
	                                              const simpleList<gridLocator>& nextCell4WayNeighbors)
	{
		// This should be simplified.. There must essentially be 4 adjacent viable cell locations
		return nextCell4WayNeighbors.where([&currentLocation](const gridLocator& location)
		                            {
			                            return location != currentLocation;
		                            })
		                            .count([](const gridLocator& location)
		                            {
			                            return location != default_value::value<gridLocator>();
		                            }) == 4;
	}

	bool mazeGenerator::fiveAdjacentWallsRule(const gridLocator& currentLocation,
	                                          const gridLocator& nextLocation,
	                                          const simpleList<gridLocator>& nextCell8WayNeighbors)
	{
		return nextCell8WayNeighbors.where([&currentLocation](const gridLocator& location)
		                            {
			                            return location != currentLocation;
		                            })
		                            .count([](const gridLocator& location)
		                            {
			                            return location != default_value::value<gridLocator>();
		                            }) >= 5;
	}

	bool mazeGenerator::fiveAdjacentWallsDirectionalRule(const gridLocator& currentLocation,
	                                                     const gridLocator& nextLocation,
	                                                     const simpleList<gridLocator>& nextCell8WayNeighbors)
	{
		brogueCompass direction = gridMethods::getAdjacentDirection(currentLocation, nextLocation);

		simpleList<brogueCompass> neighborWallDirections = nextCell8WayNeighbors.where([](const gridLocator& location)
		{
			return location != default_value::value<gridLocator>();
		}).select<brogueCompass>([&nextLocation](const gridLocator& location)
		{
			return gridMethods::getAdjacentDirection(nextLocation, location);
		});

		// Check directional flag (OR'ed) to make sure ALL 5 cells are accounted for
		//
		// NOTE*** The Compass no longer acts as a "flag" so be sure to double check
		//         the enum math!
		//
		switch (direction)
		{
		case brogueCompass::N:
			return neighborWallDirections.count([](brogueCompass x)
			{
				return (x == brogueCompass::W) || (x == brogueCompass::NW) || (x == brogueCompass::N) || (x ==
					brogueCompass::NE) || (x == brogueCompass::E);
			}) >= 5;
		case brogueCompass::S:
			return neighborWallDirections.count([](brogueCompass x)
			{
				return (x == brogueCompass::W) || (x == brogueCompass::SW) || (x == brogueCompass::S) || (x ==
					brogueCompass::SE) || (x == brogueCompass::E);
			}) >= 5;
		case brogueCompass::E:
			return neighborWallDirections.count([](brogueCompass x)
			{
				return (x == brogueCompass::N) || (x == brogueCompass::NE) || (x == brogueCompass::E) || (x ==
					brogueCompass::SE) || (x == brogueCompass::S);
			}) >= 5;
		case brogueCompass::W:
			return neighborWallDirections.count([](brogueCompass x)
			{
				return (x == brogueCompass::N) || (x == brogueCompass::NW) || (x == brogueCompass::W) || (x ==
					brogueCompass::SW) || (x == brogueCompass::S);
			}) >= 5;
		case brogueCompass::NW:
		case brogueCompass::NE:
		case brogueCompass::SE:
		case brogueCompass::SW:
		case brogueCompass::None:
		default:
			throw simpleException("Invalid maze generation movement:  mazeGenerator::fiveAdjacentWallsDirectionalRule");
		}
	}
}
