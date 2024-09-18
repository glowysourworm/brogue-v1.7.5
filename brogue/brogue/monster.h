#pragma once

#include "roguedef.h"
#include "creature.h"
#include "monster.h"
#include "monsterDeclaration.h"

// Defines all creatures, which include monsters and the player:
creatureType monsterCatalog[NUMBER_MONSTER_KINDS] = {

	//	name			ch		color			HP		def		acc		damage			reg	move	attack	blood			light	DFChance DFType         bolts       behaviorF, abilityF
	{0,	"you",	PLAYER_CHAR,	&playerInLightColor,30,	0,		100,	{1, 2, 1},		20,	100,	100,	DF_RED_BLOOD,	0,		0,		0,              {0},(MONST_MALE | MONST_FEMALE)},
	{0, "rat",			'r',	&grayBrogue,			6,		0,		80,		{1, 3, 1},		20,	100,	100,	DF_RED_BLOOD,	0,		1,		DF_URINE,       {0}},
	{0, "kobold",		'k',	&goblinColor,	7,		0,		80,		{1, 4, 1},		20,	100,	100,	DF_RED_BLOOD,	0,		0,		0,              {0}},
	{0,	"jackal",		'j',	&jackalColor,	8,		0,		70,		{2, 4, 1},		20,	50,		100,	DF_RED_BLOOD,	0,		1,		DF_URINE,              {0}},
	{0,	"eel",			'e',	&eelColor,		18,		27,		100,	{3, 7, 2},		5,	50,		100,	0,              0,		0,		0,              {0},(MONST_RESTRICTED_TO_LIQUID | MONST_IMMUNE_TO_WATER | MONST_SUBMERGES | MONST_FLITS | MONST_NEVER_SLEEPS)},
	{0,	"monkey",		'm',	&ogreColor,		12,		17,		100,	{1, 3, 1},		20,	100,	100,	DF_RED_BLOOD,	0,		1,		DF_URINE,       {0},(0), (MA_HIT_STEAL_FLEE)},
	{0, "bloat",		'b',	&poisonGasColor,4,		0,		100,	{0, 0, 0},		5,	100,	100,	DF_PURPLE_BLOOD,0,		0,		DF_BLOAT_DEATH, {0},(MONST_FLIES | MONST_FLITS), (MA_KAMIKAZE | MA_DF_ON_DEATH)},
	{0, "pit bloat",	'b',	&lightBlue,     4,		0,		100,	{0, 0, 0},		5,	100,	100,	DF_PURPLE_BLOOD,0,		0,		DF_HOLE_POTION, {0},(MONST_FLIES | MONST_FLITS), (MA_KAMIKAZE | MA_DF_ON_DEATH)},
	{0, "goblin",		'g',	&goblinColor,	15,		10,		70,		{2, 5, 1},		20,	100,	100,	DF_RED_BLOOD,	0,		0,		0,              {0}, (0),  (MA_ATTACKS_PENETRATE | MA_AVOID_CORRIDORS)},
	{0, "goblin conjurer",'g',	&goblinConjurerColor, 10,10,	70,		{2, 4, 1},		20,	100,	100,	DF_RED_BLOOD,	0,		0,		0,              {0},(MONST_MAINTAINS_DISTANCE | MONST_CAST_SPELLS_SLOWLY | MONST_CARRY_ITEM_25), (MA_CAST_SUMMON | MA_ATTACKS_PENETRATE | MA_AVOID_CORRIDORS)},
	{0, "goblin mystic",'g',	&goblinMysticColor, 10,	10,		70,		{2, 4, 1},		20,	100,	100,	DF_RED_BLOOD,	0,		0,		0,              {BOLT_SHIELDING},(MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25), (MA_AVOID_CORRIDORS)},
	{0, "goblin totem",	TOTEM_CHAR,	&orange,	30,		0,		0,		{0, 0, 0},		0,	100,	300,	DF_RUBBLE_BLOOD,IMP_LIGHT,0,	0,              {BOLT_HASTE, BOLT_SPARK},(MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_WILL_NOT_USE_STAIRS), (0)},
	{0, "pink jelly",	'J',	&pinkJellyColor,50,		0,		85,     {1, 3, 1},		0,	100,	100,	DF_PURPLE_BLOOD,0,		0,		0,              {0},(MONST_NEVER_SLEEPS), (MA_CLONE_SELF_ON_DEFEND)},
	{0, "toad",			't',	&toadColor,		18,		0,		90,		{1, 4, 1},		10,	100,	100,	DF_GREEN_BLOOD,	0,		0,		0,              {0},(0), (MA_HIT_HALLUCINATE)},
	{0, "vampire bat",	'v',	&grayBrogue,			18,		25,		100,	{2, 6, 1},		20,	50,		100,	DF_RED_BLOOD,	0,		0,		0,              {0},(MONST_FLIES | MONST_FLITS), (MA_TRANSFERENCE)},
	{0, "arrow turret", TURRET_CHAR,&black,		30,		0,		90,		{2, 6, 1},		0,	100,	250,	0,              0,		0,		0,              {BOLT_DISTANCE_ATTACK},(MONST_TURRET), (0)},
	{0, "acid mound",	'a',	&acidBackColor,	15,		10,		70,		{1, 3, 1},		5,	100,	100,	DF_ACID_BLOOD,	0,		0,		0,              {0},(MONST_DEFEND_DEGRADE_WEAPON), (MA_HIT_DEGRADE_ARMOR)},
	{0, "centipede",	'c',	&centipedeColor,20,		20,		80,		{4, 12, 1},		20,	100,	100,	DF_GREEN_BLOOD,	0,		0,		0,              {0},(0), (MA_CAUSES_WEAKNESS)},
	{0,	"ogre",			'O',	&ogreColor,		55,		60,		125,	{9, 13, 2},		20,	100,	200,	DF_RED_BLOOD,	0,		0,		0,              {0},(MONST_MALE | MONST_FEMALE), (MA_AVOID_CORRIDORS | MA_ATTACKS_STAGGER)},
	{0,	"bog monster",	'B',	&krakenColor,	55,		60,		5000,	{3, 4, 1},		3,	200,	100,	0,              0,		0,		0,              {0},(MONST_RESTRICTED_TO_LIQUID | MONST_SUBMERGES | MONST_FLITS | MONST_FLEES_NEAR_DEATH), (MA_SEIZES)},
	{0, "ogre totem",	TOTEM_CHAR,	&green,		70,		0,		0,		{0, 0, 0},		0,	100,	400,	DF_RUBBLE_BLOOD,LICH_LIGHT,0,	0,              {BOLT_HEALING, BOLT_SLOW_2},(MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_WILL_NOT_USE_STAIRS), (0)},
	{0, "spider",		's',	&whiteBrogue,			20,		70,		90,		{3, 4, 2},		20,	100,	200,	DF_GREEN_BLOOD,	0,		0,		0,              {BOLT_SPIDERWEB},(MONST_IMMUNE_TO_WEBS | MONST_CAST_SPELLS_SLOWLY | MONST_ALWAYS_USE_ABILITY), (MA_POISONS)},
	{0, "spark turret", TURRET_CHAR, &lightningColor,80,0,		100,	{0, 0, 0},		0,	100,	150,	0,              SPARK_TURRET_LIGHT,	0,	0,      {BOLT_SPARK},(MONST_TURRET), (0)},
	{0,	"wisp",         'w',	&wispLightColor,10,		90,     100,	{0,	0, 0},		5,	100,	100,	DF_ASH_BLOOD,	WISP_LIGHT,	0,	0,              {0},(MONST_IMMUNE_TO_FIRE | MONST_FLIES | MONST_FLITS | MONST_NEVER_SLEEPS | MONST_FIERY | MONST_DIES_IF_NEGATED), (MA_HIT_BURN)},
	{0, "wraith",		'W',	&wraithColor,	50,		60,		120,	{6, 13, 2},		5,	50,		100,	DF_GREEN_BLOOD,	0,		0,		0,              {0},(MONST_FLEES_NEAR_DEATH)},
	{0, "zombie",		'Z',	&vomitColor,	80,		0,		120,	{7, 12, 1},		0,	100,	100,	DF_ROT_GAS_BLOOD,0,		100,	DF_ROT_GAS_PUFF, {0}},
	{0, "troll",		'T',	&trollColor,	65,		70,		125,	{10, 15, 3},	1,	100,	100,	DF_RED_BLOOD,	0,		0,		0,              {0},(MONST_MALE | MONST_FEMALE)},
	{0,	"ogre shaman",	'O',	&green,			45,		40,		100,	{5, 9, 1},		20,	100,	200,	DF_RED_BLOOD,	0,		0,		0,              {BOLT_HASTE, BOLT_SPARK},(MONST_MAINTAINS_DISTANCE | MONST_CAST_SPELLS_SLOWLY | MONST_MALE | MONST_FEMALE), (MA_CAST_SUMMON | MA_AVOID_CORRIDORS)},
	{0, "naga",			'N',	&trollColor,	75,		70,     150,	{7, 11, 4},		10,	100,	100,	DF_GREEN_BLOOD,	0,		100,	DF_PUDDLE,      {0},(MONST_IMMUNE_TO_WATER | MONST_SUBMERGES | MONST_NEVER_SLEEPS | MONST_FEMALE), (MA_ATTACKS_ALL_ADJACENT)},
	{0, "salamander",	'S',	&salamanderColor,60,	70,     150,	{5, 11, 3},		10,	100,	100,	DF_ASH_BLOOD,	SALAMANDER_LIGHT, 100, DF_SALAMANDER_FLAME, {0},(MONST_IMMUNE_TO_FIRE | MONST_SUBMERGES | MONST_NEVER_SLEEPS | MONST_FIERY | MONST_MALE), (MA_ATTACKS_EXTEND)},
	{0, "explosive bloat",'b',	&orange,		10,		0,		100,	{0, 0, 0},		5,	100,	100,	DF_RED_BLOOD,	EXPLOSIVE_BLOAT_LIGHT,0, DF_BLOAT_EXPLOSION, {0},(MONST_FLIES | MONST_FLITS), (MA_KAMIKAZE | MA_DF_ON_DEATH)},
	{0, "dar blademaster",'d',	&purple,		35,		70,     160,	{5, 9, 2},		20,	100,	100,	DF_RED_BLOOD,	0,		0,		0,              {BOLT_BLINKING},(MONST_CARRY_ITEM_25 | MONST_MALE | MONST_FEMALE), (MA_AVOID_CORRIDORS)},
	{0, "dar priestess", 'd',	&darPriestessColor,20,	60,		100,	{2, 5, 1},		20,	100,	100,	DF_RED_BLOOD,   0,		0,		0,              {BOLT_NEGATION, BOLT_HEALING, BOLT_HASTE, BOLT_SPARK},(MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25 | MONST_FEMALE), (MA_AVOID_CORRIDORS)},
	{0, "dar battlemage",'d',	&darMageColor,	20,		60,		100,	{1, 3, 1},		20,	100,	100,	DF_RED_BLOOD,	0,		0,		0,              {BOLT_FIRE, BOLT_SLOW_2, BOLT_DISCORD},(MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25 | MONST_MALE | MONST_FEMALE), (MA_AVOID_CORRIDORS)},
	{0, "acidic jelly",	'J',	&acidBackColor,	60,		0,		115,	{2, 6, 1},		0,	100,	100,	DF_ACID_BLOOD,	0,		0,		0,              {0},(MONST_DEFEND_DEGRADE_WEAPON), (MA_HIT_DEGRADE_ARMOR | MA_CLONE_SELF_ON_DEFEND)},
	{0,	"centaur",		'C',	&tanColor,		35,		50,		175,	{4, 8, 2},		20,	50,		100,	DF_RED_BLOOD,	0,		0,		0,              {BOLT_DISTANCE_ATTACK},(MONST_MAINTAINS_DISTANCE | MONST_MALE), (0)},
	{0, "underworm",	'U',	&wormColor,		80,		40,		160,	{18, 22, 2},	3,	150,	200,	DF_WORM_BLOOD,	0,		0,		0,              {0},(MONST_NEVER_SLEEPS)},
	{0, "sentinel",		STATUE_CHAR, &sentinelColor, 50,0,		0,		{0, 0, 0},		0,	100,	175,	DF_RUBBLE_BLOOD,SENTINEL_LIGHT,0,0,             {BOLT_HEALING, BOLT_SPARK},(MONST_TURRET | MONST_CAST_SPELLS_SLOWLY | MONST_DIES_IF_NEGATED), (0)},
	{0, "dart turret", TURRET_CHAR,	&centipedeColor,20,	0,		140,	{1, 2, 1},      0,	100,	250,	0,              0,		0,		0,              {BOLT_POISON_DART},(MONST_TURRET), (MA_CAUSES_WEAKNESS)},
	{0,	"kraken",		'K',	&krakenColor,	120,	0,		150,	{15, 20, 3},	1,	50,		100,	0,              0,		0,		0,              {0},(MONST_RESTRICTED_TO_LIQUID | MONST_IMMUNE_TO_WATER | MONST_SUBMERGES | MONST_FLITS | MONST_NEVER_SLEEPS | MONST_FLEES_NEAR_DEATH), (MA_SEIZES)},
	{0,	"lich",			'L',	&whiteBrogue,			35,		80,     175,	{2, 6, 1},		0,	100,	100,	DF_ASH_BLOOD,	LICH_LIGHT,	0,	0,              {BOLT_FIRE},(MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25 | MONST_NO_POLYMORPH), (MA_CAST_SUMMON)},
	{0, "phylactery",	GEM_CHAR,&lichLightColor,30,	0,		0,		{0, 0, 0},		0,	100,	150,	DF_RUBBLE_BLOOD,LICH_LIGHT,	0,	0,              {0},(MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_ALWAYS_HUNTING | MONST_WILL_NOT_USE_STAIRS | MONST_DIES_IF_NEGATED), (MA_CAST_SUMMON | MA_ENTER_SUMMONS)},
	{0, "pixie",		'p',	&pixieColor,	10,		90,     100,	{1, 3, 1},		20,	50,		100,	DF_GREEN_BLOOD,	PIXIE_LIGHT, 0,	0,              {BOLT_NEGATION, BOLT_SLOW_2, BOLT_DISCORD, BOLT_SPARK},(MONST_MAINTAINS_DISTANCE | MONST_FLIES | MONST_FLITS | MONST_MALE | MONST_FEMALE), (0)},
	{0,	"phantom",		'P',	&ectoplasmColor,35,		70,     160,	{12, 18, 4},    0,	50,		200,	DF_ECTOPLASM_BLOOD,	0,	2,		DF_ECTOPLASM_DROPLET, {0},(MONST_INVISIBLE | MONST_FLITS | MONST_FLIES | MONST_IMMUNE_TO_WEBS)},
	{0, "flame turret", TURRET_CHAR, &lavaForeColor,40,	0,		150,	{1, 2, 1},		0,	100,	250,	0,              LAVA_LIGHT,	0,	0,              {BOLT_FIRE},(MONST_TURRET), (0)},
	{0, "imp",			'i',	&pink,			35,		90,     225,	{4, 9, 2},		10,	100,	100,	DF_GREEN_BLOOD,	IMP_LIGHT,	0,	0,              {BOLT_BLINKING},(0), (MA_HIT_STEAL_FLEE)},
	{0,	"fury",			'f',	&darkRed,		19,		90,     200,	{6, 11, 4},		20,	50,		100,	DF_RED_BLOOD,	0,		0,		0,              {0},(MONST_NEVER_SLEEPS | MONST_FLIES)},
	{0, "revenant",		'R',	&ectoplasmColor,30,		0,		200,	{15, 20, 5},	0,	100,	100,	DF_ECTOPLASM_BLOOD,	0,	0,		0,              {0},(MONST_IMMUNE_TO_WEAPONS)},
	{0, "tentacle horror",'H',	&centipedeColor,120,	95,     225,	{25, 35, 3},	1,	100,	100,	DF_PURPLE_BLOOD,0,		0,		0,              {0}},
	{0, "golem",		'G',	&grayBrogue,			400,	70,     225,	{4, 8, 1},		0,	100,	100,	DF_RUBBLE_BLOOD,0,		0,		0,              {0},(MONST_REFLECT_4 | MONST_DIES_IF_NEGATED)},
	{0, "dragon",		'D',	&dragonColor,	150,	90,     250,	{25, 50, 4},	20,	50,		200,	DF_GREEN_BLOOD,	0,		0,		0,              {BOLT_DRAGONFIRE},(MONST_IMMUNE_TO_FIRE | MONST_CARRY_ITEM_100), (MA_ATTACKS_ALL_ADJACENT)},

	// bosses
	{0, "goblin warlord",'g',	&blue,			30,		17,		100,	{3, 6, 1},		20,	100,	100,	DF_RED_BLOOD,	0,		0,		0,              {0},
		(MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25), (MA_CAST_SUMMON | MA_ATTACKS_PENETRATE | MA_AVOID_CORRIDORS)},
	{0,	"black jelly",	'J',	&black,			120,	0,		130,	{3, 8, 1},		0,	100,	100,	DF_PURPLE_BLOOD,0,		0,		0,              {0},
		(0), (MA_CLONE_SELF_ON_DEFEND)},
	{0, "vampire",		'V',	&whiteBrogue,			75,		60,     120,	{4, 15, 2},		6,	50,		100,	DF_RED_BLOOD,	0,		0,		DF_BLOOD_EXPLOSION, {BOLT_BLINKING, BOLT_DISCORD},
		(MONST_FLEES_NEAR_DEATH | MONST_MALE), (MA_TRANSFERENCE | MA_DF_ON_DEATH | MA_CAST_SUMMON | MA_ENTER_SUMMONS)},
	{0, "flamedancer",	'F',	&whiteBrogue,			65,		80,     120,	{3, 8, 2},		0,	100,	100,	DF_EMBER_BLOOD,	FLAMEDANCER_LIGHT,100,DF_FLAMEDANCER_CORONA, {BOLT_FIRE},
		(MONST_MAINTAINS_DISTANCE | MONST_IMMUNE_TO_FIRE | MONST_FIERY), (MA_HIT_BURN)},

	// special effect monsters
	{0, "spectral blade",WEAPON_CHAR, &spectralBladeColor,1, 0,	150,	{1, 1, 1},		0,	50,		100,	0,              SPECTRAL_BLADE_LIGHT,0,0,       {0},
		(MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_FLIES | MONST_WILL_NOT_USE_STAIRS | MONST_DIES_IF_NEGATED | MONST_IMMUNE_TO_WEBS | MONST_NOT_LISTED_IN_SIDEBAR)},
	{0, "spectral sword",WEAPON_CHAR, &spectralImageColor, 1,0,	150,	{1, 1, 1},		0,	50,		100,	0,              SPECTRAL_IMAGE_LIGHT,0,0,       {0},
		(MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_FLIES | MONST_WILL_NOT_USE_STAIRS | MONST_DIES_IF_NEGATED | MONST_IMMUNE_TO_WEBS)},
	{0, "stone guardian",STATUE_CHAR, &whiteBrogue,   1000,   0,		200,	{12, 17, 2},	0,	100,	100,	DF_RUBBLE,      0,      100,      DF_GUARDIAN_STEP, {0},
		(MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_ALWAYS_HUNTING | MONST_IMMUNE_TO_FIRE | MONST_IMMUNE_TO_WEAPONS | MONST_WILL_NOT_USE_STAIRS | MONST_DIES_IF_NEGATED | MONST_REFLECT_4 | MONST_ALWAYS_USE_ABILITY | MONST_GETS_TURN_ON_ACTIVATION)},
	{0, "winged guardian",STATUE_CHAR, &blue,   1000,   0,		200,	{12, 17, 2},	0,	100,	100,	DF_RUBBLE,      0,      100,      DF_SILENT_GLYPH_GLOW, {BOLT_BLINKING},
		(MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_ALWAYS_HUNTING | MONST_IMMUNE_TO_FIRE | MONST_IMMUNE_TO_WEAPONS | MONST_WILL_NOT_USE_STAIRS | MONST_DIES_IF_NEGATED | MONST_REFLECT_4 | MONST_GETS_TURN_ON_ACTIVATION | MONST_ALWAYS_USE_ABILITY), (0)},
	{0, "guardian spirit",STATUE_CHAR, &spectralImageColor,1000,0,200,	{5, 12, 2},     0,	100,	100,	0,              SPECTRAL_IMAGE_LIGHT,100,0,     {0},
		(MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_IMMUNE_TO_FIRE | MONST_IMMUNE_TO_WEAPONS | MONST_DIES_IF_NEGATED | MONST_REFLECT_4 | MONST_ALWAYS_USE_ABILITY)},
	{0, "Warden of Yendor",'Y', &yendorLightColor,1000,   0,	300,	{12, 17, 2},	0,	200,	200,	DF_RUBBLE,      YENDOR_LIGHT, 100, 0,           {0},
		(MONST_NEVER_SLEEPS | MONST_ALWAYS_HUNTING | MONST_INVULNERABLE | MONST_NO_POLYMORPH)},
	{0, "eldritch totem",TOTEM_CHAR, &glyphColor,80,    0,		0,		{0, 0, 0},		0,	100,	100,	DF_RUBBLE_BLOOD,0,      0,      0,              {0},
		(MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_ALWAYS_HUNTING | MONST_WILL_NOT_USE_STAIRS | MONST_GETS_TURN_ON_ACTIVATION | MONST_ALWAYS_USE_ABILITY), (MA_CAST_SUMMON)},
	{0, "mirrored totem",TOTEM_CHAR, &beckonColor,80,	0,		0,		{0, 0, 0},		0,	100,	100,	DF_RUBBLE_BLOOD,0,      100,	DF_MIRROR_TOTEM_STEP, {BOLT_BECKONING},
		(MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_ALWAYS_HUNTING | MONST_WILL_NOT_USE_STAIRS | MONST_GETS_TURN_ON_ACTIVATION | MONST_ALWAYS_USE_ABILITY | MONST_REFLECT_4 | MONST_IMMUNE_TO_WEAPONS | MONST_IMMUNE_TO_FIRE), (0)},

	// legendary allies
	{0,	"unicorn",		UNICORN_CHAR, &whiteBrogue,   40,		60,		175,	{2, 10, 2},		20,	50,		100,	DF_RED_BLOOD,	UNICORN_LIGHT,1,DF_UNICORN_POOP, {BOLT_HEALING, BOLT_SHIELDING},
		(MONST_MAINTAINS_DISTANCE | MONST_MALE | MONST_FEMALE), (0)},
	{0,	"ifrit",		'I',	&ifritColor,	40,		75,     175,	{5, 13, 2},		1,	50,		100,	DF_ASH_BLOOD,	IFRIT_LIGHT,0,	0,              {BOLT_DISCORD},
		(MONST_IMMUNE_TO_FIRE | MONST_FLIES | MONST_MALE), (0)},
	{0,	"phoenix",		'P',	&phoenixColor,	30,		70,     175,	{4, 10, 2},		0,	50,		100,	DF_ASH_BLOOD,	PHOENIX_LIGHT,0,0,              {0},
		(MONST_IMMUNE_TO_FIRE | MONST_FLIES | MONST_NO_POLYMORPH)},
	{0, "phoenix egg",	GEM_CHAR,&phoenixColor,	50,     0,		0,		{0, 0, 0},		0,	100,	150,	DF_ASH_BLOOD,	PHOENIX_EGG_LIGHT,	0,	0,      {0},
		(MONST_IMMUNE_TO_FIRE | MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_WILL_NOT_USE_STAIRS | MONST_NO_POLYMORPH | MONST_ALWAYS_HUNTING | MONST_IMMUNE_TO_WEAPONS), (MA_CAST_SUMMON | MA_ENTER_SUMMONS)},
	{0,	"mangrove dryad",'M',	&tanColor,      70,		60,     175,	{2, 8, 2},		6,	100,	100,	DF_ASH_BLOOD,	0,      0,      0,              {BOLT_ANCIENT_SPIRIT_VINES},
		(MONST_IMMUNE_TO_WEBS | MONST_ALWAYS_USE_ABILITY | MONST_MAINTAINS_DISTANCE | MONST_NO_POLYMORPH | MONST_MALE | MONST_FEMALE), (0)},
};

