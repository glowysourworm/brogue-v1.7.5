#pragma once

#include "dungeonConstants.h"
#include "monsterConstants.h"
#include "machine.h"
#include "lightConstants.h"
#include "color.h"
#include "autoGenerator.h"
#include "dungeonDeclaration.h"

const autoGenerator autoGeneratorCatalog[NUMBER_AUTOGENERATORS] = {
	//	 terrain					layer	DF							Machine						reqDungeon  reqLiquid   >Depth	<Depth          freq	minIncp	minSlope	maxNumber
	// Ordinary features of the dungeon
	{0,							0,		DF_GRANITE_COLUMN,			0,							FLOOR,		NOTHING,    1,		DEEPEST_LEVEL,	60,		100,	0,			4},
	{0,							0,		DF_CRYSTAL_WALL,			0,							WALL,       NOTHING,    14,		DEEPEST_LEVEL,	15,		-325,	25,			5},
	{0,							0,		DF_LUMINESCENT_FUNGUS,		0,							FLOOR,		NOTHING,    7,		DEEPEST_LEVEL,	15,		-300,	70,			14},
	{0,							0,		DF_GRASS,					0,							FLOOR,		NOTHING,    0,		10,             0,		1000,	-80,        10},
	{0,							0,		DF_DEAD_GRASS,				0,							FLOOR,		NOTHING,    4,		9,              0,		-200,	80,         10},
	{0,							0,		DF_DEAD_GRASS,				0,							FLOOR,		NOTHING,    9,		14,             0,		1200,	-80,        10},
	{0,							0,		DF_BONES,					0,							FLOOR,		NOTHING,    12,		DEEPEST_LEVEL - 1,30,		0,		0,			4},
	{0,							0,		DF_RUBBLE,					0,							FLOOR,		NOTHING,    0,		DEEPEST_LEVEL - 1,30,		0,		0,			4},
	{0,							0,		DF_FOLIAGE,					0,							FLOOR,		NOTHING,    0,		8,              15,		1000,	-333,       10},
	{0,							0,		DF_FUNGUS_FOREST,			0,							FLOOR,		NOTHING,    13,		DEEPEST_LEVEL,	30,		-600,	50,			12},
	{0,							0,		DF_BUILD_ALGAE_WELL,		0,							FLOOR,      DEEP_WATER, 10,		DEEPEST_LEVEL,	50,		0,      0,			2},
	{STATUE_INERT,				DUNGEON,0,							0,							WALL,       NOTHING,    6,		DEEPEST_LEVEL - 1,5,		-100,	35,			3},
	{STATUE_INERT,				DUNGEON,0,							0,							FLOOR,		NOTHING,    10,		DEEPEST_LEVEL - 1,50,		0,		0,			3},
	{TORCH_WALL,				DUNGEON,0,							0,							WALL,       NOTHING,    6,		DEEPEST_LEVEL - 1,5,		-200,	70,			12},

	// Pre-revealed traps
	{GAS_TRAP_POISON,           DUNGEON,0,							0,							FLOOR,		NOTHING,    2,		4,              20,		0,      0,			1},
	{NET_TRAP,                  DUNGEON,0,							0,							FLOOR,		NOTHING,    2,		5,              20,		0,      0,			1},
	{0,                         0,      0,							MT_PARALYSIS_TRAP_AREA,		FLOOR,		NOTHING,    2,		6,              20,		0,      0,			1},
	{ALARM_TRAP,                DUNGEON,0,							0,							FLOOR,		NOTHING,    4,		7,              20,		0,      0,			1},
	{GAS_TRAP_CONFUSION,        DUNGEON,0,							0,							FLOOR,		NOTHING,    2,		10,             20,		0,      0,			1},
	{FLAMETHROWER,              DUNGEON,0,							0,							FLOOR,		NOTHING,    4,		12,             20,		0,      0,			1},
	{FLOOD_TRAP,                DUNGEON,0,							0,							FLOOR,		NOTHING,    10,		14,             20,		0,      0,			1},

	// Hidden traps
	{GAS_TRAP_POISON_HIDDEN,	DUNGEON,0,							0,							FLOOR,		NOTHING,    5,		DEEPEST_LEVEL - 1,20,		100,	0,			3},
	{NET_TRAP_HIDDEN,			DUNGEON,0,							0,							FLOOR,		NOTHING,    6,		DEEPEST_LEVEL - 1,20,		100,	0,			3},
	{0,                         0,      0,							MT_PARALYSIS_TRAP_HIDDEN_AREA, FLOOR,	NOTHING,    7,		DEEPEST_LEVEL - 1,20,		100,	0,			3},
	{ALARM_TRAP_HIDDEN,			DUNGEON,0,							0,							FLOOR,		NOTHING,    8,		DEEPEST_LEVEL - 1,20,		100,	0,			2},
	{TRAP_DOOR_HIDDEN,			DUNGEON,0,							0,							FLOOR,		NOTHING,    9,		DEEPEST_LEVEL - 1,20,		100,	0,			2},
	{GAS_TRAP_CONFUSION_HIDDEN,	DUNGEON,0,							0,							FLOOR,		NOTHING,    11,		DEEPEST_LEVEL - 1,20,		100,	0,			3},
	{FLAMETHROWER_HIDDEN,		DUNGEON,0,							0,							FLOOR,		NOTHING,    13,		DEEPEST_LEVEL - 1,20,		100,	0,			3},
	{FLOOD_TRAP_HIDDEN,			DUNGEON,0,							0,							FLOOR,		NOTHING,    15,		DEEPEST_LEVEL - 1,20,		100,	0,			3},
	{0,							0,		0,							MT_SWAMP_AREA,				FLOOR,		NOTHING,    1,		DEEPEST_LEVEL - 1,30,		0,		0,			2},
	{0,							0,		DF_SUNLIGHT,				0,							FLOOR,		NOTHING,    0,		5,              15,		500,	-150,       10},
	{0,							0,		DF_DARKNESS,				0,							FLOOR,		NOTHING,    1,		15,             15,		500,	-50,        10},
	{STEAM_VENT,				DUNGEON,0,							0,							FLOOR,		NOTHING,    16,		DEEPEST_LEVEL - 1,30,		100,	0,			3},
	{CRYSTAL_WALL,              DUNGEON,0,                          0,                          WALL,       NOTHING,    DEEPEST_LEVEL,DEEPEST_LEVEL,100,0,      0,          600},

	// Dewars
	{DEWAR_CAUSTIC_GAS,         DUNGEON,DF_CARPET_AREA,             0,                          FLOOR,      NOTHING,    8,      DEEPEST_LEVEL - 1,2,      0,      0,          2},
	{DEWAR_CONFUSION_GAS,       DUNGEON,DF_CARPET_AREA,             0,                          FLOOR,      NOTHING,    8,      DEEPEST_LEVEL - 1,2,      0,      0,          2},
	{DEWAR_PARALYSIS_GAS,       DUNGEON,DF_CARPET_AREA,             0,                          FLOOR,      NOTHING,    8,      DEEPEST_LEVEL - 1,2,      0,      0,          2},
	{DEWAR_METHANE_GAS,         DUNGEON,DF_CARPET_AREA,             0,                          FLOOR,      NOTHING,    8,      DEEPEST_LEVEL - 1,2,      0,      0,          2},

	// Flavor machines
	{0,							0,		DF_LUMINESCENT_FUNGUS,		0,							FLOOR,		NOTHING,    DEEPEST_LEVEL,DEEPEST_LEVEL,100,0,      0,			200},
	{0,                         0,      0,                          MT_BLOODFLOWER_AREA,        FLOOR,      NOTHING,    1,      30,             25,     140,    -10,        3},
	{0,							0,		0,							MT_SHRINE_AREA,				FLOOR,		NOTHING,    5,		AMULET_LEVEL,   7,		0,		0,          1},
	{0,							0,		0,							MT_IDYLL_AREA,				FLOOR,		NOTHING,    1,		5,              15,		0,		0,          1},
	{0,							0,		0,							MT_REMNANT_AREA,			FLOOR,		NOTHING,    10,		DEEPEST_LEVEL,	15,		0,		0,			2},
	{0,							0,		0,							MT_DISMAL_AREA,				FLOOR,		NOTHING,    7,		DEEPEST_LEVEL,	12,		0,		0,			5},
	{0,							0,		0,							MT_BRIDGE_TURRET_AREA,		FLOOR,		NOTHING,    5,		DEEPEST_LEVEL - 1,6,		0,		0,			2},
	{0,							0,		0,							MT_LAKE_PATH_TURRET_AREA,	FLOOR,		NOTHING,    5,		DEEPEST_LEVEL - 1,6,		0,		0,			2},
	{0,							0,		0,							MT_TRICK_STATUE_AREA,		FLOOR,		NOTHING,    6,		DEEPEST_LEVEL - 1,15,		0,		0,			3},
	{0,							0,		0,							MT_SENTINEL_AREA,			FLOOR,		NOTHING,    12,		DEEPEST_LEVEL - 1,10,		0,		0,			2},
	{0,							0,		0,							MT_WORM_AREA,				FLOOR,		NOTHING,    12,		DEEPEST_LEVEL - 1,12,		0,		0,			3},
};


