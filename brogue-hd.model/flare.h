#pragma once

#include "light.h"

namespace brogueHd::model
{
	struct flare
	{
		lightSource light; // Flare light
		int coeffChangeAmount;
		// The constant amount by which the coefficient changes per frame, e.g. -25 means it gets 25% dimmer per frame.
		int coeffLimit; // Flare ends if the coefficient passes this percentage (whether going up or down).
		int xLoc, yLoc; // Current flare location.
		long coeff; // Current flare coefficient; always starts at 100.
		unsigned long turnNumber; // So we can eliminate those that fired one or more turns ago.
	};
}
