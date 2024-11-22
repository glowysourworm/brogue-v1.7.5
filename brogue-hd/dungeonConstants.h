#pragma once

#include "brogueGlobal.h"

namespace brogueHd::backend::model
{
	enum levelTypes : int
	{
		DP_BASIC,
		DP_BASIC_FIRST_ROOM,

		DP_GOBLIN_WARREN,
		DP_SENTINEL_SANCTUARY
	};

	enum class brogueStartLocation
	{
		MainEntrance = 0
	};

	enum class brogueRoomType
	{
		/// <summary>
		/// Default room is the full size of the room constraint boundary
		/// </summary>
		Default = 0,
		CrossRoom,
		SmallSymmetricalCrossRoom,
		SmallRoom,
		CircularRoom,
		ChunkyRoom,
		CaveCompact,
		CaveLargeNS,
		CaveLargeEW,

		// The kind that fills a level
		Cavern,

		// Upside-down T room at the start of depth 1
		MainEntranceRoom
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

	/// <summary>
	/// Permanent LAYERS of the dungeon map. See dungeon features for what those layers represent. (e.g. WALL could
	/// have a feature that is a TORCH)
	/// </summary>
	enum permanentDungeonLayers
	{
		// Basic tiles with nothing on them but benign features
		DUNGEON = 0,

		// Wall tiles - (see features for torch, and other data)
		WALL = Fl(1),

		// Door tiles: machine or otherwise; locked or unlocked
		DOOR = Fl(2),

		// Shallow water:  permanent; and on the edges of the water region
		WATER_SHALLOW = Fl(3),

		// Deep water: permanent. Dynamic layers are added on top of permanent layers
		WATER_DEEP = Fl(4),

		// Edge of a swamp:  permanent, and walkable
		SWAMP_SHALLOW = Fl(5),

		// Interior of a swamp:  permanent, and not easily walkable
		SWAMP_DEEP = Fl(6),

		// Lava:  permanent
		LAVA = Fl(7),

		// Chasm:  permanent. Edges of a chasm are NOT a chasm - they are a dungeon tile feature.
		CHASM = Fl(8)
	};


	/// <summary>
	/// Temporary LAYERS of the dungeon map. These can be a result of enemy or friendly actions; and live on
	/// separate grids inside brogueGrid.h
	/// </summary>
	enum class temporaryDungeonLayers
	{
		// Temporary Chasm:  Created by a bloat
		CHASM = 0,

		// Fire:  Active fires are temporary terrain alterations
		FIRE = Fl(1),

		// Gas:  All gas types are considered a hazard
		GAS = Fl(2),

		// Net:  Net traps will leave these
		NET = Fl(3),

		// Spider's Web:  These are temporary grid alterations
		SPIDER_WEB = Fl(4),

		// Temporary Water:  Pools of water will be stored when traps are set
		WATER = Fl(5)
	};

	/// <summary>
	/// Enumeration designed to specify flags for sub categories. Each flag should
	/// be accompanied by another enumeration for that category. (refactoring in progress)
	/// </summary>
	enum dungeonFeatureCategories
	{
		FEATURE_DECORATION = Fl(1),
		FEATURE_WALL = Fl(2),
		FEATURE_FOLIAGE = Fl(3),
		FEATURE_LIGHT = Fl(4),
		FEATURE_TRAP = Fl(5),
		FEATURE_BODILY_FLUIDS = Fl(6),
		FEATURE_ENEMY = Fl(7),
		FEATURE_EXPLOSION = Fl(8),
		FEATURE_MAGIC = Fl(9),
		FEATURE_GAS = Fl(10),
		FEATURE_POTION = Fl(11),
		FEATURE_RUBBLE = Fl(12),
		FEATURE_INTERACTION = Fl(13),
		FEATURE_BRIMSTONE = Fl(14),
		FEATURE_DEWARS = Fl(15),
		FEATURE_DOOR = Fl(16),
		FEATURE_OTHER_DOOR = Fl(17),
		FEATURE_ALTAR = Fl(18),
		FEATURE_SWAMP = Fl(19),
		FEATURE_CHASM = Fl(20),
		FEATURE_FIRE = Fl(21),
		FEATURE_OTHER_ITEMS = Fl(22),
		FEATURE_STENCH = Fl(23)
	};

