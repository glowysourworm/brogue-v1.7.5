#include "brogueMessageQueue.h"
#include "broguedef.h"
#include "color.h"
#include "colorConstants.h"
#include "vectordef.h"
#include <vector>

using namespace std;
using namespace brogueHd::collection;

namespace brogueHd
{
	brogueMessageQueue::brogueMessageQueue()
	{
		_messages = std::vector<messageData*>(MESSAGE_ARCHIVE_LINES);
		_flavorMessage = new messageData("You are here...", whiteBrogue, true);
	}

	brogueMessageQueue::~brogueMessageQueue()
	{
		forEach(_messages, [](messageData* message) 
		{
			delete message;
		});

		_messages.clear();
		_currentMessageIndex = -1;
	}

	void brogueMessageQueue::addMessage(char* message, color textColor, bool needsConfirmation)
	{
		// Check message limit
		if (_messages.size() == _messages.max_size() - 1)
			_messages.pop_back();

		// Insert message at the front
		_messages.insert(_messages.begin(), new messageData(message, textColor, !needsConfirmation));

		// Check confirmation before auto-advancing
		if (!needsConfirmation)
		{
			// Auto-confirm all messages
			forEach(_messages, [](messageData* amessage)
			{
				amessage->confirmed = true;
			});

			// Reset the message index
			_currentMessageIndex = 0;
		}

		else
		{
			// Check queue from the back and stop where confirmation hasn't been received
			for (short index = _messages.size(); index >= 0; index--)
			{
				// Update current index
				if (!_messages[index]->confirmed)
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
			_messages[index]->confirmed = true;
		}

		_currentMessageIndex = index;
	}

	void brogueMessageQueue::updateFlavorText(char* message, color foreColor)
	{
		_flavorMessage->update(message, foreColor, true);
	}
}