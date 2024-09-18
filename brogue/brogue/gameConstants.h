#pragma once

enum eventTypes {
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

enum RNGs {
	RNG_SUBSTANTIVE,
	RNG_COSMETIC,
	NUMBER_OF_RNGS,
};

enum displayDetailValues {
	DV_UNLIT = 0,
	DV_LIT,
	DV_DARK,
};

enum directions {
	NO_DIRECTION = -1,
	// Cardinal directions; must be 0-3:
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
	// Secondary directions; must be 4-7:
	UPLEFT = 4,
	DOWNLEFT = 5,
	UPRIGHT = 6,
	DOWNRIGHT = 7,

	DIRECTION_COUNT = 8,
};

enum textEntryTypes {
	TEXT_INPUT_NORMAL = 0,
	TEXT_INPUT_FILENAME,
	TEXT_INPUT_NUMBERS,
	TEXT_INPUT_TYPES,
};