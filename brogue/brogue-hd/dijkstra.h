#pragma once

#include "broguedef.h"
#include "creature.h"
#include "brogueCell.h"
#include <functional>

using namespace std;

namespace brogueHd
{
	struct pdsLink {
		short distance;
		short cost;
		pdsLink* left, * right;
	};

	struct pdsMap {
		boolean eightWays;

		pdsLink front;
		pdsLink links[DCOLS * DROWS];
	};

	class dijkstra
	{

	public:

        /// <summary>
        /// 
        /// </summary>
        /// <param name="mapCostPredicate">Delegate used to fetch a cost for the specified column / row of the grid</param>
        /// <param name="mapPredicate">Delegate used to mask off areas of the map from the entire algorithm. Set to TRUE to ALLOW use of the location for the algorithm.</param>
        dijkstra(std::function<bool(brogueCell)> mapPredicate,
                 std::function<short(brogueCell)> mapCostPredicate);

        ~dijkstra();

        void initialize(brogueCell source, brogueCell targets[], bool obeyCardinalMovement);

		void pdsUpdate(pdsMap* map);

		void pdsClear(pdsMap* map, short maxDistance, boolean eightWays);

		short pdsGetDistance(pdsMap* map, short x, short y);

		void pdsSetDistance(pdsMap* map, short x, short y, short distance);

		void pdsSetCosts(pdsMap* map, short** costMap);

		void pdsBatchInput(pdsMap* map, short** distanceMap, short** costMap, short maxDistance, boolean eightWays);

		void pdsBatchOutput(pdsMap* map, short** distanceMap);

		void pdsInvalidate(pdsMap* map, short maxDistance);

		void dijkstraScan(short** distanceMap, short** costMap, boolean useDiagonals);

		void calculateDistances(short** distanceMap,
								short destinationX, 
								short destinationY,
								unsigned long blockingTerrainFlags,
								creature* traveler,
								boolean canUseSecretDoors,
								boolean eightWays);

		short pathingDistance(short x1, short y1, short x2, short y2, unsigned long blockingTerrainFlags);


	private:

        std::function<bool(brogueCell)> _mapPredicate;
        std::function<short(brogueCell)> _mapCostPredicate;


            /// <summary>
            /// 2D array of output values for Dijkstra's algorithm
            /// </summary>
        public Grid<float> OutputMap{ get; private set; }

        public readonly static float MapCostMin = 1;
        public readonly static float MapCostLow = 2;
        public readonly static float MapCostMedium = 5;
        public readonly static float MapCostHigh = 10;

        /// <summary>
        /// Constant that indicates that a region is off-limits to the Dijkstra map. (THIS SHOULD BE AVOIDED AS CALLBACK COST)
        /// </summary>
        protected readonly static float MapCostInfinity = float.PositiveInfinity;

        private readonly GridBoundary _relativeBoundary;
        private readonly DijkstraMapPredicate _predicate;
        private readonly DijkstraMapAvoidCallback _avoidCallback;
        private readonly DijkstraMapLocatorCallback _locatorCallback;
        private readonly Metric _euclideanMetric;

        // Visited locations on the map
        private Grid<bool> _visitedMap;

        // Locations that have been added to the frontier
        private Grid<bool> _locationMap;

        // Frontier BST for the map
        BinarySearchTree<float, SimpleDictionary<T, T>> _frontier;

        // Completed Paths
        SimpleDictionary<T, IReadOnlyList<T>> _completedPaths;
        SimpleDictionary<T, bool> _validPaths;

        bool _initialized = false;
        bool _finished = false;
	};
}

