#pragma once

#include "resourceController.h"
#include "exceptionHandler.h"
#include "playbackData.h"
#include "gameData.h"
#include "stringExtension.h"
#include "brogueColorMap.h"
#include "typeConverter.h"
#include <string>
#include <fstream>
#include <format>

using namespace std;
using namespace brogueHd::component;
using namespace brogueHd::backend::model;
using namespace brogueHd::backend::model::game;
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

				//for (int index = 0; index < HIGH_SCORES_COUNT; index++)
				//{
				//	char description[COLS] = "Died to the great penderprime...";

				//	scoresFile->add(brogueScoreEntry(10000, time(0), description));
				//}

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
			FILE** file;
			fopen_s(file, "keymap", "r");

			char buffer[512];

			if (file == NULL)
			{
				// Output Default Keymap

				// Close / Reopen

				// TODO
				throw;
			}

			while (fgets(buffer, 512, *file) != NULL)
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

			fclose(*file);
		}
		catch (std::exception& ex)
		{
			brogueException::show(std::string("gameController::loadKeyMap:  ") + ex.what());
			throw ex;
		}
	}

	brogueColorMap* resourceController::loadColors(const char* path)
	{
		brogueColorMap* result = new brogueColorMap();

		try
		{
			ifstream stream(path);

			// collection,name,red,green,blue,redRand,greenRand,blueRand,rand,colorDances

			
			std::string nextLine;

			while (std::getline(stream, nextLine))
			{
				// Comment
				if (nextLine.starts_with('#'))
					continue;

				// Split by comma and white space
				std::string* strings = stringExtension::split(nextLine, ", ");

				if (SIZEOF(strings) != 10)
					continue;

				color nextColor;
				colorCollections collection;
				
				EnumString<colorCollections>::To(collection, strings[0]);

				nextColor.red = typeConverter::stringToshort(strings[2]);
				nextColor.green = typeConverter::stringToshort(strings[3]);
				nextColor.blue = typeConverter::stringToshort(strings[4]);
				nextColor.redRand = typeConverter::stringToshort(strings[5]);
				nextColor.greenRand = typeConverter::stringToshort(strings[6]);
				nextColor.blueRand = typeConverter::stringToshort(strings[7]);
				nextColor.rand = typeConverter::stringToshort(strings[8]);
				nextColor.colorDances = typeConverter::stringToBool(strings[9]);

				switch (collection)
				{
				case colorCollections::game:
					gameColors gameSelector;
					EnumString<gameColors>::To(gameSelector, strings[1]);
					result->setColor(gameSelector, nextColor);
					break;
				case colorCollections::bolt:
					boltColors boltSelector;
					EnumString<boltColors>::To(boltSelector, strings[1]);
					result->setColor(boltSelector, nextColor);
					break;
				case colorCollections::tile:
					tileColors tileSelector;
					EnumString<tileColors>::To(tileSelector, strings[1]);
					result->setColor(tileSelector, nextColor);
					break;
				case colorCollections::creature:
					creatureColors creatureSelector;
					EnumString<creatureColors>::To(creatureSelector, strings[1]);
					result->setColor(creatureSelector, nextColor);
					break;
				case colorCollections::light:
					lightColors lightSelector;
					EnumString<lightColors>::To(lightSelector, strings[1]);
					result->setColor(lightSelector, nextColor);
					break;
				case colorCollections::flare:
					flareColors flareSelector;
					EnumString<flareColors>::To(flareSelector, strings[1]);
					result->setColor(flareSelector, nextColor);
					break;
				case colorCollections::multipliers:
					colorMultipliers multiplierSelector;
					EnumString<colorMultipliers>::To(multiplierSelector, strings[1]);
					result->setColor(multiplierSelector, nextColor);
					break;
				case colorCollections::blood:
					bloodColors bloodSelector;
					EnumString<bloodColors>::To(bloodSelector, strings[1]);
					result->setColor(bloodSelector, nextColor);
					break;
				case colorCollections::gas:
					gasColors gasSelector;
					EnumString<gasColors>::To(gasSelector, strings[1]);
					result->setColor(gasSelector, nextColor);
					break;
				case colorCollections::interface:
					interfaceColors interfaceSelector;
					EnumString<interfaceColors>::To(interfaceSelector, strings[1]);
					result->setColor(interfaceSelector, nextColor);
					break;
				default:
					break;
				}
			} 

			return result;
		}
		catch (std::exception ex)
		{
			brogueException::show(std::format("Error opening colors.csv:  {}", ex.what()));

			delete result;
		}

		return NULL;
	}

	gameData* loadGame(const char* path)
	{
		return NULL;
	}

	playbackData* loadPlayback(const char* path)
	{
		return NULL;
	}
}