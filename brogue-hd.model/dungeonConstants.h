#pragma once

#include <simpleMacros.h>

namespace brogueHd::model
{
	using namespace simple;

	//enum levelTypes : int
	//{
	//	DP_BASIC,
	//	DP_BASIC_FIRST_ROOM,

	//	DP_GOBLIN_WARREN,
	//	DP_SENTINEL_SANCTUARY
	//};


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

	enum class brogueLevelTilingType
	{
		/// <summary>
		/// Specific tiling type to the first level. Uses bottom-center focus.
		/// </summary>
		MainEntrance = 0,

		/// <summary>
		/// Similar to main entrance; but the layoutGenerator chooses a random
		/// edge on which to put the entrance staircase.
		/// </summary>
		RandomLevelEdge = 1,

		/// <summary>
		/// Similar to random edge; but does not enforce tile packing against
		/// the level boundary.
		/// </summary>
		RandomOuterRoom = 2,

		/// <summary>
		/// The entrance staircase is centered in the level; and the rest of
		/// the tiles are packed against it.
		/// </summary>
		CenterFocus = 3,

		/// <summary>
		/// Chooses two opposing corners; and packs the entrance, and exit tiles,
		/// against those, taking turns as the level rooms are placed.
		/// </summary>
		RandomDualFocusCorners = 4,

		/// <summary>
		/// Chooses two opposing sides; and packs the entrance, and exit tiles,
		/// against those, taking turns as the level rooms are placed.
		/// </summary>
		RandomDualFocusEdges = 5
	};

	enum class brogueMazeType
	{
		/// <summary>
		/// Specifies counting rule with more "open" feel
		/// </summary>
		Open,

		/// <summary>
		/// Specifies counting rule with more boxy, or "closed in" feel
		/// </summary>
		Filled
	};

	// Dungeon flags
	enum tileFlags
	{
		DISCOVERED = EnumFlag(0),
		VISIBLE = EnumFlag(1), // cell has sufficient light and is in field of view, ready to draw.
		HAS_PLAYER = EnumFlag(2),
		HAS_MONSTER = EnumFlag(3),
		HAS_DORMANT_MONSTER = EnumFlag(4), // hidden monster on the square
		HAS_ITEM = EnumFlag(5),
		IN_FIELD_OF_VIEW = EnumFlag(6), // player has unobstructed line of sight whether or not there is enough light
		WAS_VISIBLE = EnumFlag(7),
		HAS_STAIRS = EnumFlag(8),
		SEARCHED_FROM_HERE = EnumFlag(9), // player already auto-searched here; can't auto-search here again
		IS_IN_SHADOW = EnumFlag(10), // so that a player gains an automatic stealth bonus
		MAGIC_MAPPED = EnumFlag(11),
		ITEM_DETECTED = EnumFlag(12),
		CLAIRVOYANT_VISIBLE = EnumFlag(13),
		WAS_CLAIRVOYANT_VISIBLE = EnumFlag(14),
		CLAIRVOYANT_DARKENED = EnumFlag(15), // magical blindness from a cursed ring of clairvoyance
		CAUGHT_FIRE_THIS_TURN = EnumFlag(16), // so that fire does not spread asymmetrically
		PRESSURE_PLATE_DEPRESSED = EnumFlag(17), // so that traps do not trigger repeatedly while you stand on them
		STABLE_MEMORY = EnumFlag(18), // redraws will be pulled from the memory array, not recalculated
		KNOWN_TO_BE_TRAP_FREE = EnumFlag(19),
		// keep track of where the player has stepped or watched monsters step as he knows no traps are there
		IS_IN_PATH = EnumFlag(20), // the yellow trail leading to the cursor
		IN_LOOP = EnumFlag(21), // this cell is part of a terrain loop
		IS_CHOKEPOINT = EnumFlag(22), // if this cell is blocked, part of the map will be rendered inaccessible
		IS_GATE_SITE = EnumFlag(23), // consider placing a locked door here
		IS_IN_ROOM_MACHINE = EnumFlag(24),
		IS_IN_AREA_MACHINE = EnumFlag(25),
		IS_POWERED = EnumFlag(26),
		// has been activated by machine power this turn (flag can probably be eliminated if needed)
		IMPREGNABLE = EnumFlag(27), // no tunneling allowed!
		TERRAIN_COLORS_DANCING = EnumFlag(28), // colors here will sparkle when the game is idle
		TELEPATHIC_VISIBLE = EnumFlag(29), // potions of telepathy let you see through other creatures' eyes
		WAS_TELEPATHIC_VISIBLE = EnumFlag(30), // potions of telepathy let you see through other creatures' eyes

