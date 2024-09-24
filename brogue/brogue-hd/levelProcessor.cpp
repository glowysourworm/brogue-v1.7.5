#include "levelProcessor.h"
#include "brogueLevel.h"
#include "colorConstants.h"

using namespace brogueHd::backend::processor::methods;

namespace brogueHd
{
	levelProcessor::levelProcessor(brogueLevel* level)
	{
		_level = level;
	}

	levelProcessor::~levelProcessor()
	{

	}

	void levelProcessor::initialize()
	{
		unsigned long oldSeed;
		item* theItem;
		short loc[2], i, j, x, y, px, py, flying, dir;
		boolean placedPlayer;
		creature* monst;
		dungeonLayers layer;
		unsigned long timeAway;
		short** mapToStairs;
		short** mapToPit;
		bool connectingStairsDiscovered;

		//if (oldLevelNumber == DEEPEST_LEVEL && stairDirection != -1) {
		//	return;
		//}

		// NOTE***	The intention of environment "ticks" was to allow the physical environment to "simmer"
		//			while the rogue waits to move. Lets do this a little differently - which avoids 
		//			"out of sync" errors - whatever they may have been. 
		//
		//			The environment will be decoupled from enemy reactions. So, game ticks can still be
		//			processed here. Also, the "simmering" will only be processed once, when loading
		//			the environment for the first time.
		//

		//synchronizePlayerTimeState();

		//rogue.updatedSafetyMapThisTurn = false;
		//rogue.updatedAllySafetyMapThisTurn = false;
		//rogue.updatedMapToSafeTerrainThisTurn = false;

		//rogue.cursorLoc[0] = -1;
		//rogue.cursorLoc[1] = -1;
		//rogue.lastTarget = NULL;

		connectingStairsDiscovered = stairsUpDiscovered(_level);

		//connectingStairsDiscovered = (pmap[rogue.downLoc[0]][rogue.downLoc[1]].flags & (DISCOVERED | MAGIC_MAPPED) ? true : false);
		//if (stairDirection == 0) { // fallen
		//	levels[oldLevelNumber - 1].playerExitedVia[0] = player.xLoc;
		//	levels[oldLevelNumber - 1].playerExitedVia[1] = player.yLoc;
		//}

		//if (oldLevelNumber != rogue.depthLevel) 
		//{

		// Select location for player
		px = player.xLoc;
		py = player.yLoc;

		// Check for auto-descent
		if (isAutoDescent(_level, _player->xLoc, _player->yLoc))
		{
			// Find walkable location
			brogueCell* walkableCell = findAdjacentWalkableLocation(_level, _player->xLoc, _player->yLoc);
		}

		// NEXT***  Calculate map to stairs to figure out how to initialize enemies
		//

		//	mapToStairs = allocGrid();
		//	fillGrid(mapToStairs, 0);

		//	for (flying = 0; flying <= 1; flying++) 
		//	{
		//		fillGrid(mapToStairs, 0);
		//		calculateDistances(mapToStairs, px, py, (flying ? T_OBSTRUCTS_PASSABILITY : T_PATHING_BLOCKER) | T_SACRED, NULL, true, true);

		//		for (monst = monsters->nextCreature; monst != NULL; monst = monst->nextCreature) 
		//		{
		//			x = monst->xLoc;
		//			y = monst->yLoc;

		//			if (((monst->creatureState == MONSTER_TRACKING_SCENT && (stairDirection != 0 || monst->status[STATUS_LEVITATING]))
		//				|| monst->creatureState == MONSTER_ALLY || monst == rogue.yendorWarden)
		//				&& (stairDirection != 0 || monst->currentHP > 10 || monst->status[STATUS_LEVITATING])
		//				&& ((flying != 0) == ((monst->status[STATUS_LEVITATING] != 0)
		//					|| cellHasTerrainFlag(x, y, T_PATHING_BLOCKER)
		//					|| cellHasTerrainFlag(px, py, T_AUTO_DESCENT)))
		//				&& !(monst->bookkeepingFlags & MB_CAPTIVE)
		//				&& !(monst->info.flags & (MONST_WILL_NOT_USE_STAIRS | MONST_RESTRICTED_TO_LIQUID))
		//				&& !(cellHasTerrainFlag(x, y, T_OBSTRUCTS_PASSABILITY))
		//				&& !monst->status[STATUS_ENTRANCED]
		//				&& !monst->status[STATUS_PARALYZED]
		//				&& (mapToStairs[monst->xLoc][monst->yLoc] < 30000 || monst->creatureState == MONSTER_ALLY || monst == rogue.yendorWarden)) {

		//				monst->status[STATUS_ENTERS_LEVEL_IN] = clamp(mapToStairs[monst->xLoc][monst->yLoc] * monst->movementSpeed / 100 + 1, 1, 150);
		//				switch (stairDirection) {
		//				case 1:
		//					monst->bookkeepingFlags |= MB_APPROACHING_DOWNSTAIRS;
		//					break;
		//				case -1:
		//					monst->bookkeepingFlags |= MB_APPROACHING_UPSTAIRS;
		//					break;
		//				case 0:
		//					monst->bookkeepingFlags |= MB_APPROACHING_PIT;
		//					break;
		//				default:
		//					break;
		//				}
		//			}
		//		}
		//	}
		//	freeGrid(mapToStairs);
		//}

		// NEXT*** Calculate monster maps

		//for (monst = monsters->nextCreature; monst != NULL; monst = monst->nextCreature) 
		//{
		//	if (monst->mapToMe) {
		//		freeGrid(monst->mapToMe);
		//		monst->mapToMe = NULL;
		//	}
		//	if (monst->safetyMap) {
		//		freeGrid(monst->safetyMap);
		//		monst->safetyMap = NULL;
		//	}
		//}

		// NEXT*** Moving some creatures around. Not sure why some of these arrays were pushed this way.

		//levels[oldLevelNumber - 1].monsters = monsters->nextCreature;
		//levels[oldLevelNumber - 1].dormantMonsters = dormantMonsters->nextCreature;
		//levels[oldLevelNumber - 1].items = floorItems->nextItem;

		// NEXT***  Looks like it's processing visibility to store memory data. This can be done to
		//			prepare the memorized grid.
		//

		//for (i = 0; i < DCOLS; i++) {
		//	for (j = 0; j < DROWS; j++) {
		//		if (pmap[i][j].flags & VISIBLE) {
		//			// Remember visible cells upon exiting.
		//			storeMemories(i, j);
		//		}
		//		for (layer = 0; layer < NUMBER_TERRAIN_LAYERS; layer++) {
		//			levels[oldLevelNumber - 1].mapStorage[i][j].layers[layer] = pmap[i][j].layers[layer];
		//		}
		//		levels[oldLevelNumber - 1].mapStorage[i][j].volume = pmap[i][j].volume;
		//		levels[oldLevelNumber - 1].mapStorage[i][j].flags = (pmap[i][j].flags & PERMANENT_TILE_FLAGS);
		//		levels[oldLevelNumber - 1].mapStorage[i][j].rememberedAppearance = pmap[i][j].rememberedAppearance;
		//		levels[oldLevelNumber - 1].mapStorage[i][j].rememberedTerrain = pmap[i][j].rememberedTerrain;
		//		levels[oldLevelNumber - 1].mapStorage[i][j].rememberedItemCategory = pmap[i][j].rememberedItemCategory;
		//		levels[oldLevelNumber - 1].mapStorage[i][j].rememberedItemKind = pmap[i][j].rememberedItemKind;
		//		levels[oldLevelNumber - 1].mapStorage[i][j].rememberedCellFlags = pmap[i][j].rememberedCellFlags;
		//		levels[oldLevelNumber - 1].mapStorage[i][j].rememberedTerrainFlags = pmap[i][j].rememberedTerrainFlags;
		//		levels[oldLevelNumber - 1].mapStorage[i][j].rememberedTMFlags = pmap[i][j].rememberedTMFlags;
		//		levels[oldLevelNumber - 1].mapStorage[i][j].machineNumber = pmap[i][j].machineNumber;
		//	}
		//}

		// NEXT*** The "awaySince" has to do with trying to accomplish two things:  1) allow
		//		   the "simmering" of the physical environment; and 2) to try and reconcile 
		//		   that somehow when the player returns. 
		//
		// NEW DESIGN:  (Sorry penderprime)... There's a little better way to handle this 
		//				smoothly; but the levels have to be processed in parallel while there
		//				are any active creatures. 
		//
		//				After they're dormant the level can be re-initialized using the same
		//				procedure as the first initialization. This will reset the environment
		//				and "re-simmer" it.
		//

		// levels[oldLevelNumber - 1].awaySince = rogue.absoluteTurnNumber;

		// NEXT***		Initialize character's light source. Might be a good idea to stick this
		//				light source on the creature, then process them all at once.
		//

		//	Prepare the new level
		//rogue.minersLightRadius = DCOLS - 1 << FP_BASE;
		//for (i = 0; i < rogue.depthLevel; i++) {
		//	rogue.minersLightRadius = rogue.minersLightRadius * 85 / 100;
		//}
		//rogue.minersLightRadius += (225 << FP_BASE) / 100;
		//updateColors();
		//updateRingBonuses(); // also updates miner's light

		// NEXT***		Initialize Level:  This procedure includes creating the level.
		//				So, we're going to pre-create these. Then, the scent map, and
		//				(above) all the creature safety maps are updated. So, that would
		//				be like a "tick". So, lets revisit these with each character map
		//				calculated when it's needed.
		//

		// level has not already been visited
		//if (!levels[rogue.depthLevel - 1].visited) 
		//{ 
		//	levels[rogue.depthLevel - 1].scentMap = allocGrid();
		//	scentMap = levels[rogue.depthLevel - 1].scentMap;
		//	fillGrid(levels[rogue.depthLevel - 1].scentMap, 0);
		//	// generate new level
		//	oldSeed = (unsigned long)rand_range(0, 9999);
		//	oldSeed += (unsigned long)10000 * rand_range(0, 9999);
		//	seedRandomGenerator(levels[rogue.depthLevel - 1].levelSeed);

		//	// Load up next level's monsters and items, since one might have fallen from above.
		//	monsters->nextCreature = levels[rogue.depthLevel - 1].monsters;
		//	dormantMonsters->nextCreature = levels[rogue.depthLevel - 1].dormantMonsters;
		//	floorItems->nextItem = levels[rogue.depthLevel - 1].items;

		//	levels[rogue.depthLevel - 1].monsters = NULL;
		//	levels[rogue.depthLevel - 1].dormantMonsters = NULL;
		//	levels[rogue.depthLevel - 1].items = NULL;

			// digDungeon();
			// initializeLevel();
			// setUpWaypoints();

			shuffleTerrainColors(100, false);

			// If we somehow failed to generate the amulet altar,
			// just toss an amulet in there somewhere.
			// It'll be fiiine!
			if (rogue.depthLevel == AMULET_LEVEL
				&& !numberOfMatchingPackItems(AMULET, 0, 0, false)
				&& levels[rogue.depthLevel - 1].visited == false) {

				for (theItem = floorItems->nextItem; theItem != NULL; theItem = theItem->nextItem) {
					if (theItem->category & AMULET) {
						break;
					}
				}
				for (monst = monsters->nextCreature; monst != NULL; monst = monst->nextCreature) {
					if (monst->carriedItem
						&& (monst->carriedItem->category & AMULET)) {

						theItem = monst->carriedItem;
						break;
					}
				}
				if (!theItem) {
					placeItem(generateItem(AMULET, 0), 0, 0);
				}
			}
			seedRandomGenerator(oldSeed);

			//logLevel();

			// Simulate 50 turns so the level is broken in (swamp gas accumulating, brimstone percolating, etc.).
			timeAway = 50;

		}
		else { // level has already been visited

		 // restore level
			scentMap = levels[rogue.depthLevel - 1].scentMap;
			timeAway = clamp(0, rogue.absoluteTurnNumber - levels[rogue.depthLevel - 1].awaySince, 30000);

			for (i = 0; i < DCOLS; i++) {
				for (j = 0; j < DROWS; j++) {
					for (layer = 0; layer < NUMBER_TERRAIN_LAYERS; layer++) {
						pmap[i][j].layers[layer] = levels[rogue.depthLevel - 1].mapStorage[i][j].layers[layer];
					}
					pmap[i][j].volume = levels[rogue.depthLevel - 1].mapStorage[i][j].volume;
					pmap[i][j].flags = (levels[rogue.depthLevel - 1].mapStorage[i][j].flags & PERMANENT_TILE_FLAGS);
					pmap[i][j].rememberedAppearance = levels[rogue.depthLevel - 1].mapStorage[i][j].rememberedAppearance;
					pmap[i][j].rememberedTerrain = levels[rogue.depthLevel - 1].mapStorage[i][j].rememberedTerrain;
					pmap[i][j].rememberedItemCategory = levels[rogue.depthLevel - 1].mapStorage[i][j].rememberedItemCategory;
					pmap[i][j].rememberedItemKind = levels[rogue.depthLevel - 1].mapStorage[i][j].rememberedItemKind;
					pmap[i][j].rememberedCellFlags = levels[rogue.depthLevel - 1].mapStorage[i][j].rememberedCellFlags;
					pmap[i][j].rememberedTerrainFlags = levels[rogue.depthLevel - 1].mapStorage[i][j].rememberedTerrainFlags;
					pmap[i][j].machineNumber = levels[rogue.depthLevel - 1].mapStorage[i][j].machineNumber;
				}
			}

			setUpWaypoints();

			rogue.downLoc[0] = levels[rogue.depthLevel - 1].downStairsLoc[0];
			rogue.downLoc[1] = levels[rogue.depthLevel - 1].downStairsLoc[1];
			rogue.upLoc[0] = levels[rogue.depthLevel - 1].upStairsLoc[0];
			rogue.upLoc[1] = levels[rogue.depthLevel - 1].upStairsLoc[1];

			monsters->nextCreature = levels[rogue.depthLevel - 1].monsters;
			dormantMonsters->nextCreature = levels[rogue.depthLevel - 1].dormantMonsters;
			floorItems->nextItem = levels[rogue.depthLevel - 1].items;

			levels[rogue.depthLevel - 1].monsters = NULL;
			levels[rogue.depthLevel - 1].dormantMonsters = NULL;
			levels[rogue.depthLevel - 1].items = NULL;

			for (theItem = floorItems->nextItem; theItem != NULL; theItem = theItem->nextItem) {
				restoreItem(theItem);
			}

			mapToStairs = allocGrid();
			mapToPit = allocGrid();
			fillGrid(mapToStairs, 0);
			fillGrid(mapToPit, 0);
			calculateDistances(mapToStairs, player.xLoc, player.yLoc, T_PATHING_BLOCKER, NULL, true, true);
			calculateDistances(mapToPit, levels[rogue.depthLevel - 1].playerExitedVia[0],
				levels[rogue.depthLevel - 1].playerExitedVia[0], T_PATHING_BLOCKER, NULL, true, true);
			for (monst = monsters->nextCreature; monst != NULL; monst = monst->nextCreature) {
				restoreMonster(monst, mapToStairs, mapToPit);
			}
			freeGrid(mapToStairs);
			freeGrid(mapToPit);
		}

