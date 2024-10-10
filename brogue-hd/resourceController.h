#pragma once

#include "brogueGlobal.h"
#include "broguefile.h"
#include "keyProcessor.h"
#include "brogueColorMap.h"
#include "gameData.h"
#include "playbackData.h"
#include "shaderData.h"
#include "simpleHash.h"

using namespace brogueHd::simple;
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
}