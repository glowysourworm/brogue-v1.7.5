#pragma once

#include "brogueGame.h"
#include "brogueGameTemplate.h"
#include "brogueGlobal.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIBuilder.h"
#include "eventController.h"
#include "gameGenerator.h"
#include "gridRect.h"
#include "keyProcessor.h"
#include "noiseGenerator.h"
#include "playbackProcessor.h"
#include "randomGenerator.h"
#include "renderingController.h"
#include "resourceController.h"
#include "simpleException.h"
#include "simpleString.h"
#include <exception>

using namespace brogueHd::simple;

namespace brogueHd::backend
{
	class gameController
	{
	public:

		gameController(resourceController* resourceController);
		~gameController();

		/// <summary>
		/// Initializes backend components; and starts the rendering thread.
		/// </summary>
		void initialize();

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
		void initGame(brogueGame* data);

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
		resourceController* _resourceController;
		eventController* _eventController;
		BrogueGameMode _gameMode;

		randomGenerator* _randomMain;
		randomGenerator* _randomCosmetic;
		noiseGenerator* _noiseGenerator;
		keyProcessor* _keyProcessor;
		playbackProcessor* _playbackProcessor;

		// File path for current game
		simpleString _gamePath;

		// Primary game data set
		brogueGame* _game;
	};


	gameController::gameController(resourceController* resourceController)
	{
		_game = nullptr;
		_keyProcessor = new keyProcessor();
		_playbackProcessor = new playbackProcessor();
		_randomMain = new randomGenerator(RANDOM_GENERATOR_MAIN);
		_randomCosmetic = new randomGenerator(RANDOM_GENERATOR_COSMETIC);
		_noiseGenerator = new noiseGenerator(_randomMain);
		_eventController = new eventController();
		_resourceController = resourceController;
		_renderingController = new renderingController(_eventController, resourceController, _randomCosmetic);
		_gameMode = BrogueGameMode::Title;
	}

	gameController::~gameController()
	{
		if (_game != nullptr)
		{
			delete _game;

			_game = nullptr;
		}

		delete _keyProcessor;
		delete _randomMain;
		delete _randomCosmetic;
		delete _playbackProcessor;
		delete _renderingController;
		delete _eventController;
	}

	void gameController::initialize()
	{
		_renderingController->initialize();
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
		if (_game != nullptr)
			throw simpleException("Trying to initialize game while a current one is loaded:  gameController.h");

		// Reseed Random Main
		_randomMain->reset(seed);

		brogueGameTemplate* gameTemplate = _resourceController->getBrogueDesign_v1_7_5();
		brogueUIBuilder uiBuilder(_eventController, _resourceController, _randomMain, 0);
		gridRect boundary = uiBuilder.getBrogueSceneBoundary();
		gameGenerator generator(&uiBuilder, _randomMain, _noiseGenerator, boundary);

		_game = generator.createGame(seed, gameTemplate);
	}

	void gameController::initGame(brogueGame* data)
	{
		if (_game != nullptr)
			throw simpleException("Trying to initialize game while a current one is loaded:  call closeGame() first");

		// unsigned long gameSeed (GET FROM GAME DATA) (??) (Deterministic?)

		_randomMain->reset(0);
		_game = data;
	}

	void gameController::initPlayback(const char* recordingPath)
	{
		if (_game != nullptr)
			throw simpleException("Trying to initialize playback while a current one is loaded:  call closeGame() first");

		if (recordingPath == NULL)
			throw simpleException("Recording path not specified");

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
		bool newGame;
		bool openGame;
		simpleString fileName;
		BrogueGameMode requestedMode = _renderingController->getGameModeRequest(newGame, openGame, fileName);

		if (newGame)
		{
			initNewGame(12334);
		}
		else if (openGame)
		{
			initNewGame(12334); // TODO
		}

		// Game Mode Change
		if (requestedMode != _gameMode)
		{
			// Might need to check on environment first (for now, just keep this here to manage the rendering thread)
			setMode(requestedMode);
			return true;			// Allow a cycle to initialize mode
		}

		// RUN THE GAME:	Apply keyboard / mouse to the game backend processors

		// Return Control
		switch (_gameMode)
		{
			case BrogueGameMode::Game:
			{
				// Check for game updates
				if (_game->getLevel(1)->needsUpdate())
				{
					// Update rendering thread
					_renderingController->updateGameData(_game->getLevel(1));

					// Clear update flags
					_game->getLevel(1)->clearUpdate();
				}
				
				return true;
			}
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

