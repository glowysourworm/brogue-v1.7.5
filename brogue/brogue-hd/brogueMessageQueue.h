#pragma once

#include "brogueMacros.h"
#include "messageData.h"
#include "color.h"
#include <simpleList.h>
#include <string>

using namespace std;

using namespace brogueHd::component;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model
{
	class brogueMessageQueue
	{
	public:

		brogueMessageQueue();
		~brogueMessageQueue();

		/// <summary>
		/// Adds message to the queue. If the message does NOT need confirmation, then the queue
		/// advances to the front (assumes no more confirmation needed)
		/// </summary>
		void addMessage(char* message, color textColor, bool needsConfirmation);

		/// <summary>
		/// Advances message queue to the count specified or until the last message has
		/// been processed. Confirms all intermediate messages
		/// </summary>
		bool confirmAndAdvance(short count);

		void updateFlavorText(char* message, color foreColor);

	private:

		short _currentMessageIndex;

		simpleList<messageData>* _messages;

		messageData _flavorMessage;

		//messageData messageArchive[MESSAGE_ARCHIVE_LINES];

		//short messageArchivePosition;

		//std::string currentFilePath;

		// char displayDetail[DCOLS][DROWS];		// used to make certain per-cell data accessible to external code (e.g. terminal adaptations)

		//unsigned char inputRecordBuffer[INPUT_RECORD_BUFFER + 100];
		//unsigned short locationInRecordingBuffer;
		//unsigned long randomNumbersGenerated;
		//unsigned long positionInPlaybackFile;
		//unsigned long lengthOfPlaybackFile;
		//unsigned long recordingLocation;
		//unsigned long maxLevelChanges;
		//char annotationPathname[BROGUE_FILENAME_MAX];	// pathname of annotation file
		//unsigned long gameSeed;
	};
}

