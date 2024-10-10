#pragma once

#include "brogueConsole.h"
#include "brogueGlobal.h"
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

		brogueConsoleReturn command(simpleString input, std::ostream& stream) override;
		void printHelp(std::ostream& stream) override;

	private:

		resourceController* _resourceController;
	};
}