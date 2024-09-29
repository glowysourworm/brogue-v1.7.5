#pragma once

namespace brogueHd::backend::modelConstant
{
	enum BrogueEventType : int {
		KEYSTROKE,
		MOUSE_UP,
		MOUSE_DOWN,
		RIGHT_MOUSE_DOWN,
		RIGHT_MOUSE_UP,
		MOUSE_ENTERED_CELL,
		RNG_CHECK,
		SAVED_GAME_LOADED,
		END_OF_RECORDING,
		EVENT_ERROR,
		NUMBER_OF_EVENT_TYPES, // unused
	};
}