#include "broguedef.h"
#include "gameConsole.h"
#include "developerConsole.h"
#include "resourceConsole.h"
#include "resourceController.h"
#include "stringExtension.h"
#include <iostream>
#include <string>

using namespace std;
using namespace brogueHd::console;
using namespace brogueHd::component;
using namespace brogueHd::backend::controller;

namespace brogueHd
{
	int main(int argc, char* argv[])
	{
		brogueConsole defaultConsole;
		brogueConsole currentConsole = defaultConsole;
		brogueConsoleReturn returnValue = brogueConsoleReturn::Continue;
		bool iterate = true;

		// Backend Components
		resourceController* brogueResourceController = new resourceController();

		std::string cmd = stringExtension::join(argv);

		while (returnValue != brogueConsoleReturn::Exit && iterate)
		{
			// Read console line
			std::getline(std::cin, cmd);

			// Pass command to the console component
			returnValue = currentConsole.command(cmd, std::cout);

			switch (returnValue)
			{
			case brogueConsoleReturn::Continue:
			case brogueConsoleReturn::Completed:
				currentConsole = defaultConsole;
				break;
			case brogueConsoleReturn::Completed_SetMode_Game:
				currentConsole = gameConsole(brogueResourceController);
				break;
			case brogueConsoleReturn::Completed_SetMode_Dev:
				currentConsole = developerConsole();
				break;
			case brogueConsoleReturn::Completed_SetMode_Resource:
				currentConsole = resourceConsole(brogueResourceController);
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
				currentConsole.printHelp(std::cout);
		}

		delete brogueResourceController;

		return 0;
	}
}

