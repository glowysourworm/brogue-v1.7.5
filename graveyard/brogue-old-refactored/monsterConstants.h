#pragma once

enum monsterTypes {
	MK_YOU,
	MK_RAT,
	MK_KOBOLD,
	MK_JACKAL,
	MK_EEL,
	MK_MONKEY,
	MK_BLOAT,
	MK_PIT_BLOAT,
	MK_GOBLIN,
	MK_GOBLIN_CONJURER,
	MK_GOBLIN_MYSTIC,
	MK_GOBLIN_TOTEM,
	MK_PINK_JELLY,
	MK_TOAD,
	MK_VAMPIRE_BAT,
	MK_ARROW_TURRET,
	MK_ACID_MOUND,
	MK_CENTIPEDE,
	MK_OGRE,
	MK_BOG_MONSTER,
	MK_OGRE_TOTEM,
	MK_SPIDER,
	MK_SPARK_TURRET,
	MK_WILL_O_THE_WISP,
	MK_WRAITH,
	MK_ZOMBIE,
	MK_TROLL,
	MK_OGRE_SHAMAN,
	MK_NAGA,
	MK_SALAMANDER,
	MK_EXPLOSIVE_BLOAT,
	MK_DAR_BLADEMASTER,
	MK_DAR_PRIESTESS,
	MK_DAR_BATTLEMAGE,
	MK_ACID_JELLY,
	MK_CENTAUR,
	MK_UNDERWORM,
	MK_SENTINEL,
	MK_DART_TURRET,
	MK_KRAKEN,
	MK_LICH,
	MK_PHYLACTERY,
	MK_PIXIE,
	MK_PHANTOM,
	MK_FLAME_TURRET,
	MK_IMP,
	MK_FURY,
	MK_REVENANT,
	MK_TENTACLE_HORROR,
	MK_GOLEM,
	MK_DRAGON,

	MK_GOBLIN_CHIEFTAN,
	MK_BLACK_JELLY,
	MK_VAMPIRE,
	MK_FLAMEDANCER,

	MK_SPECTRAL_BLADE,
	MK_SPECTRAL_IMAGE,
	MK_GUARDIAN,
	MK_WINGED_GUARDIAN,
	MK_CHARM_GUARDIAN,
	MK_WARDEN_OF_YENDOR,
	MK_ELDRITCH_TOTEM,
	MK_MIRRORED_TOTEM,

	MK_UNICORN,
	MK_IFRIT,
	MK_PHOENIX,
	MK_PHOENIX_EGG,
	MK_ANCIENT_SPIRIT,

	NUMBER_MONSTER_KINDS
};

// Dungeon flags
enum tileFlags {
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

enum dungeonLayers {
	NO_LAYER = -1,
	DUNGEON = 0,		// dungeon-level tile	(e.g. walls)
	LIQUID,				// liquid-level tile	(e.g. lava)
	GAS,				// gas-level tile		(e.g. fire, smoke, swamp gas)
	SURFACE,			// surface-level tile	(e.g. grass)
	NUMBER_TERRAIN_LAYERS
};


enum itemFlags {
	ITEM_IDENTIFIED = Fl(0),
	ITEM_EQUIPPED = Fl(1),
	ITEM_CURSED = Fl(2),
	ITEM_PROTECTED = Fl(3),
	// unused               = Fl(4),
	ITEM_RUNIC = Fl(5),
	ITEM_RUNIC_HINTED = Fl(6),
	ITEM_RUNIC_IDENTIFIED = Fl(7),
	ITEM_CAN_BE_IDENTIFIED = Fl(8),
	ITEM_PREPLACED = Fl(9),
	ITEM_FLAMMABLE = Fl(10),
	ITEM_MAGIC_DETECTED = Fl(11),
	ITEM_MAX_CHARGES_KNOWN = Fl(12),
	ITEM_IS_KEY = Fl(13),

	ITEM_ATTACKS_STAGGER = Fl(14),	// mace, hammer
	ITEM_ATTACKS_EXTEND = Fl(15),   // whip
	ITEM_ATTACKS_QUICKLY = Fl(16),   // rapier
	ITEM_ATTACKS_PENETRATE = Fl(17),	// spear, pike
	ITEM_ATTACKS_ALL_ADJACENT = Fl(18),	// axe, war axe
	ITEM_LUNGE_ATTACKS = Fl(19),   // rapier
	ITEM_SNEAK_ATTACK_BONUS = Fl(20),   // dagger
	ITEM_PASS_ATTACKS = Fl(21),   // flail

	ITEM_KIND_AUTO_ID = Fl(22),	// the item type will become known when the item is picked up.
	ITEM_PLAYER_AVOIDS = Fl(23),	// explore and travel will try to avoid picking the item up
};

enum dungeonFeatureTypes {
	DF_GRANITE_COLUMN = 1,
	DF_CRYSTAL_WALL,
	DF_LUMINESCENT_FUNGUS,
	DF_GRASS,
	DF_DEAD_GRASS,
	DF_BONES,
	DF_RUBBLE,
	DF_FOLIAGE,
	DF_FUNGUS_FOREST,
	DF_DEAD_FOLIAGE,