	/// <summary>
	/// Decorations that don't otherwise have a category. These are all benign.
	/// </summary>
	enum dungeonFeatureDecoration
	{
		CARPET = Fl(1),

		// camp:
		HAY = Fl(2),
		JUNK = Fl(3)
	};

	enum dungeonFeatureWall
	{
		GRANITE_COLUMN = Fl(1),
		CRYSTAL_WALL = Fl(2),

		// worm den:
		WALL_SHATTER = Fl(3)
	};

	enum dungeonFeatureFoliage
	{
		ALGAE_1 = Fl(1),
		ALGAE_2 = Fl(2),
		ALGAE_1_FREEZE = Fl(3),
		ALGAE_2_FREEZE = Fl(4),
		ALGAE_REVERT = Fl(5),
		BUILD_ALGAE_WELL = Fl(6),

		BLOODFLOWER_PODS_GROW_INITIAL = Fl(7),
		BLOODFLOWER_PODS_GROW = Fl(8),
		BLOODFLOWER_POD_BURST = Fl(9),

		DEAD_FOLIAGE = Fl(10),
		DEAD_GRASS = Fl(11),
		DEAD_GRASS_SMALL = Fl(12),

		FOLIAGE = Fl(13),
		FOLIAGE_REGROW = Fl(14),
		FOLIAGE_TRAMPLED = Fl(15),

		FUNGUS_FOREST = Fl(16),
		FUNGUS_FOREST_REGROW = Fl(17),
		FUNGUS_FOREST_TRAMPLED = Fl(18),
		FUNGUS_LUMINESCENT = Fl(19),

		GRASS = Fl(20),

		LICHEN_GROW = Fl(21),
		LICHEN_MUTATION = Fl(22),
		LICHEN_PLANTED = Fl(23)
	};

	enum dungeonFeatureLight
	{
		SUNLIGHT = Fl(1),
		DARKNESS = Fl(2)
	};

	enum dungeonFeatureTrap
	{
		POISON_GAS_TRAP = Fl(1),
		PARALYSIS_GAS_TRAP = Fl(2),
		TRAPDOOR = Fl(3),
		CONFUSION_GAS_TRAP = Fl(4),
		FLAMETHROWER_TRAP = Fl(5),
		FLOOD_TRAP = Fl(6),
		NET_TRAP = Fl(7),
		ALARM_TRAP = Fl(8)
	};

	enum dungeonFeatureBodilyFluids
	{
		RED_BLOOD = Fl(1),
		GREEN_BLOOD = Fl(2),
		PURPLE_BLOOD = Fl(3),
		WORM_BLOOD = Fl(4),
		ACID_BLOOD = Fl(5),
		ASH_BLOOD = Fl(6),
		EMBER_BLOOD = Fl(7),
		ECTOPLASM_BLOOD = Fl(8),
		RUBBLE_BLOOD = Fl(9),
		ROT_GAS_BLOOD = Fl(10),
		VOMIT = Fl(11),
		URINE = Fl(12),
		UNICORN_POOP = Fl(13)
	};

	enum dungeonFeatureEnemy
	{
		FLAMEDANCER_CORONA = Fl(1),

		SALAMANDER_FLAME = Fl(2),

		WEB_SMALL = Fl(3),
		WEB_LARGE = Fl(4),

		ANCIENT_SPIRIT_VINES = Fl(5),
		ANCIENT_SPIRIT_GRASS = Fl(6),

		// remnants:
		REMNANT = Fl(7),
		REMNANT_ASH = Fl(8)
	};

	enum dungeonFeatureExplosion
	{
		BLOAT_EXPLOSION = Fl(1),
		BLOOD_EXPLOSION = Fl(2),
		MUTATION_EXPLOSION = Fl(3)
	};

	enum dungeonFeatureMagic
	{
		REPEL_CREATURES = Fl(1),
		SHATTERING_SPELL = Fl(2),
		ECTOPLASM_DROPLET = Fl(3),
		FORCEFIELD = Fl(4),
		FORCEFIELD_MELT = Fl(5)
	};