		IS_IN_MACHINE = (IS_IN_ROOM_MACHINE | IS_IN_AREA_MACHINE),
		// sacred ground; don't generate items here, or teleport randomly to it

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
		WALL = EnumFlag(1),

		// Door tiles: machine or otherwise; locked or unlocked
		DOOR = EnumFlag(2),

		// Shallow water:  permanent; and on the edges of the water region
		WATER_SHALLOW = EnumFlag(3),

		// Deep water: permanent. Dynamic layers are added on top of permanent layers
		WATER_DEEP = EnumFlag(4),

		// Edge of a swamp:  permanent, and walkable
		SWAMP_SHALLOW = EnumFlag(5),

		// Interior of a swamp:  permanent, and not easily walkable
		SWAMP_DEEP = EnumFlag(6),

		// Lava:  permanent
		LAVA = EnumFlag(7),

		// Chasm:  permanent. Edges of a chasm are NOT a chasm - they are a dungeon tile feature.
		CHASM = EnumFlag(8)
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
		FIRE = EnumFlag(1),

		// Gas:  All gas types are considered a hazard
		GAS = EnumFlag(2),

		// Net:  Net traps will leave these
		NET = EnumFlag(3),

		// Spider's Web:  These are temporary grid alterations
		SPIDER_WEB = EnumFlag(4),

		// Temporary Water:  Pools of water will be stored when traps are set
		WATER = EnumFlag(5)
	};

	/// <summary>
	/// Enumeration designed to specify flags for sub categories. Each flag should
	/// be accompanied by another enumeration for that category. (refactoring in progress)
	/// </summary>
	enum dungeonFeatureCategories
	{
		FEATURE_DECORATION = EnumFlag(1),
		FEATURE_WALL = EnumFlag(2),
		FEATURE_FOLIAGE = EnumFlag(3),
		FEATURE_LIGHT = EnumFlag(4),
		FEATURE_TRAP = EnumFlag(5),
		FEATURE_BODILY_FLUIDS = EnumFlag(6),
		FEATURE_ENEMY = EnumFlag(7),
		FEATURE_EXPLOSION = EnumFlag(8),
		FEATURE_MAGIC = EnumFlag(9),
		FEATURE_GAS = EnumFlag(10),
		FEATURE_POTION = EnumFlag(11),
		FEATURE_RUBBLE = EnumFlag(12),
		FEATURE_INTERACTION = EnumFlag(13),
		FEATURE_BRIMSTONE = EnumFlag(14),
		FEATURE_DEWARS = EnumFlag(15),
		FEATURE_DOOR = EnumFlag(16),
		FEATURE_OTHER_DOOR = EnumFlag(17),
		FEATURE_ALTAR = EnumFlag(18),
		FEATURE_SWAMP = EnumFlag(19),
		FEATURE_CHASM = EnumFlag(20),
		FEATURE_FIRE = EnumFlag(21),
		FEATURE_OTHER_ITEMS = EnumFlag(22),
		FEATURE_STENCH = EnumFlag(23)
	};

	/// <summary>
	/// Decorations that don't otherwise have a category. These are all benign.
	/// </summary>
	enum dungeonFeatureDecoration
	{
		CARPET = EnumFlag(1),

		// camp:
		HAY = EnumFlag(2),
		JUNK = EnumFlag(3)
	};

