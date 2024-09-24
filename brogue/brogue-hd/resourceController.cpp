#pragma once

#include "resourceController.h"
#include "exceptionHandler.h"
#include "playbackData.h"
#include "gameData.h"
#include <string>
#include <fstream>

using namespace std;
using namespace brogueHd::backend::model;
using namespace brogueHd::backend::processor;

namespace brogueHd::backend::controller
{
	resourceController::resourceController(){}
	resourceController::~resourceController(){}

	brogueScoresFile* resourceController::getHighScores(short& mostRecentLineNumber)
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
			brogueException::show("Failed to read high scores file.");
			throw ex;
		}
	}

	void resourceController::loadKeymap(const keyProcessor& processor)
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

					processor.addKeyMap(input_name, output_name);
				}
			}

			fclose(file);
		}
		catch (std::exception& ex)
		{
			brogueException::show(std::string("gameController::loadKeyMap:  ") + ex.what());
			throw ex;
		}
	}

	gameData* loadGame(char* path)
	{
		return NULL;
	}

	playbackData* loadPlayback(char* path)
	{
		return NULL;
	}
}