	enum dungeonFeatureGas
	{
		ROT_GAS_PUFF = Fl(1),
		STEAM_PUFF = Fl(2),
		STEAM_ACCUMULATION = Fl(3),
		METHANE_GAS_PUFF = Fl(4),

		POISON_GAS_CLOUD = Fl(5),
		CONFUSION_GAS_TRAP_CLOUD = Fl(6),
		METHANE_GAS_ARMAGEDDON  = Fl(7),
	};

	enum dungeonFeaturePotion
	{
		// potions
		POISON_GAS_CLOUD_POTION = Fl(1),
		PARALYSIS_GAS_CLOUD_POTION = Fl(2),
		CONFUSION_GAS_CLOUD_POTION = Fl(3),
		INCINERATION_POTION = Fl(4),
		DARKNESS_POTION = Fl(5),
		HOLE_POTION = Fl(6)
	};

	enum dungeonFeatureRubble
	{
		ASH = Fl(1),
		TUNNELIZE = Fl(2)
	};

	enum dungeonFeatureInteraction
	{
		// Machine
		SACRED_GLYPHS = Fl(1),
		SILENT_GLYPH_GLOW = Fl(2),
		GUARDIAN_STEP = Fl(3),
		MIRROR_TOTEM_STEP = Fl(4),
		GLYPH_INACTIVE = Fl(5),
		GLYPH_CIRCLE = Fl(6),
		GLYPH_ACTIVE = Fl(7),
		ITEM_CAGE_OPEN = Fl(8),
		ITEM_CAGE_CLOSED = Fl(9),

		// Other
		BRIDGE_FALL_PREP,
		BRIDGE_FALL,
		LEVER_REVEAL,
		LEVER_PULL,
		LEVER_CREATE,

		// vampire in coffin
		COFFIN_BURSTS,
		COFFIN_BURNS,
		TRIGGER_AREA,

		// throwing tutorial -- button in chasm
		CAGE_DISAPPEARS,
		MEDIUM_HOLE,
		MEDIUM_LAVA_POND,
		MACHINE_PRESSURE_PLATE_USED,

		ALTAR_COMMUTE,
		MAGIC_PIPING,
		INERT_PIPE,

		// resurrection altar
		ALTAR_RESURRECT,
		MACHINE_FLOOR_TRIGGER_REPEATING,

		// sacrifice altar
		SACRIFICE_ALTAR,
		SACRIFICE_COMPLETE,
		SACRIFICE_CAGE_ACTIVE,

		// rat trap
		WALL_CRACK,

		// wooden barricade at entrance
		WOODEN_BARRICADE_BURN,

		// wooden barricade around altar, dead grass all around
		SURROUND_WOODEN_BARRICADE,

		// pools of water that, when triggered, slowly expand to fill the room
		SPREADABLE_WATER,
		SHALLOW_WATER,
		WATER_SPREADS,
		SPREADABLE_WATER_POOL,
		SPREADABLE_DEEP_WATER_POOL,

		// when triggered, the ground gradually turns into chasm:
		SPREADABLE_COLLAPSE,
		COLLAPSE,
		COLLAPSE_SPREADS,
		ADD_MACHINE_COLLAPSE_EDGE_DORMANT,

		// when triggered, a bridge appears:
		BRIDGE_ACTIVATE,
		BRIDGE_ACTIVATE_ANNOUNCE,
		BRIDGE_APPEARS,
		ADD_DORMANT_CHASM_HALO,

		// when triggered, the lava retracts:
		LAVA_RETRACTABLE,
		RETRACTING_LAVA,
		OBSIDIAN_WITH_STEAM,

		// when triggered, the door seals and caustic gas fills the room
		SHOW_POISON_GAS_VENT,
		POISON_GAS_VENT_OPEN,
		ACTIVATE_PORTCULLIS,
		OPEN_PORTCULLIS,
		VENT_SPEW_POISON_GAS,

		// when triggered, pilot light ignites and explosive gas fills the room
		SHOW_METHANE_VENT,
		METHANE_VENT_OPEN,
		VENT_SPEW_METHANE,
		PILOT_LIGHT,

