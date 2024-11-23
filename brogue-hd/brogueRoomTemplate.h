#pragma once

#include "brogueGlobal.h"
#include "dungeonConstants.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;

namespace brogueHd::backend::model
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

	struct brogueRoomTemplate : hashable
	{
	public:

		brogueRoomTemplate()
		{
			init(brogueRoomType::MainEntranceRoom, 0.0, gridRect(0, 0, 0, 0));
		}
		brogueRoomTemplate(const gridRect& levelBoundary, brogueRoomType type, short afrequency)
		{
			init(type, frequency, levelBoundary);
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

		gridRect getMaxSize(const gridRect& constraint) const
		{
			gridRect aMaxSize, aMinSize;
			getSize(roomType, aMinSize, aMaxSize, constraint);
			return aMaxSize;
		}
		gridRect getMinSize(const gridRect& constraint) const
		{
			gridRect aMaxSize, aMinSize;
			getSize(roomType, aMinSize, aMaxSize, constraint);
			return aMinSize;
		}

		brogueRoomType getRoomType() const
		{
			return roomType;
		}

		short getFrequency() const
		{
			return frequency;
		}

	private:

		brogueStartLocation startLocation;
		brogueRoomType roomType;
		gridRect minSize;
		gridRect maxSize;
		short frequency;

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

		void init(brogueRoomType type, float afrequency, const gridRect& constraint)
		{
			roomType = type;
			startLocation = brogueStartLocation::MainEntrance;
			frequency = afrequency;

			getSize(type, minSize, maxSize, constraint);
		}

		void getSize(brogueRoomType type, gridRect& aminSize, gridRect& amaxSize, const gridRect& constraint) const
		{
			switch (type)
			{
				case brogueRoomType::Default:
					aminSize = gridRect(0, 0, constraint.width, constraint.height);
					amaxSize = gridRect(0, 0, constraint.width, constraint.height);
					break;

				case brogueRoomType::CaveCompact:
					aminSize = gridRect(0, 0, 3, 4);
					amaxSize = gridRect(0, 0, 12, 8);
					break;

				case brogueRoomType::CaveLargeNS:
					aminSize = gridRect(0, 0, 3, 15);
					amaxSize = gridRect(0, 0, 12, constraint.height);
					break;

				case brogueRoomType::CaveLargeEW:
					aminSize = gridRect(0, 0, 20, 4);
					amaxSize = gridRect(0, 0, constraint.width, 8);
					break;

				case brogueRoomType::Cavern:
					aminSize = gridRect(0, 0, CAVE_MIN_WIDTH, CAVE_MIN_HEIGHT);
					amaxSize = constraint;
					break;

				case brogueRoomType::ChunkyRoom:
					aminSize = gridRect(0, 0, 2, 2);
					amaxSize = gridRect(0, 0, 7, 7);
					break;

				case brogueRoomType::CircularRoom:
					aminSize = gridRect(0, 0, 4, 4);
					amaxSize = gridRect(0, 0, 20, 20);
					break;

				case brogueRoomType::CrossRoom:
					aminSize = gridRect(0, 0, 4, 3);
					amaxSize = gridRect(0, 0, 20, 7);
					break;

				case brogueRoomType::MainEntranceRoom:
					aminSize = gridRect(0, 0, 20, 10);
					amaxSize = gridRect(0, 0, 20, 10);
					break;

				case brogueRoomType::SmallRoom:
					aminSize = gridRect(0, 0, 3, 2);
					amaxSize = gridRect(0, 0, 6, 4);
					break;

				case brogueRoomType::SmallSymmetricalCrossRoom:
					aminSize = gridRect(0, 0, 4, 4);
					amaxSize = gridRect(0, 0, 8, 5);
					break;

				default:
					throw simpleException("Unhandled room type:  brogueRoomInfo::init");
			}
		}
	};
}