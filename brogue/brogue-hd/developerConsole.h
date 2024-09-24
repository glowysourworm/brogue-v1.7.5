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

		brogueConsoleReturn command(std::string input, ostream& stream) override;
		void printHelp(ostream& stream) override;
	};
}