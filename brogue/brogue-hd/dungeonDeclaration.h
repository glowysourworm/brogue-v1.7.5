#pragma once

#include "broguedef.h"

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


	enum levelTypes : int
	{
		DP_BASIC,
		DP_BASIC_FIRST_ROOM,

		DP_GOBLIN_WARREN,
		DP_SENTINEL_SANCTUARY
	};

	enum roomTypes : int
	{
		CrossRoom = 0,
		SmallSymmetricalCrossRoom,
		SmallRoom,
		CircularRoom,
		ChunkyRoom,
		Cave,

		// The kind that fills a level
		Cavern,

		// Upside-down T room at the start of depth 1
		MainEntranceRoom
	};

	struct brogueRoomInfo
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
	};

	// Dungeon flags
	enum tileFlags 
	{
		DISCOVERED = Fl(0),
		VISIBLE = Fl(1),	// cell has sufficient light and is in field of view, ready to draw.
		HAS_PLAYER = Fl(2),
		HAS_MONSTER = Fl(3),
		HAS_DORMANT_MONSTER = Fl(4),	// hidden monster on the square
		HAS_ITEM = Fl(5),
		IN_FIELD_OF_VIEW = Fl(6),	// player has unobstructed line of sight whether or not there is enough light
		WAS_VISIBLE = Fl(7),
		HAS_STAIRS = Fl(8),
		SEARCHED_FROM_HERE = Fl(9),    // player already auto-searched here; can't auto-search here again
		IS_IN_SHADOW = Fl(10),	// so that a player gains an automatic stealth bonus
		MAGIC_MAPPED = Fl(11),
		ITEM_DETECTED = Fl(12),
		CLAIRVOYANT_VISIBLE = Fl(13),
		WAS_CLAIRVOYANT_VISIBLE = Fl(14),
		CLAIRVOYANT_DARKENED = Fl(15),	// magical blindness from a cursed ring of clairvoyance
		CAUGHT_FIRE_THIS_TURN = Fl(16),	// so that fire does not spread asymmetrically
		PRESSURE_PLATE_DEPRESSED = Fl(17),	// so that traps do not trigger repeatedly while you stand on them
		STABLE_MEMORY = Fl(18),	// redraws will be pulled from the memory array, not recalculated
		KNOWN_TO_BE_TRAP_FREE = Fl(19),	// keep track of where the player has stepped or watched monsters step as he knows no traps are there
		IS_IN_PATH = Fl(20),	// the yellow trail leading to the cursor
		IN_LOOP = Fl(21),	// this cell is part of a terrain loop
		IS_CHOKEPOINT = Fl(22),	// if this cell is blocked, part of the map will be rendered inaccessible
		IS_GATE_SITE = Fl(23),	// consider placing a locked door here
		IS_IN_ROOM_MACHINE = Fl(24),
		IS_IN_AREA_MACHINE = Fl(25),
		IS_POWERED = Fl(26),	// has been activated by machine power this turn (flag can probably be eliminated if needed)
		IMPREGNABLE = Fl(27),	// no tunneling allowed!
		TERRAIN_COLORS_DANCING = Fl(28),	// colors here will sparkle when the game is idle
		TELEPATHIC_VISIBLE = Fl(29),	// potions of telepathy let you see through other creatures' eyes
		WAS_TELEPATHIC_VISIBLE = Fl(30),	// potions of telepathy let you see through other creatures' eyes

		IS_IN_MACHINE = (IS_IN_ROOM_MACHINE | IS_IN_AREA_MACHINE), 	// sacred ground; don't generate items here, or teleport randomly to it

		PERMANENT_TILE_FLAGS = (DISCOVERED | MAGIC_MAPPED | ITEM_DETECTED | HAS_ITEM | HAS_DORMANT_MONSTER
		| HAS_STAIRS | SEARCHED_FROM_HERE | PRESSURE_PLATE_DEPRESSED
			| STABLE_MEMORY | KNOWN_TO_BE_TRAP_FREE | IN_LOOP
			| IS_CHOKEPOINT | IS_GATE_SITE | IS_IN_MACHINE | IMPREGNABLE),

		ANY_KIND_OF_VISIBLE = (VISIBLE | CLAIRVOYANT_VISIBLE | TELEPATHIC_VISIBLE),
	};
}