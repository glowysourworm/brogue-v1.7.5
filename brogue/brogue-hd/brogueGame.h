#pragma once

#include "levelData.h"
#include "brogueLevel.h"
#include "brogueCreature.h"
#include "broguePlayer.h"
#include "creatureDeclaration.h"
#include "brogueCellDisplay.h"
#include "brogueMessageQueue.h"
#include "playerCharacter.h"
#include <vector>

using namespace std;

using namespace brogueHd::backend::model::creature;

namespace brogueHd::backend::model
{
	class brogueGame
	{
	public:

		brogueGame(boolean serverMode, boolean noMenu);
		~brogueGame();

		boolean getServerMode();
		boolean getNoMenu();

		void outputMessage(char* msg, color theColor, boolean requireAcknowledgment);

	private:

		std::vector<brogueLevel*> _levels;

		brogueMessageQueue* _messageQueue;

		boolean _serverMode;
		boolean _noMenu;

		// levelData* _levels;

		// TODO: Move these to the content grid

		broguePlayer* _player;
		playerCharacter rogue;
		brogueCreature* monsters;
		brogueCreature* dormantMonsters;
		brogueCreature* graveyard;
		brogueCreature* purgatory;
		brogueItem* floorItems;
		brogueItem* packItems;
		brogueItem* monsterItemsHopper;

		short currentDepth;					// which dungeon level are we on
		//short deepesteLevel;
		boolean disturbed;					// player should stop auto-acting
		boolean gameHasEnded;				// stop everything and go to death screen
		boolean highScoreSaved;				// so that it saves the high score only once
		boolean blockCombatText;			// busy auto-fighting
		boolean autoPlayingLevel;			// seriously, don't interrupt
		boolean automationActive;			// cut some corners during redraws to speed things up

		//tcell tmap[DCOLS][DROWS];						// grids with info about the map
		//pcell pmap[DCOLS][DROWS];
		//short** scentMap;
		//cellDisplayBuffer displayBuffer[COLS][ROWS];	// used to optimize plotCharWithColor
		//short terrainRandomValues[DCOLS][DROWS][8];
		//short** safetyMap;								// used to help monsters flee
		//short** allySafetyMap;							// used to help allies flee
		//short** chokeMap;								// used to assess the importance of the map's various chokepoints
		//const short nbDirs[8][2] = { {0,-1}, {0,1}, {-1,0}, {1,0}, {-1,-1}, {-1,1}, {1,-1}, {1,1} };
		//const short cDirs[8][2] = { {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1} };
		//short numberOfWaypoints;



	private:

//		char displayedMessage[MESSAGE_LINES][COLS * 2];
//		boolean messageConfirmed[MESSAGE_LINES];
//		char combatText[COLS * 2];
//		short messageArchivePosition;
//		char messageArchive[MESSAGE_ARCHIVE_LINES][COLS * 2];
//
//		char currentFilePath[BROGUE_FILENAME_MAX];
//
//		char displayDetail[DCOLS][DROWS];		// used to make certain per-cell data accessible to external code (e.g. terminal adaptations)
//
//#ifdef AUDIT_RNG
//		FILE* RNGLogFile;
//#endif

		//unsigned char inputRecordBuffer[INPUT_RECORD_BUFFER + 100];
		//unsigned short locationInRecordingBuffer;
		//unsigned long randomNumbersGenerated;
		//unsigned long positionInPlaybackFile;
		//unsigned long lengthOfPlaybackFile;
		//unsigned long recordingLocation;
		//unsigned long maxLevelChanges;
		//char annotationPathname[BROGUE_FILENAME_MAX];	// pathname of annotation file
		//unsigned long gameSeed;

		//// Console / Game related declarations
		//playerCharacter rogue;
		////brogueConsole currentConsole;

		//boolean serverMode = false;
		//boolean noMenu = false;
	};
}

