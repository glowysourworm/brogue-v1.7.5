#pragma once

#include "brogueGlobal.h"
#include "color.h"
#include "dungeonConstants.h"
#include "lightConstants.h"

namespace brogueHd::backend::model
{
	struct dungeonFeature 
	{
		// tile info:
		tileType tile;
		permanentDungeonLayers permanentLayer;
		temporaryDungeonLayers temporaryLayer;

		// spawning pattern:
		int startProbability;
		int probabilityDecrement;
		unsigned long flags;
		char description[DCOLS];
		lightType lightFlare;
		const color* flashColor;
		int effectRadius;
		tileType propagationTerrain;
		//dungeonFeatureTypes subsequentDF;
		permanentDungeonLayers subsequentDF_P;
		temporaryDungeonLayers subsequentDF_T;
		bool messageDisplayed;
	};
}