	DF_SUNLIGHT,
	DF_DARKNESS,

	DF_SHOW_DOOR,
	DF_SHOW_POISON_GAS_TRAP,
	DF_SHOW_PARALYSIS_GAS_TRAP,
	DF_SHOW_TRAPDOOR_HALO,
	DF_SHOW_TRAPDOOR,
	DF_SHOW_CONFUSION_GAS_TRAP,
	DF_SHOW_FLAMETHROWER_TRAP,
	DF_SHOW_FLOOD_TRAP,
	DF_SHOW_NET_TRAP,
	DF_SHOW_ALARM_TRAP,

	DF_RED_BLOOD,
	DF_GREEN_BLOOD,
	DF_PURPLE_BLOOD,
	DF_WORM_BLOOD,
	DF_ACID_BLOOD,
	DF_ASH_BLOOD,
	DF_EMBER_BLOOD,
	DF_ECTOPLASM_BLOOD,
	DF_RUBBLE_BLOOD,
	DF_ROT_GAS_BLOOD,

	DF_VOMIT,
	DF_BLOAT_DEATH,
	DF_BLOAT_EXPLOSION,
	DF_BLOOD_EXPLOSION,
	DF_FLAMEDANCER_CORONA,

	DF_MUTATION_EXPLOSION,
	DF_MUTATION_LICHEN,

	DF_REPEL_CREATURES,
	DF_ROT_GAS_PUFF,
	DF_STEAM_PUFF,
	DF_STEAM_ACCUMULATION,
	DF_METHANE_GAS_PUFF,
	DF_SALAMANDER_FLAME,
	DF_URINE,
	DF_UNICORN_POOP,
	DF_PUDDLE,
	DF_ASH,
	DF_ECTOPLASM_DROPLET,
	DF_FORCEFIELD,
	DF_FORCEFIELD_MELT,
	DF_SACRED_GLYPHS,
	DF_LICHEN_GROW,
	DF_TUNNELIZE,
	DF_SHATTERING_SPELL,

	// spiderwebs
	DF_WEB_SMALL,
	DF_WEB_LARGE,

	// ancient spirit
	DF_ANCIENT_SPIRIT_VINES,
	DF_ANCIENT_SPIRIT_GRASS,

	// foliage
	DF_TRAMPLED_FOLIAGE,
	DF_SMALL_DEAD_GRASS,
	DF_FOLIAGE_REGROW,
	DF_TRAMPLED_FUNGUS_FOREST,
	DF_FUNGUS_FOREST_REGROW,

	// brimstone
	DF_ACTIVE_BRIMSTONE,
	DF_INERT_BRIMSTONE,

	// bloodwort
	DF_BLOODFLOWER_PODS_GROW_INITIAL,
	DF_BLOODFLOWER_PODS_GROW,
	DF_BLOODFLOWER_POD_BURST,

	// dewars
	DF_DEWAR_CAUSTIC,
	DF_DEWAR_CONFUSION,
	DF_DEWAR_PARALYSIS,
	DF_DEWAR_METHANE,
	DF_DEWAR_GLASS,
	DF_CARPET_AREA,

	// algae
	DF_BUILD_ALGAE_WELL,
	DF_ALGAE_1,
	DF_ALGAE_2,
	DF_ALGAE_REVERT,

	DF_OPEN_DOOR,
	DF_CLOSED_DOOR,
	DF_OPEN_IRON_DOOR_INERT,
	DF_ITEM_CAGE_OPEN,
	DF_ITEM_CAGE_CLOSE,
	DF_ALTAR_INERT,
	DF_ALTAR_RETRACT,
	DF_PORTAL_ACTIVATE,
	DF_INACTIVE_GLYPH,
	DF_ACTIVE_GLYPH,
	DF_SILENT_GLYPH_GLOW,
	DF_GUARDIAN_STEP,
	DF_MIRROR_TOTEM_STEP,
	DF_GLYPH_CIRCLE,
	DF_REVEAL_LEVER,
	DF_PULL_LEVER,
	DF_CREATE_LEVER,

	DF_BRIDGE_FALL_PREP,
	DF_BRIDGE_FALL,

	DF_PLAIN_FIRE,
	DF_GAS_FIRE,
	DF_EXPLOSION_FIRE,
	DF_DART_EXPLOSION,
	DF_BRIMSTONE_FIRE,
	DF_BRIDGE_FIRE,
	DF_FLAMETHROWER,
	DF_EMBERS,
	DF_EMBERS_PATCH,
	DF_OBSIDIAN,
	DF_ITEM_FIRE,
	DF_CREATURE_FIRE,

	DF_FLOOD,
	DF_FLOOD_2,
	DF_FLOOD_DRAIN,
	DF_HOLE_2,
	DF_HOLE_DRAIN,

	DF_DEEP_WATER_FREEZE,
	DF_ALGAE_1_FREEZE,
	DF_ALGAE_2_FREEZE,
	DF_DEEP_WATER_MELTING,
	DF_DEEP_WATER_THAW,
	DF_SHALLOW_WATER_FREEZE,
	DF_SHALLOW_WATER_MELTING,
	DF_SHALLOW_WATER_THAW,