const mutation mutationCatalog[NUMBER_MUTATORS] = {
	//Title         textColor       healthFactor    moveSpdMult attackSpdMult   defMult damMult DF% DFtype  light   monstFlags  abilityFlags    forbiddenFlags      forbiddenAbilities
	{"explosive",   &orange,        50,             100,        100,            50,     100,    0,  DF_MUTATION_EXPLOSION, EXPLOSIVE_BLOAT_LIGHT, 0, MA_DF_ON_DEATH, MONST_SUBMERGES, 0,
		"A rare mutation will cause $HIMHER to explode violently when $HESHE dies."},
	{"infested",    &lichenColor,   50,             100,        100,            50,     100,    0,  DF_MUTATION_LICHEN, 0, 0,   MA_DF_ON_DEATH, 0,               0,
		"$HESHE has been infested by deadly lichen spores; poisonous fungus will spread from $HISHER corpse when $HESHE dies."},
	{"agile",       &lightBlue,     100,            50,         100,            150,    100,    -1, 0,      0,      MONST_FLEES_NEAR_DEATH, 0, MONST_FLEES_NEAR_DEATH, 0,
		"A rare mutation greatly enhances $HISHER mobility."},
	{"juggernaut",  &brown,         300,            200,        200,            75,     200,    -1, 0,      0,      0,          MA_ATTACKS_STAGGER, MONST_MAINTAINS_DISTANCE, 0,
		"A rare mutation has hardened $HISHER flesh, increasing $HISHER health and power but compromising $HISHER speed."},
	{"grappling",   &tanColor,      150,            100,        100,            50,     100,    -1, 0,      0,      0,          MA_SEIZES,      MONST_MAINTAINS_DISTANCE, MA_SEIZES,
		"A rare mutation has caused suckered tentacles to sprout from $HISHER frame, increasing $HISHER health and allowing $HIMHER to grapple with $HISHER prey."},
	{"vampiric",    &red,           100,            100,        100,            100,    100,    -1, 0,      0,      0,          MA_TRANSFERENCE, MONST_MAINTAINS_DISTANCE, MA_TRANSFERENCE,
		"A rare mutation allows $HIMHER to heal $HIMSELFHERSELF with every attack."},
	{"toxic",       &green,         100,            100,        200,            100,    20,     -1, 0,      0,      0,          (MA_CAUSES_WEAKNESS | MA_POISONS), MONST_MAINTAINS_DISTANCE, (MA_CAUSES_WEAKNESS | MA_POISONS),
		"A rare mutation causes $HIMHER to poison $HISHER victims and sap their strength with every attack."},
	{"reflective",  &darkTurquoise, 100,            100,        100,            100,    100,    -1, 0,      0,      MONST_REFLECT_4, 0,         (MONST_REFLECT_4 | MONST_ALWAYS_USE_ABILITY), 0,
		"A rare mutation has coated $HISHER flesh with reflective scales."},
};

