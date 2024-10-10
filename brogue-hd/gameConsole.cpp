#include "gameConsole.h"
#include "brogueGlobal.h"
#include "simpleString.h"
#include "gameController.h"
#include "broguefile.h"
#include "simpleArray.h"
#include <format>
#include <thread>

using namespace brogueHd::backend::controller;
using namespace brogueHd::simple;

namespace brogueHd::console
{
	gameConsole::gameConsole() 
	{
		this->consoleName = "Brogue Game Console";
	}
	gameConsole::gameConsole(resourceController* resourceController) 
	{
		_resourceController = resourceController;
		_gameController = new gameController(resourceController);
		this->consoleName = "Brogue GAME";
	}
	gameConsole::~gameConsole() 
	{
		delete _gameController;
	}

	void gameConsole::printHelp(std::ostream& stream)
	{
		stream << "Brogue Game Console:" << std::endl << std::endl;
		stream << "\t1) New Game	   [options]" << std::endl;
		stream << "\t2) Open Game	   [options]" << std::endl;
		stream << "\t3) High Scores" << std::endl;
		stream << "\t4) Run Playback   [options]" << std::endl;
		stream << "\t5) Help (more options help)" << std::endl;
		stream << "\tElse) Exit" << std::endl;

		//stream << "--help         -h          print this help message" << std::endl;
		//stream << "--version      -V          print the version (i.e., " + simpleString(this->BrogueVersion) + ")" << std::endl;
		//stream << "--scores                   dump scores to output and exit immediately" << std::endl;
		//stream << "-n                         start a new game, skipping the menu" << std::endl;
		//stream << "-s seed                    start a new game with the specified numerical seed" << std::endl;
		//stream << "-o filename[.broguesave]   open a save file (extension optional)" << std::endl;
		//stream << "-v recording[.broguerec]   view a recording (extension optional)" << std::endl;
	}
	void gameConsole::printScores(std::ostream& stream)
	{
		short mostRecent = 0;

		brogueScoresFile* scoreFile = _resourceController->getHighScores(mostRecent);

		for (int i = 0; i < scoreFile->scoreCount; i++)
		{
			// Convert file time to readable string
			simpleString dateStr = simpleString::formatDate(scoreFile->scoreList.get(i).dateTime);

			// Output to stream (console)
			stream << std::format("{}\t{}\t{}", (int)scoreFile->scoreList.get(i).score, dateStr, scoreFile->scoreList.get(i).description) << std::endl;
		}

		delete scoreFile;
	}

	brogueConsoleReturn gameConsole::command(simpleString input, std::ostream& stream)
	{
		int choice = 0;

		if (!input.tryToInt(choice))
			return brogueConsoleReturn::CompletedWithError;

		//switch (choice)
		//{
		//case 1:
		//{
		//	stream << "New Game..." << std::endl;
		//	return brogueConsoleReturn::Completed;
		//}
		//break;
		//case 2:
		//{
		//	stream << "Open Game..." << std::endl;
		//	return brogueConsoleReturn::Completed;
		//}
		//break;
		//case 3:
		//{
		//	stream << "High Scores..." << std::endl;
		//	return brogueConsoleReturn::Completed;
		//}
		//break;
		//case 4:
		//{
		//	stream << "Run Playback..." << std::endl;
		//	return brogueConsoleReturn::Completed;
		//}
		//break;
		//case 5:
		//{
		//	stream << "Help (about options)..." << std::endl;
		//	return brogueConsoleReturn::Completed;
		//}
		//break;
		//default:
		//	return brogueConsoleReturn::Completed;
		//}

		//std::string* cmd = NULL;

		//std::string gamePath = "";
		//std::string viewPath = "";
		//unsigned long gameSeed = 0;

		//bool noMenu = false;
		//bool serverMode = false;

		//brogueEvent theEvent;
		//char path[4096], buf[100], seedDefault[100];
		//char maxSeed[40];

		//// Initialize Game Controller (some arguments are sent to controller)
		////
		//_gameController->loadKeymap();

		//// Dump Scores
		//if (hasArgument(cmd, "--scores"))
		//{
		//	printScores(stream);
		//	return brogueConsoleReturn::Completed;
		//}

		//// Version
		//if (hasArgument(cmd, "--version") || hasArgument(cmd, "-V"))
		//{
		//	stream << this->BrogueVersion << std::endl;
		//	return brogueConsoleReturn::Completed;
		//}

		//// Help
		//if (hasArgument(cmd, "--help") || hasArgument(cmd, "-h") || hasArgument(cmd, "-?"))
		//{
		//	printHelp(stream);
		//	return brogueConsoleReturn::Completed;
		//}

		//// New Game
		//if (hasArgument(cmd, "-n"))
		//{
		//	_gameController->initNewGame(0);
		//	_gameController->setMode(BrogueGameMode::Game);
		//}

		//// New Game w/ Seed
		//if (hasArgument(cmd, "--seed") || hasArgument(cmd, "-s"))
		//{
		//	unsigned long seed = getArgumentInt(cmd, "--seed");

		//	_gameController->initNewGame(seed);
		//	_gameController->setMode(BrogueGameMode::Game);
		//}

		//// No Menu
		//if (hasArgument(cmd, "--no-menu") || hasArgument(cmd, "-M"))
		//{
		//	noMenu = true;

		//	// TODO:
		//	return brogueConsoleReturn::CompletedWithError;
		//}

		//// Not Eye Hack
		//if (hasArgument(cmd, "--noteye-hack"))
		//{
		//	serverMode = true;

		//	// TODO:
		//	return brogueConsoleReturn::CompletedWithError;
		//}

		//// Open
		//if (hasArgument(cmd, "--open") || hasArgument(cmd, "-o"))
		//{
		//	gamePath = getArgument(cmd, "--open");

		//	if (gamePath == "")
		//		gamePath = getArgument(cmd, "-o");
		//	
		//	gameData* data = _resourceController->loadGame(gamePath.c_str());

		//	_gameController->initGame(data);
		//	_gameController->setMode(BrogueGameMode::Game);
		//}

		//// View
		//if (hasArgument(cmd, "--view") || hasArgument(cmd, "-v"))
		//{
		//	viewPath = getArgument(cmd, "--view");

		//	if (viewPath == "")
		//		viewPath = getArgument(cmd, "-v");

		//	_gameController->initPlayback(viewPath.c_str());
		//	_gameController->setMode(BrogueGameMode::Playback);
		//}

		bool result = true;

		// Prepare Game Window
		_gameController->initNewGame(1234);
		_gameController->setMode(BrogueGameMode::Game);

		// MAIN LOOP:  Leave 1ms thread sleep -> Grab key input -> send to game controller.
		//
		do
		{
			try
			{
				unsigned char key;

				if (std::cin >> key)
				{
					// Exit Game Loop
					if (key == 27)
						result = false;

					else
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

							_gameController->setMode(BrogueGameMode::Quit);

							// Currently nothing to do until there is more feedback from
							// game components. This will be apparent when completed.

							break;
						}
					}
				}
			}
			catch (std::exception& ex)
			{
				// Primary Exception Handler. Should show a popup if there is an error.
				//
				//simpleException::show(std::string(std::string("Brogue Error:  ") + ex.what()).c_str());
			}
		} while (result == true);

		_gameController->closeGame();

		// MEMORY! 
		delete _gameController;

		return brogueConsoleReturn::Completed;
	}
}