#pragma once

#include "brogueMacros.h"
#include "color.h"
#include "dungeonConstants.h"
#include "lightConstants.h"

using namespace brogueHd::backend::modelConstant;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model::effect
{
	struct dungeonFeature 
	{
		// tile info:
		tileType tile;
		permanentDungeonLayers permanentLayer;
		temporaryDungeonLayers temporaryLayer;

		// spawning pattern:
		short startProbability;
		short probabilityDecrement;
		unsigned long flags;
		char description[DCOLS];
		lightType lightFlare;
		const color* flashColor;
		short effectRadius;
		tileType propagationTerrain;
		//dungeonFeatureTypes subsequentDF;
		permanentDungeonLayers subsequentDF_P;
		temporaryDungeonLayers subsequentDF_T;
		bool messageDisplayed;
	};
}