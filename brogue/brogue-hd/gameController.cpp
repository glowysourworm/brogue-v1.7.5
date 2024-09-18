#include "gameController.h"
#include "gameData.h"
#include "keyProcessor.h"
#include "randomGenerator.h"
#include "broguefile.h"
#include "fileio.h"
#include "randomGenerator.h"
#include <time.h>
#include <fstream>

using namespace std;

namespace brogueHd
{
	gameController::gameController()
	{
		_gameData = NULL;
		_keyProcessor = new keyProcessor();
		_playbackProcessor = new playbackProcessor();
		_randomMain = new randomGenerator(RANDOM_GENERATOR_MAIN);
		_randomCosmetic = new randomGenerator(RANDOM_GENERATOR_COSMETIC);
	}

	gameController::~gameController()
	{
		if (_gameData != NULL)
		{
			delete _gameData;

			_gameData = NULL;
		}

		delete _keyProcessor;
		delete _randomMain;
		delete _randomCosmetic;
		delete _playbackProcessor;
	}

	brogueScoresFile* gameController::getHighScores(short& mostRecentLineNumber)
	{	
		try
		{
			brogueScoresFile* scoresFile;
			std::fstream stream;

			stream.open("BrogueHighScores.txt", fstream::in);


			// Initialize Scores (EMPTY FILE)
			if (!stream.good()) 
			{
				stream.open("BrogueHighScores.txt", fstream::out);

				// Initialize
				scoresFile = new brogueScoresFile();

				for (int index = 0; index < HIGH_SCORES_COUNT; index++)
				{
					char description[COLS] = "Died to the great penderprime...";

					scoresFile->add(brogueScoreEntry(10000, time(0), description));
				}

				// Create empty file
				scoresFile->serialize(stream);

				stream.close();

				return scoresFile;
			}

			// Read Scores
			scoresFile = brogueScoresFile::deserialize(stream);

			// Close the file
			stream.close();

			// Sort Scores (TODO?)

			// Most Recent (TODO?)

			return scoresFile;
		}
		catch (std::exception& ex)
		{
			// TODO: Complete exception handling process to foward complete diagnostic message
			printf("Failed to read high scores file.");

			throw;
		}
	}

	void gameController::loadKeymap()
	{
		try
		{
			FILE* file = fopen("keymap", "r");
			char buffer[512];

			if (file == NULL)
			{
				// Output Default Keymap

				// Close / Reopen

				// TODO
				throw;
			}

			while (fgets(buffer, 512, file) != NULL)
			{
				// split it in two (destructively)
				int mode = 1;
				char* input_name = NULL;
				char* output_name = NULL;

				for (int i = 0; buffer[i]; i++)
				{
					if (isspace(buffer[i]))
					{
						buffer[i] = '\0';
						mode = 1;
					}
					else
					{
						if (mode)
						{
							if (input_name == NULL)
								input_name = buffer + i;

							else if (output_name == NULL)
								output_name = buffer + i;
						}
						mode = 0;
					}
				}
				if (input_name != NULL && output_name != NULL)
				{
					// Comment
					if (input_name[0] == '#')
						continue;

					_keyProcessor->addKeyMap(input_name, output_name);
				}
			}

			fclose(file);
		}
		catch (std::exception& ex)
		{
			throw std::runtime_error(std::string("gameController::loadKeyMap:  ") + ex.what());
		}
	}

	void gameController::setMode(BrogueGameMode gameMode)
	{
		switch (gameMode)
		{
		case brogueHd::Menu:
			break;
		case brogueHd::MenuHighScores:
			break;
		case brogueHd::MenuOpenGame:
			break;
		case brogueHd::MenuSetSeed:
			break;
		case brogueHd::Game:
			break;
		case brogueHd::Playback:
			break;
		case brogueHd::Scum:
			break;
		case brogueHd::Quit:
			break;
		default:
			break;
		}



		//if (SDL_Init(SDL_INIT_VIDEO)) 
		//{
		//	printf("Could not start SDL.\n");
		//	return;
		//}

		//loadFont(true);
		//rogueMain();

		//TCOD_console_delete(NULL);

		rogueEvent theEvent;
		char path[BROGUE_FILENAME_MAX], buf[100], seedDefault[100];
		char maxSeed[40];
		short i, j, k;
		boolean seedTooBig;

		// (RENDERING CODE)
		// 
		/*
		// clear screen and display buffer
		for (i = 0; i < COLS; i++)
		{
			for (j = 0; j < ROWS; j++)
			{
				displayBuffer[i][j].character = 0;
				displayBuffer[i][j].needsUpdate = false;
				displayBuffer[i][j].opacity = 100;

				for (k = 0; k < 3; k++)
				{
					displayBuffer[i][j].foreColorComponents[k] = 0;
					displayBuffer[i][j].backColorComponents[k] = 0;
				}
				plotCharWithColor(' ', i, j, &black, &black);
			}
		}*/



	}

	void gameController::initNewGame()
	{

	}

	void gameController::initGame(char* gamePath, unsigned long nextGameSeed)
	{

	}
	
	void gameController::initOpenGame(char* gamePath)
	{
		// TODO:  GAME SEED (???)

		// Reset Game Data
		if (_gameData != NULL)
		{
			delete _gameData;
		}

		_gameData = new gameData();

		fileio::getAvailableFilePath(gamePath, LAST_GAME_NAME, GAME_SUFFIX);

		// Add game suffix
		strcat(gamePath, GAME_SUFFIX);

		// Copy data to game data
		strcpy(_gameData->currentFilePath, gamePath);

		//if (rogue.nextGamePath[0])
		//{
		//	strcpy(path, rogue.nextGamePath);
		//	rogue.nextGamePath[0] = '\0';
		//}
		//else {
		//	dialogChooseFile(path, GAME_SUFFIX, "Open saved game:");
		//	//chooseFile(path, "Open saved game: ", "Saved game", GAME_SUFFIX);
		//}

		//if (openFile(path)) {
		//	loadSavedGame();
		//	mainInputLoop();
		//	freeEverything();
		//}
		//else {
		//	//dialogAlert("File not found.");
		//}
		//rogue.playbackMode = false;
		//rogue.playbackOOS = false;
	}

