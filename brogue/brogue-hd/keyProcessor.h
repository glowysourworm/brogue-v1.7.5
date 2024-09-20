#pragma once

#include "broguefile.h"

using namespace std;

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
		void addKeyMap(char* inputCharacter, char* outputCharacter);

	private:

		std::map<std::string, std::string> _keyMap;
	};
}

