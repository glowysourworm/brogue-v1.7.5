#pragma once

#include "brogueGlobal.h"
#include "brogueConsole.h"
#include "simpleString.h"

namespace brogueHd::console
{
	class developerConsole : public brogueConsole
	{
	public:
		developerConsole();
		~developerConsole();

		brogueConsoleReturn command(simpleString input, std::ostream& stream) override;
		void printHelp(std::ostream& stream) override;

		bool runUnitTests();
	};
}