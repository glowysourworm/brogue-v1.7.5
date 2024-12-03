#pragma once

#include "brogueCell.h"
#include "brogueContentGrid.h"
#include "brogueGlobal.h"
#include "brogueLayout.h"
#include "creature.h"
#include "grid.h"
#include "gridRect.h"
#include "item.h"

namespace brogueHd::component
{
	class contentGenerator
	{
	public:
		contentGenerator(const gridRect& levelBoundary)
		{
			_validLocations = new grid<bool>(levelBoundary, levelBoundary);
		}

		~contentGenerator()
		{
			delete _validLocations;
		}

		brogueContentGrid* generateLevelContents(brogueLayout* layout);

	private:
		void placeStairs();

	private:
		grid<bool>* _validLocations;
	};

	brogueContentGrid* contentGenerator::generateLevelContents(brogueLayout* layout)
	{
		int i, j, dir;
		int upLoc[2], downLoc[2], **mapToStairs, **mapToPit;
		creatureInfo* monst;
		itemBase* theItem;
		char grid[DCOLS][DROWS];

		return nullptr;
		//int n = rogue.depthLevel - 1;

		// Procedure:
		//
		// 1) Find valid stair locations
		// 2) Make sure there is a valid location for the player (at least one tile)
		// 3) 

		//for (i = 0; i < DCOLS; i++) {
		//	for (j = 0; j < DROWS; j++) {
		//		grid[i][j] = validStairLoc(i, j);
		//	}
		//}

		//if (D_INSPECT_LEVELGEN) {
		//	//dumpLevelToScreen();
		//	//hiliteCharGrid(grid, &teal, 100);
		//	//temporaryMessage("Stair location candidates:", true);
		//}

		// NEXT*** Place stairs. Qualifying locations will be queried using map layers (not ready)
		//

		brogueCell* stairsLocation;
		brogueCell* torchLocation1;
		brogueCell* torchLocation2;

		//placeStairs(stairsLocation, torchLocation1, torchLocation2);

		// NEXT*** Handle qualifying locations!!! 

		////if (getQualifyingGridLocNear(downLoc, levels[n].downStairsLoc[0], levels[n].downStairsLoc[1], grid, false)) {
		////	prepareForStairs(downLoc[0], downLoc[1], grid);
		////}
		////else 
		////{
		////	getQualifyingLocNear(downLoc, levels[n].downStairsLoc[0], levels[n].downStairsLoc[1], false, 0,
		////		(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_AUTO_DESCENT | T_IS_DEEP_WATER | T_LAVA_INSTA_DEATH | T_IS_DF_TRAP),
		////		(HAS_MONSTER | HAS_ITEM | HAS_STAIRS | IS_IN_MACHINE), true, false);
		////}

		//if (rogue.depthLevel == DEEPEST_LEVEL) {
		//	pmap[downLoc[0]][downLoc[1]].layers[DUNGEON] = DUNGEON_PORTAL;
		//}
		//else {
		//	pmap[downLoc[0]][downLoc[1]].layers[DUNGEON] = DOWN_STAIRS;
		//}
		//pmap[downLoc[0]][downLoc[1]].layers[LIQUID] = NOTHING;
		//pmap[downLoc[0]][downLoc[1]].layers[SURFACE] = NOTHING;

		//if (!levels[n + 1].visited) {
		//	levels[n + 1].upStairsLoc[0] = downLoc[0];
		//	levels[n + 1].upStairsLoc[1] = downLoc[1];
		//}
		//levels[n].downStairsLoc[0] = downLoc[0];
		//levels[n].downStairsLoc[1] = downLoc[1];

		//if (getQualifyingGridLocNear(upLoc, levels[n].upStairsLoc[0], levels[n].upStairsLoc[1], grid, false)) {
		//	prepareForStairs(upLoc[0], upLoc[1], grid);
		//}
		//else { // Hopefully this never happens.
		//	getQualifyingLocNear(upLoc, levels[n].upStairsLoc[0], levels[n].upStairsLoc[1], false, 0,
		//		(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_AUTO_DESCENT | T_IS_DEEP_WATER | T_LAVA_INSTA_DEATH | T_IS_DF_TRAP),
		//		(HAS_MONSTER | HAS_ITEM | HAS_STAIRS | IS_IN_MACHINE), true, false);
		//}

		//levels[n].upStairsLoc[0] = upLoc[0];
		//levels[n].upStairsLoc[1] = upLoc[1];

		//if (rogue.depthLevel == 1) {
		//	pmap[upLoc[0]][upLoc[1]].layers[DUNGEON] = DUNGEON_EXIT;
		//}
		//else {
		//	pmap[upLoc[0]][upLoc[1]].layers[DUNGEON] = UP_STAIRS;
		//}
		//pmap[upLoc[0]][upLoc[1]].layers[LIQUID] = NOTHING;
		//pmap[upLoc[0]][upLoc[1]].layers[SURFACE] = NOTHING;

		//rogue.downLoc[0] = downLoc[0];
		//rogue.downLoc[1] = downLoc[1];
		//pmap[downLoc[0]][downLoc[1]].flags |= HAS_STAIRS;
		//rogue.upLoc[0] = upLoc[0];
		//rogue.upLoc[1] = upLoc[1];
		//pmap[upLoc[0]][upLoc[1]].flags |= HAS_STAIRS;

		//if (!levels[rogue.depthLevel - 1].visited) {

		//	// Run a field of view check from up stairs so that monsters do not spawn within sight of it.
		//	for (dir = 0; dir < 4; dir++) {
		//		if (coordinatesAreInMap(upLoc[0] + nbDirs[dir][0], upLoc[1] + nbDirs[dir][1])
		//			&& !cellHasTerrainFlag(upLoc[0] + nbDirs[dir][0], upLoc[1] + nbDirs[dir][1], T_OBSTRUCTS_PASSABILITY)) {

		//			upLoc[0] += nbDirs[dir][0];
		//			upLoc[1] += nbDirs[dir][1];
		//			break;
		//		}
		//	}
		//	zeroOutGrid(grid);
		//	getFOVMask(grid, upLoc[0], upLoc[1], max(DCOLS, DROWS) << FP_BASE, (T_OBSTRUCTS_VISION), 0, false);
		//	for (i = 0; i < DCOLS; i++) {
		//		for (j = 0; j < DROWS; j++) {
		//			if (grid[i][j]) {
		//				pmap[i][j].flags |= IN_FIELD_OF_VIEW;
		//			}
		//		}
		//	}
		//	populateItems(upLoc[0], upLoc[1]);
		//	populateMonsters();
		//}

		//// Restore items that fell from the previous depth.
		//for (theItem = floorItems->nextItem; theItem != NULL; theItem = theItem->nextItem) {
		//	restoreItem(theItem);
		//}

		//// Restore creatures that fell from the previous depth or that have been pathing toward the stairs.
		//mapToStairs = allocGrid();
		//fillGrid(mapToStairs, 0);
		//mapToPit = allocGrid();
		//fillGrid(mapToPit, 0);
		//calculateDistances(mapToStairs, player.xLoc, player.yLoc, T_PATHING_BLOCKER, NULL, true, true);
		//calculateDistances(mapToPit,
		//	levels[rogue.depthLevel - 1].playerExitedVia[0],
		//	levels[rogue.depthLevel - 1].playerExitedVia[1],
		//	T_PATHING_BLOCKER,
		//	NULL,
		//	true,
		//	true);
		//for (monst = monsters->nextCreature; monst != NULL; monst = monst->nextCreature) {
		//	restoreMonster(monst, mapToStairs, mapToPit);
		//}
		//freeGrid(mapToStairs);
		//freeGrid(mapToPit);
	}

