#pragma once

#include "gameData.h"
#include "command.h"
#include "keyProcessor.h"
#include "broguefile.h"
#include "broguedef.h"
#include "randomGenerator.h"
#include "playbackProcessor.h"

using namespace std;

namespace brogueHd
{
	class gameController
	{
	public:

		gameController();
		~gameController();

		void createGame();

		/// <summary>
		/// Gets array of high score entries from file
		/// </summary>
		brogueScoresFile* getHighScores(short& mostRecentLineNumber);

		/// <summary>
		/// Loads keymap file into the key processor
		/// </summary>
		void loadKeymap();

		/// <summary>
		/// Initializes a new game with random seed, and default file name
		/// </summary>
		void initNewGame();

		/// <summary>
		/// Initializes new game:  1) opens from game path, or 2) starts new game
		/// </summary>
		void initGame(char* gamePath, unsigned long nextGameSeed);

		/// <summary>
		/// Initializes game opened from file
		/// </summary>
		void initOpenGame(char* gamePath);

		/// <summary>
		/// Sets playback data from the path - which will read and load from file
		/// </summary>
		void initPlayback(char* recordingPath);

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
		boolean run();

	private:

		boolean runMenu();
		boolean runGame();
		boolean runOpenGame();
		boolean runHighScores();
		boolean runSetSeed();
		boolean runPlayback();
		boolean runScum();

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

