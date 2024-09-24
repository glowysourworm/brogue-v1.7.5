#pragma once

// Defines all creatures, which include monsters and the player:
typedef struct creatureType {
	enum monsterTypes monsterID; // index number for the monsterCatalog
	char monsterName[COLS];
	uchar displayChar;
	const color* foreColor;
	short maxHP;
	short defense;
	short accuracy;
	randomRange damage;
	long turnsBetweenRegen;		// turns to wait before regaining 1 HP
	short movementSpeed;
	short attackSpeed;
	enum dungeonFeatureTypes bloodType;
	enum lightType intrinsicLightType;
	short DFChance;						// percent chance to spawn the dungeon feature per awake turn
	enum dungeonFeatureTypes DFType;	// kind of dungeon feature
	enum boltType bolts[20];
	unsigned long flags;
	unsigned long abilityFlags;
} creatureType;

enum creatureStates {
	MONSTER_SLEEPING,
	MONSTER_TRACKING_SCENT,
	MONSTER_WANDERING,
	MONSTER_FLEEING,
	MONSTER_ALLY,
};

enum creatureModes {
	MODE_NORMAL,
	MODE_PERM_FLEEING
};

typedef struct creature {
	creatureType info;
	short xLoc;
	short yLoc;
	short depth;
	short currentHP;
	long turnsUntilRegen;
	short regenPerTurn;					// number of HP to regenerate every single turn
	short weaknessAmount;				// number of points of weakness that are inflicted by the weakness status
	short poisonAmount;                 // number of points of damage per turn from poison
	enum creatureStates creatureState;	// current behavioral state
	enum creatureModes creatureMode;	// current behavioral mode (higher-level than state)

	short mutationIndex;                // what mutation the monster has (or -1 for none)

	// Waypoints:
	short targetWaypointIndex;          // the index number of the waypoint we're pathing toward
	boolean waypointAlreadyVisited[MAX_WAYPOINT_COUNT]; // checklist of waypoints
	short lastSeenPlayerAt[2];          // last location at which the monster hunted the player

	short targetCorpseLoc[2];			// location of the corpse that the monster is approaching to gain its abilities
	char targetCorpseName[30];			// name of the deceased monster that we're approaching to gain its abilities
	unsigned long absorptionFlags;		// ability/behavior flags that the monster will gain when absorption is complete
	boolean absorbBehavior;				// above flag is behavior instead of ability (ignored if absorptionBolt is set)
	short absorptionBolt;               // bolt index that the monster will learn to cast when absorption is complete
	short corpseAbsorptionCounter;		// used to measure both the time until the monster stops being interested in the corpse,
										// and, later, the time until the monster finishes absorbing the corpse.
	short** mapToMe;					// if a pack leader, this is a periodically updated pathing map to get to the leader
	short** safetyMap;					// fleeing monsters store their own safety map when out of player FOV to avoid omniscience
	short ticksUntilTurn;				// how long before the creature gets its next move

	// Locally cached statistics that may be temporarily modified:
	short movementSpeed;
	short attackSpeed;

	short previousHealthPoints;         // remembers what your health proportion was at the start of the turn
	short turnsSpentStationary;			// how many (subjective) turns it's been since the creature moved between tiles
	short flashStrength;				// monster will flash soon; this indicates the percent strength of flash
	color flashColor;					// the color that the monster will flash
	short status[NUMBER_OF_STATUS_EFFECTS];
	short maxStatus[NUMBER_OF_STATUS_EFFECTS]; // used to set the max point on the status bars
	unsigned long bookkeepingFlags;
	short spawnDepth;					// keep track of the depth of the machine to which they relate (for activation monsters)
	short machineHome;                  // monsters that spawn in a machine keep track of the machine number here (for activation monsters)
	short xpxp;							// exploration experience (used to time telepathic bonding for allies)
	short newPowerCount;                // how many more times this monster can absorb a fallen monster
	short totalPowerCount;              // how many times has the monster been empowered? Used to recover abilities when negated.

	struct creature* leader;			// only if monster is a follower
	struct creature* carriedMonster;	// when vampires turn into bats, one of the bats restores the vampire when it dies
	struct creature* nextCreature;
	struct item* carriedItem;			// only used for monsters
} creature;