	void contentGenerator::placeStairs()
	{
		//int newX, newY, dir;

		brogueCell* stairsLocation;
		brogueCell* torchLocation1;
		brogueCell* torchLocation2;

		// Number of cells to clear around stairs
		int stairsClearance = 5;

		// Procedure
		// 
		// 1) Find valid location for stairwells
		//		-> Leaving 5 steps of clearance in all directions
		//		-> Nested in the wall, with two torches surrounding it
		//		-> Not on the corner of a wall 
		//		-> Is set to dungeon portal on the deepest depth
		//		-> Contains no other obstructions
		//		-> (Level 1) On the south wall of the centered-bottom room
		//
		// 2) Clear the LIQUID and SURFACE layers for each of the stair 
		//	  locations (which should not be needed once we query them from 
		//	  the CLEAR layer.
		//

		//torchLocation1->display.layers[DUNGEON] = TORCH_WALL;
		//torchLocation2->display.layers[DUNGEON] = TORCH_WALL;

		// NOTE:  Keep old code until we're ready.

		//// Add torches to either side.
		//for (dir = 0; dir < 4; dir++) {
		//	if (!cellHasTerrainFlag(x + nbDirs[dir][0], y + nbDirs[dir][1], T_OBSTRUCTS_PASSABILITY)) {
		//		newX = x - nbDirs[dir][1];
		//		newY = y - nbDirs[dir][0];
		//		pmap[newX][newY].layers[DUNGEON] = TORCH_WALL;
		//		newX = x + nbDirs[dir][1];
		//		newY = y + nbDirs[dir][0];
		//		pmap[newX][newY].layers[DUNGEON] = TORCH_WALL;
		//		break;
		//	}
		//}

		//_level->levelGrid->iterateAdjacentCells(stairsLocation->xLocation, 
		//										stairsLocation->yLocation, 
		//										[](int column, int row, brogueCell* cell)
		//{
		//	if (cell->display.layers[DUNGEON] == GRANITE)
		//		cell->display.layers[DUNGEON] = WALL;

		//	if (isTerrain(cell, T_OBSTRUCTS_PASSABILITY))
		//		cell->display.flags |= IMPREGNABLE;
		//});

		// Expose granite.
		//for (dir = 0; dir < DIRECTION_COUNT; dir++) {
		//	newX = x + nbDirs[dir][0];
		//	newY = y + nbDirs[dir][1];
		//	if (pmap[newX][newY].layers[DUNGEON] == GRANITE) {
		//		pmap[newX][newY].layers[DUNGEON] = WALL;
		//	}
		//	if (cellHasTerrainFlag(newX, newY, T_OBSTRUCTS_PASSABILITY)) {
		//		pmap[newX][newY].flags |= IMPREGNABLE;
		//	}
		//}

		//iterateOutward(_validLocations, 
		//			   stairsLocation->xLocation, 
		//			   stairsLocation->yLocation, 
		//			   stairsClearance, 
		//			   [](int column, int row)
		//{
		//	_validLocations[column, row] = false;

		//	return iterationCallback::iterate;
		//});

		// Zero out grid in the vicinity.
		//for (newX = max(0, x - 5); newX < min(DCOLS, x + 5); newX++) {
		//	for (newY = max(0, y - 5); newY < min(DROWS, y + 5); newY++) {
		//		grid[newX][newY] = false;
		//	}
		//}

		//if (getQualifyingGridLocNear(downLoc, levels[n].downStairsLoc[0], levels[n].downStairsLoc[1], grid, false)) {
		//	prepareForStairs(downLoc[0], downLoc[1], grid);
		//}
		//else 
		//{
		//	getQualifyingLocNear(downLoc, levels[n].downStairsLoc[0], levels[n].downStairsLoc[1], false, 0,
		//		(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_AUTO_DESCENT | T_IS_DEEP_WATER | T_LAVA_INSTA_DEATH | T_IS_DF_TRAP),
		//		(HAS_MONSTER | HAS_ITEM | HAS_STAIRS | IS_IN_MACHINE), true, false);
		//}

		//if (rogue.depthLevel == DEEPEST_LEVEL) {
		//	pmap[downLoc[0]][downLoc[1]].layers[DUNGEON] = DUNGEON_PORTAL;
		//}
		//else {
		//	pmap[downLoc[0]][downLoc[1]].layers[DUNGEON] = DOWN_STAIRS;
		//}
		//pmap[downLoc[0]][downLoc[1]].layers[LIQUID] = NOTHING;
		//pmap[downLoc[0]][downLoc[1]].layers[SURFACE] = NOTHING;

		//if (!levels[n + 1].visited) {
		//	levels[n + 1].upStairsLoc[0] = downLoc[0];
		//	levels[n + 1].upStairsLoc[1] = downLoc[1];
		//}
		//levels[n].downStairsLoc[0] = downLoc[0];
		//levels[n].downStairsLoc[1] = downLoc[1];

		//if (getQualifyingGridLocNear(upLoc, levels[n].upStairsLoc[0], levels[n].upStairsLoc[1], grid, false)) {
		//	prepareForStairs(upLoc[0], upLoc[1], grid);
		//}
		//else { // Hopefully this never happens.
		//	getQualifyingLocNear(upLoc, levels[n].upStairsLoc[0], levels[n].upStairsLoc[1], false, 0,
		//		(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_AUTO_DESCENT | T_IS_DEEP_WATER | T_LAVA_INSTA_DEATH | T_IS_DF_TRAP),
		//		(HAS_MONSTER | HAS_ITEM | HAS_STAIRS | IS_IN_MACHINE), true, false);
		//}

		//levels[n].upStairsLoc[0] = upLoc[0];
		//levels[n].upStairsLoc[1] = upLoc[1];

		//if (rogue.depthLevel == 1) {
		//	pmap[upLoc[0]][upLoc[1]].layers[DUNGEON] = DUNGEON_EXIT;
		//}
		//else {
		//	pmap[upLoc[0]][upLoc[1]].layers[DUNGEON] = UP_STAIRS;
		//}
		//pmap[upLoc[0]][upLoc[1]].layers[LIQUID] = NOTHING;
		//pmap[upLoc[0]][upLoc[1]].layers[SURFACE] = NOTHING;

		//rogue.downLoc[0] = downLoc[0];
		//rogue.downLoc[1] = downLoc[1];
		//pmap[downLoc[0]][downLoc[1]].flags |= HAS_STAIRS;
		//rogue.upLoc[0] = upLoc[0];
		//rogue.upLoc[1] = upLoc[1];
		//pmap[upLoc[0]][upLoc[1]].flags |= HAS_STAIRS;
	}
}
