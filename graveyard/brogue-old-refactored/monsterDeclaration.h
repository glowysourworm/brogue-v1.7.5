#pragma once



typedef struct monsterWords {
    char flavorText[COLS * 5];
    char absorbing[40];
    char absorbStatus[40];
    char attack[5][30];
    char DFMessage[DCOLS * 2];
    char summonMessage[DCOLS * 2];
} monsterWords;

typedef struct hordeType {
	enum monsterTypes leaderType;

	// membership information
	short numberOfMemberTypes;
	enum monsterTypes memberType[5];
	randomRange memberCount[5];

	// spawning information
	short minLevel;
	short maxLevel;
	short frequency;
	enum tileType spawnsIn;
	short machine;

	enum hordeFlags flags;
} hordeType;

typedef struct mutation {
    char title[100];
    const color* textColor;
    short healthFactor;
    short moveSpeedFactor;
    short attackSpeedFactor;
    short defenseFactor;
    short damageFactor;
    short DFChance;
    enum dungeonFeatureTypes DFType;
    enum lightType light;
    unsigned long monsterFlags;
    unsigned long monsterAbilityFlags;
    unsigned long forbiddenFlags;
    unsigned long forbiddenAbilityFlags;
    char description[1000];
} mutation;

typedef struct monsterClass {
    char name[30];
    short frequency;
    short maxDepth;
    enum monsterTypes memberList[15];
} monsterClass;