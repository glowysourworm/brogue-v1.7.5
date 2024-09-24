#pragma once

#include "broguefile.h"
#include "keyProcessor.h"

using namespace brogueHd::backend::processor;
using namespace brogueHd::backend::model::io;

namespace brogueHd::backend::controller
{
	class resourceController
	{
	public:

		resourceController();
		~resourceController();

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
		void loadKeymap(const keyProcessor& keyProcessor);

		/// <summary>
		/// Loads game data from file
		/// </summary>
		gameData* loadGame(char* path);

		/// <summary>
		/// Loads playback data from file
		/// </summary>
		playbackData* loadPlayback(char* path);
	};
}