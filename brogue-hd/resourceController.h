#pragma once

#include "brogueColorMap.h"
#include "brogueScoreEntry.h"
#include "gameData.h"
#include "keyProcessor.h"
#include "playbackData.h"
#include "simpleFileIO.h"

#include "shaderData.h"
#include "simpleBitmap.h"
#include "simpleList.h"
#include "simpleString.h"

#include "json.hpp"

#include "brogueGlobal.h"
#include "brogueUIConstants.h"
#include "brogueUIProgramPartConfiguration.h"
#include "gl.h"
#include "simpleArray.h"
#include "simpleException.h"
#include "simpleExt.h"
#include "simpleHash.h"
#include <chrono>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iosfwd>
#include <string>

using namespace brogueHd::simple;
using namespace brogueHd::component;
using namespace brogueHd::frontend;

namespace brogueHd::backend
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
		brogueScoreEntry* getHighScore(int index)
		{
			return _highScores->get(index);
		}

		int getHighScoreCount() const
		{
			return _highScores->count();
		}

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
		shaderData* getShader(shaderResource resource);

		/// <summary>
		/// Returns font glyphs bitmap (read into memory)
		/// </summary>
		simpleBitmap* getFontGlyphs(int zoomLevel)
		{
			return _fontGlyphs->get(zoomLevel);
		}

		simpleString* getGamesDirectory() const
		{
			return _gameDirectory;
		}

		simpleString* getPlaybackDirectory() const
		{
			return _playbackDirectory;
		}

		brogueUIProgramPartConfiguration* getUIPartConfig(brogueUIProgramPart partName)
		{
			return _programPartConfigs->get(partName);
		}

	private:

		void loadHighScores(const char* path);
		void loadPartConfigs();

	private:

		simpleHash<shaderResource, shaderData*>* _shaderCache;
		simpleHash<int, simpleBitmap*>* _fontGlyphs;
		simpleHash<brogueUIProgramPart, brogueUIProgramPartConfiguration*>* _programPartConfigs;
		simpleList<brogueScoreEntry*>* _highScores;
		simpleString* _gameDirectory;
		simpleString* _playbackDirectory;

	};

	resourceController::resourceController()
	{
		_shaderCache = new simpleHash<shaderResource, shaderData*>();
		_fontGlyphs = new simpleHash<int, simpleBitmap*>();
		_programPartConfigs = new simpleHash<brogueUIProgramPart, brogueUIProgramPartConfiguration*>();
		_highScores = new simpleList<brogueScoreEntry*>();
		_gameDirectory = nullptr;
		_playbackDirectory = nullptr;
	}
	resourceController::~resourceController()
	{
		delete _shaderCache;
		delete _fontGlyphs;
		delete _gameDirectory;
		delete _playbackDirectory;
		delete _highScores;
		delete _programPartConfigs;
	}

	bool resourceController::initialize(const char* resourceConfigFile)
	{
		// Read config file contents
		simpleString configuration = simpleFileIO::readTextFile(resourceConfigFile);

		try
		{
			// Parse the JSON file
			nlohmann::json jsonConfig = nlohmann::json::parse(configuration.c_str());

			const char* resConfig = brogueHd::ConfigResourceConfig;

			simpleString colorMaskVertSource = simpleFileIO::readTextFile(std::string(jsonConfig[resConfig][brogueHd::ConfigColorMaskVertShader]).c_str());
			simpleString colorMaskFragSource = simpleFileIO::readTextFile(std::string(jsonConfig[resConfig][brogueHd::ConfigColorMaskFragShader]).c_str());
			simpleString backgroundColorVertSource = simpleFileIO::readTextFile(std::string(jsonConfig[resConfig][brogueHd::ConfigBackgroundColorVertexShader]).c_str());
			simpleString backgroundColorFragSource = simpleFileIO::readTextFile(std::string(jsonConfig[resConfig][brogueHd::ConfigBackgroundColorFragmentShader]).c_str());
			simpleString brogueCellDisplayVertSource = simpleFileIO::readTextFile(std::string(jsonConfig[resConfig][brogueHd::ConfigBrogueCellDisplayVertShader]).c_str());
			simpleString brogueCellDisplayFragSource = simpleFileIO::readTextFile(std::string(jsonConfig[resConfig][brogueHd::ConfigBrogueCellDisplayFragShader]).c_str());
			simpleString diffuseColorUpwardVertSource = simpleFileIO::readTextFile(std::string(jsonConfig[resConfig][brogueHd::ConfigDiffuseColorUpwardVertexShader]).c_str());
			simpleString diffuseColorUpwardFragSource = simpleFileIO::readTextFile(std::string(jsonConfig[resConfig][brogueHd::ConfigDiffuseColorUpwardFragmentShader]).c_str());
			simpleString mixFrameTexturesVertSource = simpleFileIO::readTextFile(std::string(jsonConfig[resConfig][brogueHd::ConfigMixFrameTexturesVertexShader]).c_str());
			simpleString mixFrameTexturesFragSource = simpleFileIO::readTextFile(std::string(jsonConfig[resConfig][brogueHd::ConfigMixFrameTexturesFragmentShader]).c_str());

			shaderData* colorMaskVert = new shaderData(shaderResource::colorMaskVert, GL_VERTEX_SHADER, colorMaskVertSource);
			shaderData* colorMaskFrag = new shaderData(shaderResource::colorMaskFrag, GL_FRAGMENT_SHADER, colorMaskFragSource);
			shaderData* backgroundColorVert = new shaderData(shaderResource::backgroundColorVert, GL_VERTEX_SHADER, backgroundColorVertSource);
			shaderData* backgroundColorFrag = new shaderData(shaderResource::backgroundColorFrag, GL_FRAGMENT_SHADER, backgroundColorFragSource);
			shaderData* brogueCellDisplayVert = new shaderData(shaderResource::brogueCellDisplayVert, GL_VERTEX_SHADER, brogueCellDisplayVertSource);
			shaderData* brogueCellDisplayFrag = new shaderData(shaderResource::brogueCellDisplayFrag, GL_FRAGMENT_SHADER, brogueCellDisplayFragSource);
			shaderData* diffuseColorUpwardVert = new shaderData(shaderResource::diffuseColorUpwardVert, GL_VERTEX_SHADER, diffuseColorUpwardVertSource);
			shaderData* diffuseColorUpwardFrag = new shaderData(shaderResource::diffuseColorUpwardFrag, GL_FRAGMENT_SHADER, diffuseColorUpwardFragSource);
			shaderData* mixFrameTexturesVert = new shaderData(shaderResource::mixFrameTexturesVert, GL_VERTEX_SHADER, mixFrameTexturesVertSource);
			shaderData* mixFrameTexturesFrag = new shaderData(shaderResource::mixFrameTexturesFrag, GL_FRAGMENT_SHADER, mixFrameTexturesFragSource);

			_shaderCache->add(shaderResource::colorMaskVert, colorMaskVert);
			_shaderCache->add(shaderResource::colorMaskFrag, colorMaskFrag);
			_shaderCache->add(shaderResource::backgroundColorVert, backgroundColorVert);
			_shaderCache->add(shaderResource::backgroundColorFrag, backgroundColorFrag);
			_shaderCache->add(shaderResource::brogueCellDisplayVert, brogueCellDisplayVert);
			_shaderCache->add(shaderResource::brogueCellDisplayFrag, brogueCellDisplayFrag);
			_shaderCache->add(shaderResource::diffuseColorUpwardVert, diffuseColorUpwardVert);
			_shaderCache->add(shaderResource::diffuseColorUpwardFrag, diffuseColorUpwardFrag);
			_shaderCache->add(shaderResource::mixFrameTexturesVert, mixFrameTexturesVert);
			_shaderCache->add(shaderResource::mixFrameTexturesFrag, mixFrameTexturesFrag);

			simpleString fontDir = std::string(jsonConfig[resConfig][brogueHd::ConfigFontGlyphDirectory]).c_str();

			for (int index = MIN_ZOOM; index <= MAX_ZOOM; index++)
			{
				simpleString fileName = simpleExt::format(brogueHd::FontGlyphFileNameFormat_RGBA24_UNSIGNED, index).c_str();
				simpleString fullPath = simpleFileIO::filePathConcat(fontDir, fileName);

				simpleBitmap* glyphSheet = simpleBitmap::fromFile(fullPath);

				_fontGlyphs->add(index, glyphSheet);
			}

			_gameDirectory = new simpleString(std::string(jsonConfig[brogueHd::ConfigSavedGameDirectory]).c_str());
			_playbackDirectory = new simpleString(std::string(jsonConfig[brogueHd::ConfigPlaybackDirectory]).c_str());

			loadHighScores(std::string(jsonConfig[brogueHd::ConfigHighScoresFile]).c_str());
			loadPartConfigs();

			return true;
		}
		catch (std::exception& ex)
		{
			simpleException::show("Error reading resource config JSON file:  {}", ex.what());
			return false;
		}

		return false;
	}

	shaderData* resourceController::getShader(shaderResource resource)
	{
		return _shaderCache->get(resource);
	}

	gameData* resourceController::loadGame(const char* path)
	{
		return nullptr;
	}

	playbackData* resourceController::loadPlayback(const char* path)
	{
		return nullptr;
	}

	void resourceController::loadHighScores(const char* path)
	{
		try
		{
			// Need some fake data
			simpleList<brogueScoreEntry*> unsorted;

			for (int index = 0; index < 30; index++)
			{
				brogueScoreEntry* entry = new brogueScoreEntry(rand(), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), "Killed by a Dragon");

				unsorted.add(entry);
			}

			simpleList<brogueScoreEntry*> sorted = unsorted.sort([] (brogueScoreEntry* item1, brogueScoreEntry* item2)
			{
				return brogueScoreEntry::compare(*item1, *item2);
			});

			for (int index = 0; index < sorted.count(); index++)
				_highScores->add(sorted.get(index));
		}
		catch (std::exception& ex)
		{
			simpleException::show("Error reading high scores file:  {}", ex.what());
		}
	}

	void resourceController::loadPartConfigs()
	{
		// Title View
		brogueUIProgramPartConfiguration* flameDisplay = 
			new brogueUIProgramPartConfiguration(brogueUIProgramPart::FlameDisplay, 
												 shaderResource::diffuseColorUpwardVert, 
												 shaderResource::diffuseColorUpwardFrag, 
												 openglDataStreamType::brogueFlameQuad, 
												 openglBrogueCellOutputSelector::NoDisplay, 
												 15,
												 false,
												 false);

		// Title View - Color Mask (Generic Part)
		brogueUIProgramPartConfiguration* colorMask =
			new brogueUIProgramPartConfiguration(brogueUIProgramPart::ColorMask,
												 shaderResource::backgroundColorVert,
												 shaderResource::backgroundColorFrag,
												 openglDataStreamType::brogueColorQuad,
												 openglBrogueCellOutputSelector::DisplayCurrentFrame,
												 15,
												 false,
												 false);

		// Generic Parts
		brogueUIProgramPartConfiguration* viewCompositor =
			new brogueUIProgramPartConfiguration(brogueUIProgramPart::ViewCompositor,
												 shaderResource::brogueCellDisplayVert,
												 shaderResource::brogueCellDisplayFrag,
												 openglDataStreamType::brogueCellQuad,
												 openglBrogueCellOutputSelector::DisplayCurrentFrame,
												 0,
												 true,
												 false);

		brogueUIProgramPartConfiguration* button =
			new brogueUIProgramPartConfiguration(brogueUIProgramPart::Button,
												 shaderResource::brogueCellDisplayVert,
												 shaderResource::brogueCellDisplayFrag,
												 openglDataStreamType::brogueCellQuad,
												 openglBrogueCellOutputSelector::DisplayCurrentFrame,
												 0,
												 true,
												 false);

		brogueUIProgramPartConfiguration* text =
			new brogueUIProgramPartConfiguration(brogueUIProgramPart::Text,
												 shaderResource::brogueCellDisplayVert,
												 shaderResource::brogueCellDisplayFrag,
												 openglDataStreamType::brogueCellQuad,
												 openglBrogueCellOutputSelector::DisplayCurrentFrame,
												 0,
												 true,
												 false);

		brogueUIProgramPartConfiguration* background =
			new brogueUIProgramPartConfiguration(brogueUIProgramPart::Background,
												 shaderResource::backgroundColorVert,
												 shaderResource::backgroundColorFrag,
												 openglDataStreamType::brogueColorQuad,
												 openglBrogueCellOutputSelector::DisplayCurrentFrame,
												 0,
												 false,
												 false);

		brogueUIProgramPartConfiguration* menuBackground =
			new brogueUIProgramPartConfiguration(brogueUIProgramPart::MenuBackground,
												 shaderResource::brogueCellDisplayVert,
												 shaderResource::brogueCellDisplayFrag,
												 openglDataStreamType::brogueCellQuad,
												 openglBrogueCellOutputSelector::DisplayCurrentFrame,
												 0,
												 true,
												 false);

		_programPartConfigs->add(brogueUIProgramPart::FlameDisplay, flameDisplay);
		_programPartConfigs->add(brogueUIProgramPart::ColorMask, colorMask);
		_programPartConfigs->add(brogueUIProgramPart::Button, button);
		_programPartConfigs->add(brogueUIProgramPart::Text, text);
		_programPartConfigs->add(brogueUIProgramPart::Background, background);
		_programPartConfigs->add(brogueUIProgramPart::MenuBackground, menuBackground);
		_programPartConfigs->add(brogueUIProgramPart::ViewCompositor, viewCompositor);
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