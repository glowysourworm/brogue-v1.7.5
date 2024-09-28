#include "gameConsole.h"
#include "broguedef.h"
#include "gameController.h"
#include "stringExtension.h"
#include "exceptionHandler.h"
#include "broguefile.h"
#include <format>

using namespace std;

using namespace brogueHd::backend::controller;
using namespace brogueHd::component;

namespace brogueHd::console
{
	gameConsole::gameConsole() {}
	gameConsole::gameConsole(resourceController* resourceController) 
	{
		_resourceController = resourceController;
		_gameController = new gameController();
	}
	gameConsole::~gameConsole() 
	{
		delete _gameController;
	}

	void gameConsole::printHelp(ostream& stream)
	{
		stream << "--help         -h          print this help message" << std::endl;
		stream << "--version      -V          print the version (i.e., " + std::string(this->BrogueVersion) + ")" << std::endl;
		stream << "--scores                   dump scores to output and exit immediately" << std::endl;
		stream << "-n                         start a new game, skipping the menu" << std::endl;
		stream << "-s seed                    start a new game with the specified numerical seed" << std::endl;
		stream << "-o filename[.broguesave]   open a save file (extension optional)" << std::endl;
		stream << "-v recording[.broguerec]   view a recording (extension optional)" << std::endl;
	}
	void gameConsole::printScores(ostream& stream)
	{
		short mostRecent = 0;

		brogueScoresFile* scoreFile = _resourceController->getHighScores(mostRecent);

		for (int i = 0; i < scoreFile->scoreCount; i++)
		{
			// Convert file time to readable string
			std::string dateStr = stringExtension::formatDate(scoreFile->scoreList[i].dateTime);

			// Output to stream (console)
			stream << std::format("{}\t{}\t{}", (int)scoreFile->scoreList[i].score, dateStr, scoreFile->scoreList[i].description) << std::endl;
		}

		delete scoreFile;
	}

	brogueConsoleReturn gameConsole::command(std::string input, ostream& stream)
	{
		std::string* cmd = stringExtension::split(input, " ");

		std::string gamePath = "";
		std::string viewPath = "";
		unsigned long gameSeed = 0;

		bool noMenu = false;
		bool serverMode = false;

		brogueEvent theEvent;
		char path[4096], buf[100], seedDefault[100];
		char maxSeed[40];

		// Initialize Game Controller (some arguments are sent to controller)
		//
		_gameController->loadKeymap();

		// Dump Scores
		if (hasArgument(cmd, "--scores"))
		{
			printScores(stream);
			return brogueConsoleReturn::Completed;
		}

		// Version
		if (hasArgument(cmd, "--version") || hasArgument(cmd, "-V"))
		{
			stream << this->BrogueVersion << std::endl;
			return brogueConsoleReturn::Completed;
		}

		// Help
		if (hasArgument(cmd, "--help") || hasArgument(cmd, "-h") || hasArgument(cmd, "-?"))
		{
			printHelp(stream);
			return brogueConsoleReturn::Completed;
		}

		// New Game
		if (hasArgument(cmd, "-n"))
		{
			_gameController->initNewGame(0);
			_gameController->setMode(BrogueGameMode::Game);
		}

		// New Game w/ Seed
		if (hasArgument(cmd, "--seed") || hasArgument(cmd, "-s"))
		{
			unsigned long seed = getArgumentInt(cmd, "--seed");

			_gameController->initNewGame(seed);
			_gameController->setMode(BrogueGameMode::Game);
		}

		// No Menu
		if (hasArgument(cmd, "--no-menu") || hasArgument(cmd, "-M"))
		{
			noMenu = true;

			// TODO:
			return brogueConsoleReturn::CompletedWithError;
		}

		// Not Eye Hack
		if (hasArgument(cmd, "--noteye-hack"))
		{
			serverMode = true;

			// TODO:
			return brogueConsoleReturn::CompletedWithError;
		}

		// Open
		if (hasArgument(cmd, "--open") || hasArgument(cmd, "-o"))
		{
			gamePath = getArgument(cmd, "--open");

			if (gamePath == "")
				gamePath = getArgument(cmd, "-o");
			
			gameData* data = _resourceController->loadGame(gamePath.c_str());

			_gameController->initGame(data);
			_gameController->setMode(BrogueGameMode::Game);
		}

		// View
		if (hasArgument(cmd, "--view") || hasArgument(cmd, "-v"))
		{
			viewPath = getArgument<char*>(cmd, "--view");

			if (viewPath == NULL)
				viewPath = getArgument<char*>(cmd, "-v");

			_gameController->initPlayback(viewPath);
			_gameController->setMode(BrogueGameMode::Playback);
		}

		bool result = true;

		do
		{
			try
			{
				// Operates the current game mode loop to completion
				//
				result = _gameController->run();

				// Switch to next game mode
				//
				switch (_gameController->getMode())
				{
				case BrogueGameMode::Menu:
				case BrogueGameMode::MenuHighScores:
				case BrogueGameMode::MenuOpenGame:
				case BrogueGameMode::MenuSetSeed:
				case BrogueGameMode::Game:
				case BrogueGameMode::Playback:
				case BrogueGameMode::Scum:
				case BrogueGameMode::Quit:
				default:

					// Currently nothing to do until there is more feedback from
					// game components. This will be apparent when completed.

					break;
				}
			}
			catch (std::exception& ex)
			{
				// Primary Exception Handler. Should show a popup if there is an error.
				//
				brogueException::show(std::string(std::string("Brogue Error:  ") + ex.what()).c_str());
			}


		} while (result == true);

		// MEMORY! 
		delete _gameController;

		return brogueConsoleReturn::Completed;
	}
}