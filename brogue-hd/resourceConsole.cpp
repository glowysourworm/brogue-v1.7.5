#include "resourceConsole.h"
#include "typeConverter.h"

using namespace std;

using namespace brogueHd::component;

namespace brogueHd::console
{
	resourceConsole::resourceConsole(resourceController* resourceController)
	{
		_resourceController = resourceController;

		this->consoleName = "Brogue Resource Console";
	}
	resourceConsole::~resourceConsole() {}

	brogueConsoleReturn resourceConsole::command(std::string input, ostream& stream)
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
			return brogueConsoleReturn::Exit;
		}

		return brogueConsoleReturn::Completed;
	}
	void resourceConsole::printHelp(ostream& stream)
	{
		stream << "Brogue Resource Console:" << std::endl << std::endl;
		stream << "\t1) Load Colors File" << std::endl;
		stream << "\tElse) Exit" << std::endl;
	}
}