#pragma once

#include "resourceController.h"
#include "playbackData.h"
#include "gameData.h"
#include "brogueColorMap.h"
#include "fileOperations.h"
#include "keyProcessor.h"

#include "simpleEnumString.h"
#include "simpleString.h"
#include "shaderData.h"
#include "colorConstants.h"

#include "json.hpp"

#include <string>
#include <fstream>
#include <format>

using namespace brogueHd::simple;
using namespace brogueHd::component;
using namespace brogueHd::frontend::opengl;
using namespace brogueHd::backend::processor;
using namespace brogueHd::backend::model;
using namespace brogueHd::backend::model::io;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::controller
{
	class resourceController
	{
	public:

		resourceController();
		~resourceController();

		/// <summary>
		/// Loads file caches and returns any file results
		/// </summary>
		bool initialize(const char* resourcConfigFile);

		/// <summary>
		/// Gets array of high score entries from file
		/// </summary>
		brogueScoresFile* getHighScores(short& mostRecentLineNumber);

		/// <summary>
		/// Loads colors.csv file
		/// </summary>
		brogueColorMap* loadColors(const char* path);

		/// <summary>
		/// Loads key map from file
		/// </summary>
		void loadKeymap(keyProcessor& keyProcessor);

		/// <summary>
		/// Loads game data from file
		/// </summary>
		gameData* loadGame(const char* path);

		/// <summary>
		/// Loads playback data from file
		/// </summary>
		playbackData* loadPlayback(const char* path);

		/// <summary>
		/// Gets shader data from the resource controller
		/// </summary>
		shaderData getShader(shaderResource resource);
	
	private:

		simpleHash<shaderResource, shaderData*>* _shaderCache;
	};

	resourceController::resourceController()
	{
		_shaderCache = new simpleHash<shaderResource, shaderData*>();
	}
	resourceController::~resourceController()
	{
		delete _shaderCache;
	}

	bool resourceController::initialize(const char* resourceConfigFile)
	{
		// Read config file contents
		simpleString configuration = fileOperations::readFile(resourceConfigFile);

		try
		{
			// Parse the JSON file
			nlohmann::json jsonConfig = nlohmann::json::parse(configuration.c_str());

			simpleString baseVertSource = fileOperations::readFile(std::string(jsonConfig[brogueHd::ConfigBaseVertexShader]).c_str());
			simpleString baseFragSource = fileOperations::readFile(std::string(jsonConfig[brogueHd::ConfigBaseFragmentShader]).c_str());
			simpleString frameVertSource = fileOperations::readFile(std::string(jsonConfig[brogueHd::ConfigFrameVertexShader]).c_str());
			simpleString frameFragSource = fileOperations::readFile(std::string(jsonConfig[brogueHd::ConfigFrameFragmentShader]).c_str());
			simpleString frameBlendVertSource = fileOperations::readFile(std::string(jsonConfig[brogueHd::ConfigFrameBlendVertexShader]).c_str());
			simpleString frameBlendFragSource = fileOperations::readFile(std::string(jsonConfig[brogueHd::ConfigFrameBlendFragmentShader]).c_str());
			simpleString flameMenuVertSource = fileOperations::readFile(std::string(jsonConfig[brogueHd::ConfigFlameMenuVertexShader]).c_str());
			simpleString flameMenuFragSource = fileOperations::readFile(std::string(jsonConfig[brogueHd::ConfigFlameMenuFragmentShader]).c_str());

			shaderData* baseVert = new shaderData(shaderResource::brogueBaseVert, GL_VERTEX_SHADER, baseVertSource);
			shaderData* baseFrag = new shaderData(shaderResource::brogueBaseFrag, GL_FRAGMENT_SHADER, baseFragSource);
			shaderData* frameVert = new shaderData(shaderResource::brogueFrameVert, GL_VERTEX_SHADER, frameVertSource);
			shaderData* frameFrag = new shaderData(shaderResource::brogueFrameFrag, GL_FRAGMENT_SHADER, frameFragSource);
			shaderData* frameBlendVert = new shaderData(shaderResource::brogueFrameBlendVert, GL_VERTEX_SHADER, frameBlendVertSource);
			shaderData* frameBlendFrag = new shaderData(shaderResource::brogueFrameBlendFrag, GL_FRAGMENT_SHADER, frameBlendFragSource);
			shaderData* flameMenuVert = new shaderData(shaderResource::brogueFlameMenuVert, GL_VERTEX_SHADER, flameMenuVertSource);
			shaderData* flameMenuFrag = new shaderData(shaderResource::brogueFlameMenuFrag, GL_FRAGMENT_SHADER, flameMenuFragSource);

			_shaderCache->add(shaderResource::brogueBaseVert, baseVert);
			_shaderCache->add(shaderResource::brogueBaseFrag, baseFrag);
			_shaderCache->add(shaderResource::brogueFrameVert, frameVert);
			_shaderCache->add(shaderResource::brogueFrameFrag, frameFrag);
			_shaderCache->add(shaderResource::brogueFrameBlendVert, frameBlendVert);
			_shaderCache->add(shaderResource::brogueFrameBlendFrag, frameBlendFrag);
			_shaderCache->add(shaderResource::brogueFlameMenuVert, flameMenuVert);
			_shaderCache->add(shaderResource::brogueFlameMenuFrag, flameMenuFrag);

			return true;
		}
		catch (std::exception& ex)
		{
			simpleException::show("Error reading resource config JSON file:  {}", ex.what());
			return false;
		}

		return false;
	}

	shaderData resourceController::getShader(shaderResource resource)
	{
		return *_shaderCache->get(resource);
	}

	brogueScoresFile* resourceController::getHighScores(short& mostRecentLineNumber)
	{
		try
		{
			brogueScoresFile* scoresFile;
			std::fstream stream;

			stream.open(brogueHd::BrogueHighScoresFile, std::fstream::in);


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
			simpleException::showCstr("Failed to read high scores file.");
			throw ex;
		}

		return nullptr;
	}

	gameData* resourceController::loadGame(const char* path)
	{
		return nullptr;
	}

	playbackData* resourceController::loadPlayback(const char* path)
	{
		return nullptr;
	}

	void resourceController::loadKeymap(keyProcessor& processor)
	{
		try
		{
			std::ifstream stream("keymap", std::fstream::in);

			if (!stream.is_open())
			{
				simpleException::showCstr("Error opening keymap file:  either missing, or locked");
				return;
			}

			std::string lineStr;

			while (getline(stream, lineStr))
			{
				simpleString line(lineStr.c_str());
				simpleArray<simpleString> linePieces = line.split(' ');

				if (sizeof(linePieces) != 2)
					simpleException::showCstr("Invalid keymap file:  looking for two character split by only whitespace");

				processor.addKeyMap(linePieces.get(0).c_str(), linePieces.get(1).c_str());
			}

			stream.close();
		}
		catch (std::exception& ex)
		{
			simpleException::show("gameController::loadKeyMap:  ", ex.what());
			throw ex;
		}
	}

	brogueColorMap* resourceController::loadColors(const char* path)
	{
		brogueColorMap* result = new brogueColorMap();

		try
		{
			//	std::ifstream stream(path);

			//	// collection,name,red,green,blue,redRand,greenRand,blueRand,rand,colorDances

			//	
			//	std::string nextLineStr;

			//	while (std::getline(stream, nextLineStr))
			//	{
			//		// Comment
			//		if (nextLineStr.starts_with('#'))
			//			continue;

			//		simpleString nextLine(nextLineStr.c_str());

			//		// Split by comma and white space
			//		simpleArray<simpleString> strings = nextLine.split(", ");

			//		if (sizeof(strings) != 10)
			//			continue;

			//		color nextColor;
			//		colorCollections collection;
			//		
			//		EnumString<colorCollections>::To(collection, strings.get(2).c_str());

			//		nextColor.red = strings.get(2).toShort();
			//		nextColor.green = strings.get(2).toShort();
			//		nextColor.blue = strings.get(2).toShort();
			//		nextColor.redRand = strings.get(2).toShort();
			//		nextColor.greenRand = strings.get(2).toShort();
			//		nextColor.blueRand = strings.get(2).toShort();
			//		nextColor.rand = strings.get(2).toShort();
			//		nextColor.colorDances = strings.get(2).toBool();

			//		switch (collection)
			//		{
			//		case colorCollections::game:
			//			gameColors gameSelector;
			//			EnumString<gameColors>::To(gameSelector, strings.get(1).c_str());
			//			result->setColor(gameSelector, nextColor);
			//			break;
			//		case colorCollections::bolt:
			//			boltColors boltSelector;
			//			EnumString<boltColors>::To(boltSelector, strings.get(1).c_str());
			//			result->setColor(boltSelector, nextColor);
			//			break;
			//		case colorCollections::tile:
			//			tileColors tileSelector;
			//			EnumString<tileColors>::To(tileSelector, strings.get(1).c_str());
			//			result->setColor(tileSelector, nextColor);
			//			break;
			//		case colorCollections::creature:
			//			creatureColors creatureSelector;
			//			EnumString<creatureColors>::To(creatureSelector, strings.get(1).c_str());
			//			result->setColor(creatureSelector, nextColor);
			//			break;
			//		case colorCollections::light:
			//			lightColors lightSelector;
			//			EnumString<lightColors>::To(lightSelector, strings.get(1).c_str());
			//			result->setColor(lightSelector, nextColor);
			//			break;
			//		case colorCollections::flare:
			//			flareColors flareSelector;
			//			EnumString<flareColors>::To(flareSelector, strings.get(1).c_str());
			//			result->setColor(flareSelector, nextColor);
			//			break;
			//		case colorCollections::multipliers:
			//			colorMultipliers multiplierSelector;
			//			EnumString<colorMultipliers>::To(multiplierSelector, strings.get(1).c_str());
			//			result->setColor(multiplierSelector, nextColor);
			//			break;
			//		case colorCollections::blood:
			//			bloodColors bloodSelector;
			//			EnumString<bloodColors>::To(bloodSelector, strings.get(1).c_str());
			//			result->setColor(bloodSelector, nextColor);
			//			break;
			//		case colorCollections::gas:
			//			gasColors gasSelector;
			//			EnumString<gasColors>::To(gasSelector, strings.get(1).c_str());
			//			result->setColor(gasSelector, nextColor);
			//			break;
			//		case colorCollections::bInterface:
			//			interfaceColors interfaceSelector;
			//			EnumString<interfaceColors>::To(interfaceSelector, strings.get(1).c_str());
			//			result->setColor(interfaceSelector, nextColor);
			//			break;
			//		default:
			//			break;
			//		}
			//	} 

			return result;
		}
		catch (std::exception ex)
		{
			simpleException::show("Error opening colors.csv:  ", ex.what());

			delete result;
		}

		return nullptr;
	}
}