	DF_POISON_GAS_CLOUD,
	DF_CONFUSION_GAS_TRAP_CLOUD,
	DF_NET,
	DF_AGGRAVATE_TRAP,
	DF_METHANE_GAS_ARMAGEDDON,

	// potions
	DF_POISON_GAS_CLOUD_POTION,
	DF_PARALYSIS_GAS_CLOUD_POTION,
	DF_CONFUSION_GAS_CLOUD_POTION,
	DF_INCINERATION_POTION,
	DF_DARKNESS_POTION,
	DF_HOLE_POTION,
	DF_LICHEN_PLANTED,

	// other items
	DF_ARMOR_IMMOLATION,
	DF_STAFF_HOLE,
	DF_STAFF_HOLE_EDGE,

	// commutation altar
	DF_ALTAR_COMMUTE,
	DF_MAGIC_PIPING,
	DF_INERT_PIPE,

	// resurrection altar
	DF_ALTAR_RESURRECT,
	DF_MACHINE_FLOOR_TRIGGER_REPEATING,

	// sacrifice altar
	DF_SACRIFICE_ALTAR,
	DF_SACRIFICE_COMPLETE,
	DF_SACRIFICE_CAGE_ACTIVE,

	// vampire in coffin
	DF_COFFIN_BURSTS,
	DF_COFFIN_BURNS,
	DF_TRIGGER_AREA,

	// throwing tutorial -- button in chasm
	DF_CAGE_DISAPPEARS,
	DF_MEDIUM_HOLE,
	DF_MEDIUM_LAVA_POND,
	DF_MACHINE_PRESSURE_PLATE_USED,

	// rat trap
	DF_WALL_CRACK,

	// wooden barricade at entrance
	DF_WOODEN_BARRICADE_BURN,

	// wooden barricade around altar, dead grass all around
	DF_SURROUND_WOODEN_BARRICADE,

	// pools of water that, when triggered, slowly expand to fill the room
	DF_SPREADABLE_WATER,
	DF_SHALLOW_WATER,
	DF_WATER_SPREADS,
	DF_SPREADABLE_WATER_POOL,
	DF_SPREADABLE_DEEP_WATER_POOL,

	// when triggered, the ground gradually turns into chasm:
	DF_SPREADABLE_COLLAPSE,
	DF_COLLAPSE,
	DF_COLLAPSE_SPREADS,
	DF_ADD_MACHINE_COLLAPSE_EDGE_DORMANT,

	// when triggered, a bridge appears:
	DF_BRIDGE_ACTIVATE,
	DF_BRIDGE_ACTIVATE_ANNOUNCE,
	DF_BRIDGE_APPEARS,
	DF_ADD_DORMANT_CHASM_HALO,

	// when triggered, the lava retracts:
	DF_LAVA_RETRACTABLE,
	DF_RETRACTING_LAVA,
	DF_OBSIDIAN_WITH_STEAM,

	// when triggered, the door seals and caustic gas fills the room
	DF_SHOW_POISON_GAS_VENT,
	DF_POISON_GAS_VENT_OPEN,
	DF_ACTIVATE_PORTCULLIS,
	DF_OPEN_PORTCULLIS,
	DF_VENT_SPEW_POISON_GAS,

	// when triggered, pilot light ignites and explosive gas fills the room
	DF_SHOW_METHANE_VENT,
	DF_METHANE_VENT_OPEN,
	DF_VENT_SPEW_METHANE,
	DF_PILOT_LIGHT,

	// paralysis trap: trigger plate with gas vents nearby
	DF_DISCOVER_PARALYSIS_VENT,
	DF_PARALYSIS_VENT_SPEW,
	DF_REVEAL_PARALYSIS_VENT_SILENTLY,

	// thematic dungeon
	DF_AMBIENT_BLOOD,

	// statues crack for a few turns and then shatter, revealing the monster inside
	DF_CRACKING_STATUE,
	DF_STATUE_SHATTER,

	// a turret appears:
	DF_TURRET_EMERGE,

	// an elaborate worm catacomb opens up
	DF_WORM_TUNNEL_MARKER_DORMANT,
	DF_WORM_TUNNEL_MARKER_ACTIVE,
	DF_GRANITE_CRUMBLES,
	DF_WALL_OPEN,

	// the room gradually darkens
	DF_DARKENING_FLOOR,
	DF_DARK_FLOOR,
	DF_HAUNTED_TORCH_TRANSITION,
	DF_HAUNTED_TORCH,

	// bubbles rise from the mud and bog monsters spawn
	DF_MUD_DORMANT,
	DF_MUD_ACTIVATE,

	// crystals charge when hit by lightning
	DF_ELECTRIC_CRYSTAL_ON,
	DF_TURRET_LEVER,

	// idyll:
	DF_SHALLOW_WATER_POOL,
	DF_DEEP_WATER_POOL,

	// swamp:
	DF_SWAMP_WATER,
	DF_SWAMP,
	DF_SWAMP_MUD,