		// Simulate the environment!
		// First bury the player in limbo while we run the simulation,
		// so that any harmful terrain doesn't affect her during the process.
		px = player.xLoc;
		py = player.yLoc;
		player.xLoc = player.yLoc = 0;
		for (i = 0; i < 100 && i < (short)timeAway; i++) {
			updateEnvironment();
		}
		player.xLoc = px;
		player.yLoc = py;

		if (!levels[rogue.depthLevel - 1].visited) {
			levels[rogue.depthLevel - 1].visited = true;
			if (rogue.depthLevel == AMULET_LEVEL) {
				messageWithColor("An alien energy permeates the area. The Amulet of Yendor must be nearby!", &itemMessageColor, false);
			}
			else if (rogue.depthLevel == DEEPEST_LEVEL) {
				messageWithColor("An overwhelming sense of peace and tranquility settles upon you.", &lightBlue, false);
			}
		}

		// Position the player.
		if (stairDirection == 0) { // fell into the level

			getQualifyingLocNear(loc, player.xLoc, player.yLoc, true, 0,
				(T_PATHING_BLOCKER),
				(HAS_MONSTER | HAS_ITEM | HAS_STAIRS | IS_IN_MACHINE), false, false);
		}
		else {
			if (stairDirection == 1) { // heading downward
				player.xLoc = rogue.upLoc[0];
				player.yLoc = rogue.upLoc[1];
			}
			else if (stairDirection == -1) { // heading upward
				player.xLoc = rogue.downLoc[0];
				player.yLoc = rogue.downLoc[1];
			}

			placedPlayer = false;
			for (dir = 0; dir < 4 && !placedPlayer; dir++) {
				loc[0] = player.xLoc + nbDirs[dir][0];
				loc[1] = player.yLoc + nbDirs[dir][1];
				if (!cellHasTerrainFlag(loc[0], loc[1], T_PATHING_BLOCKER)
					&& !(pmap[loc[0]][loc[1]].flags & (HAS_MONSTER | HAS_ITEM | HAS_STAIRS | IS_IN_MACHINE))) {
					placedPlayer = true;
				}
			}
			if (!placedPlayer) {
				getQualifyingPathLocNear(&loc[0], &loc[1],
					player.xLoc, player.yLoc,
					true,
					T_DIVIDES_LEVEL, NULL,
					T_PATHING_BLOCKER, (HAS_MONSTER | HAS_ITEM | HAS_STAIRS | IS_IN_MACHINE),
					false);
			}
		}
		player.xLoc = loc[0];
		player.yLoc = loc[1];

