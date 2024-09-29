#pragma once

#include "brogueMacros.h"

namespace brogueHd::backend::model::machine
{
	enum machineFeatureFlags 
	{
		MF_GENERATE_ITEM = Fl(0),	// feature entails generating an item (overridden if the machine is adopting an item)
		MF_OUTSOURCE_ITEM_TO_MACHINE = Fl(1),	// item must be adopted by another machine
		MF_BUILD_VESTIBULE = Fl(2),	// call this at the origin of a door room to create a new door guard machine there
		MF_ADOPT_ITEM = Fl(3),	// this feature will take the adopted item (be it from another machine or a previous feature)
		MF_NO_THROWING_WEAPONS = Fl(4),	// the generated item cannot be a throwing weapon
		MF_GENERATE_HORDE = Fl(5),	// generate a monster horde that has all of the horde flags
		MF_BUILD_AT_ORIGIN = Fl(6),	// generate this feature at the room entrance
		// unused                       = Fl(7),	//
		MF_PERMIT_BLOCKING = Fl(8),	// permit the feature to block the map's passability (e.g. to add a locked door)
		MF_TREAT_AS_BLOCKING = Fl(9),	// treat this terrain as though it blocks, for purposes of deciding whether it can be placed there
		MF_NEAR_ORIGIN = Fl(10),	// feature must spawn in the rough quarter of tiles closest to the origin
		MF_FAR_FROM_ORIGIN = Fl(11),	// feature must spawn in the rough quarter of tiles farthest from the origin
		MF_MONSTER_TAKE_ITEM = Fl(12),	// the item associated with this feature (including if adopted) will be in possession of the horde leader that's generated
		MF_MONSTER_SLEEPING = Fl(13),	// the monsters should be asleep when generated
		MF_MONSTER_FLEEING = Fl(14),   // the monsters should be permanently fleeing when generated
		MF_EVERYWHERE = Fl(15),	// generate the feature on every tile of the machine (e.g. carpeting)
		MF_ALTERNATIVE = Fl(16),	// build only one feature that has this flag per machine; the rest are skipped
		MF_ALTERNATIVE_2 = Fl(17),	// same as MF_ALTERNATIVE, but provides for a second set of alternatives of which only one will be chosen
		MF_REQUIRE_GOOD_RUNIC = Fl(18),	// generated item must be uncursed runic
		MF_MONSTERS_DORMANT = Fl(19),	// monsters are dormant, and appear when a dungeon feature with DFF_ACTIVATE_DORMANT_MONSTER spawns on their tile
		// unused                       = Fl(20),	//
		MF_BUILD_IN_WALLS = Fl(21),	// build in an impassable tile that is adjacent to the interior
		MF_BUILD_ANYWHERE_ON_LEVEL = Fl(22),	// build anywhere on the level that is not inside the machine
		MF_REPEAT_UNTIL_NO_PROGRESS = Fl(23),	// keep trying to build this feature set until no changes are made
		MF_IMPREGNABLE = Fl(24),	// this feature's location will be immune to tunneling
		MF_IN_VIEW_OF_ORIGIN = Fl(25),	// this feature must be in view of the origin
		MF_IN_PASSABLE_VIEW_OF_ORIGIN = Fl(26),	// this feature must be in view of the origin, where "view" is blocked by pathing blockers
		MF_NOT_IN_HALLWAY = Fl(27),	// the feature location must have a passableArcCount of <= 1
		MF_NOT_ON_LEVEL_PERIMETER = Fl(28),	// don't build it in the outermost walls of the level
		MF_SKELETON_KEY = Fl(29),	// if a key is generated or adopted by this feature, it will open all locks in this machine.
		MF_KEY_DISPOSABLE = Fl(30),	// if a key is generated or adopted, it will self-destruct after being used at this current location.
	};

