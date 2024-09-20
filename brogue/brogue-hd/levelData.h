#pragma once

namespace brogueHd::backend::model
{
	// Stores the necessary info about a level so it can be regenerated:
	struct levelData 
	{
		boolean visited;
		//pcell mapStorage[DCOLS][DROWS];
		struct item* items;
		struct creature* monsters;
		struct creature* dormantMonsters;
		short** scentMap;
		unsigned long levelSeed;
		short upStairsLoc[2];
		short downStairsLoc[2];
		short playerExitedVia[2];
		unsigned long awaySince;
	};
}