	// camp:
	DF_HAY,
	DF_JUNK,

	// remnants:
	DF_REMNANT,
	DF_REMNANT_ASH,

	// chasm catwalk:
	DF_CHASM_HOLE,
	DF_CATWALK_BRIDGE,

	// lake catwalk:
	DF_LAKE_CELL,
	DF_LAKE_HALO,

	// worm den:
	DF_WALL_SHATTER,

	// monster cages open:
	DF_MONSTER_CAGE_OPENS,

	// goblin warren:
	DF_STENCH_BURN,
	DF_STENCH_SMOLDER,

	NUMBER_DUNGEON_FEATURES,
};

enum dungeonProfileTypes {
	DP_BASIC,
	DP_BASIC_FIRST_ROOM,

	DP_GOBLIN_WARREN,
	DP_SENTINEL_SANCTUARY,

	NUMBER_DUNGEON_PROFILES,
};

enum DFFlags {
	DFF_EVACUATE_CREATURES_FIRST = Fl(0),	// Creatures in the DF area get moved outside of it
	DFF_SUBSEQ_EVERYWHERE = Fl(1),	// Subsequent DF spawns in every cell that this DF spawns in, instead of only the origin
	DFF_TREAT_AS_BLOCKING = Fl(2),	// If filling the footprint of this DF with walls would disrupt level connectivity, then abort.
	DFF_PERMIT_BLOCKING = Fl(3),	// Generate this DF without regard to level connectivity.
	DFF_ACTIVATE_DORMANT_MONSTER = Fl(4),	// Dormant monsters on this tile will appear -- e.g. when a statue bursts to reveal a monster.
	DFF_CLEAR_OTHER_TERRAIN = Fl(5),	// Erase other terrain in the footprint of this DF.
	DFF_BLOCKED_BY_OTHER_LAYERS = Fl(6),	// Will not propagate into a cell if any layer in that cell has a superior priority.
	DFF_SUPERPRIORITY = Fl(7),	// Will overwrite terrain of a superior priority.
	DFF_AGGRAVATES_MONSTERS = Fl(8),    // Will act as though an aggravate monster scroll of effectRadius radius had been read at that point.
	DFF_RESURRECT_ALLY = Fl(9),    // Will bring back to life your most recently deceased ally.
};

enum boltEffects {
	BE_NONE,
	BE_ATTACK,
	BE_TELEPORT,
	BE_SLOW,
	BE_POLYMORPH,
	BE_NEGATION,
	BE_DOMINATION,
	BE_BECKONING,
	BE_PLENTY,
	BE_INVISIBILITY,
	BE_EMPOWERMENT,
	BE_DAMAGE,
	BE_POISON,
	BE_TUNNELING,
	BE_BLINKING,
	BE_ENTRANCEMENT,
	BE_OBSTRUCTION,
	BE_DISCORD,
	BE_CONJURATION,
	BE_HEALING,
	BE_HASTE,
	BE_SHIELDING,
};

enum boltFlags {
	BF_PASSES_THRU_CREATURES = Fl(0),	// Bolt continues through creatures (e.g. lightning and tunneling)
	BF_HALTS_BEFORE_OBSTRUCTION = Fl(1),    // Bolt takes effect the space before it terminates (e.g. conjuration, obstruction, blinking)
	BF_TARGET_ALLIES = Fl(2),    // Staffs/wands/creatures that shoot this bolt will auto-target allies.
	BF_TARGET_ENEMIES = Fl(3),    // Staffs/wands/creatures that shoot this bolt will auto-target enemies.
	BF_FIERY = Fl(4),    // Bolt will light flammable terrain on fire as it passes, and will ignite monsters hit.
	BF_NEVER_REFLECTS = Fl(6),    // Bolt will never reflect (e.g. spiderweb, arrows).
	BF_NOT_LEARNABLE = Fl(7),    // This technique cannot be absorbed by empowered allies.
	BF_NOT_NEGATABLE = Fl(8),    // Won't be erased by negation.
	BF_ELECTRIC = Fl(9),    // Activates terrain that has TM_PROMOTES_ON_ELECTRICITY
	BF_DISPLAY_CHAR_ALONG_LENGTH = Fl(10),   // Display the character along the entire length of the bolt instead of just at the front.
};

enum hordeFlags {
	HORDE_DIES_ON_LEADER_DEATH = Fl(0),	// if the leader dies, the horde will die instead of electing new leader
	HORDE_IS_SUMMONED = Fl(1),	// minions summoned when any creature is the same species as the leader and casts summon
	HORDE_SUMMONED_AT_DISTANCE = Fl(2),    // summons will appear across the level, and will naturally path back to the leader
	HORDE_LEADER_CAPTIVE = Fl(3),	// the leader is in chains and the followers are guards
	HORDE_NO_PERIODIC_SPAWN = Fl(4),	// can spawn only when the level begins -- not afterwards
	HORDE_ALLIED_WITH_PLAYER = Fl(5),

