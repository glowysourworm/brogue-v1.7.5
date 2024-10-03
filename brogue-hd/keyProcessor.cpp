#include "keyProcessor.h"
#include "brogueGlobal.h"
#include "broguefile.h"

namespace brogueHd::backend::processor
{
	keyProcessor::keyProcessor()
	{
		_keyMap = new simpleHash<std::string, std::string>();
	}

	keyProcessor::~keyProcessor()
	{
		delete _keyMap;
	}

	void keyProcessor::addKeyMap(const char* inputCharacter, const char* outputCharacter)
	{
		if (!_keyMap->contains(std::string(inputCharacter)))
			_keyMap->add(std::string(inputCharacter), std::string(outputCharacter));
	}
}
