#pragma once

#include "brogueGlobal.h"
#include "dungeon.h"
#include "dungeonConstants.h"
#include "randomGenerator.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleHash.h"
#include "simpleList.h"
#include "simpleMath.h"

using namespace brogueHd::simple;
using namespace brogueHd::backend;

namespace brogueHd::backend::model
{
	class brogueLevelProfile
	{
	public:

		brogueLevelProfile(short depth, levelTypes type);
		~brogueLevelProfile();

		brogueRoomInfo getRandomRoomInfo(randomGenerator* randomMain);
		brogueRoomInfo getEntranceRoom(randomGenerator* randomMain);

	private:

		brogueRoomInfo createRoomInfo(brogueRoomType roomType);

	private:

		simpleHash<brogueRoomType, brogueRoomInfo>* _roomInfo;

		// Room Frequency static data
		simpleHash<levelTypes, simpleHash<brogueRoomType, short>*>* _roomFrequencyMap;

		// Corridor static data
		simpleHash<levelTypes, short>* _corridorFrequencyMap;

		short _depth;
		levelTypes _levelType;
	};

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

	brogueLevelProfile::brogueLevelProfile(short depth, levelTypes type)
	{
		if (depth == 1 && type != levelTypes::DP_BASIC_FIRST_ROOM)
			throw simpleException("Invalid first level type:  brogueLevelProfile.cpp");

		_roomInfo = new simpleHash<brogueRoomType, brogueRoomInfo>();
		_roomFrequencyMap = new simpleHash<levelTypes, simpleHash<brogueRoomType, short>*>();
		_corridorFrequencyMap = new simpleHash< levelTypes, short>();
		_levelType = type;
		_depth = depth;

		simpleHash<brogueRoomType, short>* basicMap = new simpleHash<brogueRoomType, short>();
		simpleHash<brogueRoomType, short>* basicFirstRoomMap = new simpleHash<brogueRoomType, short>();
		simpleHash<brogueRoomType, short>* goblinWarrenMap = new simpleHash<brogueRoomType, short>();
		simpleHash<brogueRoomType, short>* sentinelSanctuariesMap = new simpleHash<brogueRoomType, short>();

		// BROGUE v1.7.5
		basicMap->add(brogueRoomType::CrossRoom, 2);
		basicMap->add(brogueRoomType::SmallSymmetricalCrossRoom, 1);
		basicMap->add(brogueRoomType::SmallRoom, 1);
		basicMap->add(brogueRoomType::CircularRoom, 1);
		basicMap->add(brogueRoomType::ChunkyRoom, 7);
		basicMap->add(brogueRoomType::CaveCompact, 1);
		basicMap->add(brogueRoomType::CaveLargeEW, 1);
		basicMap->add(brogueRoomType::CaveLargeNS, 1);
		basicMap->add(brogueRoomType::Cavern, 0);
		basicMap->add(brogueRoomType::MainEntranceRoom, 0);

		basicFirstRoomMap->add(brogueRoomType::CrossRoom, 10);
		basicFirstRoomMap->add(brogueRoomType::SmallSymmetricalCrossRoom, 0);
		basicFirstRoomMap->add(brogueRoomType::SmallRoom, 0);
		basicFirstRoomMap->add(brogueRoomType::CircularRoom, 3);
		basicFirstRoomMap->add(brogueRoomType::ChunkyRoom, 7);
		basicFirstRoomMap->add(brogueRoomType::CaveCompact, 10);
		basicFirstRoomMap->add(brogueRoomType::CaveLargeEW, 10);
		basicFirstRoomMap->add(brogueRoomType::CaveLargeNS, 10);
		basicFirstRoomMap->add(brogueRoomType::Cavern, 10);
		basicFirstRoomMap->add(brogueRoomType::MainEntranceRoom, 0);

		goblinWarrenMap->add(brogueRoomType::CrossRoom, 0);
		goblinWarrenMap->add(brogueRoomType::SmallSymmetricalCrossRoom, 0);
		goblinWarrenMap->add(brogueRoomType::SmallRoom, 1);
		goblinWarrenMap->add(brogueRoomType::CircularRoom, 0);
		goblinWarrenMap->add(brogueRoomType::ChunkyRoom, 0);
		goblinWarrenMap->add(brogueRoomType::CaveCompact, 0);
		goblinWarrenMap->add(brogueRoomType::CaveLargeEW, 0);
		goblinWarrenMap->add(brogueRoomType::CaveLargeNS, 0);
		goblinWarrenMap->add(brogueRoomType::Cavern, 0);
		goblinWarrenMap->add(brogueRoomType::MainEntranceRoom, 0);

		sentinelSanctuariesMap->add(brogueRoomType::CrossRoom, 0);
		sentinelSanctuariesMap->add(brogueRoomType::SmallSymmetricalCrossRoom, 5);
		sentinelSanctuariesMap->add(brogueRoomType::SmallRoom, 0);
		sentinelSanctuariesMap->add(brogueRoomType::CircularRoom, 1);
		sentinelSanctuariesMap->add(brogueRoomType::ChunkyRoom, 0);
		sentinelSanctuariesMap->add(brogueRoomType::CaveCompact, 0);
		sentinelSanctuariesMap->add(brogueRoomType::CaveLargeEW, 0);
		sentinelSanctuariesMap->add(brogueRoomType::CaveLargeNS, 0);
		sentinelSanctuariesMap->add(brogueRoomType::Cavern, 0);
		sentinelSanctuariesMap->add(brogueRoomType::MainEntranceRoom, 0);

		_roomFrequencyMap->add(levelTypes::DP_BASIC, basicMap);
		_roomFrequencyMap->add(levelTypes::DP_BASIC_FIRST_ROOM, basicFirstRoomMap);
		_roomFrequencyMap->add(levelTypes::DP_GOBLIN_WARREN, goblinWarrenMap);
		_roomFrequencyMap->add(levelTypes::DP_SENTINEL_SANCTUARY, sentinelSanctuariesMap);

		_corridorFrequencyMap->add(levelTypes::DP_BASIC, 10);
		_corridorFrequencyMap->add(levelTypes::DP_BASIC_FIRST_ROOM, 0);
		_corridorFrequencyMap->add(levelTypes::DP_GOBLIN_WARREN, 0);
		_corridorFrequencyMap->add(levelTypes::DP_SENTINEL_SANCTUARY, 0);

		// Set up room info's
		_roomInfo->add(brogueRoomType::CrossRoom, createRoomInfo(brogueRoomType::CrossRoom));
		_roomInfo->add(brogueRoomType::SmallSymmetricalCrossRoom, createRoomInfo(brogueRoomType::SmallSymmetricalCrossRoom));
		_roomInfo->add(brogueRoomType::SmallRoom, createRoomInfo(brogueRoomType::SmallRoom));
		_roomInfo->add(brogueRoomType::CircularRoom, createRoomInfo(brogueRoomType::CircularRoom));
		_roomInfo->add(brogueRoomType::ChunkyRoom, createRoomInfo(brogueRoomType::ChunkyRoom));
		_roomInfo->add(brogueRoomType::CaveCompact, createRoomInfo(brogueRoomType::CaveCompact));
		_roomInfo->add(brogueRoomType::CaveLargeEW, createRoomInfo(brogueRoomType::CaveLargeEW));
		_roomInfo->add(brogueRoomType::CaveLargeNS, createRoomInfo(brogueRoomType::CaveLargeNS));
		_roomInfo->add(brogueRoomType::Cavern, createRoomInfo(brogueRoomType::Cavern));
		_roomInfo->add(brogueRoomType::MainEntranceRoom, createRoomInfo(brogueRoomType::MainEntranceRoom));

	}

