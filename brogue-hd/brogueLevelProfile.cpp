#pragma once

#include "brogueLevelProfile.h"
#include "simpleList.h"
#include "brogueMath.h"
#include "brogueGlobal.h"

using namespace brogueHd::component;
using namespace brogueHd::component::math;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model
{
/*  Brogue v1.7.5

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

*/

	brogueLevelProfile::brogueLevelProfile(const randomGenerator* randomGenerator, short depth, levelTypes type)
	{
		if (depth == 1 && type != levelTypes::DP_BASIC_FIRST_ROOM)
			brogueException::show("Invalid first level type:  brogueLevelProfile.cpp");

		_roomInfo = new simpleHash<roomTypes, brogueRoomInfo>();
		_levelType = type;
		_depth = depth;

		simpleHash<roomTypes, short>* basicMap = new simpleHash<roomTypes, short>();
		simpleHash<roomTypes, short>* basicFirstRoomMap = new simpleHash<roomTypes, short>();
		simpleHash<roomTypes, short>* goblinWarrenMap = new simpleHash<roomTypes, short>();
		simpleHash<roomTypes, short>* sentinelSanctuariesMap = new simpleHash<roomTypes, short>();

		// BROGUE v1.7.5
		basicMap->add(roomTypes::CrossRoom, 2);
		basicMap->add(roomTypes::SmallSymmetricalCrossRoom, 1);
		basicMap->add(roomTypes::SmallRoom, 1);
		basicMap->add(roomTypes::CircularRoom, 1);
		basicMap->add(roomTypes::ChunkyRoom, 7);
		basicMap->add(roomTypes::Cave, 1);
		basicMap->add(roomTypes::Cavern, 0);
		basicMap->add(roomTypes::MainEntranceRoom, 0);

		basicFirstRoomMap->add(roomTypes::CrossRoom, 10);
		basicFirstRoomMap->add(roomTypes::SmallSymmetricalCrossRoom, 0);
		basicFirstRoomMap->add(roomTypes::SmallRoom, 0);
		basicFirstRoomMap->add(roomTypes::CircularRoom, 3);
		basicFirstRoomMap->add(roomTypes::ChunkyRoom, 7);
		basicFirstRoomMap->add(roomTypes::Cave, 10);
		basicFirstRoomMap->add(roomTypes::Cavern, 10);
		basicFirstRoomMap->add(roomTypes::MainEntranceRoom, 0);

		goblinWarrenMap->add(roomTypes::CrossRoom, 0);
		goblinWarrenMap->add(roomTypes::SmallSymmetricalCrossRoom, 0);
		goblinWarrenMap->add(roomTypes::SmallRoom, 1);
		goblinWarrenMap->add(roomTypes::CircularRoom, 0);
		goblinWarrenMap->add(roomTypes::ChunkyRoom, 0);
		goblinWarrenMap->add(roomTypes::Cave, 0);
		goblinWarrenMap->add(roomTypes::Cavern, 0);
		goblinWarrenMap->add(roomTypes::MainEntranceRoom, 0);

		sentinelSanctuariesMap->add(roomTypes::CrossRoom, 0);
		sentinelSanctuariesMap->add(roomTypes::SmallSymmetricalCrossRoom, 5);
		sentinelSanctuariesMap->add(roomTypes::SmallRoom, 0);
		sentinelSanctuariesMap->add(roomTypes::CircularRoom, 1);
		sentinelSanctuariesMap->add(roomTypes::ChunkyRoom, 0);
		sentinelSanctuariesMap->add(roomTypes::Cave, 0);
		sentinelSanctuariesMap->add(roomTypes::Cavern, 0);
		sentinelSanctuariesMap->add(roomTypes::MainEntranceRoom, 0);

		_roomFrequencyMap->add(levelTypes::DP_BASIC, basicMap);
		_roomFrequencyMap->add(levelTypes::DP_BASIC_FIRST_ROOM, basicFirstRoomMap);
		_roomFrequencyMap->add(levelTypes::DP_GOBLIN_WARREN, goblinWarrenMap);
		_roomFrequencyMap->add(levelTypes::DP_SENTINEL_SANCTUARY, sentinelSanctuariesMap);

		_corridorFrequencyMap->add(levelTypes::DP_BASIC, 10);
		_corridorFrequencyMap->add(levelTypes::DP_BASIC_FIRST_ROOM, 0);
		_corridorFrequencyMap->add(levelTypes::DP_GOBLIN_WARREN, 0);
		_corridorFrequencyMap->add(levelTypes::DP_SENTINEL_SANCTUARY, 0);

	}

	brogueLevelProfile::~brogueLevelProfile()
	{
		//delete _roomInfo;
	}
	
	void brogueLevelProfile::initialize()
	{
		// Set up room info's
		_roomInfo->add(roomTypes::CrossRoom, createRoomInfo(roomTypes::CrossRoom));
		_roomInfo->add(roomTypes::CrossRoom, createRoomInfo(roomTypes::SmallSymmetricalCrossRoom));
		_roomInfo->add(roomTypes::CrossRoom, createRoomInfo(roomTypes::SmallRoom));
		_roomInfo->add(roomTypes::CrossRoom, createRoomInfo(roomTypes::CircularRoom));
		_roomInfo->add(roomTypes::CrossRoom, createRoomInfo(roomTypes::ChunkyRoom));
		_roomInfo->add(roomTypes::CrossRoom, createRoomInfo(roomTypes::Cave));
		_roomInfo->add(roomTypes::CrossRoom, createRoomInfo(roomTypes::Cavern));
		_roomInfo->add(roomTypes::CrossRoom, createRoomInfo(roomTypes::MainEntranceRoom));
	}

	brogueRoomInfo brogueLevelProfile::createRoomInfo(roomTypes roomType)
	{
		if (_depth == 1)
		{
			if (roomType == roomTypes::MainEntranceRoom)
				return brogueRoomInfo(roomType, 1, _corridorFrequencyMap->get(levelTypes::DP_BASIC_FIRST_ROOM));
		}

		// ADJUST FOR DEPTH (Brogue v1.7.5)
		const short descentPercent = brogueMath<short>::clamp(100 * (_depth - 1) / (AMULET_LEVEL - 1), 0, 100);

		// Get static data to adjust
		short roomFrequency = _roomFrequencyMap->get(_levelType)->get(roomType);
		short corridorFrequency = _corridorFrequencyMap->get(_levelType);

		// Additional adjustment for first room
		switch (roomType)
		{
		case roomTypes::CrossRoom:
			roomFrequency += 20 * (100 - descentPercent) / 100;
			break;
		case roomTypes::SmallSymmetricalCrossRoom:
			roomFrequency += 10 * (100 - descentPercent) / 100;
			break;
		case roomTypes::SmallRoom:
			break;
		case roomTypes::CircularRoom:
			roomFrequency += 7 * (100 - descentPercent) / 100;
			break;
		case roomTypes::ChunkyRoom:
			break;
		case roomTypes::Cave:
			roomFrequency += 10 * (100 - descentPercent) / 100;
			break;
		case roomTypes::Cavern:
			roomFrequency += 50 * descentPercent / 100;
			break;
		case roomTypes::MainEntranceRoom:
			break;
		default:
			break;
		}

		corridorFrequency += 80 * (100 - descentPercent) / 100;

		return brogueRoomInfo(roomType, roomFrequency, corridorFrequency);
	}

	brogueRoomInfo brogueLevelProfile::getEntranceRoom()
	{
		//return _roomInfo->get(roomTypes::MainEntranceRoom);
		return brogueRoomInfo();
	}

	brogueRoomInfo brogueLevelProfile::getRandomRoomInfo()
	{
		simpleList<short> weights = _roomInfo->selectFromValues<short>([](brogueRoomInfo info)
		{
			return info.frequency;
		});

		short randomIndex = _randomGenerator->randWeighted(weights.getArray());

		return _roomInfo->getAt(randomIndex).value;

		return brogueRoomInfo();
	}
}