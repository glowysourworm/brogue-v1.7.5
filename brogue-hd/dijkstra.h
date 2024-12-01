#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleArray.h"
#include "simpleBST.h"
#include "simpleException.h"
#include "simpleHash.h"
#include "simpleList.h"
#include "simpleMath.h"
#include <functional>
#include <limits>

using namespace brogueHd::simple;

namespace brogueHd::component
{
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
	template<typename T>
	using dijkstraLocatorCallback = std::function<T(int column, int row)>;

	template<isGridLocator T>
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

		gridRect _parentBoundary;						// The larger of the two boundaries. Each grid<> instance should
		gridRect _relativeBoundary;						// use the larger parent boundary. Relative boundary is used to 
		// limit usage of the grid.

		T _sourceLocation;
		simpleArray<T>* _targetLocations;

		grid<int>* _outputMap;

		grid<bool>* _visitedMap;                        // Visited locations on the map
		grid<bool>* _locationMap;                       // Locations that have been added to the frontier

		// Frontier BST for the map
		simpleBST<float, simpleHash<T, T>*>* _frontier;

		// These maps are stored per target location
		simpleHash<T, simpleArray<T>>* _completedPaths;	// Stack allocated arrays
		simpleHash<T, bool>* _validPaths;

		bool _initialized = false;
		bool _finished = false;
		bool _obeyCardinalMovement = false;
	};

	template<isGridLocator T>
	dijkstra<T>::dijkstra(const gridRect& parentBoundary,
						  const gridRect& relativeBoundary,
						  bool obeyCardinalMovement,
						  const dijkstraPredicate& mapPredicate,
						  const dijkstraCostCallback& mapCostPredicate,
						  const dijkstraLocatorCallback<T>& locatorCallback)
	{
		_parentBoundary = parentBoundary;
		_relativeBoundary = relativeBoundary;
		_obeyCardinalMovement = obeyCardinalMovement;
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

	template<isGridLocator T>
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

	template<isGridLocator T>
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
				int outputValue = ((column == _sourceLocation.column) && (row == _sourceLocation.row) && passesPredicate) ? 0 : std::numeric_limits<int>::max();

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

	template<isGridLocator T>
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
			if (!_obeyCardinalMovement && north && east && !_visitedMap->get(column + 1, row - 1) && _mapPredicate(column + 1, row - 1))
			{
				updateOutputMap(currentWeight, column + 1, row - 1, column, row);
			}

			if (!_obeyCardinalMovement && north && west && !_visitedMap->get(column - 1, row - 1) && _mapPredicate(column - 1, row - 1))
			{
				updateOutputMap(currentWeight, column - 1, row - 1, column, row);
			}

			if (!_obeyCardinalMovement && south && east && !_visitedMap->get(column + 1, row + 1) && _mapPredicate(column + 1, row + 1))
			{
				updateOutputMap(currentWeight, column + 1, row + 1, column, row);
			}

			if (!_obeyCardinalMovement && south && west && !_visitedMap->get(column - 1, row + 1) && _mapPredicate(column - 1, row + 1))
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

	template<isGridLocator T>
	simpleArray<T> dijkstra<T>::getResultPath(T targetLocation)
	{
		if (!_finished)
			throw simpleException("Must first call dijkstra.run() before calling getResultPath(..)");

		return _completedPaths->get(targetLocation);
	}

	template<isGridLocator T>
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

		// NOTE:  ADD SOURCE LOCATION FOR INCLUSIVE PATH (not needed)

		// Reverse the path to build a result array
		simpleArray<T> resultArray(result.count());

		for (int index = result.count() - 1; index >= 0; index--)
			resultArray.set(result.count() - 1 - index, result.get(index));

		return resultArray;
	}

	template<isGridLocator T>
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
		_outputMap->set(destColumn, destRow, simpleMath::minOf(_outputMap->get(destColumn, destRow), currentWeight + cost), true);

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

	//void dijkstra::pdsUpdate(pdsMap* map) 
	//{
	//	int dir, dirs;
	//	pdsLink* left = NULL, * right = NULL, * link = NULL;

	//	dirs = map->eightWays ? 8 : 4;

	//	pdsLink* head = map->front.right;
	//	map->front.right = NULL;

	//	while (head != NULL) 
	//	{
	//		for (dir = 0; dir < dirs; dir++) 
	//		{
	//			link = head + (nbDirs[dir][0] + DCOLS * nbDirs[dir][1]);
	//			if (link < map->links || link >= map->links + DCOLS * DROWS) continue;

	//			// verify passability
	//			if (link->cost < 0) 
	//				continue;

	//			if (dir >= 4) 
	//			{
	//				pdsLink* way1, * way2;
	//				way1 = head + nbDirs[dir][0];
	//				way2 = head + DCOLS * nbDirs[dir][1];
	//				if (way1->cost == PDS_OBSTRUCTION || way2->cost == PDS_OBSTRUCTION) 
	//					continue;
	//			}

	//			if (head->distance + link->cost < link->distance) 
	//			{
	//				link->distance = head->distance + link->cost;

	//				// reinsert the touched cell; it'll be close to the beginning of the list now, so
	//				// this will be very fast.  start by removing it.

	//				if (link->right != NULL) link->right->left = link->left;
	//				if (link->left != NULL) link->left->right = link->right;

	//				left = head;
	//				right = head->right;
	//				while (right != NULL && right->distance < link->distance) {
	//					left = right;
	//					right = right->right;
	//				}
	//				if (left != NULL) left->right = link;
	//				link->right = right;
	//				link->left = left;
	//				if (right != NULL) right->left = link;
	//			}
	//		}

	//		right = head->right;

	//		head->left = NULL;
	//		head->right = NULL;

	//		head = right;
	//	}
	//}

	//void dijkstra::pdsClear(pdsMap* map, int maxDistance, boolean eightWays) 
	//{
	//	int i;

	//	map->eightWays = eightWays;

	//	map->front.right = NULL;

	//	for (i = 0; i < DCOLS * DROWS; i++) {
	//		map->links[i].distance = maxDistance;
	//		map->links[i].left = map->links[i].right = NULL;
	//	}
	//}

	//int dijkstra::pdsGetDistance(pdsMap* map, int x, int y) 
	//{
	//	pdsUpdate(map);
	//	return PDS_CELL(map, x, y)->distance;
	//}

	//void dijkstra::pdsSetDistance(pdsMap* map, int x, int y, int distance)
	//{
	//	pdsLink* left, * right, * link;

	//	if (x > 0 && y > 0 && x < DCOLS - 1 && y < DROWS - 1) 
	//	{
	//		link = PDS_CELL(map, x, y);

	//		if (link->distance > distance) 
	//		{
	//			link->distance = distance;

	//			if (link->right != NULL) link->right->left = link->left;
	//			if (link->left != NULL) link->left->right = link->right;

	//			left = &map->front;
	//			right = map->front.right;

	//			while (right != NULL && right->distance < link->distance) 
	//			{
	//				left = right;
	//				right = right->right;
	//			}

	//			link->right = right;
	//			link->left = left;
	//			left->right = link;
	//			if (right != NULL) right->left = link;
	//		}
	//	}
	//}

	//void dijkstra::pdsSetCosts(pdsMap* map, int** costMap)
	//{
	//	int i, j;

	//	for (i = 0; i < DCOLS; i++) {
	//		for (j = 0; j < DROWS; j++) {
	//			if (i != 0 && j != 0 && i < DCOLS - 1 && j < DROWS - 1) {
	//				PDS_CELL(map, i, j)->cost = costMap[i][j];
	//			}
	//			else {
	//				PDS_CELL(map, i, j)->cost = PDS_FORBIDDEN;
	//			}
	//		}
	//	}
	//}

	//void dijkstra::pdsBatchInput(pdsMap* map, int** distanceMap, int** costMap, int maxDistance, boolean eightWays) 
	//{
	//	int i, j;
	//	pdsLink* left, * right;

	//	map->eightWays = eightWays;

	//	left = NULL;
	//	right = NULL;

	//	map->front.right = NULL;

	//	for (i = 0; i < DCOLS; i++) 
	//	{
	//		for (j = 0; j < DROWS; j++) 
	//		{
	//			pdsLink* link = PDS_CELL(map, i, j);

	//			if (distanceMap != NULL) 
	//			{
	//				link->distance = distanceMap[i][j];
	//			}
	//			else 
	//			{
	//				if (costMap != NULL) 
	//				{
	//					// totally hackish; refactor
	//					link->distance = maxDistance;
	//				}
	//			}

	//			int cost;

	//			if (i == 0 || j == 0 || i == DCOLS - 1 || j == DROWS - 1) 
	//			{
	//				cost = PDS_OBSTRUCTION;
	//			}
	//			else if (costMap == NULL) 
	//			{
	//				if (cellHasTerrainFlag(i, j, T_OBSTRUCTS_PASSABILITY) && cellHasTerrainFlag(i, j, T_OBSTRUCTS_DIAGONAL_MOVEMENT))
	//				{
	//					cost = PDS_OBSTRUCTION;
	//				}

	//				else
	//				{
	//					cost = PDS_FORBIDDEN;
	//				}
	//			}
	//			else 
	//			{
	//				cost = costMap[i][j];
	//			}

	//			link->cost = cost;

	//			if (cost > 0) {
	//				if (link->distance < maxDistance) {
	//					if (right == NULL || right->distance > link->distance) {
	//						// left and right are used to traverse the list; if many cells have similar values,
	//						// some time can be saved by not clearing them with each insertion.  this time,
	//						// sadly, we have to start from the front.

	//						left = &map->front;
	//						right = map->front.right;
	//					}

	//					while (right != NULL && right->distance < link->distance) {
	//						left = right;
	//						right = right->right;
	//					}

	//					link->right = right;
	//					link->left = left;
	//					left->right = link;
	//					if (right != NULL) right->left = link;

	//					left = link;
	//				}
	//				else {
	//					link->right = NULL;
	//					link->left = NULL;
	//				}
	//			}
	//			else {
	//				link->right = NULL;
	//				link->left = NULL;
	//			}
	//		}
	//	}
	//}

	//void dijkstra::pdsBatchOutput(pdsMap* map, int** distanceMap) 
	//{
	//	int i, j;

	//	pdsUpdate(map);
	//	// transfer results to the distanceMap
	//	for (i = 0; i < DCOLS; i++) {
	//		for (j = 0; j < DROWS; j++) {
	//			distanceMap[i][j] = PDS_CELL(map, i, j)->distance;
	//		}
	//	}
	//}

	//void dijkstra::pdsInvalidate(pdsMap* map, int maxDistance)
	//{
	//	pdsBatchInput(map, NULL, NULL, maxDistance, map->eightWays);
	//}

	//void dijkstra::dijkstraScan(int** distanceMap, int** costMap, boolean useDiagonals) 
	//{
	//	static pdsMap map;

	//	pdsBatchInput(&map, distanceMap, costMap, 30000, useDiagonals);
	//	pdsBatchOutput(&map, distanceMap);
	//}

	//void dijkstra::calculateDistances(int** distanceMap,
	//								  int destinationX, 
	//								  int destinationY,
	//								  unsigned long blockingTerrainFlags,
	//								  creature* traveler,
	//								  boolean canUseSecretDoors,
	//								  boolean eightWays) 
	//{
	//	creature* monst;
	//	static pdsMap map;

	//	int i, j;

	//	for (i = 0; i < DCOLS; i++) 
	//	{
	//		for (j = 0; j < DROWS; j++) 
	//		{
	//			char cost;
	//			monst = monsterAtLoc(i, j);
	//			if (monst
	//				&& (monst->info.flags & (MONST_IMMUNE_TO_WEAPONS | MONST_INVULNERABLE))
	//				&& (monst->info.flags & (MONST_IMMOBILE | MONST_GETS_TURN_ON_ACTIVATION))) {

	//				// Always avoid damage-immune stationary monsters.
	//				cost = PDS_FORBIDDEN;
	//			}
	//			else if (canUseSecretDoors
	//				&& cellHasTMFlag(i, j, TM_IS_SECRET)
	//				&& cellHasTerrainFlag(i, j, T_OBSTRUCTS_PASSABILITY)
	//				&& !(discoveredTerrainFlagsAtLoc(i, j) & T_OBSTRUCTS_PASSABILITY)) 
	//			{
	//				cost = 1;
	//			}
	//			else if (cellHasTerrainFlag(i, j, T_OBSTRUCTS_PASSABILITY)
	//				|| (traveler && traveler == &player && !(pmap[i][j].flags & (DISCOVERED | MAGIC_MAPPED)))) 
	//			{
	//				cost = cellHasTerrainFlag(i, j, T_OBSTRUCTS_DIAGONAL_MOVEMENT) ? PDS_OBSTRUCTION : PDS_FORBIDDEN;
	//			}
	//			else if ((traveler && monsterAvoids(traveler, i, j)) || cellHasTerrainFlag(i, j, blockingTerrainFlags)) 
	//			{
	//				cost = PDS_FORBIDDEN;
	//			}
	//			else {
	//				cost = 1;
	//			}

	//			PDS_CELL(&map, i, j)->cost = cost;
	//		}
	//	}

	//	pdsClear(&map, 30000, eightWays);
	//	pdsSetDistance(&map, destinationX, destinationY, 0);
	//	pdsBatchOutput(&map, distanceMap);
	//}

	//int dijkstra::pathingDistance(int x1, int y1, int x2, int y2, unsigned long blockingTerrainFlags) 
	//{
	//	int retval, ** distanceMap = allocGrid();
	//	calculateDistances(distanceMap, x2, y2, blockingTerrainFlags, NULL, true, true);
	//	retval = distanceMap[x1][y1];
	//	freeGrid(distanceMap);
	//	return retval;
	//}
}

