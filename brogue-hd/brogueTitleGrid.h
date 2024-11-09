#pragma once
#include "brogueGlobal.h"
#include "gridRect.h"
#include <cctype>

using namespace brogueHd::component;

namespace brogueHd::frontend
{
	class brogueTitleGrid
	{
	public:

		//
		// Brogue v1.7.5 (Brian Walker a.k.a. penderprime) (preserved as well as possible)
		//
		brogueTitleGrid() {}
		~brogueTitleGrid() {}

		gridRect textBounds()
		{
			// Took one off of this guy because of the odd width of the title.
			// It's just the location of the bounding box - to center the text.
			//
			return   gridRect((COLS - MENU_TITLE_WIDTH) / 2,
							  (ROWS - MENU_TITLE_HEIGHT) / 2,
							  MENU_TITLE_WIDTH,
							  MENU_TITLE_HEIGHT);
		}
		gridRect sceneBounds()
		{
			return gridRect(0, 0, COLS, ROWS);
		}

		bool isTheText(short column, short row)
		{
			gridRect theTextBounds = this->textBounds();

			if (!theTextBounds.contains(column, row))
				return false;

			// Take off the odd column on the right
			if (column == theTextBounds.right())
				return false;

			// THESE INDICIES ARE FLIPPED:  ROW-FIRST
			return !(std::isspace(this->Title[row - theTextBounds.top()][column - theTextBounds.left()]) > 0);
		}

		const char Title[MENU_TITLE_HEIGHT][MENU_TITLE_WIDTH] = {
			"########   ########       ######         #######   ####     ###  #########",
			" ##   ###   ##   ###    ##     ###     ##      ##   ##       #    ##     #",
			" ##    ##   ##    ##   ##       ###   ##        #   ##       #    ##     #",
			" ##    ##   ##    ##   #    #    ##   #         #   ##       #    ##      ",
			" ##    ##   ##    ##  ##   ##     ## ##             ##       #    ##    # ",
			" ##   ##    ##   ##   ##   ###    ## ##             ##       #    ##    # ",
			" ######     ## ###    ##   ####   ## ##             ##       #    ####### ",
			" ##    ##   ##  ##    ##   ####   ## ##             ##       #    ##    # ",
			" ##     ##  ##   ##   ##    ###   ## ##      #####  ##       #    ##    # ",
			" ##     ##  ##   ##   ###    ##   ## ###       ##   ##       #    ##      ",
			" ##     ##  ##    ##   ##    #    #   ##       ##   ##       #    ##      ",
			" ##     ##  ##    ##   ###       ##   ###      ##   ###      #    ##     #",
			" ##    ##   ##     ##   ###     ##     ###    ###    ###    #     ##     #",
			"########   ####    ###    ######         #####        ######     #########",
			"                            ##                                            ",
			"                        ##########                                        ",
			"                            ##                                            ",
			"                            ##                                            ",
			"                           ####                                           ",
		};


	};
}