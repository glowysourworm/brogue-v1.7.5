#pragma once

#include "brogueGlobal.h"
#include "levelData.h"
#include "playerCharacter.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::backend::model
{
	class gameData
	{

		// Moved from playerCharacter since they have to do with the game processing
		//
	public:

		int depthLevel;					// which dungeon level are we on
		int deepestLevel;
		bool disturbed;					// player should stop auto-acting
		bool gameHasEnded;				// stop everything and go to death screen
		bool highScoreSaved;				// so that it saves the high score only once
		bool blockCombatText;			// busy auto-fighting
		bool autoPlayingLevel;			// seriously, don't interrupt
		bool automationActive;			// cut some corners during redraws to speed things up

	public:

		//tcell tmap[DCOLS][DROWS];						// grids with info about the map
		//pcell pmap[DCOLS][DROWS];
		//int** scentMap;
		//cellDisplayBuffer displayBuffer[COLS][ROWS];	// used to optimize plotCharWithColor
		//int terrainRandomValues[DCOLS][DROWS][8];
		//int** safetyMap;								// used to help monsters flee
		//int** allySafetyMap;							// used to help allies flee
		//int** chokeMap;								// used to assess the importance of the map's various chokepoints
		//const int nbDirs[8][2] = { {0,-1}, {0,1}, {-1,0}, {1,0}, {-1,-1}, {-1,1}, {1,-1}, {1,1} };
		//const int cDirs[8][2] = { {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1} };
		int numberOfWaypoints;

	public:

		levelData* levels;

		//creature player;
		//playerCharacter rogue;
		//creature* monsters;
		//creature* dormantMonsters;
		//creature* graveyard;
		//creature* purgatory;
		//item* floorItems;
		//item* packItems;
		//item* monsterItemsHopper;

	public:

		char displayedMessage[MESSAGE_LINES][COLS * 2];
		bool messageConfirmed[MESSAGE_LINES];
		char combatText[COLS * 2];
		int messageArchivePosition;
		char messageArchive[MESSAGE_ARCHIVE_LINES][COLS * 2];

		simpleString currentFilePath;

		char displayDetail[DCOLS][DROWS];		// used to make certain per-cell data accessible to external code (e.g. terminal adaptations)

		//unsigned char inputRecordBuffer[INPUT_RECORD_BUFFER + 100];
		unsigned int locationInRecordingBuffer;
		unsigned long randomNumbersGenerated;
		unsigned long positionInPlaybackFile;
		unsigned long lengthOfPlaybackFile;
		unsigned long recordingLocation;
		unsigned long maxLevelChanges;
		//char annotationPathname[BROGUE_FILENAME_MAX];	// pathname of annotation file
		unsigned long gameSeed;

		// Console / Game related declarations
		playerCharacter rogue;
		//brogueConsole currentConsole;

		bool serverMode = false;
		bool noMenu = false;
	};
}

