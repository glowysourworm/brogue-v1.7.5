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
#include "noiseGenerator.h"
#include "randomGenerator.h"
#include "simpleList.h"
#include "terrainGenerator.h"

namespace brogueHd::component
{
	using namespace brogueHd::model;

	class gameGenerator
	{
	public:
		gameGenerator(randomGenerator* randomMain, noiseGenerator* noiseGenerator, const gridRect& layoutParentBoundary,
		              int zoomLevel)
		{
			_randomGenerator = randomMain;
			_noiseGenerator = noiseGenerator;
			_layoutParentBoundary = new gridRect(layoutParentBoundary);
			_zoomLevel = zoomLevel;
		};

		~gameGenerator()
		{
			delete _layoutParentBoundary;
		};

		brogueGame* createGame(unsigned long seed, brogueGameTemplate* gameTemplate);

	private:
		randomGenerator* _randomGenerator;
		noiseGenerator* _noiseGenerator;
		gridRect* _layoutParentBoundary;
		int _zoomLevel;
	};

	brogueGame* gameGenerator::createGame(unsigned long seed, brogueGameTemplate* gameTemplate)
	{
		simpleList<brogueLevel*> levels;

		// Create Levels
		for (int index = 0; index < gameTemplate->getLevelCount(); index++)
		{
			brogueLevelTemplate* levelTemplate = gameTemplate->getLevel(index + 1);

			layoutGenerator layoutGen(_randomGenerator, *_layoutParentBoundary, _zoomLevel);
			terrainGenerator terrainGen(_randomGenerator);
			contentGenerator contentGen(*_layoutParentBoundary);

			// Level Layout
			brogueLayout* layout = layoutGen.generateLayout(levelTemplate);

			// Level Terrain
			terrainGen.generateTerrain(layout);

			// Level Contents
			brogueContentGrid* contentGrid = contentGen.generateLevelContents(layout);

			// Final Brogue Level
			brogueLevel* level = new brogueLevel(index + 1, layout, contentGrid); // TODO:  Remove level seed

			levels.add(level);
		}

		return new brogueGame(false, false, levels);
	}
}
