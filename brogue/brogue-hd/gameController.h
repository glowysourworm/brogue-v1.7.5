#pragma once

#include "gameData.h"
#include "command.h"
#include "keyProcessor.h"
#include "randomGenerator.h"
#include "playbackProcessor.h"

using namespace brogueHd::backend::generator;
using namespace brogueHd::backend::processor;
using namespace brogueHd::backend::model;

using namespace std;

namespace brogueHd::backend::controller
{
	class gameController
	{
	public:

		gameController();
		~gameController();

		/// <summary>
		/// Loads keymap file into the key processor
		/// </summary>
		void loadKeymap();

		void initPlayback(char* recordingPath);

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

		bool runMenu();
		bool runGame();
		bool runOpenGame();
		bool runHighScores();
		bool runSetSeed();
		bool runPlayback();
		bool runScum();

	private:

		randomGenerator* _randomMain;
		randomGenerator* _randomCosmetic;
		keyProcessor* _keyProcessor;
		playbackProcessor* _playbackProcessor;

		// File path for current game
		std::string _gamePath;

		gameData* _gameData;
	};
}

