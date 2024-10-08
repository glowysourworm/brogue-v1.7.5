#include "brogueMessageQueue.h"
#include "brogueGlobal.h"
#include "color.h"
#include "colorConstants.h"
#include "simpleList.h"

using namespace brogueHd::simple;

namespace brogueHd::backend::model
{
	brogueMessageQueue::brogueMessageQueue()
	{
		// TODO: get these from catalogs
		color whiteBrogue(255, 255, 255, 0,0, 0, 0, false);

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
			_messages->forEach([](messageData amessage)
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