#pragma once

#include "roguedef.h"
#include "dungeon.h"
#include "levelData.h"
#include "creature.h"
#include "playerCharacter.h"
#include "platform.h"

/*

GAME DEFINITIONS:  These are the global variables for the game components

*/

tcell tmap[DCOLS][DROWS];						// grids with info about the map
pcell pmap[DCOLS][DROWS];
short** scentMap;
cellDisplayBuffer displayBuffer[COLS][ROWS];	// used to optimize plotCharWithColor
short terrainRandomValues[DCOLS][DROWS][8];
short** safetyMap;								// used to help monsters flee
short** allySafetyMap;							// used to help allies flee
short** chokeMap;								// used to assess the importance of the map's various chokepoints
const short nbDirs[8][2] = { {0,-1}, {0,1}, {-1,0}, {1,0}, {-1,-1}, {-1,1}, {1,-1}, {1,1} };
const short cDirs[8][2] = { {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1} };
short numberOfWaypoints;
levelData* levels;
creature player;
playerCharacter rogue;
creature* monsters;
creature* dormantMonsters;
creature* graveyard;
creature* purgatory;
item* floorItems;
item* packItems;
item* monsterItemsHopper;

char displayedMessage[MESSAGE_LINES][COLS * 2];
boolean messageConfirmed[MESSAGE_LINES];
char combatText[COLS * 2];
short messageArchivePosition;
char messageArchive[MESSAGE_ARCHIVE_LINES][COLS * 2];

char currentFilePath[BROGUE_FILENAME_MAX];

char displayDetail[DCOLS][DROWS];		// used to make certain per-cell data accessible to external code (e.g. terminal adaptations)

#ifdef AUDIT_RNG
FILE* RNGLogFile;
#endif

unsigned char inputRecordBuffer[INPUT_RECORD_BUFFER + 100];
unsigned short locationInRecordingBuffer;
unsigned long randomNumbersGenerated;
unsigned long positionInPlaybackFile;
unsigned long lengthOfPlaybackFile;
unsigned long recordingLocation;
unsigned long maxLevelChanges;
char annotationPathname[BROGUE_FILENAME_MAX];	// pathname of annotation file
unsigned long previousGameSeed;

// Console / Game related declarations
playerCharacter rogue;
brogueConsole currentConsole;

boolean serverMode = false;
boolean noMenu = false;
unsigned long int firstSeed = 0;