		// paralysis trap: trigger plate with gas vents nearby
		DISCOVER_PARALYSIS_VENT,
		PARALYSIS_VENT_SPEW,
		REVEAL_PARALYSIS_VENT_SILENTLY,

		// thematic dungeon
		AMBIENT_BLOOD,

		// statues crack for a few turns and then shatter, revealing the monster inside
		CRACKING_STATUE,
		STATUE_SHATTER,

		// a turret appears:
		TURRET_EMERGE,

		// an elaborate worm catacomb opens up
		WORM_TUNNEL_MARKER_DORMANT,
		WORM_TUNNEL_MARKER_ACTIVE,
		GRANITE_CRUMBLES,
		WALL_OPEN,

		// the room gradually darkens
		DARKENING_FLOOR,
		DARK_FLOOR,
		HAUNTED_TORCH_TRANSITION,
		HAUNTED_TORCH,

		// bubbles rise from the mud and bog monsters spawn
		MUD_DORMANT,
		MUD_ACTIVATE,

		// crystals charge when hit by lightning
		ELECTRIC_CRYSTAL_ON,
		TURRET_LEVER,

		// monster cages open:
		MONSTER_CAGE_OPENS,
	};

	enum dungeonFeatureBrimstone
	{
		BRIMSTONE_ACTIVE = Fl(1),
		BRIMSTONE_INERT = Fl(2)
	};

	enum dungeonFeatureDewars
	{
		DEWAR_CAUSTIC = Fl(1),
		DEWAR_CONFUSION = Fl(2),
		DEWAR_PARALYSIS = Fl(3),
		DEWAR_METHANE = Fl(4),
		DEWAR_GLASS = Fl(5)
	};

	enum dungeonFeatureDoor
	{
		DOOR_OPEN = Fl(1),
		DOOR_CLOSED = Fl(2),
	};

	enum dungeonFeatureOtherDoor
	{
		IRON_DOOR_OPEN_INERT = Fl(1),
		PORTAL_ACTIVATE = Fl(2)
	};

	enum dungeonFeatureAltar
	{
		ALTAR_INERT = Fl(1),
		ALTER_RETRACT = Fl(2)
	};

	enum dungeonFeatureWater
	{
		FLOOD,
		FLOOD_2,
		FLOOD_DRAIN,

		DEEP_WATER_FREEZE,
		DEEP_WATER_MELTING,
		DEEP_WATER_THAW,
		SHALLOW_WATER_FREEZE,
		SHALLOW_WATER_MELTING,
		SHALLOW_WATER_THAW,

		// idyll:
		SHALLOW_WATER_POOL,
		DEEP_WATER_POOL,

		// lake catwalk:
		LAKE_CELL,
		LAKE_HALO,
	};

	enum dungeonFeatureSwamp
	{
		// swamp:
		SWAMP_WATER,
		SWAMP,
		SWAMP_MUD,
	};

	enum dungeonFeatureChasm
	{
		HOLE_2 = Fl(1),
		HOLE_DRAIN = Fl(2),

		// chasm catwalk:
		CHASM_HOLE,
		CATWALK_BRIDGE,
	};

	enum dungeonFeatureFire
	{
		PLAIN_FIRE = Fl(1),
		GAS_FIRE = Fl(2),
		EXPLOSION_FIRE = Fl(3),
		DART_EXPLOSION = Fl(4),
		BRIMSTONE_FIRE = Fl(5),
		BRIDGE_FIRE = Fl(6),
		FLAMETHROWER = Fl(7),
		EMBERS = Fl(8),
		EMBERS_PATCH = Fl(9),
		OBSIDIAN = Fl(10),
		ITEM_FIRE = Fl(11),
		CREATURE_FIRE = Fl(12)
	};

	enum dungeonFeatureOtherItems
	{
		ARMOR_IMMOLATION,
		STAFF_HOLE,
		STAFF_HOLE_EDGE
	};

	enum dungeonFeatureStench
	{
		// goblin warren:
		STENCH_BURN,
		STENCH_SMOLDER,
	};

