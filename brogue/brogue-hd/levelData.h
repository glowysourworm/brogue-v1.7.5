#pragma once

#include "creatureDeclaration.h"

using namespace brogueHd::backend::model::creature;

namespace brogueHd::backend::model
{
	// Stores the necessary info about a level so it can be regenerated:
	struct levelData 
	{
		bool visited;
		//pcell mapStorage[DCOLS][DROWS];
		item* items;
		creatureInfo* monsters;
		creatureInfo* dormantMonsters;
		short** scentMap;
		unsigned long levelSeed;
		short upStairsLoc[2];
		short downStairsLoc[2];
		short playerExitedVia[2];
		unsigned long awaySince;
	};
}

