#pragma once

#include "brogueCreature.h"
#include "brogueItem.h"
#include "brogueLevel.h"
#include "brogueMessageQueue.h"
#include "broguePlayer.h"
#include "color.h"
#include "playerCharacter.h"
#include "simpleList.h"

using namespace brogueHd::simple;
using namespace brogueHd::component;

namespace brogueHd::backend::model
{
	class brogueGame
	{
	public:

		brogueGame(bool serverMode, bool noMenu, const simpleList<brogueLevel*>& levels);
		~brogueGame();

		bool getServerMode();
		bool getNoMenu();

		void outputMessage(char* msg, color theColor, bool requireAcknowledgment);

		brogueLevel* getLevel(int depth) const;
		brogueLevel* getCurrentLevel() const;

	private:

		simpleList<brogueLevel*>* _levels;

		brogueMessageQueue* _messageQueue;

		bool _serverMode;
		bool _noMenu;

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

		int currentDepth;					// which dungeon level are we on
		//int deepesteLevel;
		bool disturbed;					// player should stop auto-acting
		bool gameHasEnded;				// stop everything and go to death screen
		bool highScoreSaved;				// so that it saves the high score only once
		bool blockCombatText;			// busy auto-fighting
		bool autoPlayingLevel;			// seriously, don't interrupt
		bool automationActive;			// cut some corners during redraws to speed things up

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
		//int numberOfWaypoints;



	private:

		//		char displayedMessage[MESSAGE_LINES][COLS * 2];
		//		boolean messageConfirmed[MESSAGE_LINES];
		//		char combatText[COLS * 2];
		//		int messageArchivePosition;
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
				//unsigned int locationInRecordingBuffer;
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

	brogueGame::brogueGame(bool serverMode, bool noMenu, const simpleList<brogueLevel*>& levels)
	{
		_messageQueue = new brogueMessageQueue();
		_levels = new simpleList<brogueLevel*>(levels);

		_serverMode = serverMode;
		_noMenu = noMenu;
	}
	brogueGame::~brogueGame()
	{
		delete _messageQueue;
		delete _levels;
	}

	bool brogueGame::getServerMode()
	{
		return _serverMode;
	}
	bool brogueGame::getNoMenu()
	{
		return _noMenu;
	}
	brogueLevel* brogueGame::getLevel(int depth) const
	{
		return _levels->get(depth - 1);
	}
	brogueLevel* brogueGame::getCurrentLevel() const
	{
		return _levels->get(currentDepth - 1);
	}

	void brogueGame::outputMessage(char* msg, color theColor, bool requireAcknowledgment)
	{
		_messageQueue->addMessage(msg, theColor, requireAcknowledgment);
	}
}

