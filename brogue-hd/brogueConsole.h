#pragma once

#include "brogueGlobal.h"
#include "simpleArray.h"
#include <iostream>

using namespace brogueHd::component;

namespace brogueHd::console
{
	/// <summary>
	/// Defines different "consoles" to work with the main entry loop:  DEV, RESOURCE, GAME
	/// </summary>
	class brogueConsole
	{
	public:

		const char* BrogueVersion = BROGUE_VERSION_STRING;

		simpleString consoleName;

	public:

		brogueConsole();
		~brogueConsole();

		virtual brogueConsoleReturn command(simpleString input, std::ostream& stream);
		virtual void printHelp(std::ostream& stream);

		bool hasArgument(const simpleArray<simpleString>& args, const char* argumentName);
		int getArgumentInt(const simpleArray<simpleString>& args, const char* argumentName);
		bool getArgumentBool(const simpleArray<simpleString>& args, const char* argumentName);
		simpleString getArgument(const simpleArray<simpleString>& args, const char* argumentName);
	};
}

