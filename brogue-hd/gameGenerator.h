#pragma once
#include "brogueContentGrid.h"
#include "brogueGame.h"
#include "brogueGameTemplate.h"
#include "brogueLayout.h"
#include "brogueLevel.h"
#include "brogueLevelTemplate.h"
#include "contentGenerator.h"
#include "gridRect.h"
#include "layoutGenerator.h"
#include "randomGenerator.h"
#include "simpleList.h"
#include "terrainGenerator.h"

namespace brogueHd::backend
{
	class gameGenerator
	{
	public:

		gameGenerator(randomGenerator* randomMain, const gridRect& gameBoundary)
		{
			_randomGenerator = randomMain;
			_gameBoundary = new gridRect(gameBoundary);
		};
		~gameGenerator()
		{
			delete _gameBoundary;
		};

		brogueGame* createGame(unsigned long seed, brogueGameTemplate* gameTemplate);

	private:

		randomGenerator* _randomGenerator;
		gridRect* _gameBoundary;

	};

	brogueGame* gameGenerator::createGame(unsigned long seed, brogueGameTemplate* gameTemplate)
	{
		simpleList<brogueLevel*> levels;

		// Create Levels
		for (int index = 0; index < gameTemplate->getLevelCount(); index++)
		{
			brogueLevelTemplate* levelTemplate = gameTemplate->getLevel(index + 1);

			layoutGenerator layoutGen(_randomGenerator);
			terrainGenerator terrainGen(_randomGenerator);
			contentGenerator contentGen(*_gameBoundary);

			// Level Layout
			brogueLayout* layout = layoutGen.generateLayout(levelTemplate->getProfile());

			// Level Terrain
			terrainGen.generateTerrain(layout);

			// Level Contents
			brogueContentGrid* contentGrid = contentGen.generateLevelContents(layout);

			// Final Brogue Level
			brogueLevel* level = new brogueLevel(index + 1, layout, contentGrid);		// TODO:  Remove level seed

			levels.add(level);
		}

		return new brogueGame(false, false, levels);
	}
}