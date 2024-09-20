#pragma once

namespace brogueHd::backend::model::layout
{
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
}