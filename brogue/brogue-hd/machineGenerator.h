#pragma once

#include "linearGenerationParameters.h"
#include "dungeonConstants.h"

using namespace brogueHd::backend::model::game;
using namespace brogueHd::backend::model::construction;

namespace brogueHd::backend::model::machine
{
	struct machineGenerator 
	{
		// What spawns:
		tileType terrain;
		dungeonLayers layer;

		dungeonFeatureTypes DFType;

		machineTypes machine; // Machine placement also respects BP_ placement flags in the machine blueprint

		// Parameters governing when and where it spawns:
		tileType requiredDungeonFoundationType;
		tileType requiredLiquidFoundationType;

		linearGenerationParameters parameters;

	};
}