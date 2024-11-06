#pragma once

#include "brogueGlobal.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "eventController.h"
#include "gameData.h"
#include "keyProcessor.h"
#include "playbackProcessor.h"
#include "randomGenerator.h"
#include "renderingController.h"
#include "resourceController.h"
#include "simpleException.h"
#include "simpleString.h"
#include <exception>

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
		/// Returns current mouse state maintained by rendering thread (GLFW / OpenGL)
		/// </summary>
		brogueMouseState getMouse();

		/// <summary>
		/// Returns current keyboard state maintained by rendering thread (GLFW / OpenGL)
		/// </summary>
		brogueKeyboardState getKeyboard();

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
		eventController* _eventController;
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
		_eventController = new eventController();
		_renderingController = new renderingController(_eventController, resourceController, _randomCosmetic);
		_gameMode = BrogueGameMode::Title;
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
		delete _eventController;
	}

	void gameController::loadKeymap()
	{

	}

	void gameController::setMode(BrogueGameMode gameMode)
	{
		_gameMode = gameMode;

		_renderingController->setGameMode(gameMode);
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

		return false;
	}

	bool gameController::runPlayback()
	{

		return false;
	}

	BrogueGameMode gameController::getMode()
	{
		// Forwards game mode request input from the rendering game thread
		return _renderingController->getGameModeRequest();
	}

	brogueKeyboardState gameController::getKeyboard()
	{
		return _renderingController->getKeyboardState();
	}

	brogueMouseState gameController::getMouse()
	{
		return _renderingController->getMouseState();
	}

	bool gameController::run()
	{
		// Get the current keyboard / mouse state from OpenGL
		//
		brogueKeyboardState keyboard = getKeyboard();
		brogueMouseState mouse = getMouse();

		// Check the game mode from the rendering thread
		//
		BrogueGameMode requestedMode = _renderingController->getGameModeRequest();

		// Game Mode Change
		if (requestedMode != _gameMode)
		{
			// Might need to check on environment first (for now, just keep this here to manage the rendering thread)
			setMode(requestedMode);
		}

		// RUN THE GAME:	Apply keyboard / mouse to the game backend processors

		// Return Control
		switch (_gameMode)
		{
			case BrogueGameMode::Game:
			case BrogueGameMode::Playback:
			case BrogueGameMode::Title:
				return true;

			case BrogueGameMode::Exit:
			default:
				return false;
		}
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

