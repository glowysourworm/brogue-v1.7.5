#pragma once

#include "dungeonConstants.h"
#include <simpleRect.h>
#include <simple.h>
#include <simpleException.h>

namespace brogueHd::model
{
	using namespace simple;
	using namespace simple::math;

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

	struct brogueRoomTemplate : simpleStruct
	{
	public:
		brogueRoomTemplate()
		{
			init(brogueRoomType::MainEntranceRoom, 0.0);
		}

		brogueRoomTemplate(const simpleRect& levelBoundary, brogueRoomType type, float frequency)
		{
			init(type, frequency);
		}

		brogueRoomTemplate(const brogueRoomTemplate& copy)
		{
			copyImpl(copy);
		}

		void operator=(const brogueRoomTemplate& copy)
		{
			copyImpl(copy);
		}

		bool operator==(const brogueRoomTemplate& info) const
		{
			return compare(info);
		}

		bool operator!=(const brogueRoomTemplate& info) const
		{
			return !compare(info);
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(roomType, startLocation, minSize, maxSize, frequency);
		}

		simpleRect getMaxSize() const
		{
			return maxSize;
		}

		simpleRect getMinSize() const
		{
			return minSize;
		}

		brogueRoomType getRoomType() const
		{
			return roomType;
		}

		float getFrequency() const
		{
			return frequency;
		}

	private:
		brogueStartLocation startLocation;
		brogueRoomType roomType;
		simpleRect minSize;
		simpleRect maxSize;
		float frequency;

	private:
		void copyImpl(const brogueRoomTemplate& copy)
		{
			roomType = copy.roomType;
			startLocation = copy.startLocation;
			minSize = copy.minSize;
			maxSize = copy.maxSize;
			frequency = copy.frequency;
		}

		bool compare(const brogueRoomTemplate& other) const
		{
			return roomType == other.roomType &&
				startLocation == other.startLocation &&
				minSize == other.minSize &&
				maxSize == other.maxSize &&
				frequency == other.frequency;
		}

		void init(brogueRoomType type, float afrequency)
		{
			roomType = type;
			startLocation = brogueStartLocation::MainEntrance;
			frequency = afrequency;

			getSize(type, minSize, maxSize);
		}

		void getSize(brogueRoomType type, simpleRect& aminSize, simpleRect& amaxSize) const
		{
			switch (type)
			{
			case brogueRoomType::Default:
				aminSize = simpleRect(0, 0, 2, 2);
				amaxSize = simpleRect(0, 0, 2, 2);
				break;

			case brogueRoomType::CaveCompact:
				aminSize = simpleRect(0, 0, 3, 4);
				amaxSize = simpleRect(0, 0, 12, 8);
				break;

			case brogueRoomType::CaveLargeNS:
				aminSize = simpleRect(0, 0, 3, 15);
				amaxSize = simpleRect(0, 0, 12, DROWS);
				break;

			case brogueRoomType::CaveLargeEW:
				aminSize = simpleRect(0, 0, 20, 4);
				amaxSize = simpleRect(0, 0, DCOLS, 8);
				break;

			case brogueRoomType::Cavern:
				aminSize = simpleRect(0, 0, CAVE_MIN_WIDTH, CAVE_MIN_HEIGHT);
				amaxSize = simpleRect(0, 0, DCOLS, DROWS);
				break;

			case brogueRoomType::ChunkyRoom:
				aminSize = simpleRect(0, 0, 2, 2);
				amaxSize = simpleRect(0, 0, 7, 7);
				break;

			case brogueRoomType::CircularRoom:
				aminSize = simpleRect(0, 0, 4, 4);
				amaxSize = simpleRect(0, 0, 20, 20);
				break;

			case brogueRoomType::CrossRoom:
				aminSize = simpleRect(0, 0, 4, 3);
				amaxSize = simpleRect(0, 0, 20, 7);
				break;

			case brogueRoomType::MainEntranceRoom:
				aminSize = simpleRect(0, 0, 20, 10);
				amaxSize = simpleRect(0, 0, 20, 10);
				break;

			case brogueRoomType::SmallRoom:
				aminSize = simpleRect(0, 0, 3, 2);
				amaxSize = simpleRect(0, 0, 6, 4);
				break;

			case brogueRoomType::SmallSymmetricalCrossRoom:
				aminSize = simpleRect(0, 0, 4, 4);
				amaxSize = simpleRect(0, 0, 8, 5);
				break;

			default:
				throw simpleException("Unhandled room type:  brogueRoomInfo::init");
			}
		}
	};
}
