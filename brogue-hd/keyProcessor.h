#pragma once

#include "simpleHash.h"
#include "simpleString.h"

namespace brogueHd::backend
{
	using namespace simple;

	class keyProcessor
	{
	public:
		keyProcessor();
		~keyProcessor();

		/// <summary>
		/// Adds mapping for key stroke processing
		/// </summary>
		void addKeyMap(const char* inputCharacter, const char* outputCharacter);

	private:
		simpleHash<simpleString, simpleString>* _keyMap;
	};

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
