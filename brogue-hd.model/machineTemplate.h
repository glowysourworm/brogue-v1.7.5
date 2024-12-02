#pragma once

#include "creatureBehaviorConstants.h"
#include "dungeonConstants.h"
#include "linearGenerationParameters.h"

namespace brogueHd::backend::model
{
	struct machineBase
	{
		// What spawns:
		tileType terrain;

		//machineFeature feature;	// TODO:  I added this for the enum refactoring
		//dungeonLayers layer;

		//dungeonFeatureTypes DFType;

		//machineTypes machine; // Machine placement also respects BP_ placement flags in the machine blueprint

		// Parameters governing when and where it spawns:
		tileType requiredDungeonFoundationType;
		tileType requiredLiquidFoundationType;

		linearGenerationParameters parameters;

	};

	struct machineFeature
	{
		// terrain
		//dungeonFeauture		feature;		// TODO!  This is just a placeholder
		//dungeonFeatureTypes featureDF;		// generate this DF at the feature location (0 for none)
		//tileType terrain;					// generate this terrain tile at the feature location (0 for none)
		//temporaryDungeonLayers layer;		// generate the terrain tile in this layer

		int instanceCountRange[2];		// generate this range of instances of this feature
		int minimumInstanceCount;			// abort if fewer than this

		// items: these will be ignored if the feature is adopting an item
		int itemCategory;					// generate this category of item (or -1 for random)
		int itemKind;						// generate this kind of item (or -1 for random)

		int monsterID;					// generate a monster of this kind if MF_GENERATE_MONSTER is set

		int personalSpace;				// subsequent features must be generated more than this many tiles away from this feature
		unsigned long hordeFlags;			// choose a monster horde based on this
		unsigned long itemFlags;			// assign these flags to the item
		unsigned long flags;				// feature flags
	};

	struct creatureMachineTemplate : machineBase
	{
		creatureBookkeepingFlags bookkeepingFlags;
		//int spawnDepth;					// keep track of the depth of the machine to which they relate (for activation monsters)
		int machineHome;                  // monsters that spawn in a machine keep track of the machine number here (for activation monsters)
	};


}