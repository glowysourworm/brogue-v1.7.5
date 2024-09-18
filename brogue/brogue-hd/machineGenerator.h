#pragma once

#include "generationParameters.h"

namespace brogueHd::backend::model::machine
{
	struct machineGenerator 
	{
		// What spawns:
		enum tileType terrain;
		enum dungeonLayers layer;

		enum dungeonFeatureTypes DFType;

		enum machineTypes machine; // Machine placement also respects BP_ placement flags in the machine blueprint

		// Parameters governing when and where it spawns:
		enum tileType requiredDungeonFoundationType;
		enum tileType requiredLiquidFoundationType;

		generationParameters parameters;

	};
}