	enum class tileType 
	{
		NOTHING = 0,
		GRANITE,
		FLOOR,
		FLOOR_FLOODABLE,
		CARPET,
		MARBLE_FLOOR,
		WALL,
		DOOR,
		OPEN_DOOR,
		SECRET_DOOR,
		LOCKED_DOOR,
		OPEN_IRON_DOOR_INERT,
		DOWN_STAIRS,
		UP_STAIRS,
		DUNGEON_EXIT,
		DUNGEON_PORTAL,
		TORCH_WALL, // wall lit with a torch
		CRYSTAL_WALL,
		PORTCULLIS_CLOSED,
		PORTCULLIS_DORMANT,
		WOODEN_BARRICADE,
		PILOT_LIGHT_DORMANT,
		PILOT_LIGHT,
		HAUNTED_TORCH_DORMANT,
		HAUNTED_TORCH_TRANSITIONING,
		HAUNTED_TORCH,
		WALL_LEVER_HIDDEN,
		WALL_LEVER,
		WALL_LEVER_PULLED,
		WALL_LEVER_HIDDEN_DORMANT,
		STATUE_INERT,
		STATUE_DORMANT,
		STATUE_CRACKING,
		STATUE_INSTACRACK,
		PORTAL,
		TURRET_DORMANT,
		WALL_MONSTER_DORMANT,
		DARK_FLOOR_DORMANT,
		DARK_FLOOR_DARKENING,
		DARK_FLOOR,
		MACHINE_TRIGGER_FLOOR,
		ALTAR_INERT,
		ALTAR_KEYHOLE,
		ALTAR_CAGE_OPEN,
		ALTAR_CAGE_CLOSED,
		ALTAR_SWITCH,
		ALTAR_SWITCH_RETRACTING,
		ALTAR_CAGE_RETRACTABLE,
		PEDESTAL,
		MONSTER_CAGE_OPEN,
		MONSTER_CAGE_CLOSED,
		COFFIN_CLOSED,
		COFFIN_OPEN,

		GAS_TRAP_POISON_HIDDEN,
		GAS_TRAP_POISON,
		TRAP_DOOR_HIDDEN,
		TRAP_DOOR,
		GAS_TRAP_PARALYSIS_HIDDEN,
		GAS_TRAP_PARALYSIS,
		MACHINE_PARALYSIS_VENT_HIDDEN,
		MACHINE_PARALYSIS_VENT,
		GAS_TRAP_CONFUSION_HIDDEN,
		GAS_TRAP_CONFUSION,
		FLAMETHROWER_HIDDEN,
		FLAMETHROWER,
		FLOOD_TRAP_HIDDEN,
		FLOOD_TRAP,
		NET_TRAP_HIDDEN,
		NET_TRAP,
		ALARM_TRAP_HIDDEN,
		ALARM_TRAP,
		MACHINE_POISON_GAS_VENT_HIDDEN,
		MACHINE_POISON_GAS_VENT_DORMANT,
		MACHINE_POISON_GAS_VENT,
		MACHINE_METHANE_VENT_HIDDEN,
		MACHINE_METHANE_VENT_DORMANT,
		MACHINE_METHANE_VENT,
		STEAM_VENT,
		MACHINE_PRESSURE_PLATE,
		MACHINE_PRESSURE_PLATE_USED,
		MACHINE_GLYPH,
		MACHINE_GLYPH_INACTIVE,
		DEWAR_CAUSTIC_GAS,
		DEWAR_CONFUSION_GAS,
		DEWAR_PARALYSIS_GAS,
		DEWAR_METHANE_GAS,

		DEEP_WATER,
		SHALLOW_WATER,
		MUD,
		CHASM,
		CHASM_EDGE,
		MACHINE_COLLAPSE_EDGE_DORMANT,
		MACHINE_COLLAPSE_EDGE_SPREADING,
		LAVA,
		LAVA_RETRACTABLE,
		LAVA_RETRACTING,
		SUNLIGHT_POOL,
		DARKNESS_PATCH,
		ACTIVE_BRIMSTONE,
		INERT_BRIMSTONE,
		OBSIDIAN,
		BRIDGE,
		BRIDGE_FALLING,
		BRIDGE_EDGE,
		STONE_BRIDGE,
		MACHINE_FLOOD_WATER_DORMANT,
		MACHINE_FLOOD_WATER_SPREADING,
		MACHINE_MUD_DORMANT,
		ICE_DEEP,
		ICE_DEEP_MELT,
		ICE_SHALLOW,
		ICE_SHALLOW_MELT,

