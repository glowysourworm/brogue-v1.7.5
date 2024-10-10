#pragma once

#include "broguefile.h"
#include "simpleHash.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::backend::processor
{
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
}

