#include "keyProcessor.h"
#include "brogueGlobal.h"
#include "broguefile.h"

namespace brogueHd::backend::processor
{
	keyProcessor::keyProcessor()
	{
		_keyMap = new simpleHash<simpleString, simpleString>();
	}

	keyProcessor::~keyProcessor()
	{
		delete _keyMap;
	}

	void keyProcessor::addKeyMap(const char* inputCharacter, const char* outputCharacter)
	{
		if (!_keyMap->contains(simpleString(inputCharacter)))
			_keyMap->add(simpleString(inputCharacter), simpleString(outputCharacter));
	}
}
