#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include <simple.h>
#include <simpleArray.h>
#include <simpleBST.h>
#include <simpleException.h>
#include <simpleHash.h>
#include <simpleList.h>
#include <simpleMath.h>
#include <functional>
#include <limits>

namespace brogueHd::component
{
	using namespace simple;

	//struct pdsLink {
	//	int distance;
	//	int cost;
	//	pdsLink* left, * right;
	//};

	//struct pdsMap {
	//	boolean eightWays;

	//	pdsLink front;
	//	pdsLink links[DCOLS * DROWS];
	//};

	/// <summary>
	/// Decision making predicate for dijkstra's algorithm
	/// </summary>
	using dijkstraPredicate = std::function<bool(int column, int row)>;

	/// <summary>
	/// Cost predicate:  calls user code to provide a movement cost to the map
	/// </summary>
	using dijkstraCostCallback = std::function<int(int column, int row)>;

	/// <summary>
	/// Locator Callback:  Provides locator instances to the algorithm
	/// </summary>
	template <isGridLocator T>
	using dijkstraLocatorCallback = std::function<T(int column, int row)>;

	template <isGridLocator T>
	class dijkstra
	{
	public:
		/// <summary>
		/// Creates an instance of dijkstra's map component with the specified, declarative, delegates and constraints.
		/// </summary>
		/// <param name="mapCostPredicate">Delegate used to fetch a cost for the specified column / row of the grid</param>
		/// <param name="mapPredicate">Delegate used to mask off areas of the map from the entire algorithm. Set to TRUE to ALLOW use of the location for the algorithm.</param>
		dijkstra(const gridRect& parentBoundary,
				 const gridRect& relativeBoundary,
				 bool obeyCardinalMovement,
				 bool includeEndpointsInResult,
				 const dijkstraPredicate& mapPredicate,
				 const dijkstraCostCallback& mapCostPredicate,
				 const dijkstraLocatorCallback<T>& locatorCallback);

		~dijkstra();

		void initialize(T source, const simpleArray<T>& targets);

		/// <summary>
		/// Runs Dijkstra's algorithm ONCE. WILL NOT RE-RUN.
		/// </summary>
		void run();

		/// <summary>
		/// Returns one of the result paths from dijkstra's algorithm
		/// </summary>
		simpleArray<T> getResultPath(T targetLocation);

		//void pdsUpdate(pdsMap* map);

		//void pdsClear(pdsMap* map, int maxDistance, boolean eightWays);

		//int pdsGetDistance(pdsMap* map, int x, int y);

		//void pdsSetDistance(pdsMap* map, int x, int y, int distance);

		//void pdsSetCosts(pdsMap* map, int** costMap);

		//void pdsBatchInput(pdsMap* map, int** distanceMap, int** costMap, int maxDistance, boolean eightWays);

		//void pdsBatchOutput(pdsMap* map, int** distanceMap);

		//void pdsInvalidate(pdsMap* map, int maxDistance);

		//void dijkstraScan(int** distanceMap, int** costMap, boolean useDiagonals);

		//void calculateDistances(int** distanceMap,
		//						int destinationX, 
		//						int destinationY,
		//						unsigned long blockingTerrainFlags,
		//						creature* traveler,
		//						boolean canUseSecretDoors,
		//						boolean eightWays);

		//int pathingDistance(int x1, int y1, int x2, int y2, unsigned long blockingTerrainFlags);

	private:
		/// <summary>
		/// Generates path from completed run
		/// </summary>
		simpleArray<T> generatePath(T targetLocation);


		/// <summary>
		/// Updates the output map which contains the calculated path steps
		/// </summary>
		/// <param name="currentWeight">Weight of current location</param>
		void updateOutputMap(float currentWeight, int destColumn, int destRow, int sourceColumn, int sourceRow);

	private:
		dijkstraLocatorCallback<T> _locatorCallback;
		dijkstraPredicate _mapPredicate;
		dijkstraCostCallback _mapCostPredicate;

		gridRect _parentBoundary; // The larger of the two boundaries. Each grid<> instance should
		gridRect _relativeBoundary; // use the larger parent boundary. Relative boundary is used to 
		// limit usage of the grid.

		T _sourceLocation;
		simpleArray<T>* _targetLocations;

		grid<int>* _outputMap;

		grid<bool>* _visitedMap; // Visited locations on the map
		grid<bool>* _locationMap; // Locations that have been added to the frontier

		// Frontier BST for the map
		simpleBST<float, simpleHash<T, T>*>* _frontier;

