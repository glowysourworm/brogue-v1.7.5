#pragma once

typedef struct rogueEvent {
	enum eventTypes eventType;
	signed long param1;
	signed long param2;
	boolean controlKey;
	boolean shiftKey;
} rogueEvent;

typedef struct rogueHighScoresEntry {
	signed long score;
	char date[100];
	char description[DCOLS];
} rogueHighScoresEntry;

typedef struct fileEntry {
	char* path;
	char date[100];
} fileEntry;
