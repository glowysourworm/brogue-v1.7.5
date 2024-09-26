#pragma once

#include "broguedef.h"
#include "command.h"
#include <iostream>

using namespace std;

namespace brogueHd::console
{
	/// <summary>
	/// Defines different "consoles" to work with the main entry loop:  DEV, RESOURCE, GAME
	/// </summary>
	class brogueConsole
	{
	public:

		const char* BrogueVersion = BROGUE_VERSION_STRING;

	public:

		brogueConsole();
		~brogueConsole();

		virtual brogueConsoleReturn command(std::string input, ostream& stream);
		virtual void printHelp(ostream& stream);

		bool hasArgument(const std::string args[], const char* argumentName);

		template<typename T>
		T getArgument(const std::string args[], const char* argumentName);
	};
}

