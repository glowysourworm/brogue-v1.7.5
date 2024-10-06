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

	if (argc < 2)
	{
		std::cout << "Must include the resource-config.json file path in the command line (first parameter)" << std::endl;
		return 1;
	}

	// Backend Components
	resourceController* brogueResourceController = new resourceController();

	if (!brogueResourceController->initialize(argv[1]))
	{
		std::cout << "Must include the resource-config.json file path in the command line (first parameter)" << std::endl;
		return 1;
	}

	simpleString cmd = "";

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
		else
		{
			if (currentConsole != defaultConsole)
				delete currentConsole;

			delete brogueResourceController;
			delete defaultConsole;
			return 0;
		}

		// Read console line
		std::cout << std::endl << currentConsole->consoleName << " @> ";
		std::string cmdStr;
		std::getline(std::cin, cmdStr);

		// Didn't want to rewrite getline..
		cmd = cmdStr.c_str();
	}

	if (currentConsole != defaultConsole)
		delete currentConsole;

	delete brogueResourceController;
	delete defaultConsole;

	return 0;
}