	HORDE_MACHINE_BOSS = Fl(6),	// used in machines for a boss challenge
	HORDE_MACHINE_WATER_MONSTER = Fl(7),	// used in machines where the room floods with shallow water
	HORDE_MACHINE_CAPTIVE = Fl(8),	// powerful captive monsters without any captors
	HORDE_MACHINE_STATUE = Fl(9),	// the kinds of monsters that make sense in a statue
	HORDE_MACHINE_TURRET = Fl(10),	// turrets, for hiding in walls
	HORDE_MACHINE_MUD = Fl(11),	// bog monsters, for hiding in mud
	HORDE_MACHINE_KENNEL = Fl(12),	// monsters that can appear in cages in kennels
	HORDE_VAMPIRE_FODDER = Fl(13),	// monsters that are prone to capture and farming by vampires
	HORDE_MACHINE_LEGENDARY_ALLY = Fl(14),	// legendary allies
	HORDE_NEVER_OOD = Fl(15),   // Horde cannot be generated out of depth
	HORDE_MACHINE_THIEF = Fl(16),   // monsters that can be generated in the key thief area machines
	HORDE_MACHINE_GOBLIN_WARREN = Fl(17),   // can spawn in goblin warrens
	HORDE_SACRIFICE_TARGET = Fl(18),   // can be the target of an assassination challenge; leader will get scary light.

	HORDE_MACHINE_ONLY = (HORDE_MACHINE_BOSS | HORDE_MACHINE_WATER_MONSTER
	| HORDE_MACHINE_CAPTIVE | HORDE_MACHINE_STATUE
		| HORDE_MACHINE_TURRET | HORDE_MACHINE_MUD
		| HORDE_MACHINE_KENNEL | HORDE_VAMPIRE_FODDER
		| HORDE_MACHINE_LEGENDARY_ALLY | HORDE_MACHINE_THIEF
		| HORDE_MACHINE_GOBLIN_WARREN
		| HORDE_SACRIFICE_TARGET),
};

enum monsterBehaviorFlags {
	MONST_INVISIBLE = Fl(0),	// monster is invisible
	MONST_INANIMATE = Fl(1),	// monster has abbreviated stat bar display and is immune to many things
	MONST_IMMOBILE = Fl(2),	// monster won't move or perform melee attacks
	MONST_CARRY_ITEM_100 = Fl(3),	// monster carries an item 100% of the time
	MONST_CARRY_ITEM_25 = Fl(4),	// monster carries an item 25% of the time
	MONST_ALWAYS_HUNTING = Fl(5),	// monster is never asleep or in wandering mode
	MONST_FLEES_NEAR_DEATH = Fl(6),	// monster flees when under 25% health and re-engages when over 75%
	MONST_ATTACKABLE_THRU_WALLS = Fl(7),	// can be attacked when embedded in a wall
	MONST_DEFEND_DEGRADE_WEAPON = Fl(8),	// hitting the monster damages the weapon
	MONST_IMMUNE_TO_WEAPONS = Fl(9),	// weapons ineffective
	MONST_FLIES = Fl(10),	// permanent levitation
	MONST_FLITS = Fl(11),	// moves randomly a third of the time
	MONST_IMMUNE_TO_FIRE = Fl(12),	// won't burn, won't die in lava
	MONST_CAST_SPELLS_SLOWLY = Fl(13),	// takes twice the attack duration to cast a spell
	MONST_IMMUNE_TO_WEBS = Fl(14),	// monster passes freely through webs
	MONST_REFLECT_4 = Fl(15),	// monster reflects projectiles as though wearing +4 armor of reflection
	MONST_NEVER_SLEEPS = Fl(16),	// monster is always awake
	MONST_FIERY = Fl(17),	// monster carries an aura of flame (but no automatic fire light)
	MONST_INVULNERABLE = Fl(18),	// monster is immune to absolutely everything
	MONST_IMMUNE_TO_WATER = Fl(19),	// monster moves at full speed in deep water and (if player) doesn't drop items
	MONST_RESTRICTED_TO_LIQUID = Fl(20),	// monster can move only on tiles that allow submersion
	MONST_SUBMERGES = Fl(21),	// monster can submerge in appropriate terrain
	MONST_MAINTAINS_DISTANCE = Fl(22),	// monster tries to keep a distance of 3 tiles between it and player
	MONST_WILL_NOT_USE_STAIRS = Fl(23),	// monster won't chase the player between levels
	MONST_DIES_IF_NEGATED = Fl(24),	// monster will die if exposed to negation magic
	MONST_MALE = Fl(25),	// monster is male (or 50% likely to be male if also has MONST_FEMALE)
	MONST_FEMALE = Fl(26),	// monster is female (or 50% likely to be female if also has MONST_MALE)
	MONST_NOT_LISTED_IN_SIDEBAR = Fl(27),   // monster doesn't show up in the sidebar
	MONST_GETS_TURN_ON_ACTIVATION = Fl(28),   // monster never gets a turn, except when its machine is activated
	MONST_ALWAYS_USE_ABILITY = Fl(29),   // monster will never fail to use special ability if eligible (no random factor)
	MONST_NO_POLYMORPH = Fl(30),   // monster cannot result from a polymorph spell (liches, phoenixes and Warden of Yendor)

