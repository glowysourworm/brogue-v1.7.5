#pragma once

#include "broguedef.h"
#include "broguefile.h"

using namespace std;

namespace brogueHd
{
	/// <summary>
	/// Data structure for recording info:  used during playback mode
	/// </summary>
	typedef struct playbackData {
		std::string filePath;				// path to playback file
		brogueEventFile recordedEvents;		// events from the playback file
		boolean playbackMode;				// whether we're viewing a recording instead of playing
		unsigned long currentTurnNumber;	// how many turns have elapsed
		unsigned long howManyTurns;			// how many turns are in this recording
		short howManyDepthChanges;			// how many times the player changes depths
		short playbackDelayPerTurn;			// base playback speed; modified per turn by events
		short playbackDelayThisTurn;		// playback speed as modified
		boolean playbackPaused;
		boolean playbackFastForward;		// for loading saved games and such -- disables drawing and prevents pauses
		boolean playbackOOS;				// playback out of sync -- no unpausing allowed
		boolean playbackOmniscience;		// whether to reveal all the map during playback
		boolean playbackBetweenTurns;		// i.e. waiting for a top-level input -- iff, permit playback commands
	} playbackData;

}