	enum machineTypes 
	{
		// Reward rooms:
		MT_REWARD_MULTI_LIBRARY = 1,
		MT_REWARD_MONO_LIBRARY,
		MT_REWARD_CONSUMABLES,
		MT_REWARD_PEDESTALS_PERMANENT,
		MT_REWARD_PEDESTALS_CONSUMABLE,
		MT_REWARD_COMMUTATION_ALTARS,
		MT_REWARD_RESURRECTION_ALTAR,
		MT_REWARD_ADOPTED_ITEM,
		MT_REWARD_DUNGEON,
		MT_REWARD_KENNEL,
		MT_REWARD_VAMPIRE_LAIR,
		MT_REWARD_ASTRAL_PORTAL,
		MT_REWARD_GOBLIN_WARREN,
		MT_REWARD_SENTINEL_SANCTUARY,

		// Amulet holder:
		MT_AMULET_AREA,

		// Door guard machines:
		MT_LOCKED_DOOR_VESTIBULE,
		MT_SECRET_DOOR_VESTIBULE,
		MT_SECRET_LEVER_VESTIBULE,
		MT_FLAMMABLE_BARRICADE_VESTIBULE,
		MT_STATUE_SHATTERING_VESTIBULE,
		MT_STATUE_MONSTER_VESTIBULE,
		MT_THROWING_TUTORIAL_VESTIBULE,
		MT_PIT_TRAPS_VESTIBULE,
		MT_BECKONING_OBSTACLE_VESTIBULE,
		MT_GUARDIAN_VESTIBULE,

		// Key guard machines:
		MT_KEY_REWARD_LIBRARY,
		MT_KEY_SECRET_ROOM,
		MT_KEY_THROWING_TUTORIAL_AREA,
		MT_KEY_RAT_TRAP_ROOM,
		MT_KEY_FIRE_TRANSPORTATION_ROOM,
		MT_KEY_FLOOD_TRAP_ROOM,
		MT_KEY_FIRE_TRAP_ROOM,
		MT_KEY_THIEF_AREA,
		MT_KEY_COLLAPSING_FLOOR_AREA,
		MT_KEY_PIT_TRAP_ROOM,
		MT_KEY_LEVITATION_ROOM,
		MT_KEY_WEB_CLIMBING_ROOM,
		MT_KEY_LAVA_MOAT_ROOM,
		MT_KEY_LAVA_MOAT_AREA,
		MT_KEY_POISON_GAS_TRAP_ROOM,
		MT_KEY_EXPLOSIVE_TRAP_ROOM,
		MT_KEY_BURNING_TRAP_ROOM,
		MT_KEY_STATUARY_TRAP_AREA,
		MT_KEY_GUARDIAN_WATER_PUZZLE_ROOM,
		MT_KEY_GUARDIAN_GAUNTLET_ROOM,
		MT_KEY_GUARDIAN_CORRIDOR_ROOM,
		MT_KEY_SACRIFICE_ROOM,
		MT_KEY_SUMMONING_CIRCLE_ROOM,
		MT_KEY_BECKONING_OBSTACLE_ROOM,
		MT_KEY_WORM_TRAP_AREA,
		MT_KEY_MUD_TRAP_ROOM,
		MT_KEY_ELECTRIC_CRYSTALS_ROOM,
		MT_KEY_ZOMBIE_TRAP_ROOM,
		MT_KEY_PHANTOM_TRAP_ROOM,
		MT_KEY_WORM_TUNNEL_ROOM,
		MT_KEY_TURRET_TRAP_ROOM,
		MT_KEY_BOSS_ROOM,

		// Thematic machines:
		MT_BLOODFLOWER_AREA,
		MT_SHRINE_AREA,
		MT_IDYLL_AREA,
		MT_SWAMP_AREA,
		MT_CAMP_AREA,
		MT_REMNANT_AREA,
		MT_DISMAL_AREA,
		MT_BRIDGE_TURRET_AREA,
		MT_LAKE_PATH_TURRET_AREA,
		MT_PARALYSIS_TRAP_AREA,
		MT_PARALYSIS_TRAP_HIDDEN_AREA,
		MT_TRICK_STATUE_AREA,
		MT_WORM_AREA,
		MT_SENTINEL_AREA
	};
}