const hordeType hordeCatalog[NUMBER_HORDES] = {
	// leader		#members	member list								member numbers					minL	maxL	freq	spawnsIn		machine			flags
	{MK_RAT,			0,		{0},									{{0}},							1,		5,		150},
	{MK_KOBOLD,			0,		{0},									{{0}},							1,		6,		150},
	{MK_JACKAL,			0,		{0},									{{0}},							1,		3,		100},
	{MK_JACKAL,			1,		{MK_JACKAL},							{{1, 3, 1}},					3,		7,		50},
	{MK_EEL,			0,		{0},									{{0}},							2,		17,		100,		DEEP_WATER},
	{MK_MONKEY,			0,		{0},									{{0}},							2,		9,		50},
	{MK_BLOAT,			0,		{0},									{{0}},							2,		13,		30},
	{MK_PIT_BLOAT,		0,		{0},									{{0}},							2,		13,		10},
	{MK_BLOAT,			1,		{MK_BLOAT},								{{0, 2, 1}},					14,		26,		30},
	{MK_PIT_BLOAT,		1,		{MK_PIT_BLOAT},							{{0, 2, 1}},					14,		26,		10},
	{MK_EXPLOSIVE_BLOAT,0,		{0},									{{0}},							10,		26,		10},
	{MK_GOBLIN,			0,		{0},									{{0}},							3,		10,		100},
	{MK_GOBLIN_CONJURER,0,		{0},									{{0}},							3,		10,		60},
	{MK_TOAD,			0,		{0},									{{0}},							4,		11,		100},
	{MK_PINK_JELLY,		0,		{0},									{{0}},							4,		13,		100},
	{MK_GOBLIN_TOTEM,	1,		{MK_GOBLIN},							{{2,4,1}},						5,		13,		100,		0,				MT_CAMP_AREA,	HORDE_NO_PERIODIC_SPAWN},
	{MK_ARROW_TURRET,	0,		{0},									{{0}},							5,		13,		100,		WALL,	0,                      HORDE_NO_PERIODIC_SPAWN},
	{MK_MONKEY,			1,		{MK_MONKEY},							{{2,4,1}},						5,		13,		20},
	{MK_VAMPIRE_BAT,	0,		{0},                                    {{0}},                          6,		13,		30},
	{MK_VAMPIRE_BAT,	1,		{MK_VAMPIRE_BAT},						{{1,2,1}},						6,		13,		70,      0,              0,              HORDE_NEVER_OOD},
	{MK_ACID_MOUND,		0,		{0},									{{0}},							6,		13,		100},
	{MK_GOBLIN,			3,		{MK_GOBLIN, MK_GOBLIN_MYSTIC, MK_JACKAL},{{2, 3, 1}, {1,2,1}, {1,2,1}},	6,		12,		40},
	{MK_GOBLIN_CONJURER,2,		{MK_GOBLIN_CONJURER, MK_GOBLIN_MYSTIC},	{{0,1,1}, {1,1,1}},				7,		15,		40},
	{MK_CENTIPEDE,		0,		{0},									{{0}},							7,		14,		100},
	{MK_BOG_MONSTER,	0,		{0},									{{0}},							7,		14,		80,		MUD,            0,              HORDE_NEVER_OOD},
	{MK_OGRE,			0,		{0},									{{0}},							7,		13,		100},
	{MK_EEL,			1,		{MK_EEL},								{{2, 4, 1}},					8,		22,		70,		DEEP_WATER},
	{MK_ACID_MOUND,		1,		{MK_ACID_MOUND},						{{2, 4, 1}},					9,		13,		30},
	{MK_SPIDER,			0,		{0},									{{0}},							9,		16,		100},
	{MK_DAR_BLADEMASTER,1,		{MK_DAR_BLADEMASTER},					{{0, 1, 1}},					10,		14,		100},
	{MK_WILL_O_THE_WISP,0,		{0},									{{0}},							10,		17,		100},
	{MK_WRAITH,			0,		{0},									{{0}},							10,		17,		100},
	{MK_GOBLIN_TOTEM,	4,		{MK_GOBLIN_TOTEM, MK_GOBLIN_CONJURER, MK_GOBLIN_MYSTIC, MK_GOBLIN}, {{1,2,1},{1,2,1},{1,2,1},{3,5,1}},10,17,80,0,MT_CAMP_AREA,	HORDE_NO_PERIODIC_SPAWN},
	{MK_SPARK_TURRET,	0,		{0},									{{0}},							11,		18,		100,		WALL,	0,                      HORDE_NO_PERIODIC_SPAWN},
	{MK_ZOMBIE,			0,		{0},									{{0}},							11,		18,		100},
	{MK_TROLL,			0,		{0},									{{0}},							12,		19,		100},
	{MK_OGRE_TOTEM,		1,		{MK_OGRE},								{{2,4,1}},						12,		19,		60,		0,			0,					HORDE_NO_PERIODIC_SPAWN},
	{MK_BOG_MONSTER,	1,		{MK_BOG_MONSTER},						{{2,4,1}},						12,		26,		100,		MUD},
	{MK_NAGA,			0,		{0},									{{0}},							13,		20,		100,		DEEP_WATER},
	{MK_SALAMANDER,		0,		{0},									{{0}},							13,		20,		100,		LAVA},
	{MK_OGRE_SHAMAN,	1,		{MK_OGRE},								{{1, 3, 1}},					14,		20,		100},
	{MK_CENTAUR,		1,		{MK_CENTAUR},							{{1, 1, 1}},					14,		21,		100},
	{MK_ACID_JELLY,		0,		{0},									{{0}},							14,		21,		100},
	{MK_DART_TURRET,	0,		{0},									{{0}},							15,		22,		100,		WALL,	0,                      HORDE_NO_PERIODIC_SPAWN},
	{MK_PIXIE,			0,		{0},									{{0}},							14,		21,		80},
	{MK_FLAME_TURRET,	0,		{0},									{{0}},							14,		24,		100,		WALL,	0,                      HORDE_NO_PERIODIC_SPAWN},
	{MK_DAR_BLADEMASTER,2,		{MK_DAR_BLADEMASTER, MK_DAR_PRIESTESS},	{{0, 1, 1}, {0, 1, 1}},			15,		17,		100},
	{MK_PINK_JELLY,     2,		{MK_PINK_JELLY, MK_DAR_PRIESTESS},      {{0, 1, 1}, {1, 2, 1}},			17,		23,		70},
	{MK_KRAKEN,			0,		{0},									{{0}},							15,		30,		100,		DEEP_WATER},
	{MK_PHANTOM,		0,		{0},									{{0}},							16,		23,		100},
	{MK_WRAITH,			1,		{MK_WRAITH},							{{1, 4, 1}},					16,		23,		80},
	{MK_IMP,			0,		{0},									{{0}},							17,		24,		100},
	{MK_DAR_BLADEMASTER,3,		{MK_DAR_BLADEMASTER, MK_DAR_PRIESTESS, MK_DAR_BATTLEMAGE},{{1,2,1},{1,1,1},{1,1,1}},18,25,100},
	{MK_FURY,			1,		{MK_FURY},								{{2, 4, 1}},					18,		26,		80},
	{MK_REVENANT,		0,		{0},									{{0}},							19,		27,		100},
	{MK_GOLEM,			0,		{0},									{{0}},							21,		30,		100},
	{MK_TENTACLE_HORROR,0,		{0},									{{0}},							22,		DEEPEST_LEVEL - 1,		100},
	{MK_PHYLACTERY,		0,		{0},									{{0}},							22,		DEEPEST_LEVEL - 1,		100},
	{MK_DRAGON,			0,		{0},									{{0}},							24,		DEEPEST_LEVEL - 1,		70},
	{MK_DRAGON,			1,		{MK_DRAGON},							{{1,1,1}},						27,		DEEPEST_LEVEL - 1,		30},
	{MK_GOLEM,			3,		{MK_GOLEM, MK_DAR_PRIESTESS, MK_DAR_BATTLEMAGE}, {{1, 2, 1}, {0,1,1},{0,1,1}},27,DEEPEST_LEVEL - 1,	80},
	{MK_GOLEM,			1,		{MK_GOLEM},								{{5, 10, 2}},					30,		DEEPEST_LEVEL - 1,    20},
	{MK_KRAKEN,			1,		{MK_KRAKEN},							{{5, 10, 2}},					30,		DEEPEST_LEVEL - 1,    100,		DEEP_WATER},
	{MK_TENTACLE_HORROR,2,		{MK_TENTACLE_HORROR, MK_REVENANT},		{{1, 3, 1}, {2, 4, 1}},			32,		DEEPEST_LEVEL - 1,    20},
	{MK_DRAGON,			1,		{MK_DRAGON},							{{3, 5, 1}},					34,		DEEPEST_LEVEL - 1,    20},

	// summons
	{MK_GOBLIN_CONJURER,1,		{MK_SPECTRAL_BLADE},					{{3, 5, 1}},					0,		0,		100,	0,			0,					HORDE_IS_SUMMONED | HORDE_DIES_ON_LEADER_DEATH},
	{MK_OGRE_SHAMAN,	1,		{MK_OGRE},								{{1, 1, 1}},					0,		0,		100,	0,			0,					HORDE_IS_SUMMONED},
	{MK_VAMPIRE,		1,		{MK_VAMPIRE_BAT},						{{3, 3, 1}},					0,		0,		100,	0,			0,					HORDE_IS_SUMMONED},
	{MK_LICH,			1,		{MK_PHANTOM},							{{2, 3, 1}},					0,		0,		100,	0,			0,					HORDE_IS_SUMMONED},
	{MK_LICH,			1,		{MK_FURY},								{{2, 3, 1}},					0,		0,		100,	0,			0,					HORDE_IS_SUMMONED},
	{MK_PHYLACTERY,		1,		{MK_LICH},								{{1,1,1}},						0,		0,		100,	0,			0,					HORDE_IS_SUMMONED},
	{MK_GOBLIN_CHIEFTAN,2,		{MK_GOBLIN_CONJURER, MK_GOBLIN},		{{1,1,1}, {3,4,1}},				0,		0,		100,	0,			0,					HORDE_IS_SUMMONED | HORDE_SUMMONED_AT_DISTANCE},
	{MK_PHOENIX_EGG,	1,		{MK_PHOENIX},							{{1,1,1}},						0,		0,		100,	0,			0,					HORDE_IS_SUMMONED},
	{MK_ELDRITCH_TOTEM, 1,		{MK_SPECTRAL_BLADE},					{{4, 7, 1}},					0,		0,		100,	0,			0,					HORDE_IS_SUMMONED | HORDE_DIES_ON_LEADER_DEATH},
	{MK_ELDRITCH_TOTEM, 1,		{MK_FURY},                              {{2, 3, 1}},					0,		0,		100,	0,			0,					HORDE_IS_SUMMONED | HORDE_DIES_ON_LEADER_DEATH},

	// captives
	{MK_MONKEY,			1,		{MK_KOBOLD},							{{1, 2, 1}},					1,		5,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_GOBLIN,			1,		{MK_GOBLIN},							{{1, 2, 1}},					3,		7,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_OGRE,			1,		{MK_GOBLIN},							{{3, 5, 1}},					4,		10,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_GOBLIN_MYSTIC,	1,		{MK_KOBOLD},							{{3, 7, 1}},					5,		11,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_OGRE,			1,		{MK_OGRE},								{{1, 2, 1}},					8,		15,		6,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_TROLL,			1,		{MK_TROLL},								{{1, 2, 1}},					14,		19,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_CENTAUR,		1,		{MK_TROLL},								{{1, 2, 1}},					12,		19,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_TROLL,			2,		{MK_OGRE, MK_OGRE_SHAMAN},				{{2, 3, 1}, {0, 1, 1}},			17,		19,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_DAR_BLADEMASTER,1,		{MK_TROLL},								{{1, 2, 1}},					12,		19,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_NAGA,			1,		{MK_SALAMANDER},						{{1, 2, 1}},					14,		20,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_SALAMANDER,		1,		{MK_NAGA},								{{1, 2, 1}},					13,		20,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_TROLL,			1,		{MK_SALAMANDER},						{{1, 2, 1}},					13,		19,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_IMP,			1,		{MK_FURY},								{{2, 4, 1}},					18,		26,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_PIXIE,			1,		{MK_IMP, MK_PHANTOM},					{{1, 2, 1}, {1, 2, 1}},			14,		21,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_DAR_BLADEMASTER,1,		{MK_FURY},								{{2, 4, 1}},					18,		26,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_DAR_BLADEMASTER,1,		{MK_IMP},								{{2, 3, 1}},					18,		26,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_DAR_PRIESTESS,	1,		{MK_FURY},								{{2, 4, 1}},					18,		26,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_DAR_BATTLEMAGE,	1,		{MK_IMP},								{{2, 3, 1}},					18,		26,		3,		0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_TENTACLE_HORROR,3,		{MK_DAR_BLADEMASTER, MK_DAR_PRIESTESS, MK_DAR_BATTLEMAGE},{{1,2,1},{1,1,1},{1,1,1}},20,26,3,	0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},
	{MK_GOLEM,			3,		{MK_DAR_BLADEMASTER, MK_DAR_PRIESTESS, MK_DAR_BATTLEMAGE},{{1,2,1},{1,1,1},{1,1,1}},18,25,3,	0,			0,					HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD},

	// bosses
	{MK_GOBLIN_CHIEFTAN,2,		{MK_GOBLIN_MYSTIC, MK_GOBLIN, MK_GOBLIN_TOTEM}, {{1,1,1}, {2,3,1}, {2,2,1}},2,	10,		50,		0,			0,					HORDE_MACHINE_BOSS},
	{MK_BLACK_JELLY,	0,		{0},									{{0}},							5,		15,		50,		0,			0,					HORDE_MACHINE_BOSS},
	{MK_VAMPIRE,		0,		{0},									{{0}},							10,		DEEPEST_LEVEL,	50,  0,		0,					HORDE_MACHINE_BOSS},
	{MK_FLAMEDANCER,	0,		{0},									{{0}},							10,		DEEPEST_LEVEL,	50,  0,		0,					HORDE_MACHINE_BOSS},

	// machine water monsters
	{MK_EEL,			0,		{0},									{{0}},							2,		7,		100,		DEEP_WATER,	0,					HORDE_MACHINE_WATER_MONSTER},
	{MK_EEL,			1,		{MK_EEL},								{{2, 4, 1}},					5,		15,		100,		DEEP_WATER,	0,					HORDE_MACHINE_WATER_MONSTER},
	{MK_KRAKEN,			0,		{0},									{{0}},							12,		DEEPEST_LEVEL,	100,	DEEP_WATER,	0,				HORDE_MACHINE_WATER_MONSTER},
	{MK_KRAKEN,			1,		{MK_EEL},								{{1, 2, 1}},					12,		DEEPEST_LEVEL,	80,	DEEP_WATER,	0,				HORDE_MACHINE_WATER_MONSTER},

	// dungeon captives -- no captors
	{MK_OGRE,			0,		{0},									{{0}},							4,		13,		100,		0,			0,					HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE},
	{MK_NAGA,			0,		{0},									{{0}},							12,		20,		50,		0,			0,					HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE},
	{MK_GOBLIN_MYSTIC,	0,		{0},									{{0}},							2,		8,		100,		0,			0,					HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE},
	{MK_TROLL,			0,		{0},									{{0}},							10,		20,		50,		0,			0,					HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE},
	{MK_DAR_BLADEMASTER,0,		{0},									{{0}},							8,		16,		100,		0,			0,					HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE},
	{MK_DAR_PRIESTESS,	0,		{0},									{{0}},							8,		14,		100,		0,			0,					HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE},
	{MK_WRAITH,			0,		{0},									{{0}},							11,		20,		100,		0,			0,					HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE},
	{MK_GOLEM,			0,		{0},									{{0}},							17,		23,		100,		0,			0,					HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE},
	{MK_TENTACLE_HORROR,0,		{0},									{{0}},							20,		AMULET_LEVEL,100,0,			0,					HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE},
	{MK_DRAGON,			0,		{0},									{{0}},							23,		AMULET_LEVEL,100,0,			0,					HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE},

	// machine statue monsters
	{MK_GOBLIN,			0,		{0},									{{0}},							1,		6,		100,		STATUE_DORMANT, 0,				HORDE_MACHINE_STATUE},
	{MK_OGRE,			0,		{0},									{{0}},							6,		12,		100,		STATUE_DORMANT, 0,				HORDE_MACHINE_STATUE},
	{MK_WRAITH,			0,		{0},									{{0}},							10,		17,		100,		STATUE_DORMANT, 0,				HORDE_MACHINE_STATUE},
	{MK_NAGA,			0,		{0},									{{0}},							12,		19,		100,		STATUE_DORMANT, 0,				HORDE_MACHINE_STATUE},
	{MK_TROLL,			0,		{0},									{{0}},							14,		21,		100,		STATUE_DORMANT, 0,				HORDE_MACHINE_STATUE},
	{MK_GOLEM,			0,		{0},									{{0}},							21,		30,		100,		STATUE_DORMANT, 0,				HORDE_MACHINE_STATUE},
	{MK_DRAGON,			0,		{0},									{{0}},							29,		DEEPEST_LEVEL,	100,	STATUE_DORMANT, 0,			HORDE_MACHINE_STATUE},
	{MK_TENTACLE_HORROR,0,		{0},									{{0}},							29,		DEEPEST_LEVEL,	100,	STATUE_DORMANT, 0,			HORDE_MACHINE_STATUE},

	// machine turrets
	{MK_ARROW_TURRET,	0,		{0},									{{0}},							5,		13,		100,		TURRET_DORMANT, 0,				HORDE_MACHINE_TURRET},
	{MK_SPARK_TURRET,	0,		{0},									{{0}},							11,		18,		100,		TURRET_DORMANT, 0,				HORDE_MACHINE_TURRET},
	{MK_DART_TURRET,	0,		{0},									{{0}},							15,		22,		100,		TURRET_DORMANT, 0,				HORDE_MACHINE_TURRET},
	{MK_FLAME_TURRET,	0,		{0},									{{0}},							17,		24,		100,		TURRET_DORMANT, 0,				HORDE_MACHINE_TURRET},

	// machine mud monsters
	{MK_BOG_MONSTER,	0,		{0},									{{0}},							12,		26,		100,		MACHINE_MUD_DORMANT, 0,			HORDE_MACHINE_MUD},
	{MK_KRAKEN,			0,		{0},									{{0}},							17,		26,		30,		MACHINE_MUD_DORMANT, 0,			HORDE_MACHINE_MUD},

	// kennel monsters
	{MK_MONKEY,			0,		{0},									{{0}},							1,		5,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE},
	{MK_GOBLIN,			0,		{0},									{{0}},							1,		8,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE},
	{MK_GOBLIN_CONJURER,0,		{0},									{{0}},							2,		9,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE},
	{MK_GOBLIN_MYSTIC,	0,		{0},									{{0}},							2,		9,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE},
	{MK_OGRE,			0,		{0},									{{0}},							7,		17,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE},
	{MK_TROLL,			0,		{0},									{{0}},							12,		21,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE},
	{MK_NAGA,			0,		{0},									{{0}},							13,		23,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE},
	{MK_SALAMANDER,		0,		{0},									{{0}},							9,		20,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE},
	{MK_IMP,			0,		{0},									{{0}},							15,		26,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE},
	{MK_PIXIE,			0,		{0},									{{0}},							11,		21,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE},
	{MK_DAR_BLADEMASTER,0,		{0},									{{0}},							9,		AMULET_LEVEL, 100, MONSTER_CAGE_CLOSED, 0,		HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE},
	{MK_DAR_PRIESTESS,	0,		{0},									{{0}},							12,		AMULET_LEVEL, 100, MONSTER_CAGE_CLOSED, 0,		HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE},
	{MK_DAR_BATTLEMAGE,	0,		{0},									{{0}},							13,		AMULET_LEVEL, 100, MONSTER_CAGE_CLOSED, 0,		HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE},

	// vampire bloodbags
	{MK_MONKEY,			0,		{0},									{{0}},							1,		5,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE},
	{MK_GOBLIN,			0,		{0},									{{0}},							1,		8,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE},
	{MK_GOBLIN_CONJURER,0,		{0},									{{0}},							2,		9,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE},
	{MK_GOBLIN_MYSTIC,	0,		{0},									{{0}},							2,		9,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE},
	{MK_OGRE,			0,		{0},									{{0}},							5,		15,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE},
	{MK_TROLL,			0,		{0},									{{0}},							10,		19,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE},
	{MK_NAGA,			0,		{0},									{{0}},							9,		20,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE},
	{MK_IMP,			0,		{0},									{{0}},							15,		AMULET_LEVEL,100,MONSTER_CAGE_CLOSED, 0,			HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE},
	{MK_PIXIE,			0,		{0},									{{0}},							11,		21,		100,		MONSTER_CAGE_CLOSED, 0,			HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE},
	{MK_DAR_BLADEMASTER,0,		{0},									{{0}},							9,		AMULET_LEVEL,100,MONSTER_CAGE_CLOSED, 0,			HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE},
	{MK_DAR_PRIESTESS,	0,		{0},									{{0}},							12,		AMULET_LEVEL,100,MONSTER_CAGE_CLOSED, 0,			HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE},
	{MK_DAR_BATTLEMAGE,	0,		{0},									{{0}},							13,		AMULET_LEVEL,100,MONSTER_CAGE_CLOSED, 0,			HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE},

	// key thieves
	{MK_MONKEY,			0,		{0},									{{0}},							1,		14,		100,     0,          0,                  HORDE_MACHINE_THIEF},
	{MK_IMP,			0,		{0},									{{0}},							15,		DEEPEST_LEVEL,	100, 0,      0,                  HORDE_MACHINE_THIEF},

	// sacrifice victims
	{MK_MONKEY,			0,		{0},									{{0}},							1,		5,		100,		STATUE_INSTACRACK, 0,			HORDE_SACRIFICE_TARGET},
	{MK_GOBLIN,			0,		{0},									{{0}},							3,		10,		100,		STATUE_INSTACRACK, 0,			HORDE_SACRIFICE_TARGET},
	{MK_OGRE,			0,		{0},									{{0}},							7,		13,		100,		STATUE_INSTACRACK, 0,			HORDE_SACRIFICE_TARGET},
	{MK_TROLL,			0,		{0},									{{0}},							12,		19,		100,		STATUE_INSTACRACK, 0,			HORDE_SACRIFICE_TARGET},
	{MK_WRAITH,			0,		{0},									{{0}},							10,		17,		100,		STATUE_INSTACRACK, 0,			HORDE_SACRIFICE_TARGET},
	{MK_NAGA,			0,		{0},									{{0}},							13,		20,		100,		STATUE_INSTACRACK, 0,			HORDE_SACRIFICE_TARGET},
	{MK_DAR_BLADEMASTER,0,		{0},									{{0}},							10,		20,		100,		STATUE_INSTACRACK, 0,			HORDE_SACRIFICE_TARGET},
	{MK_GOLEM,			0,		{0},									{{0}},							22,		DEEPEST_LEVEL,100,  STATUE_INSTACRACK, 0,			HORDE_SACRIFICE_TARGET},
	{MK_REVENANT,		0,		{0},									{{0}},							22,		DEEPEST_LEVEL,100,  STATUE_INSTACRACK, 0,			HORDE_SACRIFICE_TARGET},
	{MK_TENTACLE_HORROR,0,		{0},									{{0}},							22,		DEEPEST_LEVEL,100,  STATUE_INSTACRACK, 0,			HORDE_SACRIFICE_TARGET},

	// legendary allies
	{MK_UNICORN,		0,		{0},									{{0}},							1,		DEEPEST_LEVEL,	100, 0,		0,					HORDE_MACHINE_LEGENDARY_ALLY | HORDE_ALLIED_WITH_PLAYER},
	{MK_IFRIT,			0,		{0},									{{0}},							1,		DEEPEST_LEVEL,	100,	0,		0,					HORDE_MACHINE_LEGENDARY_ALLY | HORDE_ALLIED_WITH_PLAYER},
	{MK_PHOENIX_EGG,	0,		{0},									{{0}},							1,		DEEPEST_LEVEL,	100,	0,		0,					HORDE_MACHINE_LEGENDARY_ALLY | HORDE_ALLIED_WITH_PLAYER},
	{MK_ANCIENT_SPIRIT,	0,		{0},									{{0}},							1,		DEEPEST_LEVEL,	100,	0,		0,					HORDE_MACHINE_LEGENDARY_ALLY | HORDE_ALLIED_WITH_PLAYER},

	// goblin warren
	{MK_GOBLIN,			0,		{0},									{{0}},							1,		10,		100,     0,              0,              HORDE_MACHINE_GOBLIN_WARREN},
	{MK_GOBLIN_CONJURER,0,		{0},									{{0}},							1,		10,		60,      0,              0,              HORDE_MACHINE_GOBLIN_WARREN},
	{MK_GOBLIN_TOTEM,	1,		{MK_GOBLIN},							{{2,4,1}},						5,		13,		100,		0,				MT_CAMP_AREA,	HORDE_MACHINE_GOBLIN_WARREN},
	{MK_GOBLIN,			3,		{MK_GOBLIN, MK_GOBLIN_MYSTIC, MK_JACKAL},{{2, 3, 1}, {1,2,1}, {1,2,1}},	6,		12,		40,      0,              0,              HORDE_MACHINE_GOBLIN_WARREN},
	{MK_GOBLIN_CONJURER,2,		{MK_GOBLIN_CONJURER, MK_GOBLIN_MYSTIC},	{{0,1,1}, {1,1,1}},				7,		15,		40,      0,              0,              HORDE_MACHINE_GOBLIN_WARREN},
	{MK_GOBLIN_TOTEM,	4,		{MK_GOBLIN_TOTEM, MK_GOBLIN_CONJURER, MK_GOBLIN_MYSTIC, MK_GOBLIN}, {{1,2,1},{1,2,1},{1,2,1},{3,5,1}},10,17,80,0,MT_CAMP_AREA,	HORDE_MACHINE_GOBLIN_WARREN},
	{MK_GOBLIN,			1,		{MK_GOBLIN},							{{1, 2, 1}},					3,		7,		10,		0,              0,				HORDE_MACHINE_GOBLIN_WARREN | HORDE_LEADER_CAPTIVE},
};

