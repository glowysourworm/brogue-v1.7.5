#pragma once

#include "brogueConsole.h"
#include "command.h"

namespace brogueHd::console
{
	class developerConsole : public brogueConsole
	{
	public:
		developerConsole();
		~developerConsole();

		brogueConsoleReturn command(std::string input, std::ostream& stream) override;
		void printHelp(std::ostream& stream) override;

		bool runUnitTests();
	};
}