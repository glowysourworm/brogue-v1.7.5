#include "developerConsole.h"

namespace brogueHd::console
{
	developerConsole::developerConsole() {}
	developerConsole::~developerConsole() {}

	brogueConsoleReturn developerConsole::command(std::string input, ostream& stream)
	{
		return brogueConsoleReturn::Completed;	
	}

	void developerConsole::printHelp(ostream& stream)
	{
		stream << "TODO: developer console" << std::endl;
	}
}