		pmap[player.xLoc][player.yLoc].flags |= HAS_PLAYER;

		if (connectingStairsDiscovered) {
			for (i = rogue.upLoc[0] - 1; i <= rogue.upLoc[0] + 1; i++) {
				for (j = rogue.upLoc[1] - 1; j <= rogue.upLoc[1] + 1; j++) {
					if (coordinatesAreInMap(i, j)) {
						discoverCell(i, j);
					}
				}
			}
		}
		if (cellHasTerrainFlag(player.xLoc, player.yLoc, T_IS_DEEP_WATER) && !player.status[STATUS_LEVITATING]
			&& !cellHasTerrainFlag(player.xLoc, player.yLoc, (T_ENTANGLES | T_OBSTRUCTS_PASSABILITY))) {
			rogue.inWater = true;
		}

		updateMapToShore();
		updateVision(true);
		rogue.aggroRange = currentAggroValue();

		// update monster states so none are hunting if there is no scent and they can't see the player
		for (monst = monsters->nextCreature; monst != NULL; monst = monst->nextCreature) {
			updateMonsterState(monst);
		}

		rogue.playbackBetweenTurns = true;
		displayLevel();
		refreshSideBar(-1, -1, false);

		if (rogue.playerTurnNumber) {
			rogue.playerTurnNumber++; // Increment even though no time has passed.
		}
		RNGCheck();
		flushBufferToFile();
		deleteAllFlares(); // So discovering something on the same turn that you fall down a level doesn't flash stuff on the previous level.
		hideCursor();
	}

	//// Returns a color for combat text based on the identity of the victim.
	//color levelProcessor::messageColorFromVictim(creature* monst) 
	//{
	//	if (monst == _player) {
	//		return badMessageColor;
	//	}
	//	else if (_player->status[STATUS_HALLUCINATING] && !rogue.playbackOmniscience) {
	//		return whiteBrogue;
	//	}
	//	else if (monst->creatureState == MONSTER_ALLY) {
	//		return &badMessageColor;
	//	}
	//	else if (monstersAreEnemies(&_player, monst)) {
	//		return &goodMessageColor;
	//	}
	//	else {
	//		return &whiteBrogue;
	//	}
	//}
}