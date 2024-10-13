#pragma once

#include "simple.h"
#include "color.h"
#include "brogueGlobal.h"

namespace brogueHd::backend::model::game
{
	struct colorString : hashable
	{
	public:

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

		size_t getHash() const override
		{
			size_t hash = 0;

			for (int index = 0; index < COLS * 2; index++)
			{
				if (hash == 0)
					hash = hashGenerator::generateHash(theString[index], theColors[index]);

				else
					hash = hashGenerator::combineHash(hash, theString[index], theColors[index]);
			}

			return hash;
		}
	};
}