const monsterClass monsterClassCatalog[MONSTER_CLASS_COUNT] = {
	// name             frequency   maxDepth    member list
	{"abomination",     10,         -1,         {MK_BOG_MONSTER, MK_UNDERWORM, MK_KRAKEN, MK_TENTACLE_HORROR}},
	{"dar",             10,         22,         {MK_DAR_BLADEMASTER, MK_DAR_PRIESTESS, MK_DAR_BATTLEMAGE}},
	{"animal",          10,         10,         {MK_RAT, MK_MONKEY, MK_JACKAL, MK_EEL, MK_TOAD, MK_VAMPIRE_BAT, MK_CENTIPEDE, MK_SPIDER}},
	{"goblin",          10,         10,         {MK_GOBLIN, MK_GOBLIN_CONJURER, MK_GOBLIN_MYSTIC, MK_GOBLIN_TOTEM, MK_GOBLIN_CHIEFTAN, MK_SPECTRAL_BLADE}},
	{"ogre",            10,         16,         {MK_OGRE, MK_OGRE_SHAMAN, MK_OGRE_TOTEM}},
	{"dragon",          10,         -1,         {MK_DRAGON}},
	{"undead",          10,         -1,         {MK_ZOMBIE, MK_WRAITH, MK_VAMPIRE, MK_PHANTOM, MK_LICH, MK_REVENANT}},
	{"jelly",           10,         15,         {MK_PINK_JELLY, MK_BLACK_JELLY, MK_ACID_JELLY}},
	{"turret",          5,          18,         {MK_ARROW_TURRET, MK_SPARK_TURRET, MK_DART_TURRET, MK_FLAME_TURRET}},
	{"infernal",        10,         -1,         {MK_FLAMEDANCER, MK_IMP, MK_REVENANT, MK_FURY, MK_PHANTOM, MK_IFRIT}},
	{"mage",            10,         -1,         {MK_GOBLIN_CONJURER, MK_GOBLIN_MYSTIC, MK_OGRE_SHAMAN, MK_DAR_BATTLEMAGE, MK_PIXIE, MK_LICH}},
	{"waterborne",      10,         17,         {MK_EEL, MK_NAGA, MK_KRAKEN}},
	{"airborne",        10,         15,         {MK_VAMPIRE_BAT, MK_WILL_O_THE_WISP, MK_PIXIE, MK_PHANTOM, MK_FURY, MK_IFRIT, MK_PHOENIX}},
	{"fireborne",       10,         12,         {MK_WILL_O_THE_WISP, MK_SALAMANDER, MK_FLAMEDANCER, MK_PHOENIX}},
	{"troll",           10,         15,         {MK_TROLL}},
};