const floorTileType tileCatalog[NUMBER_TILETYPES] = {

	// promoteChance is in hundredths of a percent per turn

	//	char		fore color				back color		priority	ignit	fireType	discovType	promoteType		promoteChance	glowLight		flags																								description			flavorText

	// dungeon layer (this layer must have all of fore color, back color and char)
	{	' ',		&black,					&black,					100,0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		0, 0,																								"a chilly void",		""},
	{WALL_CHAR,		&wallBackColor,			&graniteBackColor,		0,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE),														"a rough granite wall",	"The granite is split open with splinters of rock jutting out at odd angles."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		0, 0,                                                                                               "the ground",			""},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		0, 0,                                                                                               "the ground",			""},
	{FLOOR_CHAR,	&carpetForeColor,		&carpetBackColor,		85,	0,	DF_EMBERS,		0,			0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "the carpet",			"Ornate carpeting fills this room, a relic of ages past."},
	{FLOOR_CHAR,	&marbleForeColor,       &marbleBackColor,       85,	0,	DF_EMBERS,		0,			0,				0,				NO_LIGHT,		0, 0,                                                                                               "the marble ground",    "Light from the nearby crystals catches the grain of the lavish marble floor."},
	{WALL_CHAR,		&wallForeColor,			&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE),														"a stone wall",			"The rough stone wall is firm and unyielding."},
	{DOOR_CHAR,		&doorForeColor,			&doorBackColor,			25,	50,	DF_EMBERS,		0,			DF_OPEN_DOOR,	0,				NO_LIGHT,		(T_OBSTRUCTS_VISION | T_OBSTRUCTS_GAS | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_STEP | TM_VISUALLY_DISTINCT), "a wooden door",	"you pass through the doorway."},
	{OPEN_DOOR_CHAR,&doorForeColor,			&doorBackColor,			25,	50,	DF_EMBERS,		0,			DF_CLOSED_DOOR,	10000,			NO_LIGHT,		(T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),           "an open door",			"you pass through the doorway."},
	{WALL_CHAR,		&wallForeColor,			&wallBackColor,			0,	50,	DF_EMBERS,		DF_SHOW_DOOR,0,				0,				NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_SECRET),	"a stone wall",		"The rough stone wall is firm and unyielding."},
	{DOOR_CHAR,		&ironDoorForeColor,		&ironDoorBackColor,		15,	50,	DF_EMBERS,		0,			DF_OPEN_IRON_DOOR_INERT,0,		NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_WITH_KEY | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_BRIGHT_MEMORY | TM_INTERRUPT_EXPLORATION_WHEN_SEEN | TM_INVERT_WHEN_HIGHLIGHTED),	"a locked iron door",	"you search your pack but do not have a matching key."},
	{OPEN_DOOR_CHAR,&whiteBrogue,                 &ironDoorBackColor,		90,	50,	DF_EMBERS,		0,			0,				0,				NO_LIGHT,		(T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VISUALLY_DISTINCT),                           "an open iron door",	"you pass through the doorway."},
	{DESCEND_CHAR,	&itemColor,				&stairsBackColor,		30,	0,	DF_PLAIN_FIRE,	0,			DF_REPEL_CREATURES, 0,			NO_LIGHT,		(T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_PROMOTES_ON_STEP | TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_BRIGHT_MEMORY | TM_INTERRUPT_EXPLORATION_WHEN_SEEN | TM_INVERT_WHEN_HIGHLIGHTED), "a downward staircase",	"stairs spiral downward into the depths."},
	{ASCEND_CHAR,	&itemColor,				&stairsBackColor,		30,	0,	DF_PLAIN_FIRE,	0,			DF_REPEL_CREATURES, 0,			NO_LIGHT,		(T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_PROMOTES_ON_STEP | TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_BRIGHT_MEMORY | TM_INTERRUPT_EXPLORATION_WHEN_SEEN | TM_INVERT_WHEN_HIGHLIGHTED), "an upward staircase",	"stairs spiral upward."},
	{OMEGA_CHAR,	&lightBlue,				&firstStairsBackColor,	30,	0,	DF_PLAIN_FIRE,	0,			DF_REPEL_CREATURES, 0,			NO_LIGHT,		(T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_PROMOTES_ON_STEP | TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_BRIGHT_MEMORY | TM_INTERRUPT_EXPLORATION_WHEN_SEEN | TM_INVERT_WHEN_HIGHLIGHTED), "the dungeon exit",		"the gilded doors leading out of the dungeon are sealed by an invisible force."},
	{OMEGA_CHAR,	&wallCrystalColor,		&firstStairsBackColor,	30,	0,	DF_PLAIN_FIRE,	0,			DF_REPEL_CREATURES, 0,			INCENDIARY_DART_LIGHT,		(T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_PROMOTES_ON_STEP | TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_BRIGHT_MEMORY | TM_INTERRUPT_EXPLORATION_WHEN_SEEN | TM_INVERT_WHEN_HIGHLIGHTED), "a crystal portal",		"dancing lights play across the plane of this sparkling crystal portal."},
	{WALL_CHAR,		&torchColor,			&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			0,				0,				TORCH_LIGHT,	(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE),														"a wall-mounted torch",	"The torch is anchored firmly to the wall and sputters quietly in the gloom."},
	{WALL_CHAR,		&wallCrystalColor,		&wallCrystalColor,		0,	0,	DF_PLAIN_FIRE,	0,			0,				0,				CRYSTAL_WALL_LIGHT,(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS | T_OBSTRUCTS_DIAGONAL_MOVEMENT), (TM_STAND_IN_TILE | TM_REFLECTS_BOLTS),"a crystal formation", "You feel the crystal's glossy surface and admire the dancing lights beneath."},
	{WALL_CHAR,		&grayBrogue,					&floorBackColor,		10,	0,	DF_PLAIN_FIRE,	0,			DF_OPEN_PORTCULLIS,	0,			NO_LIGHT,		(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_CONNECTS_LEVEL), "a heavy portcullis",	"The iron bars rattle but will not budge; they are firmly locked in place."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_PLAIN_FIRE,	0,			DF_ACTIVATE_PORTCULLIS,0,		NO_LIGHT,		(0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),                                                    "the ground",			""},
	{WALL_CHAR,		&doorForeColor,			&floorBackColor,		10,	100,DF_WOODEN_BARRICADE_BURN,0,	0,				0,				NO_LIGHT,		(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT | TM_CONNECTS_LEVEL),"a dry wooden barricade","The wooden barricade is firmly set but has dried over the years. Might it burn?"},
	{WALL_CHAR,		&torchLightColor,		&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			DF_PILOT_LIGHT,	0,				TORCH_LIGHT,	(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),			"a wall-mounted torch",	"The torch is anchored firmly to the wall, and sputters quietly in the gloom."},
	{FIRE_CHAR,		&fireForeColor,			&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			0,				0,				TORCH_LIGHT,	(T_OBSTRUCTS_EVERYTHING | T_IS_FIRE), (TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR),						"a fallen torch",		"The torch lies at the foot of the wall, spouting gouts of flame haphazardly."},
	{WALL_CHAR,		&torchColor,			&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			DF_HAUNTED_TORCH_TRANSITION,0,	TORCH_LIGHT,	(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),			"a wall-mounted torch",	"The torch is anchored firmly to the wall and sputters quietly in the gloom."},
	{WALL_CHAR,		&torchColor,			&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			DF_HAUNTED_TORCH,2000,			TORCH_LIGHT,	(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                          "a wall-mounted torch",	"The torch is anchored firmly to the wall and sputters quietly in the gloom."},
	{WALL_CHAR,		&hauntedTorchColor,		&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			0,				0,				HAUNTED_TORCH_LIGHT,(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE),                                                   "a sputtering torch",	"A dim purple flame sputters and spits atop this wall-mounted torch."},
	{WALL_CHAR,		&wallForeColor,			&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	DF_REVEAL_LEVER,0,			0,				NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_SECRET),			"a stone wall",			"The rough stone wall is firm and unyielding."},
	{LEVER_CHAR,	&wallForeColor,			&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			DF_PULL_LEVER,  0,				NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_ON_PLAYER_ENTRY | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_INVERT_WHEN_HIGHLIGHTED),"a lever", "The lever moves."},
	{LEVER_PULLED_CHAR,&wallForeColor,		&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE),                                                       "an inactive lever",    "The lever won't budge."},
	{WALL_CHAR,		&wallForeColor,         &wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,          DF_CREATE_LEVER,0,				NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_IS_WIRED),											"a stone wall",			"The rough stone wall is firm and unyielding."},
	{STATUE_CHAR,	&wallBackColor,			&statueBackColor,		0,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE),	"a marble statue",	"The cold marble statue has weathered the years with grace."},
	{STATUE_CHAR,	&wallBackColor,			&statueBackColor,		0,	0,	DF_PLAIN_FIRE,	0,			DF_CRACKING_STATUE,0,			NO_LIGHT,		(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),"a marble statue",	"The cold marble statue has weathered the years with grace."},
	{STATUE_CHAR,	&wallBackColor,			&statueBackColor,		0,	0,	DF_PLAIN_FIRE,	0,			DF_STATUE_SHATTER,3500,			NO_LIGHT,		(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_LIST_IN_SIDEBAR),"a cracking statue",	"Deep cracks ramble down the side of the statue even as you watch."},
	{STATUE_CHAR,	&wallBackColor,			&statueBackColor,		0,	0,	DF_PLAIN_FIRE,	0,			DF_STATUE_SHATTER,0,			NO_LIGHT,		(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),"a marble statue",	"The cold marble statue has weathered the years with grace."},
	{OMEGA_CHAR,	&wallBackColor,			&floorBackColor,		17,	0,	DF_PLAIN_FIRE,	0,			DF_PORTAL_ACTIVATE,0,			NO_LIGHT,		(T_OBSTRUCTS_ITEMS), (TM_STAND_IN_TILE | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),  "a stone archway",		"This ancient moss-covered stone archway radiates a strange, alien energy."},
	{WALL_CHAR,		&wallForeColor,			&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			DF_TURRET_EMERGE,0,				NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),			"a stone wall",			"The rough stone wall is firm and unyielding."},
	{WALL_CHAR,		&wallForeColor,			&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			DF_WALL_SHATTER,0,				NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),			"a stone wall",			"The rough stone wall is firm and unyielding."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_PLAIN_FIRE,	0,			DF_DARKENING_FLOOR,	0,			NO_LIGHT,		(0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),                                                    "the ground",			""},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_PLAIN_FIRE,	0,			DF_DARK_FLOOR,	1500,			NO_LIGHT,		(0), (TM_VANISHES_UPON_PROMOTION),                                                                  "the ground",			""},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_PLAIN_FIRE,	0,			0,				0,				DARKNESS_CLOUD_LIGHT, 0, 0,                                                                                         "the ground",			""},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_ON_PLAYER_ENTRY),                      "the ground",			""},
	{ALTAR_CHAR,	&altarForeColor,		&altarBackColor,		17, 0,	0,				0,			0,				0,				CANDLE_LIGHT,	(T_OBSTRUCTS_SURFACE_EFFECTS), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),							"a candle-lit altar",	"a gilded altar is adorned with candles that flicker in the breeze."},
	{GEM_CHAR,		&altarForeColor,		&altarBackColor,		17, 0,	0,				0,			0,				0,				CANDLE_LIGHT,	(T_OBSTRUCTS_SURFACE_EFFECTS), (TM_PROMOTES_WITH_KEY | TM_IS_WIRED | TM_LIST_IN_SIDEBAR),           "a candle-lit altar",	"ornate gilding spirals around a spherical depression in the top of the altar."},
	{ALTAR_CHAR,	&altarForeColor,		&altarBackColor,		17, 0,	0,				0,			DF_ITEM_CAGE_CLOSE,	0,			CANDLE_LIGHT,	(T_OBSTRUCTS_SURFACE_EFFECTS), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_WITHOUT_KEY | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),"a candle-lit altar",	"a cage, open on the bottom, hangs over this altar on a retractable chain."},
	{WALL_CHAR,		&altarBackColor,		&veryDarkGrayBrogue,			17, 0,	0,				0,			DF_ITEM_CAGE_OPEN,	0,			CANDLE_LIGHT,	(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_WITH_KEY | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),"an iron cage","the missing item must be replaced before you can access the remaining items."},
	{ALTAR_CHAR,	&altarForeColor,		&altarBackColor,		17, 0,	0,				0,			DF_ALTAR_INERT,	0,				CANDLE_LIGHT,	(T_OBSTRUCTS_SURFACE_EFFECTS), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_ON_ITEM_PICKUP | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),	"a candle-lit altar",	"a weathered stone altar is adorned with candles that flicker in the breeze."},
	{ALTAR_CHAR,	&altarForeColor,		&altarBackColor,		17, 0,	0,				0,			DF_ALTAR_RETRACT,0,				CANDLE_LIGHT,	(T_OBSTRUCTS_SURFACE_EFFECTS), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_ON_ITEM_PICKUP | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),	"a candle-lit altar",	"a weathered stone altar is adorned with candles that flicker in the breeze."},
	{WALL_CHAR,		&altarBackColor,		&veryDarkGrayBrogue,			17, 0,	0,				0,			DF_CAGE_DISAPPEARS,	0,			CANDLE_LIGHT,	(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),"an iron cage","the cage won't budge. Perhaps there is a way to raise it nearby..."},
	{ALTAR_CHAR,	&altarForeColor,		&pedestalBackColor,		17, 0,	0,				0,			0,				0,				CANDLE_LIGHT,	(T_OBSTRUCTS_SURFACE_EFFECTS), 0,																	"a stone pedestal",		"elaborate carvings wind around this ancient pedestal."},
	{ALTAR_CHAR,	&floorBackColor,		&veryDarkGrayBrogue,			17, 0,	0,				0,			0,				0,				NO_LIGHT,		(0), (TM_STAND_IN_TILE),																			"an open cage",			"the interior of the cage is filthy and reeks of decay."},
	{WALL_CHAR,		&grayBrogue,					&darkGrayBrogue,				17, 0,	0,				0,			DF_MONSTER_CAGE_OPENS,	0,		NO_LIGHT,		(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS | T_OBSTRUCTS_GAS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_WITH_KEY | TM_LIST_IN_SIDEBAR | TM_INTERRUPT_EXPLORATION_WHEN_SEEN),"a locked iron cage","the bars of the cage are firmly set and will not budge."},
	{ALTAR_CHAR,	&bridgeFrontColor,		&bridgeBackColor,		17,	20,	DF_COFFIN_BURNS,0,			DF_COFFIN_BURSTS,0,				NO_LIGHT,		(T_IS_FLAMMABLE), (TM_IS_WIRED | TM_VANISHES_UPON_PROMOTION | TM_LIST_IN_SIDEBAR),                  "a sealed coffin",		"a coffin made from thick wooden planks rests in a bed of moss."},
	{ALTAR_CHAR,	&black,					&bridgeBackColor,		17,	20,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_LIST_IN_SIDEBAR),				"an empty coffin",		"an open wooden coffin rests in a bed of moss."},

	// traps (part of dungeon layer):
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_POISON_GAS_CLOUD, DF_SHOW_POISON_GAS_TRAP, 0, 0,			NO_LIGHT,		(T_IS_DF_TRAP), (TM_IS_SECRET),                                                                     "the ground",			""},
	{TRAP_CHAR,		&poisonGasColor,		0,                      30,	0,	DF_POISON_GAS_CLOUD, 0,		0,				0,				NO_LIGHT,		(T_IS_DF_TRAP), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                        "a caustic gas trap",	"there is a hidden pressure plate in the floor above a reserve of caustic gas."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_POISON_GAS_CLOUD, DF_SHOW_TRAPDOOR,0,	0,				NO_LIGHT,		(T_AUTO_DESCENT), (TM_IS_SECRET),                                                                   "the ground",			"you plunge through a hidden trap door!"},
	{CHASM_CHAR,	&chasmForeColor,		&black,					30,	0,	DF_POISON_GAS_CLOUD,0,      0,				0,				NO_LIGHT,		(T_AUTO_DESCENT), 0,                                                                                "a hole",				"you plunge through a hole in the ground!"},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	0,              DF_SHOW_PARALYSIS_GAS_TRAP, 0, 0,           NO_LIGHT,		(T_IS_DF_TRAP), (TM_IS_SECRET | TM_IS_WIRED),                                                       "the ground",			""},
	{TRAP_CHAR,		&pink,					0,              		30,	0,	0,              0,          0,				0,				NO_LIGHT,		(T_IS_DF_TRAP), (TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                          "a paralysis trigger",	"there is a hidden pressure plate in the floor."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_PLAIN_FIRE,	DF_DISCOVER_PARALYSIS_VENT, DF_PARALYSIS_VENT_SPEW,0,NO_LIGHT,	(0), (TM_VANISHES_UPON_PROMOTION | TM_IS_SECRET | TM_IS_WIRED),                                 "the ground",			""},
	{VENT_CHAR,		&pink,                  0,              		30,	0,	DF_PLAIN_FIRE,	0,			DF_PARALYSIS_VENT_SPEW,0,		NO_LIGHT,		(0), (TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                     "an inactive gas vent",	"A dormant gas vent is connected to a reserve of paralytic gas."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_CONFUSION_GAS_TRAP_CLOUD,DF_SHOW_CONFUSION_GAS_TRAP, 0,0,NO_LIGHT,		(T_IS_DF_TRAP), (TM_IS_SECRET),                                                                     "the ground",			""},
	{TRAP_CHAR,		&confusionGasColor,		0,              		30,	0,	DF_CONFUSION_GAS_TRAP_CLOUD,0,	0,			0,				NO_LIGHT,		(T_IS_DF_TRAP), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                        "a confusion trap",		"A hidden pressure plate accompanies a reserve of psychotropic gas."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_FLAMETHROWER,	DF_SHOW_FLAMETHROWER_TRAP, 0,	0,		NO_LIGHT,		(T_IS_DF_TRAP), (TM_IS_SECRET),                                                                     "the ground",			""},
	{TRAP_CHAR,		&red,                   0,              		30,	0,	DF_FLAMETHROWER,	0,		0,				0,				NO_LIGHT,		(T_IS_DF_TRAP), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),										"a fire trap",			"A hidden pressure plate is connected to a crude flamethrower mechanism."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_FLOOD,		DF_SHOW_FLOOD_TRAP, 0,		0,				NO_LIGHT,		(T_IS_DF_TRAP), (TM_IS_SECRET),                                                                     "the ground",			""},
	{TRAP_CHAR,		&blue,                  0,              		58,	0,	DF_FLOOD,		0,			0,				0,				NO_LIGHT,		(T_IS_DF_TRAP), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),										"a flood trap",			"A hidden pressure plate is connected to floodgates in the walls and ceiling."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_NET,         DF_SHOW_NET_TRAP, 0,        0,              NO_LIGHT,		(T_IS_DF_TRAP), (TM_IS_SECRET),                                                                     "the ground",			""},
	{TRAP_CHAR,		&tanColor,              0,                      30,	0,	DF_NET,         0,          0,				0,				NO_LIGHT,		(T_IS_DF_TRAP), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                        "a net trap",           "you see netting subtly concealed in the ceiling over a hidden pressure plate."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_AGGRAVATE_TRAP, DF_SHOW_ALARM_TRAP, 0,   0,              NO_LIGHT,		(T_IS_DF_TRAP), (TM_IS_SECRET),                                                                     "the ground",			""},
	{TRAP_CHAR,		&grayBrogue,                  0,                      30,	0,	DF_AGGRAVATE_TRAP, 0,       0,				0,				NO_LIGHT,		(T_IS_DF_TRAP), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                        "an alarm trap",        "a hidden pressure plate is connected to a loud alarm mechanism."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_PLAIN_FIRE,	DF_SHOW_POISON_GAS_VENT, DF_POISON_GAS_VENT_OPEN, 0, NO_LIGHT, (0), (TM_VANISHES_UPON_PROMOTION | TM_IS_SECRET | TM_IS_WIRED),                                  "the ground",			""},
	{VENT_CHAR,		&floorForeColor,		0,              		30,	0,	DF_PLAIN_FIRE,	0,			DF_POISON_GAS_VENT_OPEN,0,		NO_LIGHT,		(0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),		"an inactive gas vent",	"An inactive gas vent is hidden in a crevice in the ground."},
	{VENT_CHAR,		&floorForeColor,		0,              		30,	0,	DF_PLAIN_FIRE,	0,			DF_VENT_SPEW_POISON_GAS,10000,	NO_LIGHT,		0, (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),														"a gas vent",			"Clouds of caustic gas are wafting out of a hidden vent in the floor."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_PLAIN_FIRE,	DF_SHOW_METHANE_VENT, DF_METHANE_VENT_OPEN,0,NO_LIGHT,		(0), (TM_VANISHES_UPON_PROMOTION | TM_IS_SECRET | TM_IS_WIRED),                                     "the ground",			""},
	{VENT_CHAR,		&floorForeColor,		0,              		30,	0,	DF_PLAIN_FIRE,	0,			DF_METHANE_VENT_OPEN,0,			NO_LIGHT,		(0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),		"an inactive gas vent",	"An inactive gas vent is hidden in a crevice in the ground."},
	{VENT_CHAR,		&floorForeColor,		0,              		30,	15,	DF_EMBERS,		0,			DF_VENT_SPEW_METHANE,5000,		NO_LIGHT,		(T_IS_FLAMMABLE), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),										"a gas vent",			"Clouds of explosive gas are wafting out of a hidden vent in the floor."},
	{VENT_CHAR,		&grayBrogue,					0,              		15,	15,	DF_EMBERS,		0,			DF_STEAM_PUFF,	250,			NO_LIGHT,		T_OBSTRUCTS_ITEMS, (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),										"a steam vent",			"A natural crevice in the floor periodically vents scalding gouts of steam."},
	{TRAP_CHAR,		&whiteBrogue,					&chasmEdgeBackColor,	15,	0,	0,				0,			DF_MACHINE_PRESSURE_PLATE_USED,0,NO_LIGHT,      (T_IS_DF_TRAP), (TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_STEP | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),"a pressure plate",		"There is an exposed pressure plate here. A thrown item might trigger it."},
	{TRAP_CHAR,		&darkGrayBrogue,				&chasmEdgeBackColor,	15,	0,	0,				0,			0,				0,				NO_LIGHT,		0, (TM_LIST_IN_SIDEBAR),                                                                            "an inactive pressure plate", "This pressure plate has already been depressed."},
	{CHASM_CHAR,	&glyphColor,            0,                      42,	0,	0,              0,          DF_INACTIVE_GLYPH,0,			GLYPH_LIGHT_DIM,(0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_ON_PLAYER_ENTRY | TM_VISUALLY_DISTINCT),"a magical glyph",      "A strange glyph, engraved into the floor, flickers with magical light."},
	{CHASM_CHAR,	&glyphColor,            0,                      42,	0,	0,              0,          DF_ACTIVE_GLYPH,10000,			GLYPH_LIGHT_BRIGHT,(0), (TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),                                        "a glowing glyph",      "A strange glyph, engraved into the floor, radiates magical light."},
	{DEWAR_CHAR,    &poisonGasColor,        &darkGrayBrogue,              10, 20, DF_DEWAR_CAUSTIC,0,         DF_DEWAR_CAUSTIC,0,             NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS | T_OBSTRUCTS_GAS | T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT | TM_LIST_IN_SIDEBAR | TM_PROMOTES_ON_PLAYER_ENTRY | TM_INVERT_WHEN_HIGHLIGHTED),"a glass dewar of caustic gas", ""},
	{DEWAR_CHAR,    &confusionGasColor,     &darkGrayBrogue,              10, 20, DF_DEWAR_CONFUSION,0,       DF_DEWAR_CONFUSION,0,           NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS | T_OBSTRUCTS_GAS | T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT | TM_LIST_IN_SIDEBAR | TM_PROMOTES_ON_PLAYER_ENTRY | TM_INVERT_WHEN_HIGHLIGHTED),"a glass dewar of confusion gas", ""},
	{DEWAR_CHAR,    &pink,                  &darkGrayBrogue,              10, 20, DF_DEWAR_PARALYSIS,0,       DF_DEWAR_PARALYSIS,0,           NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS | T_OBSTRUCTS_GAS | T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT | TM_LIST_IN_SIDEBAR | TM_PROMOTES_ON_PLAYER_ENTRY | TM_INVERT_WHEN_HIGHLIGHTED),"a glass dewar of paralytic gas", ""},
	{DEWAR_CHAR,    &methaneColor,          &darkGrayBrogue,              10, 20, DF_DEWAR_METHANE,0,         DF_DEWAR_METHANE,0,             NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS | T_OBSTRUCTS_GAS | T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT | TM_LIST_IN_SIDEBAR | TM_PROMOTES_ON_PLAYER_ENTRY | TM_INVERT_WHEN_HIGHLIGHTED),"a glass dewar of methane gas", ""},

	// liquid layer
	{LIQUID_CHAR,	&deepWaterForeColor,	&deepWaterBackColor,	40,	100,DF_STEAM_ACCUMULATION,	0,	0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE | T_IS_DEEP_WATER), (TM_ALLOWS_SUBMERGING | TM_STAND_IN_TILE | TM_EXTINGUISHES_FIRE),"the murky waters",    "the current tugs you in all directions."},
	{0,				&shallowWaterForeColor,	&shallowWaterBackColor,	55,	0,	DF_STEAM_ACCUMULATION,	0,	0,				0,				NO_LIGHT,		(0), (TM_STAND_IN_TILE | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING),                              "shallow water",		"the water is cold and reaches your knees."},
	{MUD_CHAR,		&mudForeColor,			&mudBackColor,			55,	0,	DF_PLAIN_FIRE,	0,			DF_METHANE_GAS_PUFF, 100,		NO_LIGHT,		(0), (TM_STAND_IN_TILE | TM_ALLOWS_SUBMERGING),                                                     "a bog",				"you are knee-deep in thick, foul-smelling mud."},
	{CHASM_CHAR,	&chasmForeColor,		&black,					40,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_AUTO_DESCENT), (TM_STAND_IN_TILE),																"a chasm",				"you plunge downward into the chasm!"},
	{FLOOR_CHAR,	&whiteBrogue,					&chasmEdgeBackColor,	80,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		0, 0,                                                                                               "the brink of a chasm",	"chilly winds blow upward from the stygian depths."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_PLAIN_FIRE,	0,			DF_SPREADABLE_COLLAPSE,0,		NO_LIGHT,		(0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),                                                    "the ground",			""},
	{FLOOR_CHAR,	&whiteBrogue,					&chasmEdgeBackColor,	45,	0,	DF_PLAIN_FIRE,	0,			DF_COLLAPSE_SPREADS,2500,		NO_LIGHT,		(0), (TM_VANISHES_UPON_PROMOTION),                                                                  "the crumbling ground",	"cracks are appearing in the ground beneath your feet!"},
	{LIQUID_CHAR,	&fireForeColor,			&lavaBackColor,			40,	0,	DF_OBSIDIAN,	0,			0,				0,				LAVA_LIGHT,		(T_LAVA_INSTA_DEATH), (TM_STAND_IN_TILE | TM_ALLOWS_SUBMERGING),									"lava",					"searing heat rises from the lava."},
	{LIQUID_CHAR,	&fireForeColor,			&lavaBackColor,			40,	0,	DF_OBSIDIAN,	0,			DF_RETRACTING_LAVA,	0,			LAVA_LIGHT,		(T_LAVA_INSTA_DEATH), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_ALLOWS_SUBMERGING),"lava","searing heat rises from the lava."},
	{LIQUID_CHAR,	&fireForeColor,			&lavaBackColor,			40,	0,	DF_OBSIDIAN,	0,			DF_OBSIDIAN_WITH_STEAM,	-1500,	LAVA_LIGHT,		(T_LAVA_INSTA_DEATH), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_ALLOWS_SUBMERGING),		"cooling lava",         "searing heat rises from the lava."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		90,	0,	DF_PLAIN_FIRE,	0,			0,				0,				SUN_LIGHT,		(0), (TM_STAND_IN_TILE),																			"a patch of sunlight",	"sunlight streams through cracks in the ceiling."},
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		90,	0,	DF_PLAIN_FIRE,	0,			0,				0,				DARKNESS_PATCH_LIGHT,	(0), 0,																						"a patch of shadows",	"this area happens to be cloaked in shadows -- perhaps a safe place to hide."},
	{ASH_CHAR,		&brimstoneForeColor,	&brimstoneBackColor,	40, 100,DF_INERT_BRIMSTONE,	0,		DF_INERT_BRIMSTONE,	10,			NO_LIGHT,		(T_IS_FLAMMABLE | T_SPONTANEOUSLY_IGNITES), 0,                                                      "hissing brimstone",	"the jagged brimstone hisses and spits ominously as it crunches under your feet."},
	{ASH_CHAR,		&brimstoneForeColor,	&brimstoneBackColor,	40, 0,	DF_INERT_BRIMSTONE,	0,		DF_ACTIVE_BRIMSTONE, 800,		NO_LIGHT,		(T_SPONTANEOUSLY_IGNITES), 0,                                                                       "hissing brimstone",	"the jagged brimstone hisses and spits ominously as it crunches under your feet."},
	{FLOOR_CHAR,	&darkGrayBrogue,				&obsidianBackColor,		50,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		0, 0,                                                                                               "the obsidian ground",	"the ground has fused into obsidian."},
	{BRIDGE_CHAR,	&bridgeFrontColor,		&bridgeBackColor,		45,	50,	DF_BRIDGE_FIRE,	0,			0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "a rickety rope bridge","the rickety rope bridge creaks underfoot."},
	{BRIDGE_CHAR,	&bridgeFrontColor,		&bridgeBackColor,		45,	50,	DF_BRIDGE_FALL,	0,			DF_BRIDGE_FALL, 10000,			NO_LIGHT,		(T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "a plummeting bridge",  "the bridge is plunging into the chasm before your eyes!"},
	{BRIDGE_CHAR,	&bridgeFrontColor,		&bridgeBackColor,		45,	50,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "a rickety rope bridge","the rickety rope bridge is staked to the edge of the chasm."},
	{FLOOR_CHAR,	&whiteBrogue,					&chasmEdgeBackColor,	20,	50,	DF_BRIDGE_FIRE,	0,			0,				0,				NO_LIGHT,		0, 0,                                                                                               "a stone bridge",		"the narrow stone bridge winds precariously across the chasm."},
	{0,				&shallowWaterForeColor,	&shallowWaterBackColor,	60,	0,	DF_STEAM_ACCUMULATION,	0,	DF_SPREADABLE_WATER,0,			NO_LIGHT,		(0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING),	"shallow water",	"the water is cold and reaches your knees."},
	{0,				&shallowWaterForeColor,	&shallowWaterBackColor,	60,	0,	DF_STEAM_ACCUMULATION,	0,	DF_WATER_SPREADS,2500,			NO_LIGHT,		(0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING),	"shallow water",		"the water is cold and reaches your knees."},
	{MUD_CHAR,		&mudForeColor,			&mudBackColor,			55,	0,	DF_PLAIN_FIRE,	0,			DF_MUD_ACTIVATE,0,				NO_LIGHT,		(0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_ALLOWS_SUBMERGING),			"a bog",				"you are knee-deep in thick, foul-smelling mud."},
	{BONES_CHAR,	&whiteBrogue,                 &lightBlue,             35, 100,DF_DEEP_WATER_THAW, 0,      DF_DEEP_WATER_MELTING, -100,    NO_LIGHT,       (T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "ice",                  "a sheet of ice extends into the water, ripples frozen into its glossy surface."},
	{BONES_CHAR,	&black,                 &lightBlue,             35, 100,DF_DEEP_WATER_THAW, 0,      DF_DEEP_WATER_THAW, 10000,      NO_LIGHT,       (T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "melting ice",          "cracks extend across the surface of the ice as it melts before your eyes."},
	{BONES_CHAR,	&whiteBrogue,                 &lightBlue,             35, 100,DF_SHALLOW_WATER_THAW, 0,   DF_SHALLOW_WATER_MELTING, -100, NO_LIGHT,       (T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "ice",                  "a sheet of ice extends into the water, ripples frozen into its glossy surface."},
	{BONES_CHAR,	&black,                 &lightBlue,             35, 100,DF_DEEP_WATER_THAW, 0,      DF_SHALLOW_WATER_THAW, 10000,   NO_LIGHT,       (T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "melting ice",          "cracks extend across the surface of the ice as it melts before your eyes."},

	// surface layer
	{CHASM_CHAR,	&chasmForeColor,		&black,					9,	0,	DF_PLAIN_FIRE,	0,			DF_HOLE_DRAIN,	-1000,			NO_LIGHT,		(T_AUTO_DESCENT), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "a hole",				"you plunge downward into the hole!"},
	{CHASM_CHAR,	&chasmForeColor,		&black,					9,	0,	DF_PLAIN_FIRE,	0,			DF_HOLE_DRAIN,	-1000,			DESCENT_LIGHT,	(T_AUTO_DESCENT), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "a hole",				"you plunge downward into the hole!"},
	{FLOOR_CHAR,	&whiteBrogue,					&chasmEdgeBackColor,	50,	0,	DF_PLAIN_FIRE,	0,			0,				-500,			NO_LIGHT,		(0), (TM_VANISHES_UPON_PROMOTION),																	"translucent ground",	"chilly gusts of air blow upward through the translucent floor."},
	{LIQUID_CHAR,	&deepWaterForeColor,	&deepWaterBackColor,	41,	100,DF_STEAM_ACCUMULATION,	0,	DF_FLOOD_DRAIN,	-200,			NO_LIGHT,		(T_IS_FLAMMABLE | T_IS_DEEP_WATER), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING), "sloshing water", "roiling water floods the room."},
	{0,				&shallowWaterForeColor,	&shallowWaterBackColor,	50,	0,	DF_STEAM_ACCUMULATION,	0,	DF_PUDDLE,		-100,			NO_LIGHT,		(0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING),	"shallow water",		"knee-deep water drains slowly into holes in the floor."},
	{GRASS_CHAR,	&grassColor,			0,						60,	15,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "grass-like fungus",	"grass-like fungus crunches underfoot."},
	{GRASS_CHAR,	&deadGrassColor,		0,						60,	40,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "withered fungus",		"dead fungus covers the ground."},
	{GRASS_CHAR,	&grayFungusColor,		0,						51,	10,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "withered fungus",		"groping tendrils of pale fungus rise from the muck."},
	{GRASS_CHAR,	&fungusColor,			0,						60,	10,	DF_PLAIN_FIRE,	0,			0,				0,				FUNGUS_LIGHT,	(T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "luminescent fungus",	"luminescent fungus casts a pale, eerie glow."},
	{GRASS_CHAR,	&lichenColor,			0,						60,	50,	DF_PLAIN_FIRE,	0,			DF_LICHEN_GROW,	10000,			NO_LIGHT,		(T_CAUSES_POISON | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                "deadly lichen",		"venomous barbs cover the quivering tendrils of this fast-growing lichen."},
	{GRASS_CHAR,	&hayColor,				&refuseBackColor,		57,	50,	DF_STENCH_BURN,	0,			0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "filthy hay",			"a pile of hay, matted with filth, has been arranged here as a makeshift bed."},
	{FLOOR_CHAR,	&humanBloodColor,		0,						80,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(0), (TM_STAND_IN_TILE),                                                                            "a pool of blood",		"the floor is splattered with blood."},
	{FLOOR_CHAR,	&insectBloodColor,		0,						80,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(0), (TM_STAND_IN_TILE),                                                                            "a pool of green blood", "the floor is splattered with green blood."},
	{FLOOR_CHAR,	&poisonGasColor,		0,						80,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(0), (TM_STAND_IN_TILE),                                                                            "a pool of purple blood", "the floor is splattered with purple blood."},
	{FLOOR_CHAR,	&acidBackColor,			0,						80,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		0, 0,                                                                                               "a puddle of acid",     "the floor is splattered with acid."},
	{FLOOR_CHAR,	&vomitColor,			0,						80,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(0), (TM_STAND_IN_TILE),                                                                            "a puddle of vomit",	"the floor is caked with vomit."},
	{FLOOR_CHAR,	&urineColor,			0,						80,	0,	DF_PLAIN_FIRE,	0,			0,				100,			NO_LIGHT,		(0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                               "a puddle of urine",	"a puddle of urine covers the ground."},
	{FLOOR_CHAR,	&whiteBrogue,					0,						80,	0,	DF_PLAIN_FIRE,	0,			0,				0,				UNICORN_POOP_LIGHT,(0), (TM_STAND_IN_TILE),                                                                         "unicorn poop",			"a pile of lavender-scented unicorn poop sparkles with rainbow light."},
	{FLOOR_CHAR,	&wormColor,				0,						80,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(0), (TM_STAND_IN_TILE),                                                                            "a pool of worm entrails", "worm viscera cover the ground."},
	{ASH_CHAR,		&ashForeColor,			0,						80,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(0), (TM_STAND_IN_TILE),                                                                            "a pile of ashes",		"charcoal and ash crunch underfoot."},
	{ASH_CHAR,		&ashForeColor,			0,						87,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(0), (TM_STAND_IN_TILE),                                                                            "burned carpet",		"the carpet has been scorched by an ancient fire."},
	{FLOOR_CHAR,	&shallowWaterBackColor,	0,						80,	20,	0,				0,			0,				100,			NO_LIGHT,		(T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "a puddle of water",	"a puddle of water covers the ground."},
	{BONES_CHAR,	&bonesForeColor,		0,						70,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(0), (TM_STAND_IN_TILE),                                                                            "a pile of bones",		"unidentifiable bones, yellowed with age, litter the ground."},
	{BONES_CHAR,	&grayBrogue,					0,						70,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(0), (TM_STAND_IN_TILE),                                                                            "a pile of rubble",		"rocky rubble covers the ground."},
	{BONES_CHAR,	&mudBackColor,			&refuseBackColor,		50,	20,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(0), (TM_STAND_IN_TILE),                                                                            "a pile of filthy effects","primitive tools, carvings and trinkets are strewn about the area."},
	{BONES_CHAR,    &whiteBrogue,                 0,                      70, 0,  DF_PLAIN_FIRE,  0,          0,              0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE),                                                                            "shattered glass",      "jagged chunks of glass from the broken dewar litter the ground."},
	{FLOOR_CHAR,	&ectoplasmColor,		0,						70,	0,	DF_PLAIN_FIRE,	0,			0,				0,				ECTOPLASM_LIGHT,(0), (TM_STAND_IN_TILE),                                                                            "ectoplasmic residue",	"a thick, glowing substance has congealed on the ground."},
	{ASH_CHAR,		&fireForeColor,			0,						70,	0,	DF_PLAIN_FIRE,	0,			DF_ASH,			300,			EMBER_LIGHT,	(0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                               "sputtering embers",	"sputtering embers cover the ground."},
	{WEB_CHAR,		&whiteBrogue,					0,						19,	100,DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_ENTANGLES | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),"a spiderweb",       "thick, sticky spiderwebs fill the area."},
	{WEB_CHAR,		&brown,                 0,						19,	40, DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_ENTANGLES | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),"a net",             "a dense tangle of netting fills the area."},
	{FOLIAGE_CHAR,	&foliageColor,			0,						45,	15,	DF_PLAIN_FIRE,	0,			DF_TRAMPLED_FOLIAGE, 0,			NO_LIGHT,		(T_OBSTRUCTS_VISION | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_STEP), "dense foliage",   "dense foliage fills the area, thriving on what sunlight trickles in."},
	{FOLIAGE_CHAR,	&deadFoliageColor,		0,						45,	80,	DF_PLAIN_FIRE,	0,			DF_SMALL_DEAD_GRASS, 0,			NO_LIGHT,		(T_OBSTRUCTS_VISION | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_STEP), "dead foliage",    "the decaying husk of a fungal growth fills the area."},
	{TRAMPLED_FOLIAGE_CHAR,&foliageColor,	0,						60,	15,	DF_PLAIN_FIRE,	0,			DF_FOLIAGE_REGROW, 100,			NO_LIGHT,		(T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "trampled foliage",		"dense foliage fills the area, thriving on what sunlight trickles in."},
	{FOLIAGE_CHAR,	&fungusForestLightColor,0,						45,	15,	DF_PLAIN_FIRE,	0,			DF_TRAMPLED_FUNGUS_FOREST, 0,	FUNGUS_FOREST_LIGHT,(T_OBSTRUCTS_VISION | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_STEP),"a luminescent fungal forest", "luminescent fungal growth fills the area, groping upward from the rich soil."},
	{TRAMPLED_FOLIAGE_CHAR,&fungusForestLightColor,0,				60,	15,	DF_PLAIN_FIRE,	0,			DF_FUNGUS_FOREST_REGROW, 100,	FUNGUS_LIGHT,	(T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "trampled fungal foliage", "luminescent fungal growth fills the area, groping upward from the rich soil."},
	{WALL_CHAR,		&forceFieldColor,		&forceFieldColor,		0,	0,	0,				0,			DF_FORCEFIELD_MELT, -200,		FORCEFIELD_LIGHT, (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_GAS | T_OBSTRUCTS_DIAGONAL_MOVEMENT), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_STEP),		"a green crystal",		"The translucent green crystal is melting away in front of your eyes."},
	{WALL_CHAR,		&black,                 &forceFieldColor,		0,	0,	0,				0,			0,				-10000,			FORCEFIELD_LIGHT, (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_GAS | T_OBSTRUCTS_DIAGONAL_MOVEMENT), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),		"a dissolving crystal",		"The translucent green crystal is melting away in front of your eyes."},
	{CHASM_CHAR,    &sacredGlyphColor,      0,                      57, 0,  0,              0,          0,              0,              SACRED_GLYPH_LIGHT, (T_SACRED), 0,                                                                                  "a sacred glyph",       "a sacred glyph adorns the floor, glowing with a powerful warding enchantment."},
	{CHAIN_TOP_LEFT,&grayBrogue,					0,						20,	0,	0,				0,			0,				0,				NO_LIGHT,		0, 0,																								"an iron manacle",		"a thick iron manacle is anchored to the ceiling."},
	{CHAIN_BOTTOM_RIGHT, &grayBrogue,				0,						20,	0,	0,				0,			0,				0,				NO_LIGHT,		0, 0,																								"an iron manacle",		"a thick iron manacle is anchored to the floor."},
	{CHAIN_TOP_RIGHT, &grayBrogue,				0,						20,	0,	0,				0,			0,				0,				NO_LIGHT,		0, 0,																								"an iron manacle",		"a thick iron manacle is anchored to the ceiling."},
	{CHAIN_BOTTOM_LEFT, &grayBrogue,				0,						20,	0,	0,				0,			0,				0,				NO_LIGHT,		0, 0,																								"an iron manacle",		"a thick iron manacle is anchored to the floor."},
	{CHAIN_TOP,		&grayBrogue,					0,						20,	0,	0,				0,			0,				0,				NO_LIGHT,		0, 0,																								"an iron manacle",		"a thick iron manacle is anchored to the wall."},
	{CHAIN_BOTTOM,	&grayBrogue,					0,						20,	0,	0,				0,			0,				0,				NO_LIGHT,		0, 0,																								"an iron manacle",		"a thick iron manacle is anchored to the wall."},
	{CHAIN_LEFT,	&grayBrogue,					0,						20,	0,	0,				0,			0,				0,				NO_LIGHT,		0, 0,																								"an iron manacle",		"a thick iron manacle is anchored to the wall."},
	{CHAIN_RIGHT,	&grayBrogue,					0,						20,	0,	0,				0,			0,				0,				NO_LIGHT,		0, 0,																								"an iron manacle",		"a thick iron manacle is anchored to the wall."},
	{0,				0,						0,						1,	0,	0,				0,			0,				10000,			PORTAL_ACTIVATE_LIGHT,(0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),											"blinding light",		"blinding light streams out of the archway."},
	{0,				0,						0,						100,0,	0,				0,			0,				10000,			GLYPH_LIGHT_BRIGHT,(0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                            "a red glow",           "a red glow fills the area."},

	// fire tiles
	{FIRE_CHAR,		&fireForeColor,			0,						10,	0,	0,				0,			DF_EMBERS,		500,			FIRE_LIGHT,		(T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),				"billowing flames",		"flames billow upward."},
	{FIRE_CHAR,		&fireForeColor,			0,						10,	0,	0,				0,			0,				2500,			BRIMSTONE_FIRE_LIGHT,(T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),           "sulfurous flames",		"sulfurous flames leap from the unstable bed of brimstone."},
	{FIRE_CHAR,		&fireForeColor,			0,						10,	0,	0,				0,			DF_OBSIDIAN,	5000,			FIRE_LIGHT,		(T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),				"clouds of infernal flame", "billowing infernal flames eat at the floor."},
	{FIRE_CHAR,		&fireForeColor,			0,						10,	0,	0,              0,			0,              8000,			FIRE_LIGHT,		(T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),				"a cloud of burning gas", "burning gas fills the air with flame."},
	{FIRE_CHAR,		&yellow,				0,						10,	0,	0,				0,			0,              10000,			EXPLOSION_LIGHT,(T_IS_FIRE | T_CAUSES_EXPLOSIVE_DAMAGE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT), "a violent explosion",	"the force of the explosion slams into you."},
	{FIRE_CHAR,		&whiteBrogue,					0,						10,	0,	0,				0,			0,				10000,			INCENDIARY_DART_LIGHT ,(T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),			"a flash of fire",		"flames burst out of the incendiary dart."},
	{FIRE_CHAR,		&whiteBrogue,                 0,						10,	0,	0,				0,			DF_EMBERS,		3000,			FIRE_LIGHT,		(T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),				"crackling flames",		"crackling flames rise from the blackened item."},
	{FIRE_CHAR,		&whiteBrogue,                 0,						10,	0,	0,				0,			DF_EMBERS,		3000,			FIRE_LIGHT,		(T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),				"greasy flames",		"greasy flames rise from the corpse."},

	// gas layer
	{	' ',		0,						&poisonGasColor,		35,	100,DF_GAS_FIRE,	0,			0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE | T_CAUSES_DAMAGE), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES),							"a cloud of caustic gas", "you can feel the purple gas eating at your flesh."},
	{	' ',		0,						&confusionGasColor,		35,	100,DF_GAS_FIRE,	0,			0,				0,				CONFUSION_GAS_LIGHT,(T_IS_FLAMMABLE | T_CAUSES_CONFUSION), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES_QUICKLY),			"a cloud of confusion gas", "the rainbow-colored gas tickles your brain."},
	{	' ',		0,						&vomitColor,			35,	100,DF_GAS_FIRE,	0,			0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE | T_CAUSES_NAUSEA), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES_QUICKLY),					"a cloud of putrescence", "the stench of rotting flesh is overpowering."},
	{	' ',		0,						&vomitColor,			35,	0,  DF_GAS_FIRE,	0,			0,				0,				NO_LIGHT,		(T_CAUSES_NAUSEA), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES_QUICKLY),                                  "a cloud of putrid smoke", "you retch violently at the smell of the greasy smoke."},
	{	' ',		0,						&pink,					35,	100,DF_GAS_FIRE,	0,			0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE | T_CAUSES_PARALYSIS), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES_QUICKLY),				"a cloud of paralytic gas", "the pale gas causes your muscles to stiffen."},
	{	' ',		0,						&methaneColor,			35,	100,DF_GAS_FIRE,    0,          DF_EXPLOSION_FIRE, 0,			NO_LIGHT,		(T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_EXPLOSIVE_PROMOTE),                                        "a cloud of explosive gas",	"the smell of explosive swamp gas fills the air."},
	{	' ',		0,						&whiteBrogue,					35,	0,	DF_GAS_FIRE,	0,			0,				0,				NO_LIGHT,		(T_CAUSES_DAMAGE), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES_QUICKLY),									"a cloud of scalding steam", "scalding steam fills the air!"},
	{	' ',		0,						0,						35,	0,	DF_GAS_FIRE,	0,			0,				0,				DARKNESS_CLOUD_LIGHT,	(0), (TM_STAND_IN_TILE),                                                                    "a cloud of supernatural darkness", "everything is obscured by an aura of supernatural darkness."},
	{	' ',		0,						&darkRed,				35,	0,	DF_GAS_FIRE,	0,			0,				0,				NO_LIGHT,		(T_CAUSES_HEALING), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES_QUICKLY),                                 "a cloud of healing spores", "bloodwort spores, renowned for their healing properties, fill the air."},

	// bloodwort pods
	{FOLIAGE_CHAR,  &bloodflowerForeColor,  &bloodflowerBackColor,  10, 20, DF_PLAIN_FIRE,  0,          DF_BLOODFLOWER_PODS_GROW, 100,  NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_IS_FLAMMABLE), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT), "a bloodwort stalk", "this spindly plant grows seed pods famous for their healing properties."},
	{GOLD_CHAR,     &bloodflowerPodForeColor, 0,                    11, 20, DF_BLOODFLOWER_POD_BURST,0, DF_BLOODFLOWER_POD_BURST, 0,    NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_PLAYER_ENTRY | TM_VISUALLY_DISTINCT | TM_INVERT_WHEN_HIGHLIGHTED), "a bloodwort pod", "the bloodwort seed pod bursts, releasing a cloud of healing spores."},

	// shrine accoutrements
	{BRIDGE_CHAR,	&black,                 &bedrollBackColor,      57,	50,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "an abandoned bedroll", "a bedroll lies in the corner, disintegrating with age."},

	// algae
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_PLAIN_FIRE,	0,			DF_ALGAE_1,		100,			NO_LIGHT,		0, 0,                                                                                               "the ground",			""},
	{LIQUID_CHAR,	&deepWaterForeColor,    &deepWaterBackColor,	40,	100,DF_STEAM_ACCUMULATION,	0,	DF_ALGAE_1,     500,			LUMINESCENT_ALGAE_BLUE_LIGHT,(T_IS_FLAMMABLE | T_IS_DEEP_WATER), (TM_STAND_IN_TILE | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING),	"luminescent waters",	"blooming algae fills the waters with a swirling luminescence."},
	{LIQUID_CHAR,	&deepWaterForeColor,    &deepWaterBackColor,	39,	100,DF_STEAM_ACCUMULATION,	0,	DF_ALGAE_REVERT,300,			LUMINESCENT_ALGAE_GREEN_LIGHT,(T_IS_FLAMMABLE | T_IS_DEEP_WATER), (TM_STAND_IN_TILE | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING),	"luminescent waters",	"blooming algae fills the waters with a swirling luminescence."},

	// ancient spirit terrain
	{VINE_CHAR,		&lichenColor,           0,						19,	100,DF_PLAIN_FIRE,	0,			DF_ANCIENT_SPIRIT_GRASS,1000,	NO_LIGHT,		(T_ENTANGLES | T_CAUSES_DAMAGE | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT | TM_PROMOTES_ON_PLAYER_ENTRY),"thorned vines",       "thorned vines make a rustling noise as they quiver restlessly."},
	{GRASS_CHAR,	&grassColor,			0,						60,	15,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE), (TM_STAND_IN_TILE),                                                               "a tuft of grass",      "tufts of lush grass have improbably pushed upward through the stone ground."},

	// Yendor amulet floor tile
	{FLOOR_CHAR,	&floorForeColor,		&floorBackColor,		95,	0,	DF_PLAIN_FIRE,	0,			0,              0,				NO_LIGHT,       0, (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_ON_ITEM_PICKUP),                         "the ground",           ""},

	// commutation device
	{ALTAR_CHAR,	&altarForeColor,		&greenAltarBackColor,   17, 0,	0,				0,			DF_ALTAR_COMMUTE,0,             NO_LIGHT,       (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_SWAP_ENCHANTS_ACTIVATION | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),	"a commutation altar",	"crude diagrams on this altar and its twin invite you to place items upon them."},
	{ALTAR_CHAR,	&black,                 &greenAltarBackColor,   17, 0,	0,				0,			0,				0,				NO_LIGHT,       (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),							"a scorched altar",     "scorch marks cover the surface of the altar, but it is cold to the touch."},
	{'+',           &veryDarkGrayBrogue,          0,                      45, 0,  DF_PLAIN_FIRE,  0,          DF_INERT_PIPE,  0,              CONFUSION_GAS_LIGHT, (0), (TM_IS_WIRED | TM_VANISHES_UPON_PROMOTION),                                               "glowing glass pipes",  "glass pipes are set into the floor and emit a soft glow of shifting color."},
	{'+',           &black,                 0,                      45, 0,  DF_PLAIN_FIRE,  0,          0,              0,              NO_LIGHT,       (0), (0),                                                                                           "charred glass pipes",  "the inside of the glass pipes are charred."},

	// resurrection altar
	{ALTAR_CHAR,	&altarForeColor,		&goldAltarBackColor,    17, 0,	0,				0,			DF_ALTAR_RESURRECT,0,           CANDLE_LIGHT,   (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),           "a resurrection altar",	"the souls of the dead surround you. A deceased ally might be called back."},
	{ALTAR_CHAR,	&black,                 &goldAltarBackColor,    16, 0,	0,				0,			0,				0,				NO_LIGHT,       (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),							"a scorched altar",     "scorch marks cover the surface of the altar, but it is cold to the touch."},
	{0,             0,                      0,                      95,	0,	0,              0,			0,				0,				NO_LIGHT,		(0), (TM_IS_WIRED | TM_PROMOTES_ON_PLAYER_ENTRY),                                                   "the ground",			""},

	// sacrifice altar
	{ALTAR_CHAR,	&altarForeColor,		&altarBackColor,        17, 0,	0,				0,			DF_SACRIFICE_ALTAR,0,           CANDLE_LIGHT,   (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT), "a sacrifice altar",	"demonological symbols decorate this altar."},
	{ALTAR_CHAR,	&altarForeColor,		&altarBackColor,        17, 0,	0,				0,			DF_SACRIFICE_COMPLETE,0,        CANDLE_LIGHT,   (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_PROMOTES_ON_SACRIFICE_ENTRY), "a sacrifice altar",	"demonological symbols decorate this altar."},
	{LIQUID_CHAR,	&fireForeColor,			&lavaBackColor,			40,	0,	DF_OBSIDIAN,	0,			0,				0,				LAVA_LIGHT,		(T_LAVA_INSTA_DEATH), (TM_ALLOWS_SUBMERGING | TM_LIST_IN_SIDEBAR),                                  "sacrificial pit",      "the smell of burnt flesh lingers over this pit of lava."},
	{WALL_CHAR,		&altarBackColor,		&veryDarkGrayBrogue,			17, 0,	0,				0,			DF_SACRIFICE_CAGE_ACTIVE,	0,	CANDLE_LIGHT,	(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),"an iron cage","the cage won't budge. Perhaps there is a way to raise it nearby..."},
	{STATUE_CHAR,	&wallBackColor,			&statueBackColor,		0,	0,	DF_PLAIN_FIRE,	0,			0,				0,				DEMONIC_STATUE_LIGHT,	(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE),	"a demonic statue",	"An obsidian statue of a leering demon looms over the room."},

	// doorway statues
	{STATUE_CHAR,	&wallBackColor,			&statueBackColor,		0,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_CONNECTS_LEVEL),	"a marble statue",	"The cold marble statue has weathered the years with grace."},
	{STATUE_CHAR,	&wallBackColor,			&statueBackColor,		0,	0,	DF_PLAIN_FIRE,	0,			DF_CRACKING_STATUE,0,			NO_LIGHT,		(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_CONNECTS_LEVEL),"a marble statue",	"The cold marble statue has weathered the years with grace."},

	// extensible stone bridge
	{CHASM_CHAR,	&chasmForeColor,		&black,					40,	0,	DF_PLAIN_FIRE,	0,			0,              0,              NO_LIGHT,		(T_AUTO_DESCENT), (TM_STAND_IN_TILE),                                                               "a chasm",				"you plunge downward into the chasm!"},
	{FLOOR_CHAR,	&whiteBrogue,					&chasmEdgeBackColor,	40,	0,	DF_PLAIN_FIRE,	0,			DF_BRIDGE_ACTIVATE,6000,        NO_LIGHT,		(0), (TM_VANISHES_UPON_PROMOTION),                                                                  "a stone bridge",		"the narrow stone bridge is extending across the chasm."},
	{FLOOR_CHAR,	&whiteBrogue,					&chasmEdgeBackColor,	80,	0,	DF_PLAIN_FIRE,	0,			DF_BRIDGE_ACTIVATE_ANNOUNCE,0,	NO_LIGHT,		(0), (TM_IS_WIRED),                                                                                 "the brink of a chasm",	"chilly winds blow upward from the stygian depths."},

	// rat trap
	{WALL_CHAR,		&wallForeColor,			&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			DF_WALL_CRACK,  0,              NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),			"a stone wall",			"The rough stone wall is firm and unyielding."},
	{WALL_CHAR,		&wallForeColor,			&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			DF_WALL_SHATTER,500,			NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_LIST_IN_SIDEBAR),     "a cracking wall",		"Cracks are running ominously across the base of this rough stone wall."},

	// electric crystals
	{ELECTRIC_CRYSTAL_CHAR, &wallCrystalColor, &graniteBackColor,   0,  0,  DF_PLAIN_FIRE,  0,          DF_ELECTRIC_CRYSTAL_ON, 0,      NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_PROMOTES_ON_ELECTRICITY | TM_IS_CIRCUIT_BREAKER | TM_IS_WIRED | TM_LIST_IN_SIDEBAR), "a darkened crystal globe", "A slight electric shock startles you when you touch the inert crystal globe."},
	{ELECTRIC_CRYSTAL_CHAR, &whiteBrogue,         &wallCrystalColor,      0,  0,  DF_PLAIN_FIRE,  0,          0,              0,              CRYSTAL_WALL_LIGHT,(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR), "a shining crystal globe", "Crackling light streams out of the crystal globe."},
	{LEVER_CHAR,	&wallForeColor,			&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			DF_TURRET_LEVER,0,              NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_PLAYER_ENTRY | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_INVERT_WHEN_HIGHLIGHTED),"a lever", "The lever moves."},

	// worm tunnels
	{0,				0,						0,						100,0,	0,				0,			DF_WORM_TUNNEL_MARKER_ACTIVE,0, NO_LIGHT,       (0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),                                                    "",                     ""},
	{0,				0,						0,						100,0,	0,				0,			DF_GRANITE_CRUMBLES,-2000,		NO_LIGHT,		(0), (TM_VANISHES_UPON_PROMOTION),                                                                  "a rough granite wall",	"The granite is split open with splinters of rock jutting out at odd angles."},
	{WALL_CHAR,		&wallForeColor,			&wallBackColor,			0,	0,	DF_PLAIN_FIRE,	0,			DF_WALL_SHATTER,0,				NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_CONNECTS_LEVEL),"a stone wall", "The rough stone wall is firm and unyielding."},

	// zombie crypt
	{FIRE_CHAR,		&fireForeColor,			&statueBackColor,       0,	0,	DF_PLAIN_FIRE,	0,			0,				0,				BURNING_CREATURE_LIGHT,	(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_IS_FIRE), (TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR),"a ceremonial brazier",		"The ancient brazier smolders with a deep crimson flame."},

	// goblin warren
	{FLOOR_CHAR,    &mudBackColor,          &refuseBackColor,       85,	0,	DF_STENCH_SMOLDER,0,		0,				0,				NO_LIGHT,		(T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "the mud floor",		"Rotting animal matter has been ground into the mud floor; the stench is awful."},
	{WALL_CHAR,		&mudWallForeColor,		&mudWallBackColor,		0,	0,	DF_PLAIN_FIRE,	0,			0,				0,				NO_LIGHT,		(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE),														"a mud-covered wall",	"A malodorous layer of clay and fecal matter coats the wall."},
	{OMEGA_CHAR,	&mudWallForeColor,      &refuseBackColor,		25,	50,	DF_EMBERS,		0,			0,              0,				NO_LIGHT,		(T_OBSTRUCTS_VISION | T_OBSTRUCTS_GAS | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VISUALLY_DISTINCT), "hanging animal skins",	"you push through the animal skins that hang across the threshold."},
};