	enum dungeonFeatureWall
	{
		GRANITE_COLUMN = EnumFlag(1),
		CRYSTAL_WALL = EnumFlag(2),

		// worm den:
		WALL_SHATTER = EnumFlag(3)
	};

	enum dungeonFeatureFoliage
	{
		ALGAE_1 = EnumFlag(1),
		ALGAE_2 = EnumFlag(2),
		ALGAE_1_FREEZE = EnumFlag(3),
		ALGAE_2_FREEZE = EnumFlag(4),
		ALGAE_REVERT = EnumFlag(5),
		BUILD_ALGAE_WELL = EnumFlag(6),

		BLOODFLOWER_PODS_GROW_INITIAL = EnumFlag(7),
		BLOODFLOWER_PODS_GROW = EnumFlag(8),
		BLOODFLOWER_POD_BURST = EnumFlag(9),

		DEAD_FOLIAGE = EnumFlag(10),
		DEAD_GRASS = EnumFlag(11),
		DEAD_GRASS_SMALL = EnumFlag(12),

		FOLIAGE = EnumFlag(13),
		FOLIAGE_REGROW = EnumFlag(14),
		FOLIAGE_TRAMPLED = EnumFlag(15),

		FUNGUS_FOREST = EnumFlag(16),
		FUNGUS_FOREST_REGROW = EnumFlag(17),
		FUNGUS_FOREST_TRAMPLED = EnumFlag(18),
		FUNGUS_LUMINESCENT = EnumFlag(19),

		GRASS = EnumFlag(20),

		LICHEN_GROW = EnumFlag(21),
		LICHEN_MUTATION = EnumFlag(22),
		LICHEN_PLANTED = EnumFlag(23)
	};

	enum dungeonFeatureLight
	{
		SUNLIGHT = EnumFlag(1),
		DARKNESS = EnumFlag(2)
	};

	enum dungeonFeatureTrap
	{
		POISON_GAS_TRAP = EnumFlag(1),
		PARALYSIS_GAS_TRAP = EnumFlag(2),
		TRAPDOOR = EnumFlag(3),
		CONFUSION_GAS_TRAP = EnumFlag(4),
		FLAMETHROWER_TRAP = EnumFlag(5),
		FLOOD_TRAP = EnumFlag(6),
		NET_TRAP = EnumFlag(7),
		ALARM_TRAP = EnumFlag(8)
	};

	enum dungeonFeatureBodilyFluids
	{
		RED_BLOOD = EnumFlag(1),
		GREEN_BLOOD = EnumFlag(2),
		PURPLE_BLOOD = EnumFlag(3),
		WORM_BLOOD = EnumFlag(4),
		ACID_BLOOD = EnumFlag(5),
		ASH_BLOOD = EnumFlag(6),
		EMBER_BLOOD = EnumFlag(7),
		ECTOPLASM_BLOOD = EnumFlag(8),
		RUBBLE_BLOOD = EnumFlag(9),
		ROT_GAS_BLOOD = EnumFlag(10),
		VOMIT = EnumFlag(11),
		URINE = EnumFlag(12),
		UNICORN_POOP = EnumFlag(13)
	};

	enum dungeonFeatureEnemy
	{
		FLAMEDANCER_CORONA = EnumFlag(1),

		SALAMANDER_FLAME = EnumFlag(2),

		WEB_SMALL = EnumFlag(3),
		WEB_LARGE = EnumFlag(4),

		ANCIENT_SPIRIT_VINES = EnumFlag(5),
		ANCIENT_SPIRIT_GRASS = EnumFlag(6),

		// remnants:
		REMNANT = EnumFlag(7),
		REMNANT_ASH = EnumFlag(8)
	};

	enum dungeonFeatureExplosion
	{
		BLOAT_EXPLOSION = EnumFlag(1),
		BLOOD_EXPLOSION = EnumFlag(2),
		MUTATION_EXPLOSION = EnumFlag(3)
	};

