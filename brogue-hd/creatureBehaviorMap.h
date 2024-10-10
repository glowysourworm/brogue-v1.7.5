#pragma once

#include "grid.h"
#include "gridLocator.h"
#include "brogueCell.h"
#include "brogueObject.h"
#include "creatureBehavior.h"
#include "simpleList.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model::creature;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model::creature
{
	class creatureBehaviorMap
	{
	public:

		creatureBehaviorMap(){};
		~creatureBehaviorMap(){};

		/// <summary>
		/// Updates creature behavior maps based on gloabl maps. Should be run each turn.
		/// </summary>
		void update(grid<brogueCell>* layoutGrid, grid<brogueObject>* contentGrid){};

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

		grid<bool>*	 _visibilityMap;
		grid<bool>*  _visibilityMapLastTurn;	// Map from last turn
		grid<short>* _goalMap;					// This map varies depending on the creature (also, friendly or enemy, "mapToMe")
		grid<short>* _fleeMap;
		grid<short>* _scentMap;

		// Collections of cells left over from updating the visibility. These will indicate cells that
		// have changed.
		simpleList<gridLocator>* _visibleCells;
		simpleList<gridLocator>* _visibleCellsDiff;			// Difference between last turn and this turn

		// UNSURE:  These should be sent in during calculation
		short _numberOfWaypoints;
		gridLocator* _waypoints;

		// ???
		//grid<short>* _mapToStairs;
		//grid<short>* _mapToPit;
		// ???


		//grid<short>* _safetyMap;
		//grid<short>* _allySafetyMap;
		//grid<short>* _chokePointMap;

		//// Waypoints:
		//short targetWaypointIndex;          // the index number of the waypoint we're pathing toward
		//boolean waypointAlreadyVisited[MAX_WAYPOINT_COUNT]; // checklist of waypoints
		//short lastSeenPlayerAt[2];          // last location at which the monster hunted the player

		//short** mapToMe;					// if a pack leader, this is a periodically updated pathing map to get to the leader
		//short** safetyMap;					// fleeing monsters store their own safety map when out of player FOV to avoid omniscience
	};
}