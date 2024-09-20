#pragma once

#include "broguedef.h"
#include "creature.h"
#include "gridCell.h"
#include "grid.h"
#include "gridDefinitions.h"
#include "binarySearchTree.h"
#include <functional>

using namespace std;

using namespace brogueHd::component;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::math
{
	//struct pdsLink {
	//	short distance;
	//	short cost;
	//	pdsLink* left, * right;
	//};

	//struct pdsMap {
	//	boolean eightWays;

	//	pdsLink front;
	//	pdsLink links[DCOLS * DROWS];
	//};

    template<gridCellConstraint T>
	class dijkstra
	{

	public:

        /// <summary>
        /// Creates an instance of dijkstra's map component with the specified, declarative, delegates and constraints.
        /// </summary>
        /// <param name="mapCostPredicate">Delegate used to fetch a cost for the specified column / row of the grid</param>
        /// <param name="mapPredicate">Delegate used to mask off areas of the map from the entire algorithm. Set to TRUE to ALLOW use of the location for the algorithm.</param>
		dijkstra<T>::dijkstra(gridRect parentBoundary,
							  gridRect relativeBoundary,
						      bool obeyCardinalMovement,
							  std::function<bool(short, short)> mapPredicate,
							  std::function<short(short, short)> mapCostPredicate,
							  std::function<T(short, short)> locatorCallback);

        ~dijkstra();

        void initialize(T source, T targets[], bool obeyCardinalMovement);

		/// <summary>
		/// Runs Dijkstra's algorithm ONCE. WILL NOT RE-RUN.
		/// </summary>
		void run();

		/// <summary>
		/// Returns one of the result paths from dijkstra's algorithm
		/// </summary>
		T* getResultPath(T targetLocation);

		//void pdsUpdate(pdsMap* map);

		//void pdsClear(pdsMap* map, short maxDistance, boolean eightWays);

		//short pdsGetDistance(pdsMap* map, short x, short y);

		//void pdsSetDistance(pdsMap* map, short x, short y, short distance);

		//void pdsSetCosts(pdsMap* map, short** costMap);

		//void pdsBatchInput(pdsMap* map, short** distanceMap, short** costMap, short maxDistance, boolean eightWays);

		//void pdsBatchOutput(pdsMap* map, short** distanceMap);

		//void pdsInvalidate(pdsMap* map, short maxDistance);

		//void dijkstraScan(short** distanceMap, short** costMap, boolean useDiagonals);

		//void calculateDistances(short** distanceMap,
		//						short destinationX, 
		//						short destinationY,
		//						unsigned long blockingTerrainFlags,
		//						creature* traveler,
		//						boolean canUseSecretDoors,
		//						boolean eightWays);

		//short pathingDistance(short x1, short y1, short x2, short y2, unsigned long blockingTerrainFlags);

	private:

		/// <summary>
		/// Generates path from completed run
		/// </summary>
		T* generatePath(T targetLocation);


		/// <summary>
		/// Updates the output map which contains the calculated path steps
		/// </summary>
		/// <param name="currentWeight">Weight of current location</param>
		void updateOutputMap(float currentWeight, int destColumn, int destRow, int sourceColumn, int sourceRow);

	private:

		std::function<T(short, short)> _locatorCallback;
        std::function<bool(short, short)> _mapPredicate;
        std::function<short(short, short)> _mapCostPredicate;

		gridRect _parentBoundary;						// The larger of the two boundaries. Each grid<> instance should
		gridRect _relativeBoundary;						// use the larger parent boundary. Relative boundary is used to 
														// limit usage of the grid.

		T _sourceLocation;
		T[] _targetLocations;

        array2D<short>* _outputMap;

		array2D<bool>* _visitedMap;                        // Visited locations on the map
		array2D<bool>* _locationMap;                       // Locations that have been added to the frontier

        // Frontier BST for the map
		binarySearchTree<float, std::map<T, T>>* _frontier;

		// These maps are stored per target location
		std::map<T, T[]> _completedPaths;
		std::map<T, bool> _validPaths;

        bool _initialized = false;
        bool _finished = false;
		bool _obeyCardinalMovement = false;
	};
}