#pragma mark Dungeon Feature definitions

// Features in the gas layer use the startprob as volume, ignore probdecr, and spawn in only a single point.
// Intercepts and slopes are in units of 0.01.
dungeonFeature dungeonFeatureCatalog[NUMBER_DUNGEON_FEATURES] = {
	// tileType					layer		start	decr	fl	txt  flare   fCol fRad	propTerrain	subseqDF
	{0}, // nothing
	{GRANITE,					DUNGEON,	80,		70,		DFF_CLEAR_OTHER_TERRAIN},
	{CRYSTAL_WALL,				DUNGEON,	200,	50,		DFF_CLEAR_OTHER_TERRAIN},
	{LUMINESCENT_FUNGUS,		SURFACE,	60,		8,		DFF_BLOCKED_BY_OTHER_LAYERS},
	{GRASS,						SURFACE,	75,		5,		DFF_BLOCKED_BY_OTHER_LAYERS},
	{DEAD_GRASS,				SURFACE,	75,		5,		DFF_BLOCKED_BY_OTHER_LAYERS,	"", 0,	0,	0,		0,			DF_DEAD_FOLIAGE},
	{BONES,						SURFACE,	75,		23,		0},
	{RUBBLE,					SURFACE,	45,		23,		0},
	{FOLIAGE,					SURFACE,	100,	33,		(DFF_BLOCKED_BY_OTHER_LAYERS)},
	{FUNGUS_FOREST,				SURFACE,	100,	45,		(DFF_BLOCKED_BY_OTHER_LAYERS)},
	{DEAD_FOLIAGE,				SURFACE,	50,		30,		(DFF_BLOCKED_BY_OTHER_LAYERS)},

	// misc. liquids
	{SUNLIGHT_POOL,				LIQUID,		65,		6,		0},
	{DARKNESS_PATCH,			LIQUID,		65,		11,		0},

	// Dungeon features spawned during gameplay:

	// revealed secrets
	{DOOR,						DUNGEON,	0,		0,		0, "", GENERIC_FLASH_LIGHT},
	{GAS_TRAP_POISON,			DUNGEON,	0,		0,		0, "", GENERIC_FLASH_LIGHT},
	{GAS_TRAP_PARALYSIS,		DUNGEON,	0,		0,		0, "", GENERIC_FLASH_LIGHT},
	{CHASM_EDGE,				LIQUID,		100,	100,	0, "", GENERIC_FLASH_LIGHT},
	{TRAP_DOOR,					LIQUID,		0,		0,		DFF_CLEAR_OTHER_TERRAIN, "", GENERIC_FLASH_LIGHT, 0, 0, 0, DF_SHOW_TRAPDOOR_HALO},
	{GAS_TRAP_CONFUSION,		DUNGEON,	0,		0,		0, "", GENERIC_FLASH_LIGHT},
	{FLAMETHROWER,				DUNGEON,	0,		0,		0, "", GENERIC_FLASH_LIGHT},
	{FLOOD_TRAP,				DUNGEON,	0,		0,		0, "", GENERIC_FLASH_LIGHT},
	{NET_TRAP,                  DUNGEON,	0,		0,		0, "", GENERIC_FLASH_LIGHT},
	{ALARM_TRAP,                DUNGEON,	0,		0,		0, "", GENERIC_FLASH_LIGHT},

	// bloods
	// Start probability is actually a percentage for bloods.
	// Base probability is 15 + (damage * 2/3), and then take the given percentage of that.
	// If it's a gas, we multiply the base by an additional 100.
	// Thus to get a starting gas volume of a poison potion (1000), with a hit for 10 damage, use a starting probability of 48.
	{RED_BLOOD,					SURFACE,	100,	25,		0},
	{GREEN_BLOOD,				SURFACE,	100,	25,		0},
	{PURPLE_BLOOD,				SURFACE,	100,	25,		0},
	{WORM_BLOOD,				SURFACE,	100,	25,		0},
	{ACID_SPLATTER,				SURFACE,	200,	25,		0},
	{ASH,						SURFACE,	50,		25,		0},
	{EMBERS,					SURFACE,	125,	25,		0},
	{ECTOPLASM,					SURFACE,	110,	25,		0},
	{RUBBLE,					SURFACE,	33,		25,		0},
	{ROT_GAS,					GAS,		12,		0,		0},

	// monster effects
	{VOMIT,						SURFACE,	30,		10,		0},
	{POISON_GAS,				GAS,		2000,	0,		0},
	{GAS_EXPLOSION,				SURFACE,	350,	100,	0,	"", EXPLOSION_FLARE_LIGHT},
	{RED_BLOOD,					SURFACE,	150,	30,		0},
	{FLAMEDANCER_FIRE,			SURFACE,	200,	75,		0},

	// mutation effects
	{GAS_EXPLOSION,				SURFACE,	350,	100,	0,	"The corpse detonates with terrifying force!", EXPLOSION_FLARE_LIGHT},
	{LICHEN,					SURFACE,	70,		60,		0,  "Poisonous spores burst from the corpse!"},

	// misc
	{NOTHING,					GAS,		0,		0,		DFF_EVACUATE_CREATURES_FIRST},
	{ROT_GAS,					GAS,		15,		0,		0},
	{STEAM,						GAS,		325,	0,		0},
	{STEAM,						GAS,		15,		0,		0},
	{METHANE_GAS,				GAS,		2,		0,		0},
	{EMBERS,					SURFACE,	0,		0,		0},
	{URINE,						SURFACE,	65,		25,		0},
	{UNICORN_POOP,				SURFACE,	65,		40,		0},
	{PUDDLE,					SURFACE,	13,		25,		0},
	{ASH,						SURFACE,	0,		0,		0},
	{ECTOPLASM,					SURFACE,	0,		0,		0},
	{FORCEFIELD,				SURFACE,	100,	50,		0},
	{FORCEFIELD_MELT,           SURFACE,    0,      0,      0},
	{SACRED_GLYPH,              LIQUID,     100,    100,    0,  "", EMPOWERMENT_LIGHT},
	{LICHEN,					SURFACE,	2,		100,	(DFF_BLOCKED_BY_OTHER_LAYERS)}, // Lichen won't spread through lava.
	{RUBBLE,					SURFACE,	45,		23,		(DFF_ACTIVATE_DORMANT_MONSTER)},
	{RUBBLE,					SURFACE,	0,		0,		(DFF_ACTIVATE_DORMANT_MONSTER)},

	{SPIDERWEB,                 SURFACE,    15,     12,     0},
	{SPIDERWEB,                 SURFACE,    100,    39,     0},

	{ANCIENT_SPIRIT_VINES,      SURFACE,    75,     70,     0},
	{ANCIENT_SPIRIT_GRASS,      SURFACE,    50,     47,     0},

	// foliage
	{TRAMPLED_FOLIAGE,			SURFACE,	0,		0,		0},
	{DEAD_GRASS,				SURFACE,	75,		75,		0},
	{FOLIAGE,					SURFACE,	0,		0,		(DFF_BLOCKED_BY_OTHER_LAYERS)},
	{TRAMPLED_FUNGUS_FOREST,	SURFACE,	0,		0,		0},
	{FUNGUS_FOREST,				SURFACE,	0,		0,		(DFF_BLOCKED_BY_OTHER_LAYERS)},

	// brimstone
	{ACTIVE_BRIMSTONE,			LIQUID,		0,		0,		0},
	{INERT_BRIMSTONE,			LIQUID,		0,		0,		0,	"", 0,	0,	0,		0,			DF_BRIMSTONE_FIRE},

	// bloodwort
	{BLOODFLOWER_POD,           SURFACE,    60,     60,     DFF_EVACUATE_CREATURES_FIRST},
	{BLOODFLOWER_POD,           SURFACE,    10,     10,     DFF_EVACUATE_CREATURES_FIRST},
	{HEALING_CLOUD,				GAS,		350,	0,		0},

	// dewars
	{POISON_GAS,                GAS,        20000,  0,      0, "the dewar shatters and pressurized caustic gas explodes outward!", 0, &poisonGasColor, 4, 0, DF_DEWAR_GLASS},
	{CONFUSION_GAS,             GAS,        20000,  0,      0, "the dewar shatters and pressurized confusion gas explodes outward!", 0, &confusionGasColor, 4, 0, DF_DEWAR_GLASS},
	{PARALYSIS_GAS,             GAS,        20000,  0,      0, "the dewar shatters and pressurized paralytic gas explodes outward!", 0, &pink, 4, 0, DF_DEWAR_GLASS},
	{METHANE_GAS,               GAS,        20000,  0,      0, "the dewar shatters and pressurized methane gas explodes outward!", 0, &methaneColor, 4, 0, DF_DEWAR_GLASS},
	{BROKEN_GLASS,              SURFACE,    100,    70,     0},
	{CARPET,                    DUNGEON,    120,    20,     0},

	// algae
	{DEEP_WATER_ALGAE_WELL,     DUNGEON,    0,      0,      DFF_SUPERPRIORITY},
	{DEEP_WATER_ALGAE_1,		LIQUID,		50,		100,	0,  "", 0,  0,   0,     DEEP_WATER, DF_ALGAE_2},
	{DEEP_WATER_ALGAE_2,        LIQUID,     0,      0,      0},
	{DEEP_WATER,                LIQUID,     0,      0,      DFF_SUPERPRIORITY},

	// doors, item cages, altars, glyphs, guardians -- reusable machine components
	{OPEN_DOOR,					DUNGEON,	0,		0,		0},
	{DOOR,						DUNGEON,	0,		0,		0},
	{OPEN_IRON_DOOR_INERT,		DUNGEON,	0,		0,		0,  "", GENERIC_FLASH_LIGHT},
	{ALTAR_CAGE_OPEN,			DUNGEON,	0,		0,		0,	"the cages lift off of the altars as you approach.", GENERIC_FLASH_LIGHT},
	{ALTAR_CAGE_CLOSED,			DUNGEON,	0,		0,		(DFF_EVACUATE_CREATURES_FIRST), "the cages lower to cover the altars.", GENERIC_FLASH_LIGHT},
	{ALTAR_INERT,				DUNGEON,	0,		0,		0},
	{FLOOR_FLOODABLE,			DUNGEON,	0,		0,		0,	"the altar retracts into the ground with a grinding sound.", GENERIC_FLASH_LIGHT},
	{PORTAL_LIGHT,				SURFACE,	0,		0,		(DFF_EVACUATE_CREATURES_FIRST | DFF_ACTIVATE_DORMANT_MONSTER), "the archway flashes, and you catch a glimpse of another world!"},
	{MACHINE_GLYPH_INACTIVE,    DUNGEON,    0,      0,      0},
	{MACHINE_GLYPH,             DUNGEON,    0,      0,      0},
	{GUARDIAN_GLOW,             SURFACE,    0,      0,      0,  ""},
	{GUARDIAN_GLOW,             SURFACE,    0,      0,      0,  "the glyph beneath you glows, and the guardians take a step!"},
	{GUARDIAN_GLOW,             SURFACE,    0,      0,      0,  "the mirrored totem flashes, reflecting the red glow of the glyph beneath you."},
	{MACHINE_GLYPH,             DUNGEON,    200,    95,     DFF_BLOCKED_BY_OTHER_LAYERS},
	{WALL_LEVER,                DUNGEON,    0,      0,      0,  "you notice a lever hidden behind a loose stone in the wall.", GENERIC_FLASH_LIGHT},
	{WALL_LEVER_PULLED,         DUNGEON,    0,      0,      0},
	{WALL_LEVER_HIDDEN,         DUNGEON,    0,      0,      0},

	{BRIDGE_FALLING,            LIQUID,     200,    100,    0, "", 0, 0, 0, BRIDGE},
	{CHASM,                     LIQUID,     0,      0,      0, "", GENERIC_FLASH_LIGHT, 0, 0, 0, DF_BRIDGE_FALL_PREP},

	// fire
	{PLAIN_FIRE,				SURFACE,	0,		0,		0},
	{GAS_FIRE,					SURFACE,	0,		0,		0},
	{GAS_EXPLOSION,				SURFACE,	60,		17,		0},
	{DART_EXPLOSION,			SURFACE,	0,		0,		0},
	{BRIMSTONE_FIRE,			SURFACE,	0,		0,		0},
	{0,                         0,          0,		0,		0,	"the rope bridge snaps from the heat and plunges into the chasm!", FALLEN_TORCH_FLASH_LIGHT,	0,	0,		0,		DF_BRIDGE_FALL},
	{PLAIN_FIRE,				SURFACE,	100,	37,		0},
	{EMBERS,					SURFACE,	0,		0,		0},
	{EMBERS,					SURFACE,	100,	94,		0},
	{OBSIDIAN,					SURFACE,	0,		0,		0},
	{ITEM_FIRE,                 SURFACE,    0,      0,      0,  "", FALLEN_TORCH_FLASH_LIGHT},
	{CREATURE_FIRE,             SURFACE,    0,      0,      0,  "", FALLEN_TORCH_FLASH_LIGHT},

	{FLOOD_WATER_SHALLOW,		SURFACE,	225,	37,		0,	"", 0,	0,	0,		0,			DF_FLOOD_2},
	{FLOOD_WATER_DEEP,			SURFACE,	175,	37,		0,	"the area is flooded as water rises through imperceptible holes in the ground."},
	{FLOOD_WATER_SHALLOW,		SURFACE,	10,		25,		0},
	{HOLE,						SURFACE,	200,	100,	0},
	{HOLE_EDGE,					SURFACE,	0,		0,		0},

	// ice effects
	{ICE_DEEP,                  LIQUID,     150,	50,		DFF_EVACUATE_CREATURES_FIRST,	"", 0,	0,	0,		DEEP_WATER,			DF_ALGAE_1_FREEZE},
	{ICE_DEEP,                  LIQUID,     150,	50,		DFF_EVACUATE_CREATURES_FIRST,	"", 0,	0,	0,		DEEP_WATER_ALGAE_1,	DF_ALGAE_2_FREEZE},
	{ICE_DEEP,                  LIQUID,     150,	50,		DFF_EVACUATE_CREATURES_FIRST,	"", 0,	0,	0,		DEEP_WATER_ALGAE_2,	DF_SHALLOW_WATER_FREEZE},
	{ICE_DEEP_MELT,             LIQUID,     0,      0,      0},
	{DEEP_WATER,                LIQUID,     0,      0,      0},
	{ICE_SHALLOW,               LIQUID,     100,	50,		DFF_EVACUATE_CREATURES_FIRST,	"", 0,	0,	0,		SHALLOW_WATER},
	{ICE_SHALLOW_MELT,          LIQUID,     0,      0,      0},
	{SHALLOW_WATER,             LIQUID,     0,      0,      0},

	// gas trap effects
	{POISON_GAS,				GAS,		1000,	0,		0,	"a cloud of caustic gas sprays upward from the floor!"},
	{CONFUSION_GAS,				GAS,		300,	0,		0,	"a sparkling cloud of confusion gas sprays upward from the floor!"},
	{NETTING,                   SURFACE,    300,    90,     0,  "a net falls from the ceiling!"},
	{0,                         0,          0,      0,      DFF_AGGRAVATES_MONSTERS, "a piercing shriek echoes through the nearby rooms!", 0, 0, DCOLS / 2},
	{METHANE_GAS,				GAS,		10000,	0,		0}, // debugging toy

	// potions
	{POISON_GAS,				GAS,		1000,	0,		0,	"", 0,	&poisonGasColor,4},
	{PARALYSIS_GAS,				GAS,		1000,	0,		0,	"", 0,	&pink,4},
	{CONFUSION_GAS,				GAS,		1000,	0,		0,	"", 0,	&confusionGasColor, 4},
	{PLAIN_FIRE,				SURFACE,	100,	37,		0,	"", EXPLOSION_FLARE_LIGHT},
	{DARKNESS_CLOUD,			GAS,		200,	0,		0},
	{HOLE_EDGE,					SURFACE,	300,	100,	0,	"", 0,	&darkBlue,3,0,			DF_HOLE_2},
	{LICHEN,					SURFACE,	70,		60,		0},

	// other items
	{PLAIN_FIRE,				SURFACE,	100,	45,		0,	"", 0,	&yellow,3},
	{HOLE_GLOW,					SURFACE,	200,	100,	DFF_SUBSEQ_EVERYWHERE,	"", 0,	&darkBlue,3,0,			DF_STAFF_HOLE_EDGE},
	{HOLE_EDGE,					SURFACE,	100,	100,	0},

	// machine components

	// commutation altars
	{COMMUTATION_ALTAR_INERT,   DUNGEON,    0,      0,      0,  "the items on the two altars flash with a brilliant light!", SCROLL_ENCHANTMENT_LIGHT},
	{PIPE_GLOWING,              SURFACE,    90,     60,     0},
	{PIPE_INERT,                SURFACE,    0,      0,      0,  "", SCROLL_ENCHANTMENT_LIGHT},

	// resurrection altars
	{RESURRECTION_ALTAR_INERT,  DUNGEON,    0,      0,      DFF_RESURRECT_ALLY, "An old friend emerges from a bloom of sacred light!", EMPOWERMENT_LIGHT},
	{MACHINE_TRIGGER_FLOOR_REPEATING, LIQUID, 300,  100,    DFF_SUPERPRIORITY},

	// sacrifice altars
	{SACRIFICE_ALTAR,           DUNGEON,    0,      0,      0,  "a demonic presence whispers its demand: \"bring to me the marked sacrifice!\""},
	{SACRIFICE_LAVA,            DUNGEON,    0,      0,      0,  "demonic cackling echoes through the room as the altar plunges downward!"},
	{ALTAR_CAGE_RETRACTABLE,    DUNGEON,    0,      0,      0},

	// coffin bursts open to reveal vampire:
	{COFFIN_OPEN,				DUNGEON,	0,		0,		DFF_ACTIVATE_DORMANT_MONSTER,	"the coffin opens and a dark figure rises!", 0, &darkGrayBrogue, 3},
	{PLAIN_FIRE,				SURFACE,	0,		0,		DFF_ACTIVATE_DORMANT_MONSTER,	"as flames begin to lick the coffin, its tenant bursts forth!", 0, 0, 0, 0, DF_EMBERS_PATCH},
	{MACHINE_TRIGGER_FLOOR,		DUNGEON,	200,	100,	0},

	// throwing tutorial:
	{ALTAR_INERT,				DUNGEON,	0,		0,		0,	"the cage lifts off of the altar.", GENERIC_FLASH_LIGHT},
	{TRAP_DOOR,					LIQUID,		225,	100,	(DFF_CLEAR_OTHER_TERRAIN | DFF_SUBSEQ_EVERYWHERE), "", 0, 0, 0, 0, DF_SHOW_TRAPDOOR_HALO},
	{LAVA,						LIQUID,		225,	100,	(DFF_CLEAR_OTHER_TERRAIN)},
	{MACHINE_PRESSURE_PLATE_USED,DUNGEON,   0,      0,      0},

	// rat trap:
	{RAT_TRAP_WALL_CRACKING,    DUNGEON,    0,      0,      0,  "a scratching sound emanates from the nearby walls!", 0, 0, 0, 0, DF_RUBBLE},

	// wooden barricade at entrance:
	{PLAIN_FIRE,				SURFACE,	0,		0,		0,	"flames quickly consume the wooden barricade."},

	// wooden barricade around altar:
	{WOODEN_BARRICADE,          DUNGEON,	220,	100,	(DFF_TREAT_AS_BLOCKING | DFF_SUBSEQ_EVERYWHERE), "", 0, 0, 0, 0, DF_SMALL_DEAD_GRASS},

	// shallow water flood machine:
	{MACHINE_FLOOD_WATER_SPREADING,	LIQUID,	0,		0,		0,	"you hear a heavy click, and the nearby water begins flooding the area!"},
	{SHALLOW_WATER,				LIQUID,		0,		0,		0},
	{MACHINE_FLOOD_WATER_SPREADING,LIQUID,	100,	100,	0,	"", 0,	0,	0,		FLOOR_FLOODABLE,			DF_SHALLOW_WATER},
	{MACHINE_FLOOD_WATER_DORMANT,LIQUID,	250,	100,	(DFF_TREAT_AS_BLOCKING), "", 0, 0, 0, 0,            DF_SPREADABLE_DEEP_WATER_POOL},
	{DEEP_WATER,				LIQUID,		90,		100,	(DFF_CLEAR_OTHER_TERRAIN | DFF_PERMIT_BLOCKING)},

	// unstable floor machine:
	{MACHINE_COLLAPSE_EDGE_SPREADING,LIQUID,0,		0,		0,	"you hear a deep rumbling noise as the floor begins to collapse!"},
	{CHASM,						LIQUID,		0,		0,		DFF_CLEAR_OTHER_TERRAIN, "", 0, 0, 0, 0, DF_SHOW_TRAPDOOR_HALO},
	{MACHINE_COLLAPSE_EDGE_SPREADING,LIQUID,100,	100,	0,	"", 0,	0,	0,	FLOOR_FLOODABLE,	DF_COLLAPSE},
	{MACHINE_COLLAPSE_EDGE_DORMANT,LIQUID,	0,		0,		0},

	// levitation bridge machine:
	{CHASM_WITH_HIDDEN_BRIDGE_ACTIVE,LIQUID,100,    100,    0,  "", 0, 0,  0,  CHASM_WITH_HIDDEN_BRIDGE,  DF_BRIDGE_APPEARS},
	{CHASM_WITH_HIDDEN_BRIDGE_ACTIVE,LIQUID,100,    100,    0,  "a stone bridge extends from the floor with a grinding sound.", 0, 0,  0,  CHASM_WITH_HIDDEN_BRIDGE,  DF_BRIDGE_APPEARS},
	{STONE_BRIDGE,				LIQUID,		0,		0,		0},
	{MACHINE_CHASM_EDGE,        LIQUID,     100,	100,	0},

	// retracting lava pool:
	{LAVA_RETRACTABLE,          LIQUID,     100,    100,    0,  "", 0, 0,  0,  LAVA},
	{LAVA_RETRACTING,			LIQUID,		0,		0,		0,	"hissing fills the air as the lava begins to cool."},
	{OBSIDIAN,					SURFACE,	0,		0,		0,	"", 0,	0,	0,		0,			DF_STEAM_ACCUMULATION},

	// hidden poison vent machine:
	{MACHINE_POISON_GAS_VENT_DORMANT,DUNGEON,0,		0,		0,	"you notice an inactive gas vent hidden in a crevice of the floor.", GENERIC_FLASH_LIGHT},
	{MACHINE_POISON_GAS_VENT,	DUNGEON,	0,		0,		0,	"deadly purple gas starts wafting out of hidden vents in the floor!"},
	{PORTCULLIS_CLOSED,			DUNGEON,	0,		0,		DFF_EVACUATE_CREATURES_FIRST,	"with a heavy mechanical sound, an iron portcullis falls from the ceiling!", GENERIC_FLASH_LIGHT},
	{PORTCULLIS_DORMANT,		DUNGEON,	0,		0,		0,  "the portcullis slowly rises from the ground into a slot in the ceiling.", GENERIC_FLASH_LIGHT},
	{POISON_GAS,				GAS,		25,		0,		0},

	// hidden methane vent machine:
	{MACHINE_METHANE_VENT_DORMANT,DUNGEON,0,		0,		0,	"you notice an inactive gas vent hidden in a crevice of the floor.", GENERIC_FLASH_LIGHT},
	{MACHINE_METHANE_VENT,		DUNGEON,	0,		0,		0,	"explosive methane gas starts wafting out of hidden vents in the floor!", 0, 0, 0, 0, DF_VENT_SPEW_METHANE},
	{METHANE_GAS,				GAS,		60,		0,		0},
	{PILOT_LIGHT,				DUNGEON,	0,		0,		0,	"a torch falls from its mount and lies sputtering on the floor.", FALLEN_TORCH_FLASH_LIGHT},

	// paralysis trap:
	{MACHINE_PARALYSIS_VENT,    DUNGEON,    0,		0,		0,	"you notice an inactive gas vent hidden in a crevice of the floor.", GENERIC_FLASH_LIGHT},
	{PARALYSIS_GAS,				GAS,		350,	0,		0,	"paralytic gas sprays upward from hidden vents in the floor!", 0, 0, 0, 0, DF_REVEAL_PARALYSIS_VENT_SILENTLY},
	{MACHINE_PARALYSIS_VENT,    DUNGEON,    0,		0,		0},

	// thematic dungeon:
	{RED_BLOOD,					SURFACE,	75,     25,		0},

	// statuary:
	{STATUE_CRACKING,			DUNGEON,	0,		0,		0,	"cracks begin snaking across the marble surface of the statue!", 0, 0, 0, 0, DF_RUBBLE},
	{RUBBLE,					SURFACE,	120,	100,	DFF_ACTIVATE_DORMANT_MONSTER,	"the statue shatters!", 0, &darkGrayBrogue, 3, 0, DF_RUBBLE},

	// hidden turrets:
	{WALL,                      DUNGEON,	0,		0,		DFF_ACTIVATE_DORMANT_MONSTER,	"you hear a click, and the stones in the wall shift to reveal turrets!", 0, 0, 0, 0, DF_RUBBLE},

	// worm tunnels:
	{WORM_TUNNEL_MARKER_DORMANT,LIQUID,     5,      5,      0,  "", 0,  0,  GRANITE},
	{WORM_TUNNEL_MARKER_ACTIVE, LIQUID,     0,      0,      0},
	{FLOOR,                     DUNGEON,    0,      0,      (DFF_SUPERPRIORITY | DFF_ACTIVATE_DORMANT_MONSTER),  "", 0, 0,  0,  0,  DF_TUNNELIZE},
	{FLOOR,                     DUNGEON,    0,      0,      0,  "the nearby wall cracks and collapses in a cloud of dust!", 0, &darkGrayBrogue,  5,  0,  DF_TUNNELIZE},

	// haunted room:
	{DARK_FLOOR_DARKENING,		DUNGEON,	0,		0,		0,	"the light in the room flickers and you feel a chill in the air."},
	{DARK_FLOOR,				DUNGEON,	0,		0,		DFF_ACTIVATE_DORMANT_MONSTER,	"", 0, 0, 0, 0, DF_ECTOPLASM_DROPLET},
	{HAUNTED_TORCH_TRANSITIONING,DUNGEON,   0,      0,      0},
	{HAUNTED_TORCH,             DUNGEON,    0,      0,      0},

	// mud pit:
	{MACHINE_MUD_DORMANT,		LIQUID,		100,	100,	0},
	{MUD,						LIQUID,		0,		0,		DFF_ACTIVATE_DORMANT_MONSTER,	"across the bog, bubbles rise ominously from the mud."},

	// electric crystals:
	{ELECTRIC_CRYSTAL_ON,       DUNGEON,    0,      0,      0, "the crystal absorbs the electricity and begins to glow.", CHARGE_FLASH_LIGHT},
	{WALL,                      DUNGEON,	0,		0,		DFF_ACTIVATE_DORMANT_MONSTER,	"the wall above the lever shifts to reveal a spark turret!"},

	// idyll:
	{SHALLOW_WATER,				LIQUID,		150,	100,	(DFF_PERMIT_BLOCKING)},
	{DEEP_WATER,				LIQUID,		90,		100,	(DFF_TREAT_AS_BLOCKING | DFF_CLEAR_OTHER_TERRAIN | DFF_SUBSEQ_EVERYWHERE), "", 0, 0, 0, 0, DF_SHALLOW_WATER_POOL},

	// swamp:
	{SHALLOW_WATER,				LIQUID,		30,		100,	0},
	{GRAY_FUNGUS,				SURFACE,	80,		50,		0,	"", 0, 0, 0, 0, DF_SWAMP_MUD},
	{MUD,						LIQUID,		75,		5,		0,	"", 0, 0, 0, 0, DF_SWAMP_WATER},

	// camp:
	{HAY,						SURFACE,	90,		87,		0},
	{JUNK,						SURFACE,	20,		20,		0},

	// remnants:
	{CARPET,					DUNGEON,	110,	20,		DFF_SUBSEQ_EVERYWHERE,	"", 0, 0, 0, 0, DF_REMNANT_ASH},
	{BURNED_CARPET,				SURFACE,	120,	100,	0},

	// chasm catwalk:
	{CHASM,						LIQUID,		0,		0,		DFF_CLEAR_OTHER_TERRAIN, "", 0, 0, 0, 0, DF_SHOW_TRAPDOOR_HALO},
	{STONE_BRIDGE,				LIQUID,		0,		0,		DFF_CLEAR_OTHER_TERRAIN},

	// lake catwalk:
	{DEEP_WATER,				LIQUID,		0,		0,		DFF_CLEAR_OTHER_TERRAIN, "", 0, 0, 0, 0, DF_LAKE_HALO},
	{SHALLOW_WATER,				LIQUID,		160,	100,	0},

	// worms pop out of walls:
	{RUBBLE,					SURFACE,	120,	100,	DFF_ACTIVATE_DORMANT_MONSTER,	"the nearby wall explodes in a shower of stone fragments!", 0, &darkGrayBrogue, 3, 0, DF_RUBBLE},

	// monster cages open:
	{MONSTER_CAGE_OPEN,			DUNGEON,	0,		0,		0},

	// goblin warren:
	{STENCH_SMOKE_GAS,          GAS,		50,		0,		0, "", 0, 0, 0, 0, DF_PLAIN_FIRE},
	{STENCH_SMOKE_GAS,          GAS,		50,		0,		0, "", 0, 0, 0, 0, DF_EMBERS},
};

