#pragma once

#include "dungeonConstants.h"
#include <simpleRect.h>
#include <simple.h>
#include <simpleException.h>

#include "brogueSymbolTemplate.h"
#include "simpleSize.h"

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

	class brogueRoomTemplate : public simpleObject
	{
	public:
		brogueRoomTemplate()
		{
			_roomType = brogueRoomType::SmallRoom;		// Default Room Parameters
			_frequency = 1.0f;
			_tileSize = new simpleSize(2, 2);
			_wallSymbol = new brogueSymbolTemplate();
			_floorSymbol = new brogueSymbolTemplate();
		}
		~brogueRoomTemplate() override
		{
			delete _tileSize;
			delete _wallSymbol;
			delete _floorSymbol;
		}

		brogueRoomTemplate(brogueRoomType type, float frequency, const simpleSize& tileSize)
		{
			_roomType = type;
			_frequency = frequency;
			_tileSize = new simpleSize(tileSize);
			_wallSymbol = new brogueSymbolTemplate();
			_floorSymbol = new brogueSymbolTemplate();
		}

		brogueRoomTemplate(const brogueRoomTemplate& copy)
		{
			_wallSymbol = nullptr;
			_floorSymbol = nullptr;
			_tileSize = nullptr;

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
			return hashGenerator::generateHash(_roomType, _frequency, _tileSize, _wallSymbol, _floorSymbol);
		}

		simpleSize getTileSize() const
		{
			return *_tileSize;
		}

		brogueRoomType getRoomType() const
		{
			return _roomType;
		}

		float getFrequency() const
		{
			return _frequency;
		}

		brogueSymbolTemplate getWallSymbol() const
		{
			return *_wallSymbol;
		}
		brogueSymbolTemplate getFloorSymbol() const
		{
			return *_floorSymbol;
		}

	private:

		brogueRoomType _roomType;
		simpleSize* _tileSize;
		float _frequency;

		brogueSymbolTemplate* _wallSymbol;
		brogueSymbolTemplate* _floorSymbol;

	private:

		void copyImpl(const brogueRoomTemplate& copy)
		{
			delete _tileSize;
			delete _wallSymbol;
			delete _floorSymbol;

			_roomType = copy.getRoomType();
			_frequency = copy.getFrequency();
			_tileSize = new simpleSize(copy.getTileSize());
			_wallSymbol = new brogueSymbolTemplate(copy.getWallSymbol());
			_floorSymbol = new brogueSymbolTemplate(copy.getFloorSymbol());
		}

		bool compare(const brogueRoomTemplate& other) const
		{
			return _roomType == other.getRoomType() &&
				_frequency == other.getFrequency() &&
				*_tileSize == other.getTileSize() &&
				*_wallSymbol == other.getWallSymbol() &&
				*_floorSymbol == other.getFloorSymbol();
		}
	};
}
