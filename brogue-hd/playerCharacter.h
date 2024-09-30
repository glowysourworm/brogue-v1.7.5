#pragma once

#include "brogueGlobal.h"

namespace brogueHd::backend::model::game
{
	class playerCharacter
	{
	public:

		//short depthLevel;					// which dungeon level are we on
		//short deepestLevel;
		//boolean disturbed;					// player should stop auto-acting
		//boolean gameHasEnded;				// stop everything and go to death screen
		//boolean highScoreSaved;				// so that it saves the high score only once
		//boolean blockCombatText;			// busy auto-fighting
		//boolean autoPlayingLevel;			// seriously, don't interrupt
		//boolean automationActive;			// cut some corners during redraws to speed things up
		bool justRested;					// previous turn was a rest -- used in stealth
		bool justSearched;				// previous turn was a search -- used in manual searches
		bool cautiousMode;				// used to prevent careless deaths caused by holding down a key
		bool receivedLevitationWarning;	// only warn you once when you're hovering dangerously over liquid
		bool updatedSafetyMapThisTurn;	// so it's updated no more than once per turn
		bool updatedAllySafetyMapThisTurn;	// so it's updated no more than once per turn
		bool updatedMapToSafeTerrainThisTurn;// so it's updated no more than once per turn
		bool updatedMapToShoreThisTurn;		// so it's updated no more than once per turn
		bool easyMode;					// enables easy mode
		bool inWater;					// helps with the blue water filter effect
		bool heardCombatThisTurn;		// so you get only one "you hear combat in the distance" per turn
		bool creaturesWillFlashThisTurn;	// there are creatures out there that need to flash before the turn ends
		bool staleLoopMap;				// recalculate the loop map at the end of the turn
		bool alreadyFell;				// so the player can fall only one depth per turn
		bool eligibleToUseStairs;		// so the player uses stairs only when he steps onto them
		bool trueColorMode;				// whether lighting effects are disabled
		bool displayAggroRangeMode;      // whether your stealth range is displayed
		bool quit;						// to skip the typical end-game theatrics when the player quits
		unsigned long seed;					// the master seed for generating the entire dungeon
		short RNG;							// which RNG are we currently using?
		unsigned long gold;					// how much gold we have
		unsigned long goldGenerated;		// how much gold has been generated on the levels, not counting gold held by monsters
		short strength;
		unsigned short monsterSpawnFuse;	// how much longer till a random monster spawns

		//item* weapon;
		//item* armor;
		//item* ringLeft;
		//item* ringRight;

		//flare** flares;
		short flareCount;
		short flareCapacity;

		//creature* yendorWarden;

		//lightSource minersLight;
		int64_t minersLightRadius;
		short ticksTillUpdateEnvironment;	// so that some periodic things happen in objective time
		unsigned short scentTurnNumber;		// helps make scent-casting work
		unsigned long playerTurnNumber;     // number of input turns in recording. Does not increment during paralysis.
		unsigned long absoluteTurnNumber;   // number of turns since the beginning of time. Always increments.
		signed long milliseconds;			// milliseconds since launch, to decide whether to engage cautious mode
		short xpxpThisTurn;					// how many squares the player explored this turn
		short aggroRange;                   // distance from which monsters will notice you

		short previousPoisonPercent;        // and your poison proportion, to display percentage alerts for each.

		short upLoc[2];						// upstairs location this level
		short downLoc[2];					// downstairs location this level

		short cursorLoc[2];					// used for the return key functionality
		//creature* lastTarget;				// to keep track of the last monster the player has thrown at or zapped
		short rewardRoomsGenerated;			// to meter the number of reward machines
		short machineNumber;				// so each machine on a level gets a unique number
		short sidebarLocationList[ROWS * 2][2];	// to keep track of which location each line of the sidebar references

		// maps
		short** mapToShore;					// how many steps to get back to shore
		short** mapToSafeTerrain;			// so monsters can get to safety

		//// recording info
		//boolean playbackMode;				// whether we're viewing a recording instead of playing
		//unsigned long currentTurnNumber;	// how many turns have elapsed
		//unsigned long howManyTurns;			// how many turns are in this recording
		//short howManyDepthChanges;			// how many times the player changes depths
		//short playbackDelayPerTurn;			// base playback speed; modified per turn by events
		//short playbackDelayThisTurn;		// playback speed as modified
		//boolean playbackPaused;
		//boolean playbackFastForward;		// for loading saved games and such -- disables drawing and prevents pauses
		//boolean playbackOOS;				// playback out of sync -- no unpausing allowed
		//boolean playbackOmniscience;		// whether to reveal all the map during playback
		//boolean playbackBetweenTurns;		// i.e. waiting for a top-level input -- iff, permit playback commands
		//unsigned long nextAnnotationTurn;	// the turn number during which to display the next annotation
		//char nextAnnotation[5000];			// the next annotation
		//unsigned long locationInAnnotationFile; // how far we've read in the annotations file

		// metered items
		int64_t foodSpawned;					// amount of nutrition units spawned so far this game
		short lifePotionFrequency;
		short lifePotionsSpawned;
		short strengthPotionFrequency;
		short enchantScrollFrequency;

		// ring bonuses:
		short clairvoyance;
		short stealthBonus;
		short regenerationBonus;
		short lightMultiplier;
		short awarenessBonus;
		short transference;
		short wisdomBonus;
		short reaping;

		// feats:
		//boolean featRecord[FEAT_COUNT];

		// waypoints:
		//short** wpDistance[MAX_WAYPOINT_COUNT];
		//short wpCount;
		//short wpCoordinates[MAX_WAYPOINT_COUNT][2];
		//short wpRefreshTicker;

		// cursor trail:
		short cursorPathIntensity;
		bool cursorMode;
	};
}

