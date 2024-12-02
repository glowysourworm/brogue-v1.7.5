#pragma once

#include "brogueCell.h"
#include "brogueObject.h"
#include "creatureBehavior.h"
#include "grid.h"
#include "gridLocator.h"
#include "simpleList.h"

namespace brogueHd::model
{
	class creatureBehaviorMap
	{
	public:
		creatureBehaviorMap()
		{
		};

		~creatureBehaviorMap()
		{
		};

		/// <summary>
		/// Updates creature behavior maps based on gloabl maps. Should be run each turn.
		/// </summary>
		void update(grid<brogueCell>* layoutGrid, grid<brogueObject>* contentGrid)
		{
		};

	private:
		creatureBehavior* _behavior;

		/*	Creature States

			MONSTER_SLEEPING,
			MONSTER_TRACKING_SCENT,
			MONSTER_WANDERING,
			MONSTER_FLEEING,
			MONSTER_ALLY
		*/

		/*  Creature Modes

			MODE_NORMAL,
			MODE_PERM_FLEEING

		*/

		grid<bool>* _visibilityMap;
		grid<bool>* _visibilityMapLastTurn; // Map from last turn
		grid<int>* _goalMap; // This map varies depending on the creature (also, friendly or enemy, "mapToMe")
		grid<int>* _fleeMap;
		grid<int>* _scentMap;

		// Collections of cells left over from updating the visibility. These will indicate cells that
		// have changed.
		simpleList<gridLocator>* _visibleCells;
		simpleList<gridLocator>* _visibleCellsDiff; // Difference between last turn and this turn

		// UNSURE:  These should be sent in during calculation
		int _numberOfWaypoints;
		gridLocator* _waypoints;

		// ???
		//grid<int>* _mapToStairs;
		//grid<int>* _mapToPit;
		// ???


		//grid<int>* _safetyMap;
		//grid<int>* _allySafetyMap;
		//grid<int>* _chokePointMap;

		//// Waypoints:
		//int targetWaypointIndex;          // the index number of the waypoint we're pathing toward
		//boolean waypointAlreadyVisited[MAX_WAYPOINT_COUNT]; // checklist of waypoints
		//int lastSeenPlayerAt[2];          // last location at which the monster hunted the player

		//int** mapToMe;					// if a pack leader, this is a periodically updated pathing map to get to the leader
		//int** safetyMap;					// fleeing monsters store their own safety map when out of player FOV to avoid omniscience
	};
}
