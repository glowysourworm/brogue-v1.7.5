#pragma once

// keeps track of graphics so we only redraw if the cell has changed:
typedef struct cellDisplayBuffer {
	uchar character;
	char foreColorComponents[3];
	char backColorComponents[3];
	char opacity;
	boolean needsUpdate;
} cellDisplayBuffer;

typedef struct pcell {								// permanent cell; have to remember this stuff to save levels
	enum tileType layers[NUMBER_TERRAIN_LAYERS];	// terrain
	unsigned long flags;							// non-terrain cell flags
	unsigned short volume;							// quantity of gas in cell
	unsigned char machineNumber;
	cellDisplayBuffer rememberedAppearance;			// how the player remembers the cell to look
	enum itemCategory rememberedItemCategory;		// what category of item the player remembers lying there
	short rememberedItemKind;                       // what kind of item the player remembers lying there
	short rememberedItemQuantity;                   // how many of the item the player remembers lying there
	enum tileType rememberedTerrain;				// what the player remembers as the terrain (i.e. highest priority terrain upon last seeing)
	unsigned long rememberedCellFlags;              // map cell flags the player remembers from that spot
	unsigned long rememberedTerrainFlags;           // terrain flags the player remembers from that spot
	unsigned long rememberedTMFlags;                // TM flags the player remembers from that spot
} pcell;

typedef struct tcell {			// transient cell; stuff we don't need to remember between levels
	short light[3];				// RGB components of lighting
	short oldLight[3];			// compare with subsequent lighting to determine whether to refresh cell
} tcell;

typedef struct randomRange {
	short lowerBound;
	short upperBound;
	short clumpFactor;
} randomRange;

typedef struct color {
	// base RGB components:
	short red;
	short green;
	short blue;

	// random RGB components to add to base components:
	short redRand;
	short greenRand;
	short blueRand;

	// random scalar to add to all components:
	short rand;

	// Flag: this color "dances" with every refresh:
	boolean colorDances;
} color;

typedef struct keyLocationProfile {
	short x;
	short y;
	short machine;
	boolean disposableHere;
} keyLocationProfile;

typedef struct item {
	unsigned short category;
	short kind;
	unsigned long flags;
	randomRange damage;
	short armor;
	short charges;
	short enchant1;
	short enchant2;
	short timesEnchanted;
	enum monsterTypes vorpalEnemy;
	short strengthRequired;
	unsigned short quiverNumber;
	uchar displayChar;
	color* foreColor;
	color* inventoryColor;
	short quantity;
	char inventoryLetter;
	char inscription[DCOLS];
	short xLoc;
	short yLoc;
	keyLocationProfile keyLoc[KEY_ID_MAXIMUM];
	short originDepth;
	struct item* nextItem;
} item;

typedef struct itemTable {
	char* name;
	char* flavor;
	char callTitle[30];
	short frequency;
	short marketValue;
	short strengthRequired;
	randomRange range;
	boolean identified;
	boolean called;
	char description[1500];
} itemTable;


typedef struct dungeonFeature {
	// tile info:
	enum tileType tile;
	enum dungeonLayers layer;

	// spawning pattern:
	short startProbability;
	short probabilityDecrement;
	unsigned long flags;
	char description[DCOLS];
	enum lightType lightFlare;
	const color* flashColor;
	short effectRadius;
	enum tileType propagationTerrain;
	enum dungeonFeatureTypes subsequentDF;
	boolean messageDisplayed;
} dungeonFeature;

// Level profiles, affecting what rooms get chosen and how they're connected:
typedef struct dungeonProfile {
	// Room type weights (in the natural dungeon, these are also adjusted based on depth):
	short roomFrequencies[ROOM_TYPE_COUNT];

	short corridorChance;
} dungeonProfile;

typedef struct floorTileType {
	// appearance:
	uchar displayChar;
	const color* foreColor;
	const color* backColor;
	short drawPriority;                     // priority (lower number means higher priority); governs drawing as well as tile replacement comparisons.
	char chanceToIgnite;					// chance to burn if a flame terrain is on one of the four cardinal neighbors
	enum dungeonFeatureTypes fireType;		// spawn this DF when the terrain ignites (or, if it's T_IS_DF_TRAP, when the pressure plate clicks)
	enum dungeonFeatureTypes discoverType;	// spawn this DF when successfully searched if T_IS_SECRET is set
	enum dungeonFeatureTypes promoteType;	// creates this dungeon spawn type when it promotes for some other reason (random promotion or promotion through machine activation)
	short promoteChance;					// percent chance per turn to spawn the promotion type; will also vanish upon doing so if T_VANISHES_UPON_PROMOTION is set
	short glowLight;						// if it glows, this is the ID of the light type
	unsigned long flags;
	unsigned long mechFlags;
	char description[COLS];
	char flavorText[COLS];
} floorTileType;

typedef struct lightSource {
	const color* lightColor;
	randomRange lightRadius;
	short radialFadeToPercent;
	boolean passThroughCreatures; // generally no, but miner light does
} lightSource;

typedef struct flare {
	lightSource* light;                 // Flare light
	short coeffChangeAmount;            // The constant amount by which the coefficient changes per frame, e.g. -25 means it gets 25% dimmer per frame.
	short coeffLimit;                   // Flare ends if the coefficient passes this percentage (whether going up or down).
	short xLoc, yLoc;                   // Current flare location.
	long coeff;                         // Current flare coefficient; always starts at 100.
	unsigned long turnNumber;           // So we can eliminate those that fired one or more turns ago.
} flare;