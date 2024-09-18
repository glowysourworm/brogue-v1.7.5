#include "brogueGame.h"
#include "brogueMessageQueue.h"

namespace brogueHd
{
	brogueGame::brogueGame(boolean serverMode, boolean noMenu)
	{
		_messageQueue = new brogueMessageQueue();

		_serverMode = serverMode;
		_noMenu = noMenu;
	}
	brogueGame::~brogueGame()
	{
		delete _messageQueue;
	}

	boolean brogueGame::getServerMode()
	{
		return _serverMode;
	}
	boolean brogueGame::getNoMenu()
	{
		return _noMenu;
	}

	void brogueGame::outputMessage(char* msg, color theColor, boolean requireAcknowledgment)
	{
		_messageQueue->addMessage(msg, theColor, requireAcknowledgment);
	}
}