	void gameController::initPlayback(char* recordingPath)
	{
		if (recordingPath == NULL)
			throw std::runtime_error("Recording path not specified");

		try
		{
			_playbackProcessor->initialize(recordingPath);
		}
		catch (std::exception ex)
		{
			throw ex;
		}

		//	// Procedure
		//	//
		//	// 1) Load BrogueEvent list (keystrokes)
		//	// 2) Prepare display data in advance for processing
		//	//
		//	

		//	randomNumbersGenerated = 0;
		//	rogue.playbackMode = true;
		//	initializeRogue(0); // Seed argument is ignored because we're in playback.
		//	if (!rogue.gameHasEnded) {
		//		startLevel(rogue.depthLevel, 1);
		//		rogue.playbackPaused = true;
		//		displayAnnotation(); // in case there's an annotation for turn 0
		//	}

		//	while (!rogue.gameHasEnded && rogue.playbackMode) {
		//		if (rogue.playbackPaused) {
		//			rogue.playbackPaused = false;
		//			pausePlayback();
		//		}

		//		rogue.RNG = RNG_COSMETIC; // dancing terrain colors can't influence recordings
		//		rogue.playbackBetweenTurns = true;
		//		nextBrogueEvent(&theEvent, false, true, false);
		//		rogue.RNG = RNG_SUBSTANTIVE;

		//		executeEvent(&theEvent);
		//	}

		//	freeEverything();
		//}

		////dialogChooseFile(path, RECORDING_SUFFIX, "View recording:");
		////chooseFile(path, "View recording: ", "Recording", RECORDING_SUFFIX);

		//if (openFile(path)) 
		//{

		//}
		//else 
		//{
		//	// announce file not found
		//}
		////rogue.playbackMode = false;
		////rogue.playbackOOS = false;
	}

	void gameController::initGame(char* gamePath, unsigned long gameSeed)
	{
		_randomMain->reset(gameSeed);

		// Reset Game Data
		if (_gameData != NULL)
		{
			delete _gameData;
		}

		_gameData = new gameData();

		fileio::getAvailableFilePath(gamePath, LAST_GAME_NAME, GAME_SUFFIX);

		// Add game suffix
		strcat(gamePath, GAME_SUFFIX);

		// Copy data to game data
		strcpy(_gameData->currentFilePath, gamePath);

		//initializeRogue(rogue.nextGameSeed);
		//startLevel(rogue.depthLevel, 1); // descending into level 1

		//mainInputLoop();
		//freeEverything();
	}

	boolean gameController::runMenu()
	{

	}

	boolean gameController::runGame()
	{

	}

	boolean gameController::runSetSeed()
	{

	}

	boolean gameController::runOpenGame()
	{
		//if (rogue.nextGamePath[0]) 
		//{
		//	strcpy(path, rogue.nextGamePath);
		//	rogue.nextGamePath[0] = '\0';
		//}
		//else {
		//	dialogChooseFile(path, GAME_SUFFIX, "Open saved game:");
		//	//chooseFile(path, "Open saved game: ", "Saved game", GAME_SUFFIX);
		//}

		//if (openFile(path)) {
		//	loadSavedGame();
		//	mainInputLoop();
		//	freeEverything();
		//}
		//else {
		//	//dialogAlert("File not found.");
		//}
		//rogue.playbackMode = false;
		//rogue.playbackOOS = false;
	}

	boolean gameController::runPlayback()
	{
		//rogue.nextGame = NG_NOTHING;

		//path[0] = '\0';
		//if (rogue.nextGamePath[0]) {
		//	strcpy(path, rogue.nextGamePath);
		//	rogue.nextGamePath[0] = '\0';
		//}
		//else {
		//	dialogChooseFile(path, RECORDING_SUFFIX, "View recording:");
		//	//chooseFile(path, "View recording: ", "Recording", RECORDING_SUFFIX);
		//}

		//if (openFile(path)) {
		//	randomNumbersGenerated = 0;
		//	rogue.playbackMode = true;
		//	initializeRogue(0); // Seed argument is ignored because we're in playback.
		//	if (!rogue.gameHasEnded) {
		//		startLevel(rogue.depthLevel, 1);
		//		rogue.playbackPaused = true;
		//		displayAnnotation(); // in case there's an annotation for turn 0
		//	}

		//	while (!rogue.gameHasEnded && rogue.playbackMode) {
		//		if (rogue.playbackPaused) {
		//			rogue.playbackPaused = false;
		//			pausePlayback();
		//		}

		//		rogue.RNG = RNG_COSMETIC; // dancing terrain colors can't influence recordings
		//		rogue.playbackBetweenTurns = true;
		//		nextBrogueEvent(&theEvent, false, true, false);
		//		rogue.RNG = RNG_SUBSTANTIVE;

		//		executeEvent(&theEvent);
		//	}

		//	freeEverything();
		//}
		//else {
		//	// announce file not found
		//}
		//rogue.playbackMode = false;
		//rogue.playbackOOS = false;
	}

	boolean gameController::runHighScores()
	{
		//rogue.nextGame = NG_NOTHING;
		//printHighScores(false);
	}

	boolean gameController::runScum()
	{
		//rogue.nextGame = NG_NOTHING;
		//scum(1, 1000, 5);
	}
}