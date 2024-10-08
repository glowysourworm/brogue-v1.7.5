#pragma once

#include "color.h"
#include "brogueGlobal.h"

namespace brogueHd::backend::model::game
{
	struct colorString 
	{

		char theString[COLS * 2];
		color theColors[COLS * 2];

		colorString()
		{
		}
		colorString(const char* message, color foreColor)
		{
			// Initialize arrays
			updateAll(message, foreColor);
		}

		void update(short index, char character, color color)
		{
			theString[index] = character;
			theColors[index] = color;
		}

		void updateAll(const char* message, color foreColor)
		{
			for (short index = 0; index < COLS * 2 && index < sizeof(message); index++)
			{
				theString[index] = message[index];
				theColors[index] = foreColor;
			}
		}
	};
}