#pragma once

#include "brogueConsole.h"
#include "command.h"
#include "resourceController.h"
#include <iostream>

using namespace brogueHd::backend::controller;

namespace brogueHd::console
{
	class resourceConsole : public brogueConsole
	{
	public:
		resourceConsole(resourceController* resourceController);
		~resourceConsole();

		brogueConsoleReturn command(std::string input, std::ostream& stream) override;
		void printHelp(std::ostream& stream) override;

	private:

		resourceController* _resourceController;
	};
}