const char monsterBehaviorFlagDescriptions[32][COLS] = {
	"is invisible",								// MONST_INVISIBLE
	"is an inanimate object",					// MONST_INANIMATE
	"cannot move",								// MONST_IMMOBILE
	"",                                         // MONST_CARRY_ITEM_100
	"",                                         // MONST_CARRY_ITEM_25
	"",                                         // MONST_ALWAYS_HUNTING
	"flees at low health",						// MONST_FLEES_NEAR_DEATH
	"",											// MONST_ATTACKABLE_THRU_WALLS
	"corrodes weapons when hit",				// MONST_DEFEND_DEGRADE_WEAPON
	"is immune to weapon damage",				// MONST_IMMUNE_TO_WEAPONS
	"flies",									// MONST_FLIES
	"moves erratically",						// MONST_FLITS
	"is immune to fire",						// MONST_IMMUNE_TO_FIRE
	"",											// MONST_CAST_SPELLS_SLOWLY
	"cannot be entangled",						// MONST_IMMUNE_TO_WEBS
	"can reflect magic spells",                 // MONST_REFLECT_4
	"never sleeps",								// MONST_NEVER_SLEEPS
	"burns unceasingly",						// MONST_FIERY
	"is invulnerable",                          // MONST_INVULNERABLE
	"is at home in water",						// MONST_IMMUNE_TO_WATER
	"cannot venture onto dry land",				// MONST_RESTRICTED_TO_LIQUID
	"submerges",								// MONST_SUBMERGES
	"keeps $HISHER distance",					// MONST_MAINTAINS_DISTANCE
	"",											// MONST_WILL_NOT_USE_STAIRS
	"is animated purely by magic",				// MONST_DIES_IF_NEGATED
	"",                                         // MONST_MALE
	"",                                         // MONST_FEMALE
	"",                                         // MONST_NOT_LISTED_IN_SIDEBAR
	"moves only when activated",                // MONST_GETS_TURN_ON_ACTIVATION
};

