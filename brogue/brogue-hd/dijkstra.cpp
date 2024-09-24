#include "dijkstra.h"
#include "gridRect.h"
#include "exceptionHandler.h"
#include "mapextension.h"
#include "binarySearchTree.h"
#include <functional>

using namespace std;

using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::math
{

	template<isGridLocator T>
	dijkstra<T>::dijkstra(gridRect parentBoundary,
						  gridRect relativeBoundary,
						  bool obeyCardinalMovement,
						  std::function<bool(short, short)> mapPredicate,
						  std::function<short(short, short)> mapCostPredicate,
						  std::function<T(short, short)> locatorCallback)
	{
		_parentBoundary = parentBoundary;
		_relativeBoundary = relativeBoundary;
		_obeyCardinalMovement = obeyCardinalMovement;
		_mapPredicate = mapPredicate;
		_mapCostPredicate = mapCostPredicate;
		_locatorCallback = locatorCallback;

		// Set these to be the full size of the parent
		_outputMap = new array2D<short>(parentBoundary);
		_visitedMap = new array2D<bool>(parentBoundary);
		_locationMap = new array2D<bool>(parentBoundary);

		_frontier = new binarySearchTree<float, std::map<T, T>>();
	}

	template<isGridLocator T>
	dijkstra<T>::~dijkstra()
	{
		delete _outputMap;
		delete _visitedMap;
		delete _locationMap;

		delete _frontier;
	}

	template<isGridLocator T>
	void dijkstra<T>::initialize(T source, T targets[])
	{
		_sourceLocation = sourceLocation;
		_targetLocations = targetLocations;

		// Clear out the frontier
		_frontier->clear();

		// Clear the result
		_completedPaths.clear();
		_validPaths.clear();

		iterate(_relativeBoundary, [](short column, short row)
		{
			// CHECK PREDICATE TO SET MAP COST OFF LIMITS
			var passesPredicate = _predicate(column, row);

			// Initialize output map (set to infinity for initialization
			short outputValue = ((column == _sourceLocation->column) && (row == _sourceLocation->row) && passesPredicate) ? 0 : SHRT_MAX;
			
			_outputMap->set(column, row, outputValue);

			// Initialize visited map
			_visitedMap->set(column, row, false);

			// Initialize location map
			_locationMap->set(column, row, false);

			return iterationCallback::iterate;
		});

		_initialized = true;
		_finished = false;
	}

	template<isGridLocator T>
	void dijkstra<T>::run()
	{
		if (!_initialized)
			brogueException::show("Must call initialize() before run() dijkstra.h");

		if (_finished)
			return;

		// Track goal progress
		std::map<T, bool> goalDict;

		// Not sure about hashing
		for (int index = 0; index < SIZEOF(_targetLocations); index++)
			goalDict.insert(_targetLocations[index], _targetLocations[index]);

		// Process the first element
		short column = _sourceLocation->column;
		short row = _sourceLocation->row;

		T lastLocation = NULL;

		// Iterate while any target not reached (AND) not visited
		while (!_visitedMap->get(column, row) &&
				any(goalDict, [](T key, T value)
				{
					return !value;
				});
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
			if (north && !_visitedMap->get(column, row - 1) && _predicate(column, row - 1))
			{
				updateOutputMap(currentWeight, column, row - 1, column, row);
			}

			if (south && !_visitedMap->get(column, row + 1) && _predicate(column, row + 1))
			{
				updateOutputMap(currentWeight, column, row + 1, column, row);
			}

			if (east && !_visitedMap->get(column + 1, row) && _predicate(column + 1, row))
			{
				updateOutputMap(currentWeight, column + 1, row, column, row);
			}

			if (west && !_visitedMap->get(column - 1, row) && _predicate(column - 1, row))
			{
				updateOutputMap(currentWeight, column - 1, row, column, row);
			}

			// NON-CARDINAL LOCATIONS
			if (!this.ObeyCardinalMovement && north && east && !_visitedMap->get(column + 1, row - 1) && _predicate(column + 1, row - 1))
			{
				updateOutputMap(currentWeight, column + 1, row - 1, column, row);
			}

			if (!this.ObeyCardinalMovement && north && west && !_visitedMap->get(column - 1, row - 1) && _predicate(column - 1, row - 1))
			{
				updateOutputMap(currentWeight, column - 1, row - 1, column, row);
			}

			if (!this.ObeyCardinalMovement && south && east && !_visitedMap->get(column + 1, row + 1) && _predicate(column + 1, row + 1))
			{
				updateOutputMap(currentWeight, column + 1, row + 1, column, row);
			}

			if (!this.ObeyCardinalMovement && south && west && !_visitedMap->get(column - 1, row + 1) && _predicate(column - 1, row + 1))
			{
				updateOutputMap(currentWeight, column - 1, row + 1, column, row);
			}

			// Update goal dictionary
			lastLocator = _locatorCallback(column, row);

			// Get locator from the goal dictionary
			T goalLocator = firstOrDefaultKey(goalDict, [](T key, bool value)
			{
				return key.column == lastLocator.column && key.row == lastLocator.row;
			});

			// O(1)
			if (goalLocator != NULL)
				goalDict[goalLocator] = true;

			// Select next location from frontier queue - using the smallest weight
			if (_frontier.count() > 0)
			{
				// Lists in the frontier must have an entry
				std::map<T, T> nextCostDict = _frontier->min();
				float nextCost = _frontier->minKey();

				// Get the first from the dictionary
				// var nextNode = nextCostDict.First();

				T nextNode = NULL;

				// CHECK FOR GOAL LOCATION!
				forEach(goalDict, [](T key, bool value)
				{
					if (!value)
					{
						nextNode = firstOrDefaultKey(nextCostDict, [](T ckey, T cvalue)
						{
							return ckey.column == location.column &&
								   ckey.row == location.row;
						});

						if (nextNode != NULL)
							return iterationCallback::breakAndReturn;
					}
				});

				// Otherwise, set to the next location (should be first dictionary key)
				if (nextNode == NULL)
					nextNode = nextCostDict.begin()->first;

				// Maintain frontier hash
				var removed = nextCostDict.erase(nextNode);

				if (nextCostDict.count() == 0)
					_frontier.remove(nextCost);

				// Move to next location
				column = nextNode.Column;
				row = nextNode.Row;
			}
		}

		// No goals were found for the last locator; OR any failed paths were found (TODO: Take a look at fail conditions)
		if (!any(goalDict, [](T key, bool value)
			{
				return key.column == lastLocator.column && key.row == lastLocator.row;
			}) ||
			any(goalDict, [](T key, bool value)
			{
				return !value;
			})
		{
			brogueException::show("Dijkstra's Map was unable to find the current goal location");
		}

		// GENERATE PATHS
		for (int index = 0; index < _targetLocations; index++)
		{
			T[] completedPath = generatePath(_targetLocations[index]);

			_completedPaths.insert(_targetLocations[index], completedPath);
		}

		_finished = true;
	}

	template<isGridLocator T>
	T* dijkstra<T>::getResultPath(T targetLocation)
	{
		if (!_finished)
			brogueException::show("Must first call dijkstra.run() before calling getResultPath(..)");

		return _completedPaths[targetLocation];
	}

	template<isGridLocator T>
	T* dijkstra<T>::generatePath(T targetLocation)
	{
		if (!contains(_targetLocations, targetLocation))
			brogueException::show("Requested target location not specified by the constructor dijkstra.h");

		// Reverse ordered - starting with target
		std::vector<T> result;

		result.push_back(targetLocation);

		// Validate
		bool valid = _predicate(targetLocation.column, targetLocation.row);

		if (!valid)
			brogueException::show("Invalid target location:  dijkstra<T>::generatePath(...)");

		T currentLocation = targetLocation;
		T goalLocation = _sourceLocation;

		// Find the "easiest" route to the goal (also, see operators for gridCell)
		while (currentLocation != goalLocation)
		{
			short column = currentLocation.Column;
			short row = currentLocation.Row;

			short north = row - 1 >= _relativeBoundary.top();
			short south = row + 1 <= _relativeBoundary.bottom();
			short east = column + 1 <= _relativeBoundary.right();
			short west = column - 1 >= _relativeBoundary.left();

			short lowestWeight = SHRT_MAX;
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

			if (lowestWeight == SHRT_MAX)
			{
				brogueException::show("Mishandled Dijkstra Map dijkstra.generatePath()");
			}

			currentLocation = lowestWeightLocation;

			// Add this to the path
			if (!any(result, [&lowestWeightLocation](T alocation)
			{
				return alocation == lowestWeightLocation;
			})
			{
				result.push_back(lowestWeightLocation);

				// VAILDATE
				valid &= _predicate(lowestWeightLocation.Column, lowestWeightLocation.Row);

				if (!valid)
					brogueException::show("Invalid path found:  dijkstra.generatePath");
			}

			else
			{
				brogueException::show("Loop in Dijkstra Map path finding");
			}
		}

		// VAILDATE
		valid &= _predicate(goalLocation.Column, goalLocation.Row);

		// NOTE:  ADD SOURCE LOCATION FOR INCLUSIVE PATH (not needed)

		// Reverse the path to build a result array
		T resultArray = T[result.size()];

		for (int index = result.size() - 1; index >= 0; index--)
			resultArray[result.size() - 1 - index] = result[index];
		
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
		short oldWeight = _outputMap->get(destColumn, destRow);

		// Callback for next step weight
		short cost = _mapCostPredicate(destColumn, destRow);

		// Update the output map
		_outputMap->set(destColumn, destRow, min(_outputMap->get(destColumn, destRow), currentWeight + cost));

		// Update the frontier
		short newWeight = _outputMap->get(destColumn, destRow);

		// Check for a new location in the frontier
		if (!_locationMap->get(destColumn, destRow))
		{
			T newLocator = _locatorCallback(destColumn, destRow);

			// Check for existing weight list
			std::map<T, T> weightList;

			// Initialize the new locator on the frontier
			if (_frontier.containsKey(newWeight))
			{
				weightList = _frontier.search(newWeight);
				weightList.insert(newLocator, newLocator);
			}
			else
			{
				weightList.insert(newLocator, newLocator);
				_frontier.insert(newWeight, weightList);
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
		if (!_frontier.containsKey(oldWeight) &&
			!_frontier.containsKey(newWeight))
		{
			std::map<T, T> newList;

			newList.insert(destLocator, destLocator);

			_frontier.Insert(newWeight, newList);
		}

		// Old weight list exists; New weight list is absent
		else if (_frontier.containsKey(oldWeight) &&
				!_frontier.containsKey(newWeight))
		{
			std::map<T, T> oldList = _frontier->search(oldWeight);
			std::map<T, T> newList;

			newList.insert(destLocator, destLocator);

			// Check for existing locator
			if (oldList.contains(destLocator))
				oldList.remove(destLocator);

			// Remove unused node
			if (oldList.count() == 0)
				_frontier.remove(oldWeight);

			// Insert new node in the frontier
			_frontier->insert(newWeight, newList);
		}

		// Old weight is absent; New weight exists
		else if (!_frontier.containsKey(oldWeight) &&
				  _frontier.containsKey(newWeight))
		{
			std::map<T, T> newList = _frontier->search(newWeight);

			// Locator doesn't exist in list
			if (!newList.contains(destLocator))
				 newList.insert(destLocator, destLocator);
		}

		// Both old and new weight lists exist
		else
		{
			std::map<T, T> oldList = _frontier->search(oldWeight);
			std::map<T, T> newList = _frontier->search(newWeight);

			// Check that they're different lists
			if (oldList != newList)
			{
				// Check that old weight list has element removed
				if (oldList.containsKey(destLocator))
					oldList.remove(destLocator);

				if (oldList.count() == 0)
					_frontier.remove(oldWeight);

				// Check that new weight list has element added
				if (!newList.containsKey(destLocator))
					 newList.insert(destLocator, destLocator);
			}
		}
	}

	//void dijkstra::pdsUpdate(pdsMap* map) 
	//{
	//	short dir, dirs;
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

	//void dijkstra::pdsClear(pdsMap* map, short maxDistance, boolean eightWays) 
	//{
	//	short i;

	//	map->eightWays = eightWays;

	//	map->front.right = NULL;

	//	for (i = 0; i < DCOLS * DROWS; i++) {
	//		map->links[i].distance = maxDistance;
	//		map->links[i].left = map->links[i].right = NULL;
	//	}
	//}

	//short dijkstra::pdsGetDistance(pdsMap* map, short x, short y) 
	//{
	//	pdsUpdate(map);
	//	return PDS_CELL(map, x, y)->distance;
	//}

	//void dijkstra::pdsSetDistance(pdsMap* map, short x, short y, short distance)
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

	//void dijkstra::pdsSetCosts(pdsMap* map, short** costMap)
	//{
	//	short i, j;

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

	//void dijkstra::pdsBatchInput(pdsMap* map, short** distanceMap, short** costMap, short maxDistance, boolean eightWays) 
	//{
	//	short i, j;
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

	//void dijkstra::pdsBatchOutput(pdsMap* map, short** distanceMap) 
	//{
	//	short i, j;

	//	pdsUpdate(map);
	//	// transfer results to the distanceMap
	//	for (i = 0; i < DCOLS; i++) {
	//		for (j = 0; j < DROWS; j++) {
	//			distanceMap[i][j] = PDS_CELL(map, i, j)->distance;
	//		}
	//	}
	//}

	//void dijkstra::pdsInvalidate(pdsMap* map, short maxDistance)
	//{
	//	pdsBatchInput(map, NULL, NULL, maxDistance, map->eightWays);
	//}

	//void dijkstra::dijkstraScan(short** distanceMap, short** costMap, boolean useDiagonals) 
	//{
	//	static pdsMap map;

	//	pdsBatchInput(&map, distanceMap, costMap, 30000, useDiagonals);
	//	pdsBatchOutput(&map, distanceMap);
	//}

	//void dijkstra::calculateDistances(short** distanceMap,
	//								  short destinationX, 
	//								  short destinationY,
	//								  unsigned long blockingTerrainFlags,
	//								  creature* traveler,
	//								  boolean canUseSecretDoors,
	//								  boolean eightWays) 
	//{
	//	creature* monst;
	//	static pdsMap map;

	//	short i, j;

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

	//short dijkstra::pathingDistance(short x1, short y1, short x2, short y2, unsigned long blockingTerrainFlags) 
	//{
	//	short retval, ** distanceMap = allocGrid();
	//	calculateDistances(distanceMap, x2, y2, blockingTerrainFlags, NULL, true, true);
	//	retval = distanceMap[x1][y1];
	//	freeGrid(distanceMap);
	//	return retval;
	//}
}