		// These maps are stored per target location
		simpleHash<T, simpleArray<T>>* _completedPaths; // Stack allocated arrays
		simpleHash<T, bool>* _validPaths;

		bool _initialized = false;
		bool _finished = false;
		bool _obeyCardinalMovement = false;
		bool _includeEndpoints = false;
	};

	template <isGridLocator T>
	dijkstra<T>::dijkstra(const gridRect& parentBoundary,
						  const gridRect& relativeBoundary,
						  bool obeyCardinalMovement,
						  bool includeEndpointsInResult,
						  const dijkstraPredicate& mapPredicate,
						  const dijkstraCostCallback& mapCostPredicate,
						  const dijkstraLocatorCallback<T>& locatorCallback)
	{
		_parentBoundary = parentBoundary;
		_relativeBoundary = relativeBoundary;
		_obeyCardinalMovement = obeyCardinalMovement;
		_includeEndpoints = includeEndpointsInResult;
		_mapPredicate = mapPredicate;
		_mapCostPredicate = mapCostPredicate;
		_locatorCallback = locatorCallback;

		// Set these to be the full size of the parent
		_outputMap = new grid<int>(parentBoundary, relativeBoundary);
		_visitedMap = new grid<bool>(parentBoundary, relativeBoundary);
		_locationMap = new grid<bool>(parentBoundary, relativeBoundary);

		_frontier = new simpleBST<float, simpleHash<T, T>*>();
		_completedPaths = new simpleHash<T, simpleArray<T>>();
		_validPaths = new simpleHash<T, bool>();
	}

	template <isGridLocator T>
	dijkstra<T>::~dijkstra()
	{
		delete _outputMap;
		delete _visitedMap;
		delete _locationMap;
		delete _completedPaths;
		delete _validPaths;

		// Must delete the allocated hash table memory
		_frontier->iterate([] (float key, simpleHash<T, T>* value)
		{
			delete value; // -> ~simpleBSTNode()

			return iterationCallback::iterate;
		});

		delete _frontier;
	}

	template <isGridLocator T>
	void dijkstra<T>::initialize(T source, const simpleArray<T>& targets)
	{
		_sourceLocation = source;
		_targetLocations = new simpleArray<T>(targets);

		// Clear out the frontier
		_frontier->iterate([] (float key, simpleHash<T, T>* value)
		{
			// MEMORY!
			delete value;

			return iterationCallback::iterate;
		});

		_frontier->clear();

		// Clear the result
		_completedPaths->clear();
		_validPaths->clear();

		for (int column = _relativeBoundary.left(); column <= _relativeBoundary.right(); column++)
		{
			for (int row = _relativeBoundary.top(); row <= _relativeBoundary.bottom(); row++)
			{
				// CHECK PREDICATE TO SET MAP COST OFF LIMITS
				bool passesPredicate = _mapPredicate(column, row);

				// Initialize output map (set to infinity for initialization
				int outputValue = ((column == _sourceLocation.column) && (row == _sourceLocation.row) &&
									  passesPredicate)
					? 0
					: std::numeric_limits<int>::max();

				_outputMap->set(column, row, outputValue, true);

				// Initialize visited map
				_visitedMap->set(column, row, false, true);

				// Initialize location map
				_locationMap->set(column, row, false, true);
			}
		}

		_initialized = true;
		_finished = false;
	}

