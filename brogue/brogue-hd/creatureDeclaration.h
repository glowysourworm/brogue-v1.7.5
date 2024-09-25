#pragma once

#include "lightConstants.h"
#include "dungeonConstants.h"
#include "color.h"

using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model::creature
{
	struct creatureWords 
	{
		char flavorText[COLS * 5];
		char absorbing[40];
		char absorbStatus[40];
		char attack[5][30];
		char DFMessage[DCOLS * 2];
		char summonMessage[DCOLS * 2];

	};

	struct hordeType 
	{
		creatureTypes leaderType;

		// membership information
		short numberOfMemberTypes;
		creatureTypes memberType[5];
		randomRange memberCount[5];

		// spawning information
		short minLevel;
		short maxLevel;
		short frequency;
		tileType spawnsIn;
		short machine;

		hordeFlags flags;

	};

	struct mutation 
	{
		char title[100];
		const color* textColor;
		short healthFactor;
		short moveSpeedFactor;
		short attackSpeedFactor;
		short defenseFactor;
		short damageFactor;
		short DFChance;
		permanentDungeonLayers DFType;
		lightType light;
		unsigned long monsterFlags;
		unsigned long monsterAbilityFlags;
		unsigned long forbiddenFlags;
		unsigned long forbiddenAbilityFlags;
		char description[1000];
	};

	struct creatureClass 
	{
		char name[30];
		short frequency;
		short maxDepth;
		creatureTypes memberList[15];
	};

	// Defines all creatures, which include monsters and the player:
	struct creatureInfo
	{
		creatureTypes id; // index number for the monsterCatalog
		char name[COLS];
		char symbol;

		short maxHP;
		short defense;
		short accuracy;
		randomRange damage;
		long turnsBetweenRegen;		// turns to wait before regaining 1 HP
		short movementSpeed;
		short attackSpeed;

		dungeonFeatureBodilyFluids bloodType;
		lightType intrinsicLightType;

		// NEXT*** THESE NEED TO BE BROKEN UP; AND REDUCED. Dungeon features are too big to 
		//		   try and encapsulate the ones that are specific to creatures. So, probably
		//		   will be some sort of "special effect" class, or "dungeon alteration", which
		//		   will be shared among brogueObject instances.

		short DFChance;						// percent chance to spawn the dungeon feature per awake turn
		permanentDungeonLayers DFType;	// kind of dungeon feature
		boltType bolts[20];
		unsigned long flags;
		unsigned long abilityFlags;
	};

	enum creatureTypes 
	{
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
}