		HOLE,
		HOLE_GLOW,
		HOLE_EDGE,
		FLOOD_WATER_DEEP,
		FLOOD_WATER_SHALLOW,
		GRASS,
		DEAD_GRASS,
		GRAY_FUNGUS,
		LUMINESCENT_FUNGUS,
		LICHEN,
		HAY,
		RED_BLOOD,
		GREEN_BLOOD,
		PURPLE_BLOOD,
		ACID_SPLATTER,
		VOMIT,
		URINE,
		UNICORN_POOP,
		WORM_BLOOD,
		ASH,
		BURNED_CARPET,
		PUDDLE,
		BONES,
		RUBBLE,
		JUNK,
		BROKEN_GLASS,
		ECTOPLASM,
		EMBERS,
		SPIDERWEB,
		NETTING,
		FOLIAGE,
		DEAD_FOLIAGE,
		TRAMPLED_FOLIAGE,
		FUNGUS_FOREST,
		TRAMPLED_FUNGUS_FOREST,
		FORCEFIELD,
		FORCEFIELD_MELT,
		SACRED_GLYPH,
		MANACLE_TL,
		MANACLE_BR,
		MANACLE_TR,
		MANACLE_BL,
		MANACLE_T,
		MANACLE_B,
		MANACLE_L,
		MANACLE_R,
		PORTAL_LIGHT,
		GUARDIAN_GLOW,

		PLAIN_FIRE,
		BRIMSTONE_FIRE,
		FLAMEDANCER_FIRE,
		GAS_FIRE,
		GAS_EXPLOSION,
		DART_EXPLOSION,
		ITEM_FIRE,
		CREATURE_FIRE,

		POISON_GAS,
		CONFUSION_GAS,
		ROT_GAS,
		STENCH_SMOKE_GAS,
		PARALYSIS_GAS,
		METHANE_GAS,
		STEAM,
		DARKNESS_CLOUD,
		HEALING_CLOUD,

		BLOODFLOWER_STALK,
		BLOODFLOWER_POD,

		HAVEN_BEDROLL,

		DEEP_WATER_ALGAE_WELL,
		DEEP_WATER_ALGAE_1,
		DEEP_WATER_ALGAE_2,

		ANCIENT_SPIRIT_VINES,
		ANCIENT_SPIRIT_GRASS,

		AMULET_SWITCH,

		COMMUTATION_ALTAR,
		COMMUTATION_ALTAR_INERT,
		PIPE_GLOWING,
		PIPE_INERT,

		RESURRECTION_ALTAR,
		RESURRECTION_ALTAR_INERT,
		MACHINE_TRIGGER_FLOOR_REPEATING,

		SACRIFICE_ALTAR_DORMANT,
		SACRIFICE_ALTAR,
		SACRIFICE_LAVA,
		SACRIFICE_CAGE_DORMANT,
		DEMONIC_STATUE,

		STATUE_INERT_DOORWAY,
		STATUE_DORMANT_DOORWAY,

		CHASM_WITH_HIDDEN_BRIDGE,
		CHASM_WITH_HIDDEN_BRIDGE_ACTIVE,
		MACHINE_CHASM_EDGE,

		RAT_TRAP_WALL_DORMANT,
		RAT_TRAP_WALL_CRACKING,

		ELECTRIC_CRYSTAL_OFF,
		ELECTRIC_CRYSTAL_ON,
		TURRET_LEVER,

		WORM_TUNNEL_MARKER_DORMANT,
		WORM_TUNNEL_MARKER_ACTIVE,
		WORM_TUNNEL_OUTER_WALL,

		BRAZIER,

		MUD_FLOOR,
		MUD_WALL,
		MUD_DOORWAY
	};
}