	brogueLevelProfile::~brogueLevelProfile()
	{
		_roomFrequencyMap->iterate([] (levelTypes type, simpleHash<brogueRoomType, short>* map)
		{
			delete map;

			return iterationCallback::iterate;
		});

		delete _roomInfo;
		delete _roomFrequencyMap;
		delete _corridorFrequencyMap;
	}

	brogueRoomInfo brogueLevelProfile::createRoomInfo(brogueRoomType roomType)
	{
		if (_depth == 1)
		{
			if (roomType == brogueRoomType::MainEntranceRoom)
				return brogueRoomInfo(roomType, 1, _corridorFrequencyMap->get(levelTypes::DP_BASIC_FIRST_ROOM));
		}

		// ADJUST FOR DEPTH (Brogue v1.7.5)
		const short descentPercent = simpleMath::clamp(100 * (_depth - 1) / (AMULET_LEVEL - 1), 0, 100);

		// Get static data to adjust
		short roomFrequency = _roomFrequencyMap->get(_levelType)->get(roomType);
		short corridorFrequency = _corridorFrequencyMap->get(_levelType);

		// Additional adjustment for first room
		switch (roomType)
		{
			case brogueRoomType::CrossRoom:
				roomFrequency += 20 * (100 - descentPercent) / 100;
				break;
			case brogueRoomType::SmallSymmetricalCrossRoom:
				roomFrequency += 10 * (100 - descentPercent) / 100;
				break;
			case brogueRoomType::SmallRoom:
				break;
			case brogueRoomType::CircularRoom:
				roomFrequency += 7 * (100 - descentPercent) / 100;
				break;
			case brogueRoomType::ChunkyRoom:
				break;
			case brogueRoomType::CaveCompact:
			case brogueRoomType::CaveLargeNS:
			case brogueRoomType::CaveLargeEW:
				roomFrequency += 10 * (100 - descentPercent) / 100;
				break;
			case brogueRoomType::Cavern:
				roomFrequency += 50 * descentPercent / 100;
				break;
			case brogueRoomType::MainEntranceRoom:
				break;
			default:
				break;
		}

		corridorFrequency += 80 * (100 - descentPercent) / 100;

		return brogueRoomInfo(roomType, roomFrequency, corridorFrequency);
	}

	brogueRoomInfo brogueLevelProfile::getEntranceRoom(randomGenerator* randomMaim)
	{
		return _roomInfo->get(brogueRoomType::MainEntranceRoom);
	}

	brogueRoomInfo brogueLevelProfile::getRandomRoomInfo(randomGenerator* randomMaim)
	{
		simpleList<short> weights = _roomInfo->except([] (brogueRoomType type, const brogueRoomInfo& info)
		{
			return type == brogueRoomType::MainEntranceRoom;

		}).selectFromValues<short>([] (brogueRoomInfo info)
		{
			return info.frequency;
		});

		short randomIndex = randomMaim->randWeighted(weights.toArray());

		return _roomInfo->getAt(randomIndex)->value;
	}
}