#include "brogueGlobal.h"
#include "gameConsole.h"
#include "developerConsole.h"
#include "resourceConsole.h"
#include "resourceController.h"
#include "simpleHash.h"
#include "stringExtension.h"
#include <iostream>
#include <string>

#include <functional>

using namespace std;
using namespace brogueHd;
using namespace brogueHd::console;
using namespace brogueHd::component;
using namespace brogueHd::backend::controller;
using namespace brogueHd::backend::model;

int main(int argc, char* argv[])
{
	brogueConsole* defaultConsole = new brogueConsole();
	brogueConsole* currentConsole = defaultConsole;
	brogueConsoleReturn returnValue = brogueConsoleReturn::Continue;
	bool iterate = true;

	// Backend Components
	resourceController* brogueResourceController = new resourceController();

	std::string cmd = stringExtension::join(argv, argc);

	while (returnValue != brogueConsoleReturn::Exit && iterate)
	{
		// Pass command to the console component
		returnValue = currentConsole->command(cmd, std::cout);

		switch (returnValue)
		{
		case brogueConsoleReturn::Continue:
		case brogueConsoleReturn::Completed:
			currentConsole = defaultConsole;
			break;
		case brogueConsoleReturn::Completed_SetMode_Game:
		{
			if (currentConsole != defaultConsole)
				delete currentConsole;

			currentConsole = new gameConsole(brogueResourceController);
		}
		break;
		case brogueConsoleReturn::Completed_SetMode_Dev:
		{
			if (currentConsole != defaultConsole)
				delete currentConsole;

			currentConsole = new developerConsole();
		}
		break;
		case brogueConsoleReturn::Completed_SetMode_Resource:
		{
			if (currentConsole != defaultConsole)
				delete currentConsole;

			currentConsole = new resourceConsole(brogueResourceController);
		}
		break;
		case brogueConsoleReturn::CompletedWithError:
			break;
		case brogueConsoleReturn::Exit:
		default:
			iterate = false;
			break;
		}

		// Print Help for the menu loop
		if (iterate)
			currentConsole->printHelp(std::cout);

		// Read console line
		std::getline(std::cin, cmd);
	}

	delete brogueResourceController;
	delete currentConsole;
	delete defaultConsole;

	return 0;
}

