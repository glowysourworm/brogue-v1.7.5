#pragma once

#include "brogueConsole.h"
#include "command.h"
#include "resourceController.h"

using namespace brogueHd::backend::controller;

namespace brogueHd::console
{
	class resourceConsole : public brogueConsole
	{
	public:
		resourceConsole(resourceController* resourceController);
		~resourceConsole();

		brogueConsoleReturn command(std::string input, ostream& stream) override;
		void printHelp(ostream& stream) override;

	private:

		resourceController* _resourceController;
	};
}