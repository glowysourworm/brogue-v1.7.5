#pragma once

#include "brogueConsole.h"
#include "command.h"

namespace brogueHd::console
{
	class resourceConsole : public brogueConsole
	{
	public:
		resourceConsole();
		~resourceConsole();

		brogueConsoleReturn command(std::string input, ostream& stream) override;
		void printHelp(ostream& stream) override;
	};
}