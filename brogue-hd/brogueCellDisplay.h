#pragma once

#include "light.h"
#include "color.h"
#include "brogueGlobal.h"

using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model::layout
{
	struct brogueCellDisplay
	{
		char character;
		color foreColor;
		color backColor;
		char opacity;
		bool needsUpdate;

		//light lighting;
		

		//light oldLight;									// compare with subsequent lighting to determine whether to refresh cell

		//enum dungeonLayers layerFlags;					// Designate which of the layers the cell inhabits
		//unsigned long flags;							// non-terrain cell flags
		//unsigned short volume;							// quantity of gas in cell

		//// REFACTORED VOLUME:  Separates gases
		//unsigned short poisonGasVolume;
		//unsigned short swampGasVolume;

		//unsigned char machineNumber;
		////brogueCellDisplay rememberedAppearance;			// how the player remembers the cell to look
		////enum itemCategory rememberedItemCategory;		// what category of item the player remembers lying there
		////short rememberedItemKind;                       // what kind of item the player remembers lying there
		////short rememberedItemQuantity;                   // how many of the item the player remembers lying there
		////enum tileType rememberedTerrain;				// what the player remembers as the terrain (i.e. highest priority terrain upon last seeing)
		////unsigned long rememberedCellFlags;              // map cell flags the player remembers from that spot
		////unsigned long rememberedTerrainFlags;           // terrain flags the player remembers from that spot
		////unsigned long rememberedTMFlags;                // TM flags the player remembers from that spot


		//// transient cell; stuff we don't need to remember between levels
		////
		//light light;									// RGB components of lighting
		//light oldLight;									// compare with subsequent lighting to determine whether to refresh cell

		brogueCellDisplay(const brogueCellDisplay& copy)
		{
			update(copy);
		}

		void update(const brogueCellDisplay& copy)
		{
			character = copy.character;
			foreColor = copy.foreColor;									// Careful with instances. These are non-unique
			backColor = copy.backColor;									// Careful with instances. These are non-unique
			opacity = copy.opacity;
			needsUpdate = copy.needsUpdate;
			//lighting = copy.lighting;											// Careful with instances. These are non-unique
		}

		bool compare(const brogueCellDisplay& display)
		{
			return character == display.character &&
				   foreColor.compare(display.foreColor) &&
				   backColor.compare(display.backColor) &&
				   opacity == display.opacity &&
				   needsUpdate == display.needsUpdate;// &&
					//lighting.compare(display.lighting);
		}
	};
}

