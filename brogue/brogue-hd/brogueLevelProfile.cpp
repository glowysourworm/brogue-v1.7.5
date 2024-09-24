#pragma once

#include "brogueLevelProfile.h"
#include "brogueMath.h"
#include "exceptionHandler.h"
#include "mapExtension.h"
#include <vector>

using namespace std;

using namespace brogueHd::backend::math;
using namespace brogueHd::backend::model::game;
using namespace brogueHd::backend::extension;

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

		_roomInfo = new std::map<roomTypes, brogueRoomInfo>();
		_levelType = type;
		_depth = depth;

		std::map<roomTypes, short> basicMap;
		std::map<roomTypes, short> basicFirstRoomMap;
		std::map<roomTypes, short> goblinWarrenMap;
		std::map<roomTypes, short> sentinelSanctuariesMap;

		// BROGUE v1.7.5
		basicMap[roomTypes::CrossRoom] = 2;
		basicMap[roomTypes::SmallSymmetricalCrossRoom] = 1;
		basicMap[roomTypes::SmallRoom] = 1;
		basicMap[roomTypes::CircularRoom] = 1;
		basicMap[roomTypes::ChunkyRoom] = 7;
		basicMap[roomTypes::Cave] = 1;
		basicMap[roomTypes::Cavern] = 0;
		basicMap[roomTypes::MainEntranceRoom] = 0;

		basicFirstRoomMap[roomTypes::CrossRoom] = 10;
		basicFirstRoomMap[roomTypes::SmallSymmetricalCrossRoom] = 0;
		basicFirstRoomMap[roomTypes::SmallRoom] = 0;
		basicFirstRoomMap[roomTypes::CircularRoom] = 3;
		basicFirstRoomMap[roomTypes::ChunkyRoom] = 7;
		basicFirstRoomMap[roomTypes::Cave] = 10;
		basicFirstRoomMap[roomTypes::Cavern] = 10;
		basicFirstRoomMap[roomTypes::MainEntranceRoom] = 0;

		goblinWarrenMap[roomTypes::CrossRoom] = 0;
		goblinWarrenMap[roomTypes::SmallSymmetricalCrossRoom] = 0;
		goblinWarrenMap[roomTypes::SmallRoom] = 1;
		goblinWarrenMap[roomTypes::CircularRoom] = 0;
		goblinWarrenMap[roomTypes::ChunkyRoom] = 0;
		goblinWarrenMap[roomTypes::Cave] = 0;
		goblinWarrenMap[roomTypes::Cavern] = 0;
		goblinWarrenMap[roomTypes::MainEntranceRoom] = 0;

		sentinelSanctuariesMap[roomTypes::CrossRoom] = 0;
		sentinelSanctuariesMap[roomTypes::SmallSymmetricalCrossRoom] = 5;
		sentinelSanctuariesMap[roomTypes::SmallRoom] = 0;
		sentinelSanctuariesMap[roomTypes::CircularRoom] = 1;
		sentinelSanctuariesMap[roomTypes::ChunkyRoom] = 0;
		sentinelSanctuariesMap[roomTypes::Cave] = 0;
		sentinelSanctuariesMap[roomTypes::Cavern] = 0;
		sentinelSanctuariesMap[roomTypes::MainEntranceRoom] = 0;

		_roomFrequencyMap[levelTypes::DP_BASIC] = basicMap;
		_roomFrequencyMap[levelTypes::DP_BASIC_FIRST_ROOM] = basicFirstRoomMap;
		_roomFrequencyMap[levelTypes::DP_GOBLIN_WARREN] = goblinWarrenMap;
		_roomFrequencyMap[levelTypes::DP_SENTINEL_SANCTUARY] = sentinelSanctuariesMap;

		_corridorFrequencyMap[levelTypes::DP_BASIC] = 10;
		_corridorFrequencyMap[levelTypes::DP_BASIC_FIRST_ROOM] = 0;
		_corridorFrequencyMap[levelTypes::DP_GOBLIN_WARREN] = 0;
		_corridorFrequencyMap[levelTypes::DP_SENTINEL_SANCTUARY] = 0;
	}

	brogueLevelProfile::~brogueLevelProfile()
	{
		delete _roomInfo;
	}
	
	void brogueLevelProfile::initialize()
	{
		// Set up room info's
		_roomInfo->insert_or_assign(roomTypes::CrossRoom, createRoomInfo(roomTypes::CrossRoom));
		_roomInfo->insert_or_assign(roomTypes::CrossRoom, createRoomInfo(roomTypes::SmallSymmetricalCrossRoom));
		_roomInfo->insert_or_assign(roomTypes::CrossRoom, createRoomInfo(roomTypes::SmallRoom));
		_roomInfo->insert_or_assign(roomTypes::CrossRoom, createRoomInfo(roomTypes::CircularRoom));
		_roomInfo->insert_or_assign(roomTypes::CrossRoom, createRoomInfo(roomTypes::ChunkyRoom));
		_roomInfo->insert_or_assign(roomTypes::CrossRoom, createRoomInfo(roomTypes::Cave));
		_roomInfo->insert_or_assign(roomTypes::CrossRoom, createRoomInfo(roomTypes::Cavern));
		_roomInfo->insert_or_assign(roomTypes::CrossRoom, createRoomInfo(roomTypes::MainEntranceRoom));
	}

	brogueRoomInfo brogueLevelProfile::createRoomInfo(roomTypes roomType)
	{
		if (_depth == 1)
		{
			if (roomType == roomTypes::MainEntranceRoom)
				return brogueRoomInfo(roomType, 1, _corridorFrequencyMap[levelTypes::DP_BASIC_FIRST_ROOM]);
		}

		// ADJUST FOR DEPTH (Brogue v1.7.5)
		const short descentPercent = brogueMath<short>::clamp(100 * (_depth - 1) / (AMULET_LEVEL - 1), 0, 100);

		// Get static data to adjust
		short roomFrequency = _roomFrequencyMap[_levelType][roomType];
		short corridorFrequency = _corridorFrequencyMap[_levelType];

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
		return _roomInfo->at(roomTypes::MainEntranceRoom);
	}

	brogueRoomInfo brogueLevelProfile::getRandomRoomInfo()
	{
		std::vector<short> weights = mapExtension<roomTypes, brogueRoomInfo, short>::selectFromValues(*_roomInfo, [](brogueRoomInfo info)
		{
			return info.frequency;
		});

		short randomIndex = _randomGenerator->randWeighted(weights);

		return mapExtension<roomTypes, brogueRoomInfo, short>::getValueAt(*_roomInfo, randomIndex);
	}
}