	NEGATABLE_TRAITS = (MONST_INVISIBLE | MONST_DEFEND_DEGRADE_WEAPON | MONST_IMMUNE_TO_WEAPONS | MONST_FLIES
	| MONST_FLITS | MONST_IMMUNE_TO_FIRE | MONST_REFLECT_4 | MONST_FIERY | MONST_MAINTAINS_DISTANCE),
	MONST_TURRET = (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE |
		MONST_ATTACKABLE_THRU_WALLS | MONST_WILL_NOT_USE_STAIRS),
	LEARNABLE_BEHAVIORS = (MONST_INVISIBLE | MONST_FLIES | MONST_IMMUNE_TO_FIRE | MONST_REFLECT_4),
	MONST_NEVER_VORPAL_ENEMY = (MONST_INANIMATE | MONST_INVULNERABLE | MONST_IMMOBILE | MONST_RESTRICTED_TO_LIQUID | MONST_GETS_TURN_ON_ACTIVATION | MONST_MAINTAINS_DISTANCE),
	MONST_NEVER_MUTATED = (MONST_INVISIBLE | MONST_INANIMATE | MONST_IMMOBILE | MONST_INVULNERABLE),
};

enum monsterAbilityFlags {
	MA_HIT_HALLUCINATE = Fl(0),	// monster can hit to cause hallucinations
	MA_HIT_STEAL_FLEE = Fl(1),	// monster can steal an item and then run away
	MA_HIT_BURN = Fl(2),    // monster can hit to set you on fire
	MA_ENTER_SUMMONS = Fl(3),	// monster will "become" its summoned leader, reappearing when that leader is defeated
	MA_HIT_DEGRADE_ARMOR = Fl(4),	// monster damages armor
	MA_CAST_SUMMON = Fl(5),	// requires that there be one or more summon hordes with this monster type as the leader
	MA_SEIZES = Fl(6),	// monster seizes enemies before attacking
	MA_POISONS = Fl(7),	// monster's damage is dealt in the form of poison
	MA_DF_ON_DEATH = Fl(8),	// monster spawns its DF when it dies
	MA_CLONE_SELF_ON_DEFEND = Fl(9),	// monster splits in two when struck
	MA_KAMIKAZE = Fl(10),	// monster dies instead of attacking
	MA_TRANSFERENCE = Fl(11),	// monster recovers 40 or 90% of the damage that it inflicts as health
	MA_CAUSES_WEAKNESS = Fl(12),	// monster attacks cause weakness status in target
	MA_ATTACKS_PENETRATE = Fl(13),   // monster attacks all adjacent enemies, like an axe
	MA_ATTACKS_ALL_ADJACENT = Fl(14),   // monster attacks penetrate one layer of enemies, like a spear
	MA_ATTACKS_EXTEND = Fl(15),   // monster attacks from a distance in a cardinal direction, like a whip
	MA_ATTACKS_STAGGER = Fl(16),   // monster attacks will push the player backward by one space if there is room
	MA_AVOID_CORRIDORS = Fl(17),   // monster will avoid corridors when hunting

	SPECIAL_HIT = (MA_HIT_HALLUCINATE | MA_HIT_STEAL_FLEE | MA_HIT_DEGRADE_ARMOR | MA_POISONS
	| MA_TRANSFERENCE | MA_CAUSES_WEAKNESS | MA_HIT_BURN | MA_ATTACKS_STAGGER),
	LEARNABLE_ABILITIES = (MA_TRANSFERENCE | MA_CAUSES_WEAKNESS),

