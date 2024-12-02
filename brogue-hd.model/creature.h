#pragma once

#include "brogueGlobal.h"
#include "color.h"
#include "creatureBehaviorConstants.h"
#include "creatureConstants.h"
#include "dungeonConstants.h"
#include "lightConstants.h"
#include "randomRange.h"

namespace brogueHd::model
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

	struct horde
	{
		creatureTypes leaderType;

		// membership information
		int numberOfMemberTypes;
		creatureTypes memberType[5];
		randomRange memberCount[5];

		// spawning information
		int minLevel;
		int maxLevel;
		int frequency;
		tileType spawnsIn;
		int machine;

		hordeFlags flags;
	};

	struct mutation
	{
		char title[100];
		const color* textColor;
		int healthFactor;
		int moveSpeedFactor;
		int attackSpeedFactor;
		int defenseFactor;
		int damageFactor;
		int DFChance;
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
		int frequency;
		int maxDepth;
		creatureTypes memberList[15];
	};

	// Defines all creatures, which include monsters and the player:
	struct creatureInfo
	{
		creatureTypes id; // index number for the monsterCatalog
		char name[COLS];
		char symbol;

		int maxHP;
		int defense;
		int accuracy;
		randomRange damage;
		long turnsBetweenRegen; // turns to wait before regaining 1 HP
		int movementSpeed;
		int attackSpeed;

		dungeonFeatureBodilyFluids bloodType;
		lightType intrinsicLightType;

		// NEXT*** THESE NEED TO BE BROKEN UP; AND REDUCED. Dungeon features are too big to 
		//		   try and encapsulate the ones that are specific to creatures. So, probably
		//		   will be some sort of "special effect" class, or "dungeon alteration", which
		//		   will be shared among brogueObject instances.

		int DFChance; // percent chance to spawn the dungeon feature per awake turn
		permanentDungeonLayers DFType; // kind of dungeon feature
		//boltType bolts[20];
		unsigned long flags;
		unsigned long abilityFlags;
	};
}
