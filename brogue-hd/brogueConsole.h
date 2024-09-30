#pragma once

#include "brogueGlobal.h"
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

		int getArgumentInt(const std::string args[], const char* argumentName);
		bool getArgumentBool(const std::string args[], const char* argumentName);
		std::string getArgument(const std::string args[], const char* argumentName);
	};
}

