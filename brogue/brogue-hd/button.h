#pragma once

#include "broguedef.h"
#include "color.h"
#include "brogueCellDisplay.h"

using namespace brogueHd::backend::model::layout;

namespace brogueHd
{
	typedef struct brogueButton {
		char text[COLS * 3];			// button label; can include color escapes
		short x;					// button's leftmost cell will be drawn at (x, y)
		short y;
		signed long hotkey[10];		// up to 10 hotkeys to trigger the button
		color buttonColor;			// background of the button; further gradient-ized when displayed
		short opacity;				// further reduced by 50% if not enabled
		char symbol[COLS];			// Automatically replace the nth asterisk in the button label text with
									// the nth character supplied here, if one is given.
									// (Primarily to display magic character and item symbols in the inventory display.)
		unsigned long flags;
	} brogueButton;

	enum buttonDrawStates {
		BUTTON_NORMAL = 0,
		BUTTON_HOVER,
		BUTTON_PRESSED,
	};

	enum BUTTON_FLAGS {
		B_DRAW = Fl(0),
		B_ENABLED = Fl(1),
		B_GRADIENT = Fl(2),
		B_HOVER_ENABLED = Fl(3),
		B_WIDE_CLICK_AREA = Fl(4),
		B_KEYPRESS_HIGHLIGHT = Fl(5),
	};


	typedef struct buttonState {
		// Indices of the buttons that are doing stuff:
		short buttonFocused;
		short buttonDepressed;

		// Index of the selected button:
		short buttonChosen;

		// The buttons themselves:
		short buttonCount;
		brogueButton buttons[50];

		// The window location, to determine whether a click is a cancelation:
		short winX;
		short winY;
		short winWidth;
		short winHeight;

		// Graphical buffers:
		brogueCellDisplay dbuf[COLS][ROWS]; // Where buttons are drawn.
		brogueCellDisplay rbuf[COLS][ROWS]; // Reversion screen state.

	} buttonState;
}