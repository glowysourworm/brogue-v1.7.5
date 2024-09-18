#pragma once

typedef struct autoGenerator {
	// What spawns:
	enum tileType terrain;
	enum dungeonLayers layer;

	enum dungeonFeatureTypes DFType;

	enum machineTypes machine; // Machine placement also respects BP_ placement flags in the machine blueprint

	// Parameters governing when and where it spawns:
	enum tileType requiredDungeonFoundationType;
	enum tileType requiredLiquidFoundationType;
	short minDepth;
	short maxDepth;
	short frequency;
	short minNumberIntercept; // actually intercept * 100
	short minNumberSlope; // actually slope * 100
	short maxNumber;
} autoGenerator;