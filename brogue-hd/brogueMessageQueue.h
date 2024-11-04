#pragma once

#include "brogueGlobal.h"
#include "color.h"
#include "messageData.h"
#include "simple.h"
#include "simpleList.h"

using namespace brogueHd::simple;
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

	brogueMessageQueue::brogueMessageQueue()
	{
		// TODO: get these from catalogs
		color whiteBrogue(1, 1, 1, 1);

		//_messages = new simpleList<messageData>();
		_flavorMessage = messageData("You are here...", whiteBrogue, true);
	}

	brogueMessageQueue::~brogueMessageQueue()
	{
		_messages->clear();
		_currentMessageIndex = -1;
	}

	void brogueMessageQueue::addMessage(char* message, color textColor, bool needsConfirmation)
	{
		// Check message limit
		if (_messages->count() == MESSAGE_ARCHIVE_LINES - 1)
			_messages->removeAt(_messages->count() - 1);

		// Insert message at the front
		_messages->insert(0, messageData(message, textColor, !needsConfirmation));

		// Check confirmation before auto-advancing
		if (!needsConfirmation)
		{
			// Auto-confirm all messages
			_messages->forEach([] (messageData amessage)
			{
				amessage.confirmed = true;

				return iterationCallback::iterate;
			});

			// Reset the message index
			_currentMessageIndex = 0;
		}

		else
		{
			// Check queue from the back and stop where confirmation hasn't been received
			for (short index = _messages->count(); index >= 0; index--)
			{
				// Update current index
				if (!_messages->get(index).confirmed)
				{
					_currentMessageIndex = index;
					break;
				}
			}
		}
	}

	bool brogueMessageQueue::confirmAndAdvance(short count)
	{
		short index = _currentMessageIndex;

		for (index = _currentMessageIndex; index >= (_currentMessageIndex - count) && index >= 0; index--)
		{
			messageData data = _messages->get(index);

			data.confirmed = true;
		}

		_currentMessageIndex = index;

		return true;
	}

	void brogueMessageQueue::updateFlavorText(char* message, color foreColor)
	{
		_flavorMessage.update(message, foreColor, true);
	}
}

