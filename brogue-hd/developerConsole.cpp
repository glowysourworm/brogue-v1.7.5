#include "developerConsole.h"
#include "typeConverter.h"

namespace brogueHd::console
{
	developerConsole::developerConsole() 
	{
		this->consoleName = "Brogue Developer Console";
	}
	developerConsole::~developerConsole() {}

	brogueConsoleReturn developerConsole::command(std::string input, ostream& stream)
	{
		int choice = 0;
		
		if (!typeConverter::tryStringToInt(input, choice))
			return brogueConsoleReturn::CompletedWithError;

		switch (choice)
		{
		case 1:
		{
			stream << "Running unit tests..." << std::endl;
			return brogueConsoleReturn::Completed;
		}
		break;
		default:
			return brogueConsoleReturn::Completed;
		}
	}

	void developerConsole::printHelp(ostream& stream)
	{
		stream << "Brogue Developer Console:" << std::endl << std::endl;
		stream << "\t1) Run Unit Tests" << std::endl;
		stream << "\tElse) Exit" << std::endl;
	}
}

