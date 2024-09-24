#include "keyProcessor.h"
#include "broguedef.h"
#include "broguefile.h"

using namespace std;

namespace brogueHd::backend::processor
{
	keyProcessor::keyProcessor()
	{
		_keyMap = new std::map<std::string, std::string>();
	}

	keyProcessor::~keyProcessor()
	{
		delete _keyMap;
	}

	void keyProcessor::addKeyMap(char* inputCharacter, char* outputCharacter)
	{
		if (!_keyMap->contains(std::string(inputCharacter)))
			_keyMap->insert(std::string(inputCharacter), std::string(outputCharacter));
	}
}
