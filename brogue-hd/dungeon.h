#pragma once

#include "simple.h"
#include "dungeonConstants.h"

using namespace brogueHd::simple;
using namespace brogueHd::backend::modelConstant;

namespace brogueHd::backend::model::game
{

	/*

	dungeonProfile dungeonProfileCatalog[NUMBER_DUNGEON_PROFILES] = {
		// Room frequencies:
		//      0. Cross room
		//      1. Small symmetrical cross room
		//      2. Small room
		//      3. Circular room
		//      4. Chunky room
		//      5. Cave
		//      6. Cavern (the kind that fills a level)
		//      7. Entrance room (the big upside-down T room at the start of depth 1)

		// Room frequencies
		// 0    1   2   3   4   5   6   7   Corridor chance
		{{2,    1,  1,  1,  7,  1,  0,  0}, 10},    // Basic dungeon generation (further adjusted by depth)
		{{10,   0,  0,  3,  7,  10, 10, 0}, 0},     // First room for basic dungeon generation (further adjusted by depth)

		{{0,    0,  1,  0,  0,  0,  0,  0}, 0},     // Goblin warrens
		{{0,    5,  0,  1,  0,  0,  0,  0}, 0},     // Sentinel sanctuaries
	};

	*/

	struct brogueRoomInfo : hashable
	{
		roomTypes type;
		short frequency;
		short corridorChance;

		brogueRoomInfo()
		{}
		brogueRoomInfo(roomTypes atype, short afrequency, short acorridorChance)
		{
			type = atype;
			frequency = afrequency;
			corridorChance = acorridorChance;
		}

		bool operator==(const brogueRoomInfo& info)
		{
			return type == info.type &&
				   frequency == info.frequency &&
				   corridorChance == info.corridorChance;
		}
		bool operator!=(const brogueRoomInfo& info)
		{
			return type != info.type ||
				   frequency != info.frequency ||
				   corridorChance != info.corridorChance;
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(type, frequency, corridorChance);
		}
	};
}