	MA_NON_NEGATABLE_ABILITIES = (MA_ATTACKS_PENETRATE | MA_ATTACKS_ALL_ADJACENT | MA_ATTACKS_EXTEND | MA_ATTACKS_STAGGER),
	MA_NEVER_VORPAL_ENEMY = (MA_KAMIKAZE),
	MA_NEVER_MUTATED = (MA_KAMIKAZE),
};



enum monsterBookkeepingFlags {
	MB_WAS_VISIBLE = Fl(0),	// monster was visible to player last turn
	MB_TELEPATHICALLY_REVEALED = Fl(1),    // player can magically see monster and adjacent cells
	MB_PREPLACED = Fl(2),	// monster dropped onto the level and requires post-processing
	MB_APPROACHING_UPSTAIRS = Fl(3),	// following the player up the stairs
	MB_APPROACHING_DOWNSTAIRS = Fl(4),	// following the player down the stairs
	MB_APPROACHING_PIT = Fl(5),	// following the player down a pit
	MB_LEADER = Fl(6),	// monster is the leader of a horde
	MB_FOLLOWER = Fl(7),	// monster is a member of a horde
	MB_CAPTIVE = Fl(8),	// monster is all tied up
	MB_SEIZED = Fl(9),	// monster is being held
	MB_SEIZING = Fl(10),	// monster is holding another creature immobile
	MB_SUBMERGED = Fl(11),	// monster is currently submerged and hence invisible until it attacks
	MB_JUST_SUMMONED = Fl(12),	// used to mark summons so they can be post-processed
	MB_WILL_FLASH = Fl(13),	// this monster will flash as soon as control is returned to the player
	MB_BOUND_TO_LEADER = Fl(14),	// monster will die if the leader dies or becomes separated from the leader
	MB_MARKED_FOR_SACRIFICE = Fl(15),   // scary glow, monster can be sacrificed in the appropriate machine
	MB_ABSORBING = Fl(16),	// currently learning a skill by absorbing an enemy corpse
	MB_DOES_NOT_TRACK_LEADER = Fl(17),	// monster will not follow its leader around
	MB_IS_FALLING = Fl(18),	// monster is plunging downward at the end of the turn
	MB_IS_DYING = Fl(19),	// monster has already been killed and is awaiting the end-of-turn graveyard sweep (or in purgatory)
	MB_GIVEN_UP_ON_SCENT = Fl(20),	// to help the monster remember that the scent map is a dead end
	MB_IS_DORMANT = Fl(21),	// lurking, waiting to burst out
	MB_HAS_SOUL = Fl(22),   // slaying the monster will count toward weapon auto-ID
	MB_ALREADY_SEEN = Fl(23),   // seeing this monster won't interrupt exploration
};

enum terrainFlagCatalog {
	T_OBSTRUCTS_PASSABILITY = Fl(0),		// cannot be walked through
	T_OBSTRUCTS_VISION = Fl(1),		// blocks line of sight
	T_OBSTRUCTS_ITEMS = Fl(2),		// items can't be on this tile
	T_OBSTRUCTS_SURFACE_EFFECTS = Fl(3),		// grass, blood, etc. cannot exist on this tile
	T_OBSTRUCTS_GAS = Fl(4),		// blocks the permeation of gas
	T_OBSTRUCTS_DIAGONAL_MOVEMENT = Fl(5),        // can't step diagonally around this tile
	T_SPONTANEOUSLY_IGNITES = Fl(6),		// monsters avoid unless chasing player or immune to fire
	T_AUTO_DESCENT = Fl(7),		// automatically drops creatures down a depth level and does some damage (2d6)
	T_LAVA_INSTA_DEATH = Fl(8),		// kills any non-levitating non-fire-immune creature instantly
	T_CAUSES_POISON = Fl(9),		// any non-levitating creature gets 10 poison
	T_IS_FLAMMABLE = Fl(10),		// terrain can catch fire
	T_IS_FIRE = Fl(11),		// terrain is a type of fire; ignites neighboring flammable cells
	T_ENTANGLES = Fl(12),		// entangles players and monsters like a spiderweb
	T_IS_DEEP_WATER = Fl(13),		// steals items 50% of the time and moves them around randomly
	T_CAUSES_DAMAGE = Fl(14),		// anything on the tile takes max(1-2, 10%) damage per turn
	T_CAUSES_NAUSEA = Fl(15),		// any creature on the tile becomes nauseous
	T_CAUSES_PARALYSIS = Fl(16),		// anything caught on this tile is paralyzed
	T_CAUSES_CONFUSION = Fl(17),		// causes creatures on this tile to become confused
	T_CAUSES_HEALING = Fl(18),       // heals 20% max HP per turn for any player or non-inanimate monsters
	T_IS_DF_TRAP = Fl(19),		// spews gas of type specified in fireType when stepped on
	T_CAUSES_EXPLOSIVE_DAMAGE = Fl(20),		// is an explosion; deals higher of 15-20 or 50% damage instantly, but not again for five turns
	T_SACRED = Fl(21),       // monsters that aren't allies of the player will avoid stepping here

