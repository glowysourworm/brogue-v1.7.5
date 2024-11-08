#pragma once

#include "brogueConsole.h"
#include "brogueGlobal.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "gameController.h"
#include "resourceController.h"
#include "simpleException.h"
#include "simpleString.h"
#include <chrono>
#include <exception>
#include <iosfwd>
#include <iostream>
#include <thread>

using namespace brogueHd::simple;
using namespace brogueHd::backend::controller;

namespace brogueHd::console
{
	class gameConsole : public brogueConsole
	{
	public:
		gameConsole();
		gameConsole(resourceController* resourceController);
		~gameConsole();

		brogueConsoleReturn command(simpleString input, std::ostream& stream);
		void printHelp(std::ostream& stream) override;

	private:

		void printScores(std::ostream& stream);

	private:

		resourceController* _resourceController;
		gameController* _gameController;
	};

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
		stream << "\t1) Start Game	   [options]" << std::endl;
		stream << "\t2) Help (more options help)" << std::endl;
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
		//short mostRecent = 0;

		//brogueScoresFile* scoreFile = _resourceController->getHighScores(mostRecent);

		//for (int i = 0; i < scoreFile->scoreCount; i++)
		//{
		//	// Convert file time to readable string
		//	const char* dateStr = simpleExt::formatDate(scoreFile->scoreList.get(i).dateTime, false).c_str();

		//	// Output to stream (console)
		//	stream << simpleExt::format("{}\t{}\t{}", scoreFile->scoreList.get(i).score, dateStr, scoreFile->scoreList.get(i).description) << std::endl;
		//}

		//delete scoreFile;
	}

	brogueConsoleReturn gameConsole::command(simpleString input, std::ostream& stream)
	{
		int choice = 0;

		if (!input.tryToInt(choice))
			return brogueConsoleReturn::CompletedWithError;

		int intervalMilliseconds = 30;
		bool run = true;

		// Initialize:  Primary call to init OpenGL Rendering
		//
		_gameController->initialize();
		_gameController->setMode(BrogueGameMode::Title);

		// Operates the current game mode loop to completion
		//
		while (run)
		{
			try
			{
				// Run the game, in its current mode, for one cycle (rendering thread separate)
				//
				run = _gameController->run();

				// Allow a short thread sleep for the rendering thread
				//
				std::this_thread::sleep_for(std::chrono::milliseconds(intervalMilliseconds));
			}
			catch (std::exception& ex)
			{
				// Primary Exception Handler. Should show a popup if there is an error.
				//
				throw simpleException("Brogue Program Error ~ Please see error log for details");
			}

		}

		_gameController->closeGame();

		return brogueConsoleReturn::Completed;
	}
}