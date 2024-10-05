#pragma once

#include <Windows.h>
#include "resource.h"
#include "resourceController.h"
#include "playbackData.h"
#include "gameData.h"
#include "brogueColorMap.h"
#include "typeConverter.h"

#include "brogueGlobal.h"
#include "stringExtension.h"
#include "shaderData.h"

#include <string>
#include <fstream>
#include <format>

using namespace brogueHd::component;
using namespace brogueHd::frontend::opengl;
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

			stream.open("BrogueHighScores.txt", std::fstream::in);


			// Initialize Scores (EMPTY FILE)
			if (!stream.good())
			{
				stream.open("BrogueHighScores.txt", std::fstream::out);

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

		return NULL;
	}

	gameData* resourceController::loadGame(const char* path)
	{
		return NULL;
	}

	playbackData* resourceController::loadPlayback(const char* path)
	{
		return NULL;
	}

	void resourceController::loadKeymap(keyProcessor& processor)
	{
		try
		{
			std::ifstream stream("keymap", std::fstream::in);

			if (!stream.is_open())
			{
				brogueException::show("Error opening keymap file:  either missing, or locked");
				return;
			}

			std::string line;

			while (getline(stream, line))
			{
				simpleArray<std::string> linePieces = stringExtension::split(line, " ");

				if (sizeof(linePieces) != 2)
					brogueException::show("Invalid keymap file:  looking for two character split by only whitespace");

				processor.addKeyMap(linePieces.get(0).c_str(), linePieces.get(1).c_str());
			}

			stream.close();
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
			std::ifstream stream(path);

			// collection,name,red,green,blue,redRand,greenRand,blueRand,rand,colorDances

			
			std::string nextLine;

			while (std::getline(stream, nextLine))
			{
				// Comment
				if (nextLine.starts_with('#'))
					continue;

				// Split by comma and white space
				simpleArray<std::string> strings = stringExtension::split(nextLine, ", ");

				if (sizeof(strings) != 10)
					continue;

				color nextColor;
				colorCollections collection;
				
				EnumString<colorCollections>::To(collection, strings.get(2));

				nextColor.red = typeConverter::stringToshort(strings.get(2));
				nextColor.green = typeConverter::stringToshort(strings.get(2));
				nextColor.blue = typeConverter::stringToshort(strings.get(2));
				nextColor.redRand = typeConverter::stringToshort(strings.get(2));
				nextColor.greenRand = typeConverter::stringToshort(strings.get(2));
				nextColor.blueRand = typeConverter::stringToshort(strings.get(2));
				nextColor.rand = typeConverter::stringToshort(strings.get(2));
				nextColor.colorDances = typeConverter::stringToBool(strings.get(2));

				switch (collection)
				{
				case colorCollections::game:
					gameColors gameSelector;
					EnumString<gameColors>::To(gameSelector, strings.get(1));
					result->setColor(gameSelector, nextColor);
					break;
				case colorCollections::bolt:
					boltColors boltSelector;
					EnumString<boltColors>::To(boltSelector, strings.get(1));
					result->setColor(boltSelector, nextColor);
					break;
				case colorCollections::tile:
					tileColors tileSelector;
					EnumString<tileColors>::To(tileSelector, strings.get(1));
					result->setColor(tileSelector, nextColor);
					break;
				case colorCollections::creature:
					creatureColors creatureSelector;
					EnumString<creatureColors>::To(creatureSelector, strings.get(1));
					result->setColor(creatureSelector, nextColor);
					break;
				case colorCollections::light:
					lightColors lightSelector;
					EnumString<lightColors>::To(lightSelector, strings.get(1));
					result->setColor(lightSelector, nextColor);
					break;
				case colorCollections::flare:
					flareColors flareSelector;
					EnumString<flareColors>::To(flareSelector, strings.get(1));
					result->setColor(flareSelector, nextColor);
					break;
				case colorCollections::multipliers:
					colorMultipliers multiplierSelector;
					EnumString<colorMultipliers>::To(multiplierSelector, strings.get(1));
					result->setColor(multiplierSelector, nextColor);
					break;
				case colorCollections::blood:
					bloodColors bloodSelector;
					EnumString<bloodColors>::To(bloodSelector, strings.get(1));
					result->setColor(bloodSelector, nextColor);
					break;
				case colorCollections::gas:
					gasColors gasSelector;
					EnumString<gasColors>::To(gasSelector, strings.get(1));
					result->setColor(gasSelector, nextColor);
					break;
				case colorCollections::bInterface:
					interfaceColors interfaceSelector;
					EnumString<interfaceColors>::To(interfaceSelector, strings.get(1));
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
	shaderData resourceController::loadShader(shaderResource resource)
	{
		std::string source = "";

		switch (resource)
		{
		case shaderResource::brogueBaseVert:

			source = this->loadTextResource(IDR_BROGUE_BASE_VERT);
			return shaderData(resource, GL_VERTEX_SHADER,  source);

		case shaderResource::brogueBaseFrag:

			source = this->loadTextResource(IDR_BROGUE_BASE_FRAG);
			return shaderData(resource, GL_FRAGMENT_SHADER, source);

		case shaderResource::brogueFrameVert:

			source = this->loadTextResource(IDR_BROGUE_FRAME_FRAG);
			return shaderData(resource, GL_VERTEX_SHADER, source);

		case shaderResource::brogueFrameFrag:

			source = this->loadTextResource(IDR_BROGUE_FRAME_FRAG);
			return shaderData(resource, GL_FRAGMENT_SHADER, source);

		default:
			brogueException::show("Unhandled Resource Type:  resourceController::loadShader");
		}
	}

	std::string resourceController::loadTextResource(int idrHandle)
	{
		// Some internet code...
		HMODULE instance = NULL;
		HRSRC hresource = FindResource(instance, MAKEINTRESOURCE(idrHandle), L"TEXT");

		if (hresource)
		{
			HGLOBAL hLoadedResource = LoadResource(instance, hresource);

			if (hLoadedResource)
			{
				LPVOID pLockedResource = LockResource(hLoadedResource);

				if (pLockedResource)
				{
					DWORD dwResourceSize = SizeofResource(instance, hresource);

					if (0 != dwResourceSize)
					{
						// Use pLockedResource and dwResourceSize however you want
						char* buffer = new char[dwResourceSize];

						for (int index = 0; index < dwResourceSize; index++)
						{
							buffer[index] = ((char*)hLoadedResource)[index];
						}

						std::string string(buffer);

						delete [] buffer;

						return string;
					}
				}
			}
		}

		return "";
	}
}