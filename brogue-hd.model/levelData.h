#pragma once

#include "creature.h"
#include "item.h"

namespace brogueHd::model
{
	// Stores the necessary info about a level so it can be regenerated:
	struct levelData
	{
		bool visited;
		//pcell mapStorage[DCOLS][DROWS];
		itemBase* items;
		creatureInfo* monsters;
		creatureInfo* dormantMonsters;
		int** scentMap;
		unsigned long levelSeed;
		int upStairsLoc[2];
		int downStairsLoc[2];
		int playerExitedVia[2];
		unsigned long awaySince;
	};
}