	enum dungeonFeatureMagic
	{
		REPEL_CREATURES = EnumFlag(1),
		SHATTERING_SPELL = EnumFlag(2),
		ECTOPLASM_DROPLET = EnumFlag(3),
		FORCEFIELD = EnumFlag(4),
		FORCEFIELD_MELT = EnumFlag(5)
	};

	enum dungeonFeatureGas
	{
		ROT_GAS_PUFF = EnumFlag(1),
		STEAM_PUFF = EnumFlag(2),
		STEAM_ACCUMULATION = EnumFlag(3),
		METHANE_GAS_PUFF = EnumFlag(4),

		POISON_GAS_CLOUD = EnumFlag(5),
		CONFUSION_GAS_TRAP_CLOUD = EnumFlag(6),
		METHANE_GAS_ARMAGEDDON = EnumFlag(7),
	};

	enum dungeonFeaturePotion
	{
		// potions
		POISON_GAS_CLOUD_POTION = EnumFlag(1),
		PARALYSIS_GAS_CLOUD_POTION = EnumFlag(2),
		CONFUSION_GAS_CLOUD_POTION = EnumFlag(3),
		INCINERATION_POTION = EnumFlag(4),
		DARKNESS_POTION = EnumFlag(5),
		HOLE_POTION = EnumFlag(6)
	};

	enum dungeonFeatureRubble
	{
		ASH = EnumFlag(1),
		TUNNELIZE = EnumFlag(2)
	};

	enum dungeonFeatureInteraction
	{
		// Machine
		SACRED_GLYPHS = EnumFlag(1),
		SILENT_GLYPH_GLOW = EnumFlag(2),
		GUARDIAN_STEP = EnumFlag(3),
		MIRROR_TOTEM_STEP = EnumFlag(4),
		GLYPH_INACTIVE = EnumFlag(5),
		GLYPH_CIRCLE = EnumFlag(6),
		GLYPH_ACTIVE = EnumFlag(7),
		ITEM_CAGE_OPEN = EnumFlag(8),
		ITEM_CAGE_CLOSED = EnumFlag(9),

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
		BRIMSTONE_ACTIVE = EnumFlag(1),
		BRIMSTONE_INERT = EnumFlag(2)
	};

	enum dungeonFeatureDewars
	{
		DEWAR_CAUSTIC = EnumFlag(1),
		DEWAR_CONFUSION = EnumFlag(2),
		DEWAR_PARALYSIS = EnumFlag(3),
		DEWAR_METHANE = EnumFlag(4),
		DEWAR_GLASS = EnumFlag(5)
	};

	enum dungeonFeatureDoor
	{
		DOOR_OPEN = EnumFlag(1),
		DOOR_CLOSED = EnumFlag(2),
	};

	enum dungeonFeatureOtherDoor
	{
		IRON_DOOR_OPEN_INERT = EnumFlag(1),
		PORTAL_ACTIVATE = EnumFlag(2)
	};

	enum dungeonFeatureAltar
	{
		ALTAR_INERT = EnumFlag(1),
		ALTER_RETRACT = EnumFlag(2)
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
		HOLE_2 = EnumFlag(1),
		HOLE_DRAIN = EnumFlag(2),

		// chasm catwalk:
		CHASM_HOLE,
		CATWALK_BRIDGE,
	};

	enum dungeonFeatureFire
	{
		PLAIN_FIRE = EnumFlag(1),
		GAS_FIRE = EnumFlag(2),
		EXPLOSION_FIRE = EnumFlag(3),
		DART_EXPLOSION = EnumFlag(4),
		BRIMSTONE_FIRE = EnumFlag(5),
		BRIDGE_FIRE = EnumFlag(6),
		FLAMETHROWER = EnumFlag(7),
		EMBERS = EnumFlag(8),
		EMBERS_PATCH = EnumFlag(9),
		OBSIDIAN = EnumFlag(10),
		ITEM_FIRE = EnumFlag(11),
		CREATURE_FIRE = EnumFlag(12)
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
