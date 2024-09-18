#pragma once

namespace brogueHd::backend::model::machine
{
	struct machineFeature 
	{
		// terrain
		enum dungeonFeatureTypes featureDF;	// generate this DF at the feature location (0 for none)
		enum tileType terrain;				// generate this terrain tile at the feature location (0 for none)
		enum dungeonLayers layer;			// generate the terrain tile in this layer

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
}