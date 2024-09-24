#pragma once

#include "brogueConsole.h"
#include "gameController.h"
#include "resourceController.h"
#include <iostream>

using namespace std;

using namespace brogueHd::backend::controller;

namespace brogueHd::console
{
	class gameConsole : public brogueConsole
	{
	public:
		gameConsole();
		gameConsole(resourceController* resourceController);
		~gameConsole();

		brogueConsoleReturn command(std::string input, ostream& stream);
		void printHelp(ostream& stream) override;

	private:

		void printScores(ostream& stream);

	private:

		resourceController* _resourceController;
		gameController* _gameController;
	};
}