	T_OBSTRUCTS_SCENT = (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_VISION | T_AUTO_DESCENT | T_LAVA_INSTA_DEATH | T_IS_DEEP_WATER | T_SPONTANEOUSLY_IGNITES),
	T_PATHING_BLOCKER = (T_OBSTRUCTS_PASSABILITY | T_AUTO_DESCENT | T_IS_DF_TRAP | T_LAVA_INSTA_DEATH | T_IS_DEEP_WATER | T_IS_FIRE | T_SPONTANEOUSLY_IGNITES),
	T_DIVIDES_LEVEL = (T_OBSTRUCTS_PASSABILITY | T_AUTO_DESCENT | T_IS_DF_TRAP | T_LAVA_INSTA_DEATH | T_IS_DEEP_WATER),
	T_LAKE_PATHING_BLOCKER = (T_AUTO_DESCENT | T_LAVA_INSTA_DEATH | T_IS_DEEP_WATER | T_SPONTANEOUSLY_IGNITES),
	T_WAYPOINT_BLOCKER = (T_OBSTRUCTS_PASSABILITY | T_AUTO_DESCENT | T_IS_DF_TRAP | T_LAVA_INSTA_DEATH | T_IS_DEEP_WATER | T_SPONTANEOUSLY_IGNITES),
	T_MOVES_ITEMS = (T_IS_DEEP_WATER | T_LAVA_INSTA_DEATH),
	T_CAN_BE_BRIDGED = (T_AUTO_DESCENT),
	T_OBSTRUCTS_EVERYTHING = (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_VISION | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS | T_OBSTRUCTS_DIAGONAL_MOVEMENT),
	T_HARMFUL_TERRAIN = (T_CAUSES_POISON | T_IS_FIRE | T_CAUSES_DAMAGE | T_CAUSES_PARALYSIS | T_CAUSES_CONFUSION | T_CAUSES_EXPLOSIVE_DAMAGE),
	T_RESPIRATION_IMMUNITIES = (T_CAUSES_DAMAGE | T_CAUSES_CONFUSION | T_CAUSES_PARALYSIS | T_CAUSES_NAUSEA),
};

enum terrainMechanicalFlagCatalog {
	TM_IS_SECRET = Fl(0),		// successful search or being stepped on while visible transforms it into discoverType
	TM_PROMOTES_WITH_KEY = Fl(1),		// promotes if the key is present on the tile (in your pack, carried by monster, or lying on the ground)
	TM_PROMOTES_WITHOUT_KEY = Fl(2),		// promotes if the key is NOT present on the tile (in your pack, carried by monster, or lying on the ground)
	TM_PROMOTES_ON_STEP = Fl(3),		// promotes when a creature, player or item is on the tile (whether or not levitating)
	TM_PROMOTES_ON_ITEM_PICKUP = Fl(4),		// promotes when an item is lifted from the tile (primarily for altars)
	TM_PROMOTES_ON_PLAYER_ENTRY = Fl(5),		// promotes when the player enters the tile (whether or not levitating)
	TM_PROMOTES_ON_SACRIFICE_ENTRY = Fl(6),		// promotes when the sacrifice target enters the tile (whether or not levitating)
	TM_PROMOTES_ON_ELECTRICITY = Fl(7),        // promotes when hit by a lightning bolt
	TM_ALLOWS_SUBMERGING = Fl(8),		// allows submersible monsters to submerge in this terrain
	TM_IS_WIRED = Fl(9),		// if wired, promotes when powered, and sends power when promoting
	TM_IS_CIRCUIT_BREAKER = Fl(10),       // prevents power from circulating in its machine
	TM_GAS_DISSIPATES = Fl(11),		// does not just hang in the air forever
	TM_GAS_DISSIPATES_QUICKLY = Fl(12),		// dissipates quickly
	TM_EXTINGUISHES_FIRE = Fl(13),		// extinguishes burning terrain or creatures
	TM_VANISHES_UPON_PROMOTION = Fl(14),		// vanishes when creating promotion dungeon feature, even if the replacement terrain priority doesn't require it
	TM_REFLECTS_BOLTS = Fl(15),       // magic bolts reflect off of its surface randomly (similar to pmap flag IMPREGNABLE)
	TM_STAND_IN_TILE = Fl(16),		// earthbound creatures will be said to stand "in" the tile, not on it
	TM_LIST_IN_SIDEBAR = Fl(17),       // terrain will be listed in the sidebar with a description of the terrain type
	TM_VISUALLY_DISTINCT = Fl(18),       // terrain will be color-adjusted if necessary so the character stands out from the background
	TM_BRIGHT_MEMORY = Fl(19),       // no blue fade when this tile is out of sight
	TM_EXPLOSIVE_PROMOTE = Fl(20),       // when burned, will promote to promoteType instead of burningType if surrounded by tiles with T_IS_FIRE or TM_EXPLOSIVE_PROMOTE
	TM_CONNECTS_LEVEL = Fl(21),       // will be treated as passable for purposes of calculating level connectedness, irrespective of other aspects of this terrain layer
	TM_INTERRUPT_EXPLORATION_WHEN_SEEN = Fl(22),    // will generate a message when discovered during exploration to interrupt exploration
	TM_INVERT_WHEN_HIGHLIGHTED = Fl(23),       // will flip fore and back colors when highlighted with pathing
	TM_SWAP_ENCHANTS_ACTIVATION = Fl(24),       // in machine, swap item enchantments when two suitable items are on this terrain, and activate the machine when that happens
};

enum statusEffects {
	STATUS_SEARCHING = 0,
	STATUS_DONNING,
	STATUS_WEAKENED,
	STATUS_TELEPATHIC,
	STATUS_HALLUCINATING,
	STATUS_LEVITATING,
	STATUS_SLOWED,
	STATUS_HASTED,
	STATUS_CONFUSED,
	STATUS_BURNING,
	STATUS_PARALYZED,
	STATUS_POISONED,
	STATUS_STUCK,
	STATUS_NAUSEOUS,
	STATUS_DISCORDANT,
	STATUS_IMMUNE_TO_FIRE,
	STATUS_EXPLOSION_IMMUNITY,
	STATUS_NUTRITION,
	STATUS_ENTERS_LEVEL_IN,
	STATUS_MAGICAL_FEAR,
	STATUS_ENTRANCED,
	STATUS_DARKNESS,
	STATUS_LIFESPAN_REMAINING,
	STATUS_SHIELDED,
	STATUS_INVISIBLE,
	STATUS_AGGRAVATING,
	NUMBER_OF_STATUS_EFFECTS,
};