	template <isGridLocator T>
	void dijkstra<T>::run()
	{
		if (!_initialized)
			throw simpleException("Must call initialize() before run() dijkstra.h");

		if (_finished)
			return;

		// Track goal progress
		simpleHash<T, bool> goalDict;

		// Initialize goals to false
		for (int index = 0; index < _targetLocations->count(); index++)
			goalDict.add(_targetLocations->get(index), false);

		// Process the first element
		int column = _sourceLocation.column;
		int row = _sourceLocation.row;

		T lastLocator = default_value::value<T>();

		// Iterate while any target not reached (AND) not visited
		while (!_visitedMap->get(column, row) &&
			goalDict.any([] (T key, bool value)
		{
			return !value;
		}))
		{
			// Set current parameters
			float currentWeight = _outputMap->get(column, row);

			// Mark the element as visited
			_visitedMap->set(column, row, true);

			// Search cardinally adjacent elements (N,S,E,W)
			bool north = row - 1 >= _relativeBoundary.top();
			bool south = row + 1 <= _relativeBoundary.bottom();
			bool east = column + 1 <= _relativeBoundary.right();
			bool west = column - 1 >= _relativeBoundary.left();

			// Dijkstra Weight = Current Value + ("Change in Location Cost" + "Input Map Cost")
			//
			// PROBLEM:        Negative gradient "costs" cause problems because they interrupt the
			//                 accumulated weight. Example: Walk-up-and-then-down a mountain. The
			//                 other side of the mountain will subtract off the accumulated cost of
			//                 climbing it.
			//
			// SOLUTION:       Use "Cost Map" instead of "Gradient Map"
			//

			// CARDINAL LOCATIONS
			if (north && !_visitedMap->get(column, row - 1) && _mapPredicate(column, row - 1))
			{
				updateOutputMap(currentWeight, column, row - 1, column, row);
			}

			if (south && !_visitedMap->get(column, row + 1) && _mapPredicate(column, row + 1))
			{
				updateOutputMap(currentWeight, column, row + 1, column, row);
			}

			if (east && !_visitedMap->get(column + 1, row) && _mapPredicate(column + 1, row))
			{
				updateOutputMap(currentWeight, column + 1, row, column, row);
			}

			if (west && !_visitedMap->get(column - 1, row) && _mapPredicate(column - 1, row))
			{
				updateOutputMap(currentWeight, column - 1, row, column, row);
			}

			// NON-CARDINAL LOCATIONS
			if (!_obeyCardinalMovement && north && east && !_visitedMap->get(column + 1, row - 1) && _mapPredicate(
				column + 1, row - 1))
			{
				updateOutputMap(currentWeight, column + 1, row - 1, column, row);
			}

			if (!_obeyCardinalMovement && north && west && !_visitedMap->get(column - 1, row - 1) && _mapPredicate(
				column - 1, row - 1))
			{
				updateOutputMap(currentWeight, column - 1, row - 1, column, row);
			}

			if (!_obeyCardinalMovement && south && east && !_visitedMap->get(column + 1, row + 1) && _mapPredicate(
				column + 1, row + 1))
			{
				updateOutputMap(currentWeight, column + 1, row + 1, column, row);
			}

			if (!_obeyCardinalMovement && south && west && !_visitedMap->get(column - 1, row + 1) && _mapPredicate(
				column - 1, row + 1))
			{
				updateOutputMap(currentWeight, column - 1, row + 1, column, row);
			}

			// Update goal dictionary
			lastLocator = _locatorCallback(column, row);

			// Get locator from the goal dictionary
			T goalLocator = goalDict.firstOrDefaultKey([&lastLocator] (T key, bool value)
			{
				return key.column == lastLocator.column && key.row == lastLocator.row;
			});

			// O(1)
			if (goalLocator != default_value::value<T>())
				goalDict.set(goalLocator, true);

			// Select next location from frontier queue - using the smallest weight
			if (_frontier->count() > 0)
			{
				// Lists in the frontier must have an entry
				simpleHash<T, T>* nextCostDict = _frontier->minValue();
				float nextCost = _frontier->minKey();

				// Get the first from the dictionary
				// var nextNode = nextCostDict.First();

				T nextNode = default_value::value<T>();

				// CHECK FOR GOAL LOCATION!
				goalDict.forEach([&nextNode, &nextCostDict] (T location, bool value)
				{
					if (!value)
					{
						nextNode = nextCostDict->firstOrDefaultKey([&location] (T ckey, T cvalue)
						{
							return ckey.column == location.column &&
								ckey.row == location.row;
						});

						if (nextNode != default_value::value<T>())
							return iterationCallback::breakAndReturn;
					}

					return iterationCallback::iterate;
				});

				// Otherwise, set to the next location (should be first dictionary key)
				if (nextNode == default_value::value<T>())
					nextNode = nextCostDict->getAt(0)->getKey();

				// Maintain frontier hash
				nextCostDict->remove(nextNode);

				// Remove the frontier node
				if (nextCostDict->count() == 0)
				{
					_frontier->remove(nextCost);

					// MEMORY
					delete nextCostDict;
				}

				// Move to next location
				column = nextNode.column;
				row = nextNode.row;
			}
		}

		// No goals were found for the last locator; OR any failed paths were found (TODO: Take a look at fail conditions)
		if (!goalDict.any([&lastLocator] (T key, bool value)
		{
			return key.column == lastLocator.column && key.row == lastLocator.row;
		}) ||
			goalDict.any([] (T key, bool value)
		{
			return !value;
		}))
		{
			throw simpleException("Dijkstra's Map was unable to find the current goal location");
		}

		// GENERATE PATHS
		for (int index = 0; index < _targetLocations->count(); index++)
		{
			simpleArray<T> completedPath = this->generatePath(_targetLocations->get(index));

			_completedPaths->add(_targetLocations->get(index), completedPath);
		}

		_finished = true;
	}

