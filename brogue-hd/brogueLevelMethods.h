#pragma once

#include "brogueCell.h"
#include "brogueLevel.h"
#include "dungeonConstants.h"
#include "terrainDeclaration.h"

using namespace brogueHd::backend::model;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::methods
{

	static bool stairsUpDiscovered(brogueLevel* level)
	{
		//(pmap[rogue.downLoc[0]][rogue.downLoc[1]].flags & (DISCOVERED | MAGIC_MAPPED) ? true : false);
		//if (stairDirection == 0) { // fallen
		//	levels[oldLevelNumber - 1].playerExitedVia[0] = player.xLoc;
		//	levels[oldLevelNumber - 1].playerExitedVia[1] = player.yLoc;
		//}
		return false;
	}

	static bool stairsDownDiscovered(brogueLevel* level)
	{
		//(pmap[rogue.downLoc[0]][rogue.downLoc[1]].flags & (DISCOVERED | MAGIC_MAPPED) ? true : false);
		//if (stairDirection == 0) { // fallen
		//	levels[oldLevelNumber - 1].playerExitedVia[0] = player.xLoc;
		//	levels[oldLevelNumber - 1].playerExitedVia[1] = player.yLoc;
		//}
		return false;
	}

	//#define terrainFlags(x, y)					(tileCatalog[pmap[x][y].layers[DUNGEON]].flags \
	//											| tileCatalog[pmap[x][y].layers[LIQUID]].flags \
	//											| tileCatalog[pmap[x][y].layers[SURFACE]].flags \
	//											| tileCatalog[pmap[x][y].layers[GAS]].flags)
	//
	//#define terrainMechFlags(x, y)				(tileCatalog[pmap[x][y].layers[DUNGEON]].mechFlags \
	//											| tileCatalog[pmap[x][y].layers[LIQUID]].mechFlags \
	//											| tileCatalog[pmap[x][y].layers[SURFACE]].mechFlags \
	//											| tileCatalog[pmap[x][y].layers[GAS]].mechFlags)


		//#define cellHasTerrainFlag(x, y, flagMask)	((flagMask) & terrainFlags((x), (y)) ? true : false)
	static bool isTerrain(brogueLevel* level, short column, short row, terrainFlagCatalog terrainFlag)
	{
		//brogueCell* cell = level->levelGrid->getCell(column, row);

		// Need to move physical data from the display struct
		//
		//for (int index = 0; index < SIZEOF(cell->display.layers); index++)
		//{
		//	if ((cell->display.layers[index] & terrainFlag) != 0)
		//		return true;
		//}

		return false;
	}

	static bool isTerrain(brogueCell* cell, terrainFlagCatalog terrainFlag)
	{
		// Need to move physical data from the display struct
		//
		//for (int index = 0; index < SIZEOF(cell->display.layers); index++)
		//{
		//	if ((cell->display.layers[index] & terrainFlag) != 0)
		//		return true;
		//}

		return false;
	}

	static bool isPathingBlocker(brogueLevel* level, short column, short row)
	{
		//brogueCell* cell = level->levelGrid->getCell(column, row);

		// Need to move physical data from the display struct
		//
		//for (int index = 0; index < SIZEOF(cell->display.layers); index++)
		//{
		//	if ((cell->display.layers[index] & T_PATHING_BLOCKER) != 0)
		//		return true;
		//}

		return false;
	}

	static bool isAutoDescent(brogueLevel* level, short column, short row)
	{
		//brogueCell* cell = level->levelGrid->getCell(column, row);

		// Need to move physical data from the display struct
		//
		//for (int index = 0; index < SIZEOF(cell->display.layers); index++)
		//{
		//	if ((cell->display.layers[index] & T_AUTO_DESCENT) != 0)
		//		return true;
		//}

		return false;
	}

	static bool terrainLayersContains(const tileType* layers, terrainFlagCatalog flagMask)
	{
		//for (int index = 0; index < SIZEOF(layers); index++)
		//{
		//	if ((layers[index] & T_AUTO_DESCENT) != 0)
		//		return true;
		//}

		return false;
	}

	static brogueCell* findAdjacentWalkableLocation(brogueLevel* level, short column, short row)
	{
		//return level->levelGrid->checkAdjacentCells(column, row, [](brogueCell* cell)
		//{
		//	return  terrainLayersContains(cell->display.layers, T_AUTO_DESCENT) ||
		//			terrainLayersContains(cell->display.layers, T_PATHING_BLOCKER);
		//});

		return NULL;
	}

	static brogueCell* findValidStairLocation(brogueLevel* level)
	{
		//short newX, newY, dir, neighborWallCount;

		// TODO: Fix this using walkable layer

		//if (x < 1 || x >= DCOLS - 1 || y < 1 || y >= DROWS - 1 || pmap[x][y].layers[DUNGEON] != WALL) {
		//	return false;
		//}

		// TODO: Fix this using regions w/ walkable layer. Add edge locations.

		//for (dir = 0; dir < DIRECTION_COUNT; dir++)
		//{
		//	newX = x + nbDirs[dir][0];
		//	newY = y + nbDirs[dir][1];
		//	if (pmap[newX][newY].flags & IS_IN_MACHINE)
		//	{
		//		return false;
		//	}
		//}

		//neighborWallCount = 0;

		//for (dir = 0; dir < 4; dir++) 
		//{
		//	newX = x + nbDirs[dir][0];
		//	newY = y + nbDirs[dir][1];

		//	if (cellHasTerrainFlag(newX, newY, T_OBSTRUCTS_PASSABILITY)) {
		//		// neighbor is a wall
		//		neighborWallCount++;
		//	}
		//	else {
		//		// neighbor is not a wall
		//		if (cellHasTerrainFlag(newX, newY, T_PATHING_BLOCKER)
		//			|| passableArcCount(newX, newY) >= 2) {
		//			return false;
		//		}
		//		// now check the two diagonals between the walls

		//		newX = x - nbDirs[dir][0] + nbDirs[dir][1];
		//		newY = y - nbDirs[dir][1] + nbDirs[dir][0];
		//		if (!cellHasTerrainFlag(newX, newY, T_OBSTRUCTS_PASSABILITY)) {
		//			return false;
		//		}

		//		newX = x - nbDirs[dir][0] - nbDirs[dir][1];
		//		newY = y - nbDirs[dir][1] - nbDirs[dir][0];
		//		if (!cellHasTerrainFlag(newX, newY, T_OBSTRUCTS_PASSABILITY)) {
		//			return false;
		//		}
		//	}
		//}
		//if (neighborWallCount != 3) {
		//	return false;
		//}
		//return true;

		return NULL;
	}
}