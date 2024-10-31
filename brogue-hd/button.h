#pragma once

#include "simple.h"
#include "brogueGlobal.h"
#include "colorString.h"
#include "brogueCellDisplay.h"

using namespace brogueHd::backend::model::layout;

namespace brogueHd
{
	enum brogueTextAlignment
	{
		Left = 0,
		Center = 1,
		Right = 2
	};
	
	struct buttonData : hashable
	{
	public:

		buttonData()
		{
			textAlignment = brogueTextAlignment::Left;
		}
		buttonData(const char* buttonText, color gradient1, color gradient2, brogueTextAlignment alignment, short ahotKeyIndex)
		{
			text = colorString(buttonText, colors::white());
			background1 = gradient1;
			background2 = gradient2;
			hotkeyIndex = ahotKeyIndex;
			textAlignment = alignment;
		}
		buttonData(const buttonData& copy)
		{
			copyImpl(copy);
		}
		void operator=(const buttonData& copy)
		{
			copyImpl(copy);
		}
		bool operator==(const buttonData& other)
		{
			return compare(other);
		}
		bool operator!=(const buttonData& other)
		{
			return !compare(other);
		}

		colorString text;			// button label; can include color escapes
		short hotkeyIndex;			// Hot key index
		color background1;			// background of the button; further gradient-ized when displayed
		color background2;			// (gradient background)
		short opacity;				// further reduced by 50% if not enabled
		//char symbol[COLS];		// Automatically replace the nth asterisk in the button label text with
									// the nth character supplied here, if one is given.
									// (Primarily to display magic character and item symbols in the inventory display.)
		//unsigned long flags;

		brogueTextAlignment textAlignment;

		size_t getHash() const override
		{
			return text.getHash();
		}
	private:

		void copyImpl(const buttonData& copy)
		{
			text = copy.text;
			background1 = copy.background1;
			background2 = copy.background2;
			opacity = copy.opacity;
			textAlignment = copy.textAlignment;
			hotkeyIndex = copy.hotkeyIndex;
		}

		bool compare(const buttonData& other)
		{
			return text == other.text &&
				   background1 == other.background1 &&
				   background2 == other.background2 &&
				   opacity == other.opacity &&
				   textAlignment == other.textAlignment &&
				   hotkeyIndex == other.hotkeyIndex;
		}
	};

	//enum buttonDrawStates {
	//	BUTTON_NORMAL = 0,
	//	BUTTON_HOVER,
	//	BUTTON_PRESSED,
	//};

	//enum BUTTON_FLAGS {
	//	B_DRAW = Fl(0),
	//	B_ENABLED = Fl(1),
	//	B_GRADIENT = Fl(2),
	//	B_HOVER_ENABLED = Fl(3),
	//	B_WIDE_CLICK_AREA = Fl(4),
	//	B_KEYPRESS_HIGHLIGHT = Fl(5),
	//};


	//typedef struct buttonState {
	//	// Indices of the buttons that are doing stuff:
	//	short buttonFocused;
	//	short buttonDepressed;

	//	// Index of the selected button:
	//	short buttonChosen;

	//	// The buttons themselves:
	//	short buttonCount;
	//	brogueButton buttons[50];

	//	// The window location, to determine whether a click is a cancelation:
	//	short winX;
	//	short winY;
	//	short winWidth;
	//	short winHeight;

	//	// Graphical buffers:
	//	brogueCellDisplay dbuf[COLS][ROWS]; // Where buttons are drawn.
	//	brogueCellDisplay rbuf[COLS][ROWS]; // Reversion screen state.

	//} buttonState;
}