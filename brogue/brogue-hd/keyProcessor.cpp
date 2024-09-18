#include "keyProcessor.h"
#include "broguedef.h"
#include "broguefile.h"

using namespace std;

namespace brogueHd
{
	keyProcessor::keyProcessor()
	{
		_keyMap = new keyMap[KEY_ID_MAXIMUM];
	}

	keyProcessor::~keyProcessor()
	{
		delete _keyMap;
	}

	void keyProcessor::addKeyMap(char* inputCharacter, char* outputCharacter)
	{
		if (_keyMap.find(inputCharacter) == NULL)
			_keyMap.insert(inputCharacter, outputCharacter);
	}
}
