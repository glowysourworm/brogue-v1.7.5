#pragma once

#include <cstdint>

namespace brogueHd::model
{
	class playerCharacter
	{
	public:
		//int depthLevel;					// which dungeon level are we on
		//int deepestLevel;
		//boolean disturbed;					// player should stop auto-acting
		//boolean gameHasEnded;				// stop everything and go to death screen
		//boolean highScoreSaved;				// so that it saves the high score only once
		//boolean blockCombatText;			// busy auto-fighting
		//boolean autoPlayingLevel;			// seriously, don't interrupt
		//boolean automationActive;			// cut some corners during redraws to speed things up
		bool justRested; // previous turn was a rest -- used in stealth
		bool justSearched; // previous turn was a search -- used in manual searches
		bool cautiousMode; // used to prevent careless deaths caused by holding down a key
		bool receivedLevitationWarning; // only warn you once when you're hovering dangerously over liquid
		bool updatedSafetyMapThisTurn; // so it's updated no more than once per turn
		bool updatedAllySafetyMapThisTurn; // so it's updated no more than once per turn
		bool updatedMapToSafeTerrainThisTurn; // so it's updated no more than once per turn
		bool updatedMapToShoreThisTurn; // so it's updated no more than once per turn
		bool easyMode; // enables easy mode
		bool inWater; // helps with the blue water filter effect
		bool heardCombatThisTurn; // so you get only one "you hear combat in the distance" per turn
		bool creaturesWillFlashThisTurn; // there are creatures out there that need to flash before the turn ends
		bool staleLoopMap; // recalculate the loop map at the end of the turn
		bool alreadyFell; // so the player can fall only one depth per turn
		bool eligibleToUseStairs; // so the player uses stairs only when he steps onto them
		bool trueColorMode; // whether lighting effects are disabled
		bool displayAggroRangeMode; // whether your stealth range is displayed
		bool quit; // to skip the typical end-game theatrics when the player quits
		unsigned long seed; // the master seed for generating the entire dungeon
		int RNG; // which RNG are we currently using?
		unsigned long gold; // how much gold we have
		unsigned long goldGenerated;
		// how much gold has been generated on the levels, not counting gold held by monsters
		int strength;
		unsigned int monsterSpawnFuse; // how much longer till a random monster spawns

		//item* weapon;
		//item* armor;
		//item* ringLeft;
		//item* ringRight;

		//flare** flares;
		int flareCount;
		int flareCapacity;

		//creature* yendorWarden;

		//lightSource minersLight;
		int64_t minersLightRadius;
		int ticksTillUpdateEnvironment; // so that some periodic things happen in objective time
		unsigned int scentTurnNumber; // helps make scent-casting work
		unsigned long playerTurnNumber; // number of input turns in recording. Does not increment during paralysis.
		unsigned long absoluteTurnNumber; // number of turns since the beginning of time. Always increments.
		signed long milliseconds; // milliseconds since launch, to decide whether to engage cautious mode
		int xpxpThisTurn; // how many squares the player explored this turn
		int aggroRange; // distance from which monsters will notice you

		int previousPoisonPercent; // and your poison proportion, to display percentage alerts for each.

		int upLoc[2]; // upstairs location this level
		int downLoc[2]; // downstairs location this level

		int cursorLoc[2]; // used for the return key functionality
		//creature* lastTarget;				// to keep track of the last monster the player has thrown at or zapped
		int rewardRoomsGenerated; // to meter the number of reward machines
		int machineNumber; // so each machine on a level gets a unique number
		int sidebarLocationList[ROWS * 2][2]; // to keep track of which location each line of the sidebar references

		// maps
		int** mapToShore; // how many steps to get back to shore
		int** mapToSafeTerrain; // so monsters can get to safety

		//// recording info
		//boolean playbackMode;				// whether we're viewing a recording instead of playing
		//unsigned long currentTurnNumber;	// how many turns have elapsed
		//unsigned long howManyTurns;			// how many turns are in this recording
		//int howManyDepthChanges;			// how many times the player changes depths
		//int playbackDelayPerTurn;			// base playback speed; modified per turn by events
		//int playbackDelayThisTurn;		// playback speed as modified
		//boolean playbackPaused;
		//boolean playbackFastForward;		// for loading saved games and such -- disables drawing and prevents pauses
		//boolean playbackOOS;				// playback out of sync -- no unpausing allowed
		//boolean playbackOmniscience;		// whether to reveal all the map during playback
		//boolean playbackBetweenTurns;		// i.e. waiting for a top-level input -- iff, permit playback commands
		//unsigned long nextAnnotationTurn;	// the turn number during which to display the next annotation
		//char nextAnnotation[5000];			// the next annotation
		//unsigned long locationInAnnotationFile; // how far we've read in the annotations file

		// metered items
		int64_t foodSpawned; // amount of nutrition units spawned so far this game
		int lifePotionFrequency;
		int lifePotionsSpawned;
		int strengthPotionFrequency;
		int enchantScrollFrequency;

		// ring bonuses:
		int clairvoyance;
		int stealthBonus;
		int regenerationBonus;
		int lightMultiplier;
		int awarenessBonus;
		int transference;
		int wisdomBonus;
		int reaping;

		// feats:
		//boolean featRecord[FEAT_COUNT];

		// waypoints:
		//int** wpDistance[MAX_WAYPOINT_COUNT];
		//int wpCount;
		//int wpCoordinates[MAX_WAYPOINT_COUNT][2];
		//int wpRefreshTicker;

		// cursor trail:
		int cursorPathIntensity;
		bool cursorMode;
	};
}
