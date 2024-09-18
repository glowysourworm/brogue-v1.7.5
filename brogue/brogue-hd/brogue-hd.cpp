
#include "broguedef.h"
#include "gameController.h"
#include "fileio.h"
#include <random>

using namespace std;

namespace brogueHd
{
	std::string BrogueVersion = "1.7.5";

	static boolean endswith(const char* str, const char* ending)
	{
		int str_len = strlen(str), ending_len = strlen(ending);
		if (str_len < ending_len) return false;
		return strcmp(str + str_len - ending_len, ending) == 0 ? true : false;
	}

	static void append(char* str, const char* ending, int bufsize) {
		int str_len = strlen(str), ending_len = strlen(ending);
		if (str_len + ending_len + 1 > bufsize) return;
		strcpy(str + str_len, ending);
	}

	static void printCommandlineHelp() {
		printf("%s",
				"--help         -h          print this help message\n"
				"--version      -V          print the version (i.e., " + BrogueVersion + ")\n"
				"--scores                   dump scores to output and exit immediately\n"
				"-n                         start a new game, skipping the menu\n"
				"-s seed                    start a new game with the specified numerical seed\n"
				"-o filename[.broguesave]   open a save file (extension optional)\n"
				"-v recording[.broguerec]   view a recording (extension optional)\n"
		);
		return;
	}

	std::string dateToString(time_t time)
	{
		char buffer[80];
		std::strftime(buffer, 80, "%Y-%m-%d-%H:%M:%S", localtime(&time));
		std::string result(buffer);

		return result;
	}

	void dumpScores(gameController* controller) {
		
		short mostRecent = 0;

		brogueScoresFile* scoreFile = controller->getHighScores(mostRecent);

		for (int i = 0; i < scoreFile->scoreCount; i++)
		{
			// Convert file time to readable string
			std::string dateStr = dateToString(scoreFile->scoreList[i].dateTime);

			// Print output to console
			printf("%d\t%s\t%s\n", (int)scoreFile->scoreList[i].score, dateStr, scoreFile->scoreList[i].description);
		}

		delete scoreFile;
	}

	static boolean hasArgument(char* args[], const char* argument)
	{
		for (int index = 0; index < SIZEOF(args); index++)
		{
			if (STRCMP(args[index], argument) == 0)
				return true;
		}

		return false;
	}

	static char* getArgument(char* args[], const char* argument)
	{
		for (int index = 0; index < SIZEOF(args); index++)
		{
			if (STRCMP(args[index], argument) == 0 &&
				index < SIZEOF(args) + 1)
				return args[index + 1];
		}
		
		return NULL;
	}

	static void teardown(gameController* controller)
	{
		// TODO:  Shutdown procedure

		delete controller;
	}

	int main(int argc, char* argv[])
	{
		gameController* controller = new gameController();

		char* gamePath = NULL;
		char* viewPath = NULL;
		unsigned long gameSeed = 0;

		boolean noMenu = false;
		boolean serverMode = false;

		rogueEvent theEvent;
		char path[BROGUE_FILENAME_MAX], buf[100], seedDefault[100];
		char maxSeed[40];

		// Initialize Game Controller (some arguments are sent to controller)
		//
		controller->loadKeymap();

		// Dump Scores
		if (hasArgument(argv, "--scores"))
		{
			dumpScores(controller);
			teardown(controller);
			return 0;
		}

		// Version
		if (hasArgument(argv, "--version") || hasArgument(argv, "-V"))
		{
			printf("%s\n", BROGUE_VERSION_STRING);
			teardown(controller);
			return 0;
		}

		// Help
		if (hasArgument(argv, "--help") || hasArgument(argv, "-h") || hasArgument(argv, "-?"))
		{
			printCommandlineHelp();
			teardown(controller);
			return 0;
		}

		// New Game
		if (hasArgument(argv, "-n"))
		{
			controller->initNewGame();
			controller->setMode(BrogueGameMode::Game);
		}

		// New Game w/ Seed
		if (hasArgument(argv, "--seed") || hasArgument(argv, "-s"))
		{
			char* seedStr = getArgument(argv, "--seed");
			
			if (seedStr == NULL) 
				getArgument(argv, "-s");

			if (seedStr != NULL)
			{
				controller->initGame(gamePath, gameSeed);
				controller->setMode(BrogueGameMode::Game);
			}
		}

		// No Menu
		if (hasArgument(argv, "--no-menu") || hasArgument(argv, "-M"))
		{
			noMenu = true;

			// TODO:
			return 1;
		}

		// Not Eye Hack
		if (hasArgument(argv, "--noteye-hack")) 
		{
			serverMode = true;

			// TODO:
			return 1;
		}

		// Open
		if (hasArgument(argv, "--open") || hasArgument(argv, "-o"))
		{
			gamePath = getArgument(argv, "--open");
				
			if (gamePath == NULL)
				getArgument(argv, "-o");

			if (!endswith(gamePath, GAME_SUFFIX)) 
			{
				append(gamePath, GAME_SUFFIX, 4096);
			}

			controller->initOpenGame(gamePath);
			controller->setMode(BrogueGameMode::Game);
		}

		// View
		if (hasArgument(argv, "--view") || hasArgument(argv, "-v"))
		{
			viewPath = getArgument(argv, "--view"); 
			
			if (viewPath == NULL) 
				viewPath = getArgument(argv, "-v");

			if (!endswith(viewPath, RECORDING_SUFFIX)) 
			{
				append(viewPath, RECORDING_SUFFIX, 4096);
			}

			controller->initPlayback(viewPath);
			controller->setMode(BrogueGameMode::Playback);
		}

		boolean result = true;

		do
		{
			try
			{
				// Operates the current game mode loop to completion
				//
				result = controller->run();

				// Switch to next game mode
				//
				switch (controller->getMode())
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
				throw std::runtime_error(std::string("Brogue Error:  ") + ex.what());
			}


		} while (result == true);

		teardown(controller);

		return 0;
	}
}

