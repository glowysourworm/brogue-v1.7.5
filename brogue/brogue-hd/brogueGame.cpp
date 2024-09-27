#include "brogueGame.h"
#include "brogueMessageQueue.h"

namespace brogueHd::backend::model
{
	brogueGame::brogueGame(bool serverMode, bool noMenu)
	{
		_messageQueue = new brogueMessageQueue();

		_serverMode = serverMode;
		_noMenu = noMenu;
	}
	brogueGame::~brogueGame()
	{
		delete _messageQueue;
	}

	bool brogueGame::getServerMode()
	{
		return _serverMode;
	}
	bool brogueGame::getNoMenu()
	{
		return _noMenu;
	}

	void brogueGame::outputMessage(char* msg, color theColor, bool requireAcknowledgment)
	{
		_messageQueue->addMessage(msg, theColor, requireAcknowledgment);
	}
}