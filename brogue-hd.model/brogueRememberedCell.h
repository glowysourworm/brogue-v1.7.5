#pragma once

#include "brogueCellDisplay.h"

namespace brogueHd::model
{
	struct brogueRememberedCell
	{
		brogueCellDisplay rememberedAppearance; // how the player remembers the cell to look
		enum itemCategory rememberedItemCategory; // what category of item the player remembers lying there
		int rememberedItemKind; // what kind of item the player remembers lying there
		int rememberedItemQuantity; // how many of the item the player remembers lying there
		enum tileType rememberedTerrain;
		// what the player remembers as the terrain (i.e. highest priority terrain upon last seeing)
		unsigned long rememberedCellFlags; // map cell flags the player remembers from that spot
		unsigned long rememberedTerrainFlags; // terrain flags the player remembers from that spot
		unsigned long rememberedTMFlags; // TM flags the player remembers from that spot
	};
}
