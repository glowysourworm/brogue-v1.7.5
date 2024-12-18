#pragma once

#include "brogueGlyphMap.h"
#include "color.h"
#include "gridLocator.h"
#include "simple.h"
#include "simpleException.h"

namespace brogueHd::component
{
	struct brogueCellDisplay : gridLocator
	{
	public:
		// TODO:  CHANGE THIS TO WORK ON THE WHOLE DISPLAY PROBLEM STARTING WITH BROGUE 1.7.5.
		//
		// Glyph Sheet:  16 Columns x 17 Rows
		//
		// (Probably just going to need some more "embedded-into-the-ui-components" design)
		//
		static double CellWidth(int zoomLevel)
		{
			// Must derive these from dividing the glyph sheet into a grid
			switch (zoomLevel)
			{
			case 1: return 0;
			case 2: return 0;
			case 3: return 0;
			case 4: return 0;
			case 5: return 0;
			case 6: return 0;
			case 7: return 0;
			case 8: return 0;
			case 9: return 15.0;
			case 10: return 16.0;
			case 11: return 17.0;
			case 12: return 18.0;
			case 13: return 19.0;
			default:
				throw simpleException("Invalid zoom level:  brogueCellDisplay::CellWidth");
			}
		}

		static double CellHeight(int zoomLevel)
		{
			// Must derive these from dividing the glyph sheet into a grid
			// The texture height is rounded to fit the view port. So, the cell
			// size affects texture sampling. This cell width / height must be
			// as close as possible to an integral number of cells to match the
			// height. (which matches the glyph sheeet)
			//
			// Update:  Texel fetch fixed the texture 2D feedback error. So, the
			//			glyph sheets will be fixed to an integral number of pixels
			//			as well.
			//
			switch (zoomLevel)
			{
			case 1: return 0;
			case 2: return 0;
			case 3: return 0;
			case 4: return 0;
			case 5: return 0;
			case 6: return 0;
			case 7: return 0;
			case 8: return 0;
			case 9: return 25.0;
			case 10: return 27.0;
			case 11: return 29.0;
			case 12: return 31.0;
			case 13: return 31.0;
			default:
				throw simpleException("Invalid zoom level:  brogueCellDisplay::CellHeight");
			}
		}

	public:
		int character;
		color foreColor;
		color backColor;
		float opacity;
		bool noDisplay;
		bool needsUpdate;

		//light lighting;


		//light oldLight;									// compare with subsequent lighting to determine whether to refresh cell

		//enum dungeonLayers layerFlags;					// Designate which of the layers the cell inhabits
		//unsigned long flags;							// non-terrain cell flags
		//unsigned int volume;							// quantity of gas in cell

		//// REFACTORED VOLUME:  Separates gases
		//unsigned int poisonGasVolume;
		//unsigned int swampGasVolume;

		//unsigned char machineNumber;
		////brogueCellDisplay rememberedAppearance;			// how the player remembers the cell to look
		////enum itemCategory rememberedItemCategory;		// what category of item the player remembers lying there
		////int rememberedItemKind;                       // what kind of item the player remembers lying there
		////int rememberedItemQuantity;                   // how many of the item the player remembers lying there
		////enum tileType rememberedTerrain;				// what the player remembers as the terrain (i.e. highest priority terrain upon last seeing)
		////unsigned long rememberedCellFlags;              // map cell flags the player remembers from that spot
		////unsigned long rememberedTerrainFlags;           // terrain flags the player remembers from that spot
		////unsigned long rememberedTMFlags;                // TM flags the player remembers from that spot


		//// transient cell; stuff we don't need to remember between levels
		////
		//light light;									// RGB components of lighting
		//light oldLight;									// compare with subsequent lighting to determine whether to refresh cell

		brogueCellDisplay()
			: brogueCellDisplay(-1, -1)
		{
		}

		brogueCellDisplay(int column, int row)
			: brogueCellDisplay(column, row, colors::transparent())
		{
		}

		brogueCellDisplay(int column, int row, const color& abackColor)
			: brogueCellDisplay(column, row, abackColor, colors::transparent(), brogueGlyphMap::Empty)
		{
		}
		brogueCellDisplay(int column, int row, const color& abackColor, const color& aforeColor, char symbol)
		{
			this->column = column;
			this->row = row;
			character = symbol;
			foreColor = aforeColor;
			backColor = abackColor;
			opacity = 1.0f;
			needsUpdate = false;
			noDisplay = false;
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

		/// <summary>
		/// Sets UI parameteres of the cell; but not the location data
		/// </summary>
		void setUI(const brogueCellDisplay& other)
		{
			character = other.character;
			foreColor = other.foreColor; // Careful with instances. These are non-unique
			backColor = other.backColor; // Careful with instances. These are non-unique
			opacity = other.opacity;
			noDisplay = other.noDisplay;
		}

		bool compare(const brogueCellDisplay& display) const
		{
			return character == display.character &&
				foreColor.compare(display.foreColor) &&
				backColor.compare(display.backColor) &&
				opacity == display.opacity &&
				needsUpdate == display.needsUpdate &&
				noDisplay == display.noDisplay &&
				column == display.column &&
				row == display.row;
			//lighting.compare(display.lighting);
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(column, row, character, foreColor.getHash(), backColor.getHash(),
			                                   opacity, needsUpdate, noDisplay);
		}

	private:
		void copyImpl(const brogueCellDisplay& copy)
		{
			column = copy.column;
			row = copy.row;
			character = copy.character;
			foreColor = copy.foreColor; // Careful with instances. These are non-unique
			backColor = copy.backColor; // Careful with instances. These are non-unique
			opacity = copy.opacity;
			needsUpdate = copy.needsUpdate;
			noDisplay = copy.noDisplay;
		}
	};
}
