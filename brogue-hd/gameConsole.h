#pragma once

#include "brogueConsole.h"
#include "gameController.h"
#include "resourceController.h"
#include <iostream>

using namespace brogueHd::backend::controller;

namespace brogueHd::console
{
	class gameConsole : public brogueConsole
	{
	public:
		gameConsole();
		gameConsole(resourceController* resourceController);
		~gameConsole();

		brogueConsoleReturn command(std::string input, std::ostream& stream);
		void printHelp(std::ostream& stream) override;

	private:

		void printScores(std::ostream& stream);

	private:

		resourceController* _resourceController;
		gameController* _gameController;
	};
}