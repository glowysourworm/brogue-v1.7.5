#include "brogueGlobal.h"
#include "developerConsole.h"
#include "gameConsole.h"
#include "resourceConsole.h"
#include "resourceController.h"
#include <iostream>


#include "brogueConsole.h"
#include "simpleString.h"
#include <SDL_main.h>

using namespace brogueHd;
using namespace simple;
using namespace simple::math;
using namespace brogueHd::console;
using namespace brogueHd::component;
using namespace brogueHd::backend;

int main(int argc, char* argv[])
{
	brogueConsole* defaultConsole = new brogueConsole();
	brogueConsole* currentConsole = defaultConsole;
	brogueConsoleReturn returnValue = brogueConsoleReturn::Continue;
	bool iterate = true;

	if (argc < 2)
	{
		std::cout << "Must include the resource-config.json file path in the command line (first parameter)" <<
			std::endl;
		return 1;
	}

	// Backend Components
	resourceController* brogueResourceController = new resourceController();

	if (!brogueResourceController->initialize(argv[1]))
	{
		std::cout << "Must include the resource-config.json file path in the command line (first parameter)" <<
			std::endl;
		return 1;
	}

	simpleString cmd = "";

	while (returnValue != brogueConsoleReturn::Exit && iterate)
	{
		// Pass command to the console component
		if (!cmd.isEmptyOrWhiteSpace())
		{
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
		}

		// Print Help for the menu loop
		if (iterate)
			currentConsole->printHelp(std::cout);
		else
		{
			if (currentConsole != defaultConsole)
				delete currentConsole;

			delete brogueResourceController;
			delete defaultConsole;
			return 0;
		}

		cmd.clear();

		// Read console line
		std::cout << std::endl << currentConsole->consoleName << " @> ";
		std::cin >> cmd;
	}

	if (currentConsole != defaultConsole)
		delete currentConsole;

	delete brogueResourceController;
	delete defaultConsole;

	return 0;
}
