#pragma once

#include "broguefile.h"
#include "simpleHash.h"
#include <string>

using namespace brogueHd::component;

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

		simpleHash<std::string, std::string>* _keyMap;
	};
}