#pragma mark Dungeon Profiles

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

#pragma mark Lights

// radius is in units of 0.01
const lightSource lightCatalog[NUMBER_LIGHT_KINDS] = {
	//color					radius range			fade%	passThroughCreatures
	{0},																// NO_LIGHT
	{&minersLightColor,		{0, 0, 1},				35,		true},		// miners light
	{&fireBoltColor,		{300, 400, 1},			0,		false},		// burning creature light
	{&wispLightColor,		{400, 800, 1},			0,		false},		// will-o'-the-wisp light
	{&fireBoltColor,		{300, 400, 1},			0,		false},		// salamander glow
	{&pink,					{600, 600, 1},			0,		true},		// imp light
	{&pixieColor,			{400, 600, 1},			50,		false},		// pixie light
	{&lichLightColor,		{1500, 1500, 1},		0,		false},		// lich light
	{&flamedancerCoronaColor,{1000, 2000, 1},		0,		false},		// flamedancer light
	{&sentinelLightColor,	{300, 500, 1},			0,		false},		// sentinel light
	{&unicornLightColor,	{300, 400, 1},			0,		false},		// unicorn light
	{&ifritLightColor,		{300, 600, 1},			0,		false},		// ifrit light
	{&fireBoltColor,		{400, 600, 1},			0,		false},		// phoenix light
	{&fireBoltColor,		{150, 300, 1},			0,		false},		// phoenix egg light
	{&yendorLightColor,		{1500, 1500, 1},        0,		false},		// Yendorian light
	{&spectralBladeLightColor,{350, 350, 1},		0,		false},		// spectral blades
	{&summonedImageLightColor,{350, 350, 1},		0,		false},		// weapon images
	{&lightningColor,		{250, 250, 1},			35,		false},		// lightning turret light
	{&explosiveAuraColor,   {150, 200, 1},          0,      true},      // explosive bloat light
	{&lightningColor,		{300, 300, 1},			0,		false},		// bolt glow
	{&telepathyColor,		{200, 200, 1},			0,		true},		// telepathy light
	{&sacrificeTargetColor, {250, 250, 1},          0,      true},      // sacrifice doom light

	// flares:
	{&scrollProtectionColor,{600, 600, 1},			0,		true},		// scroll of protection flare
	{&scrollEnchantmentColor,{600, 600, 1},			0,		true},		// scroll of enchantment flare
	{&potionStrengthColor,  {600, 600, 1},			0,		true},		// potion of strength flare
	{&empowermentFlashColor,{600, 600, 1},			0,		true},		// empowerment flare
	{&genericFlashColor,    {300, 300, 1},			0,		true},		// generic flash flare
	{&fireFlashColor,		{800, 800, 1},			0,		false},		// fallen torch flare
	{&summoningFlashColor,  {600, 600, 1},			0,		true},		// summoning flare
	{&explosionFlareColor,  {5000, 5000, 1},        0,      true},      // explosion (explosive bloat or incineration potion)
	{&quietusFlashColor,	{300, 300, 1},			0,		true},		// quietus activation flare
	{&slayingFlashColor,	{300, 300, 1},			0,		true},		// slaying activation flare
	{&lightningColor,       {800, 800, 1},          0,      false},     // electric crystal activates

	// glowing terrain:
	{&torchLightColor,		{1000, 1000, 1},		50,		false},		// torch
	{&lavaLightColor,		{300, 300, 1},			50,		false},		// lava
	{&sunLightColor,		{200, 200, 1},			25,		true},		// sunlight
	{&darknessPatchColor,	{400, 400, 1},			0,		true},		// darkness patch
	{&fungusLightColor,		{300, 300, 1},			50,		false},		// luminescent fungus
	{&fungusForestLightColor,{500, 500, 1},			0,		false},		// luminescent forest
	{&algaeBlueLightColor,	{300, 300, 1},			0,		false},		// luminescent algae blue
	{&algaeGreenLightColor,	{300, 300, 1},			0,		false},		// luminescent algae green
	{&ectoplasmColor,		{200, 200, 1},			50,		false},		// ectoplasm
	{&unicornLightColor,	{200, 200, 1},			0,		false},		// unicorn poop light
	{&lavaLightColor,		{200, 200, 1},			50,		false},		// embers
	{&lavaLightColor,		{500, 1000, 1},			0,		false},		// fire
	{&lavaLightColor,		{200, 300, 1},			0,		false},		// brimstone fire
	{&explosionColor,		{DCOLS * 100,DCOLS * 100,1},100,	false},		// explosions
	{&dartFlashColor,		{15 * 100,15 * 100,1},		0,		false},		// incendiary darts
	{&portalActivateLightColor,	{DCOLS * 100,DCOLS * 100,1},0,	false},		// portal activation
	{&confusionLightColor,	{300, 300, 1},			100,	false},		// confusion gas
	{&darknessCloudColor,	{500, 500, 1},			0,		true},		// darkness cloud
	{&forceFieldLightColor,	{200, 200, 1},			50,		false},		// forcefield
	{&crystalWallLightColor,{300, 500, 1},			50,		false},		// crystal wall
	{&torchLightColor,		{200, 400, 1},			0,		false},		// candle light
	{&hauntedTorchColor,	{400, 600, 1},          0,		false},		// haunted torch
	{&glyphLightColor,      {100, 100, 1},          0,      false},     // glyph dim light
	{&glyphLightColor,      {300, 300, 1},          0,      false},     // glyph bright light
	{&sacredGlyphColor,     {300, 300, 1},          0,      false},     // sacred glyph light
	{&descentLightColor,    {600, 600, 1},          0,      false},     // magical pit light
	{&sacrificeTargetColor, {800, 1200, 1},          0,      true},      // demonic statue light
};
