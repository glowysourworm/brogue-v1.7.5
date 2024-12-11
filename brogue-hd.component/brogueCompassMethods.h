#pragma once
#include "gridDefinitions.h"

namespace brogueHd::component
{
	class brogueCompassMethods
	{
	public:
		static brogueCompass getOppositeDirection(brogueCompass direction)
		{
			switch (direction)
			{
			case brogueCompass::N:
				return brogueCompass::S;
			case brogueCompass::S:
				return brogueCompass::N;
			case brogueCompass::E:
				return brogueCompass::W;
			case brogueCompass::W:
				return brogueCompass::E;
			case brogueCompass::NE:
				return brogueCompass::SW;
			case brogueCompass::NW:
				return brogueCompass::SE;
			case brogueCompass::SE:
				return brogueCompass::NW;
			case brogueCompass::SW:
				return brogueCompass::NE;
			default:
				throw simpleException("Invalid compass direction:  brogueCompassMethods.h");
			}
		}
	};
}
