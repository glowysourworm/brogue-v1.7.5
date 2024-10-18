#pragma once

#include "gameData.h"
#include "brogueGlobal.h"
#include "keyProcessor.h"
#include "randomGenerator.h"
#include "playbackProcessor.h"
#include "renderingController.h"
#include "simpleString.h"

using namespace brogueHd::simple;
using namespace brogueHd::backend::generator;
using namespace brogueHd::backend::processor;
using namespace brogueHd::backend::model;

namespace brogueHd::backend::controller
{
	class gameController
	{
	public:

		gameController(resourceController* resourceController);
		~gameController();

		/// <summary>
		/// Loads keymap file into the key processor
		/// </summary>
		void loadKeymap();

		void initPlayback(const char* recordingPath);

		/// <summary>
		/// Initializes a new game with random seed, and default file name
		/// </summary>
		void initNewGame(unsigned long seed);

		/// <summary>
		/// Initializes game from game data (from file)
		/// </summary>
		void initGame(gameData* data);

		/// <summary>
		/// Returns current mode of controller. This is part of a state machine involving
		/// the run() method. 
		/// </summary>
		BrogueGameMode getMode();

		/// <summary>
		/// Sets the mode of the controller - prepping the game variables. This should be done
		/// after the user is made aware of the change; or the game is in the appropriate state.
		/// </summary>
		void setMode(BrogueGameMode mode);

		/// <summary>
		/// Executes game controller in the current mode:  1) takes user commands, 2) renders
		/// video output for the current mode.
		/// </summary>
		bool run();

		/// <summary>
		/// Deletes game data and resets the controller
		/// </summary>
		void closeGame();

	private:

	private:

		bool runMenu();
		bool runGame();
		bool runOpenGame();
		bool runHighScores();
		bool runSetSeed();
		bool runPlayback();
		bool runScum();

	private:

		renderingController* _renderingController;
		BrogueGameMode _gameMode;	

		randomGenerator* _randomMain;
		randomGenerator* _randomCosmetic;
		keyProcessor* _keyProcessor;
		playbackProcessor* _playbackProcessor;

		// File path for current game
		simpleString _gamePath;

		gameData* _gameData;
	};


	gameController::gameController(resourceController* resourceController)
	{
		_gameData = nullptr;
		_keyProcessor = new keyProcessor();
		_playbackProcessor = new playbackProcessor();
		_randomMain = new randomGenerator(RANDOM_GENERATOR_MAIN);
		_randomCosmetic = new randomGenerator(RANDOM_GENERATOR_COSMETIC);
		_renderingController = new renderingController(resourceController, _randomCosmetic);
	}

	gameController::~gameController()
	{
		if (_gameData != nullptr)
		{
			delete _gameData;

			_gameData = nullptr;
		}

		delete _keyProcessor;
		delete _randomMain;
		delete _randomCosmetic;
		delete _playbackProcessor;
		delete _renderingController;
	}

	void gameController::loadKeymap()
	{

	}

	void gameController::setMode(BrogueGameMode gameMode)
	{
		_gameMode = gameMode;

		_renderingController->setViewMode(gameMode);

		//if (SDL_Init(SDL_INIT_VIDEO)) 
		//{
		//	printf("Could not start SDL.\n");
		//	return;
		//}

		//loadFont(true);
		//rogueMain();

		//TCOD_console_delete(NULL);

		//rogueEvent theEvent;
		//char path[BROGUE_FILENAME_MAX], buf[100], seedDefault[100];
		//char maxSeed[40];
		//short i, j, k;
		//boolean seedTooBig;

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

	void gameController::closeGame()
	{
		//if (_gameData != NULL)
		//{ 
		//	delete _gameData;
		//	_gameData = NULL;
		//}

		//gameController::closeWindow();
	}

	void gameController::initNewGame(unsigned long seed)
	{
		//if (_gameData != NULL)
		//	simpleException::show("Trying to initialize game while a current one is loaded:  call closeGame() first");

		//_randomMain->reset(seed);

		//// Reset Game Data
		//if (_gameData != NULL)
		//{
		//	delete _gameData;
		//}

		//_gameData = new gameData();

		//gameController::openWindow();
	}

	void gameController::initGame(gameData* data)
	{
		if (_gameData != NULL)
			simpleException::showCstr("Trying to initialize game while a current one is loaded:  call closeGame() first");

		// unsigned long gameSeed (GET FROM GAME DATA)

		_randomMain->reset(0);

		// Reset Game Data
		if (_gameData != NULL)
		{
			delete _gameData;
		}

		_gameData = data;

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

	void gameController::initPlayback(const char* recordingPath)
	{
		if (_gameData != NULL)
			simpleException::showCstr("Trying to initialize playback while a current one is loaded:  call closeGame() first");

		if (recordingPath == NULL)
			simpleException::showCstr("Recording path not specified");

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

	bool gameController::runMenu()
	{
		return false;
	}

	bool gameController::runGame()
	{
		return false;
	}

	bool gameController::runSetSeed()
	{
		return false;
	}

	bool gameController::runOpenGame()
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

		return false;
	}

	bool gameController::runPlayback()
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

		return false;
	}

	BrogueGameMode gameController::getMode()
	{
		return BrogueGameMode::Game;
	}

	bool gameController::run()
	{
		switch (_gameMode)
		{
		case BrogueGameMode::Menu:
			break;
		case BrogueGameMode::MenuHighScores:
			break;
		case BrogueGameMode::MenuOpenGame:
			break;
		case BrogueGameMode::MenuSetSeed:
			break;
		case BrogueGameMode::Game:
			break;
		case BrogueGameMode::Playback:
			break;
		case BrogueGameMode::Scum:
			break;
		case BrogueGameMode::Quit:
			break;
		default:
			break;
		}

		return true;
	}

	bool gameController::runHighScores()
	{
		//rogue.nextGame = NG_NOTHING;
		//printHighScores(false);

		return false;
	}

	bool gameController::runScum()
	{
		//rogue.nextGame = NG_NOTHING;
		//scum(1, 1000, 5);

		return false;
	}
}