const char monsterAbilityFlagDescriptions[32][COLS] = {
	"can induce hallucinations",				// MA_HIT_HALLUCINATE
	"can steal items",							// MA_HIT_STEAL_FLEE
	"lights enemies on fire when $HESHE hits",  // MA_HIT_BURN
	"can possess $HISHER summoned allies",		// MA_ENTER_SUMMONS
	"corrodes armor when $HESHE hits",			// MA_HIT_DEGRADE_ARMOR
	"can summon allies",						// MA_CAST_SUMMON
	"immobilizes $HISHER prey",					// MA_SEIZES
	"injects poison when $HESHE hits",			// MA_POISONS
	"",											// MA_DF_ON_DEATH
	"divides in two when struck",				// MA_CLONE_SELF_ON_DEFEND
	"dies when $HESHE attacks",					// MA_KAMIKAZE
	"recovers health when $HESHE inflicts damage",// MA_TRANSFERENCE
	"saps strength when $HESHE inflicts damage",// MA_CAUSE_WEAKNESS

	"attacks up to two opponents in a line",    // MA_ATTACKS_PENETRATE
	"attacks all adjacent opponents at once",   // MA_ATTACKS_ALL_ADJACENT
	"attacks with a whip",                      // MA_ATTACKS_EXTEND
	"pushes opponents backward when $HESHE hits", // MA_ATTACKS_STAGGER
	"avoids attacking in corridors in a group", // MA_AVOID_CORRIDORS
};

const char monsterBookkeepingFlagDescriptions[32][COLS] = {
	"",											// MB_WAS_VISIBLE
	"is telepathically bonded with you",		// MB_TELEPATHICALLY_REVEALED
	"",											// MB_PREPLACED
	"",											// MB_APPROACHING_UPSTAIRS
	"",											// MB_APPROACHING_DOWNSTAIRS
	"",											// MB_APPROACHING_PIT
	"",											// MB_LEADER
	"",											// MB_FOLLOWER
	"",											// MB_CAPTIVE
	"has been immobilized",						// MB_SEIZED
	"is currently holding $HISHER prey immobile",// MB_SEIZING
	"is submerged",								// MB_SUBMERGED
	"",											// MB_JUST_SUMMONED
	"",											// MB_WILL_FLASH
	"is anchored to reality by $HISHER summoner",// MB_BOUND_TO_LEADER
	"is marked for demonic sacrifice",          // MB_MARKED_FOR_SACRIFICE
};