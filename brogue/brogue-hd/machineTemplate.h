#pragma once

#include "creatureBehaviorConstants.h"
#include "dungeonFeature.h"
#include "machineConstants.h"
#include "linearGenerationParameters.h"

using namespace brogueHd::backend::modelConstant;
using namespace brogueHd::backend::model::game;
using namespace brogueHd::backend::modelConstant;
using namespace brogueHd::backend::model::construction;

namespace brogueHd::backend::model::gameTemplate
{
	struct machineBase
	{
		// What spawns:
		tileType terrain;

		machineFeature feature;	// TODO:  I added this for the enum refactoring
		//dungeonLayers layer;

		//dungeonFeatureTypes DFType;

		machineTypes machine; // Machine placement also respects BP_ placement flags in the machine blueprint

		// Parameters governing when and where it spawns:
		tileType requiredDungeonFoundationType;
		tileType requiredLiquidFoundationType;

		linearGenerationParameters parameters;

	};

	struct machineFeature
	{
		// terrain
		dungeonFeauture		feature;		// TODO!  This is just a placeholder
		//dungeonFeatureTypes featureDF;		// generate this DF at the feature location (0 for none)
		//tileType terrain;					// generate this terrain tile at the feature location (0 for none)
		//temporaryDungeonLayers layer;		// generate the terrain tile in this layer

		short instanceCountRange[2];		// generate this range of instances of this feature
		short minimumInstanceCount;			// abort if fewer than this

		// items: these will be ignored if the feature is adopting an item
		short itemCategory;					// generate this category of item (or -1 for random)
		short itemKind;						// generate this kind of item (or -1 for random)

		short monsterID;					// generate a monster of this kind if MF_GENERATE_MONSTER is set

		short personalSpace;				// subsequent features must be generated more than this many tiles away from this feature
		unsigned long hordeFlags;			// choose a monster horde based on this
		unsigned long itemFlags;			// assign these flags to the item
		unsigned long flags;				// feature flags
	};

	struct creatureMachineTemplate : machineBase
	{
		creatureBookkeepingFlags bookkeepingFlags;
		//short spawnDepth;					// keep track of the depth of the machine to which they relate (for activation monsters)
		short machineHome;                  // monsters that spawn in a machine keep track of the machine number here (for activation monsters)
	};


}