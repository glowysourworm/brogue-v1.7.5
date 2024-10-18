#pragma once

#include "light.h"
#include "color.h"
#include "simple.h"
#include "gridLocator.h"

using namespace brogueHd::simple;
using namespace brogueHd::component;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model::layout
{
	struct brogueCellDisplay : gridLocator
	{
	public:

		// TODO:  CHANGE THIS TO WORK ON THE WHOLE DISPLAY PROBLEM STARTING WITH BROGUE 1.7.5.
		static constexpr float CellHeight = 15.0f;
		static constexpr float CellWidth = 10.0f;

	public:

		char character;
		color foreColor;
		color backColor;
		float opacity;
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

		brogueCellDisplay()
		{
			character = default_value::value<char>();
			foreColor = default_value::value<color>();
			backColor = default_value::value<color>();
			opacity = 1.0f;
			needsUpdate = false;
		}
		brogueCellDisplay(const brogueCellDisplay& copy)
		{
			copyImpl(copy);
		}

		void operator=(const brogueCellDisplay& copy)
		{
			copyImpl(copy);
		}
		bool operator==(const brogueCellDisplay& other) const
		{
			return compare(other);
		}
		bool operator!=(const brogueCellDisplay& other) const
		{
			return !compare(other);
		}

		void update(const brogueCellDisplay& copy)
		{
			copyImpl(copy);
		}

		bool compare(const brogueCellDisplay& display) const
		{
			return character == display.character &&
				   foreColor.compare(display.foreColor) &&
				   backColor.compare(display.backColor) &&
				   opacity == display.opacity &&
				   needsUpdate == display.needsUpdate &&
				   column == display.column &&
				   row == display.row;
					//lighting.compare(display.lighting);
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(column, row, character, foreColor, backColor, opacity, needsUpdate);
		}

	private:

		void copyImpl(const brogueCellDisplay& copy)
		{
			column = copy.column;
			row = copy.row;
			character = copy.character;
			foreColor = copy.foreColor;									// Careful with instances. These are non-unique
			backColor = copy.backColor;									// Careful with instances. These are non-unique
			opacity = copy.opacity;
			needsUpdate = copy.needsUpdate;			
		}
	};
}