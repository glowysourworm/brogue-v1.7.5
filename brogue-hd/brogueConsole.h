#pragma once

#include "brogueGlobal.h"
#include "simpleArray.h"
#include <iostream>

using namespace std;
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

		std::string consoleName;

	public:

		brogueConsole();
		~brogueConsole();

		virtual brogueConsoleReturn command(std::string input, ostream& stream);
		virtual void printHelp(ostream& stream);

		bool hasArgument(const simpleArray<std::string>& args, const char* argumentName);
		int getArgumentInt(const simpleArray<std::string>& args, const char* argumentName);
		bool getArgumentBool(const simpleArray<std::string>& args, const char* argumentName);
		std::string getArgument(const simpleArray<std::string>& args, const char* argumentName);
	};
}