	template <isGridLocator T>
	simpleArray<T> dijkstra<T>::getResultPath(T targetLocation)
	{
		if (!_finished)
			throw simpleException("Must first call dijkstra.run() before calling getResultPath(..)");

		return _completedPaths->get(targetLocation);
	}

	template <isGridLocator T>
	simpleArray<T> dijkstra<T>::generatePath(T targetLocation)
	{
		if (!_targetLocations->contains(targetLocation))
			throw simpleException("Requested target location not specified by the constructor dijkstra.h");

		// Reverse ordered - starting with target
		simpleList<T> result;

		result.add(targetLocation);

		// Validate
		bool valid = _mapPredicate(targetLocation.column, targetLocation.row);

		if (!valid)
			throw simpleException("Invalid target location:  dijkstra<T>::generatePath(...)");

		T currentLocation = targetLocation;
		T goalLocation = _sourceLocation;

		// Find the "easiest" route to the goal (also, see operators for gridCell)
		while (currentLocation != goalLocation)
		{
			int column = currentLocation.column;
			int row = currentLocation.row;

			int north = row - 1 >= _relativeBoundary.top();
			int south = row + 1 <= _relativeBoundary.bottom();
			int east = column + 1 <= _relativeBoundary.right();
			int west = column - 1 >= _relativeBoundary.left();

			int lowestWeight = std::numeric_limits<int>::max();
			T lowestWeightLocation = currentLocation;

			if (north && (_outputMap->get(column, row - 1) < lowestWeight))
			{
				lowestWeightLocation = _locatorCallback(column, row - 1);
				lowestWeight = _outputMap->get(column, row - 1);
			}

			if (south && (_outputMap->get(column, row + 1) < lowestWeight))
			{
				lowestWeightLocation = _locatorCallback(column, row + 1);
				lowestWeight = _outputMap->get(column, row + 1);
			}

			if (east && (_outputMap->get(column + 1, row) < lowestWeight))
			{
				lowestWeightLocation = _locatorCallback(column + 1, row);
				lowestWeight = _outputMap->get(column + 1, row);
			}

			if (west && (_outputMap->get(column - 1, row) < lowestWeight))
			{
				lowestWeightLocation = _locatorCallback(column - 1, row);
				lowestWeight = _outputMap->get(column - 1, row);
			}

			if (north && east && !_obeyCardinalMovement && (_outputMap->get(column + 1, row - 1) < lowestWeight))
			{
				lowestWeightLocation = _locatorCallback(column + 1, row - 1);
				lowestWeight = _outputMap->get(column + 1, row - 1);
			}

			if (north && west && !_obeyCardinalMovement && (_outputMap->get(column - 1, row - 1) < lowestWeight))
			{
				lowestWeightLocation = _locatorCallback(column - 1, row - 1);
				lowestWeight = _outputMap->get(column - 1, row - 1);
			}

			if (south && east && !_obeyCardinalMovement && (_outputMap->get(column + 1, row + 1) < lowestWeight))
			{
				lowestWeightLocation = _locatorCallback(column + 1, row + 1);
				lowestWeight = _outputMap->get(column + 1, row + 1);
			}

			if (south && west && !_obeyCardinalMovement && (_outputMap->get(column - 1, row + 1) < lowestWeight))
			{
				lowestWeightLocation = _locatorCallback(column - 1, row + 1);
				lowestWeight = _outputMap->get(column - 1, row + 1);
			}

			if (lowestWeight == std::numeric_limits<int>::max())
			{
				throw simpleException("Mishandled Dijkstra Map dijkstra.generatePath()");
			}

			currentLocation = lowestWeightLocation;

			// Add this to the path
			if (!result.any([&lowestWeightLocation] (T alocation)
			{
				return alocation == lowestWeightLocation;
			}))
			{
				result.add(lowestWeightLocation);

				// VAILDATE
				valid &= _mapPredicate(lowestWeightLocation.column, lowestWeightLocation.row);

				if (!valid)
					throw simpleException("Invalid path found:  dijkstra.generatePath");
			}

			else
			{
				throw simpleException("Loop in Dijkstra Map path finding");
			}
		}

		// VAILDATE
		valid &= _mapPredicate(goalLocation.column, goalLocation.row);

		if (!valid)
			throw simpleException("Invalid path generated:  source point is not included in the map predicate:  dijkstra.h");

		// Final location
		result.add(goalLocation);

		// Remove endpoints (optional)
		for (int index = result.count() - 1; index >= 0 && !_includeEndpoints; index--)
		{
			if (result.get(index) == _sourceLocation ||
				result.get(index) == goalLocation)
				result.removeAt(index);
		}

		// Reverse the array for the proper solution
		return result.toArray().reverse();
	}

