#pragma once

#include "broguefile.h"
#include "simpleString.h"

namespace brogueHd::backend::model
{
	using namespace simple;

	/// <summary>
	/// Data structure for recording info:  used during playback mode
	/// </summary>
	typedef struct playbackData
	{
		simpleString filePath; // path to playback file
		brogueEventFile recordedEvents; // events from the playback file
		bool playbackMode; // whether we're viewing a recording instead of playing
		unsigned long currentTurnNumber; // how many turns have elapsed
		unsigned long howManyTurns; // how many turns are in this recording
		int howManyDepthChanges; // how many times the player changes depths
		int playbackDelayPerTurn; // base playback speed; modified per turn by events
		int playbackDelayThisTurn; // playback speed as modified
		bool playbackPaused;
		bool playbackFastForward; // for loading saved games and such -- disables drawing and prevents pauses
		bool playbackOOS; // playback out of sync -- no unpausing allowed
		bool playbackOmniscience; // whether to reveal all the map during playback
		bool playbackBetweenTurns; // i.e. waiting for a top-level input -- iff, permit playback commands
	} playbackData;
}