	template <isGridLocator T>
	void dijkstra<T>::updateOutputMap(float currentWeight, int destColumn, int destRow, int sourceColumn, int sourceRow)
	{
		// Procedure
		//
		// 1) Get the existing (old) weight from the output map
		// 2) Calculate the new weight and update the output map
		// 3) Fetch the old / new weight lists from the frontier BST
		// 4) Update the old / new weight lists and the frontier
		//
		// NOTE*** The weight lists should be very small - so running the update should
		//         not depend heavily on the List<>.Contains(...) performance.
		//
		//         Also, the AVL binary search tree has O(log n) performance for inserts
		//         / removals / and searches.
		//

		// Pre-fetch the cost list for this frontier location
		int oldWeight = _outputMap->get(destColumn, destRow);

		// Callback for next step weight
		int cost = _mapCostPredicate(destColumn, destRow);

		// Update the output map
		_outputMap->set(destColumn, destRow,
						simpleMath::minOf(_outputMap->get(destColumn, destRow), currentWeight + cost), true);

		// Update the frontier
		int newWeight = _outputMap->get(destColumn, destRow);

		// Check for a new location in the frontier
		if (!_locationMap->get(destColumn, destRow))
		{
			T newLocator = _locatorCallback(destColumn, destRow);

			// Check for existing weight list
			simpleHash<T, T>* weightList = new simpleHash<T, T>();

			// Initialize the new locator on the frontier
			if (_frontier->containsKey(newWeight))
			{
				weightList = _frontier->search(newWeight);
				weightList->add(newLocator, newLocator);
			}
			else
			{
				weightList->add(newLocator, newLocator);
				_frontier->insert(newWeight, weightList);
			}


			// Mark location map
			_locationMap->set(destColumn, destRow, true);

			return;
		}

		// ----- Otherwise, update existing locators -----

		// Get locator for the destination location
		T destLocator = _locatorCallback(destColumn, destRow);

		// UPDATE THE FRONTIER

		// Both weights are absent from the frontier
		if (!_frontier->containsKey(oldWeight) &&
			!_frontier->containsKey(newWeight))
		{
			simpleHash<T, T>* newList = new simpleHash<T, T>();

			newList->add(destLocator, destLocator);

			_frontier->insert(newWeight, newList);
		}

		// Old weight list exists; New weight list is absent
		else if (_frontier->containsKey(oldWeight) &&
			!_frontier->containsKey(newWeight))
		{
			simpleHash<T, T>* oldList = _frontier->search(oldWeight);
			simpleHash<T, T>* newList = new simpleHash<T, T>();

			newList->add(destLocator, destLocator);

			// Check for existing locator
			if (oldList->contains(destLocator))
				oldList->remove(destLocator);

			// Remove unused node
			if (oldList->count() == 0)
			{
				_frontier->remove(oldWeight);

				// MEMORY
				delete oldList;
			}

			// Insert new node in the frontier
			_frontier->insert(newWeight, newList);
		}

		// Old weight is absent; New weight exists
		else if (!_frontier->containsKey(oldWeight) &&
			_frontier->containsKey(newWeight))
		{
			simpleHash<T, T>* newList = _frontier->search(newWeight);

			// Locator doesn't exist in list
			if (!newList->contains(destLocator))
				newList->add(destLocator, destLocator);
		}

		// Both old and new weight lists exist
		else
		{
			simpleHash<T, T>* oldList = _frontier->search(oldWeight);
			simpleHash<T, T>* newList = _frontier->search(newWeight);

			// Check that they're different lists
			if (oldList != newList)
			{
				// Check that old weight list has element removed
				if (oldList->contains(destLocator))
					oldList->remove(destLocator);

				if (oldList->count() == 0)
				{
					_frontier->remove(oldWeight);

					// MEMORY!
					delete oldList;
				}

				// Check that new weight list has element added
				if (!newList->contains(destLocator))
					newList->add(destLocator, destLocator);
			}
		}
	}
}
