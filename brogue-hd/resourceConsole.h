#pragma once

#include "brogueConsole.h"
#include "brogueGlobal.h"
#include "resourceController.h"
#include "simpleString.h"
#include <iosfwd>
#include <iostream>

using namespace brogueHd::simple;
using namespace brogueHd::backend;

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

	resourceConsole::resourceConsole(resourceController* resourceController)
	{
		_resourceController = resourceController;

		this->consoleName = "Brogue Resource Console";
	}
	resourceConsole::~resourceConsole() {}

	brogueConsoleReturn resourceConsole::command(simpleString input, std::ostream& stream)
	{
		int choice = 0;

		if (!input.tryToInt(choice))
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
	void resourceConsole::printHelp(std::ostream& stream)
	{
		stream << "Brogue Resource Console:" << std::endl << std::endl;
		stream << "\t1) Load Colors File" << std::endl;
		stream << "\tElse) Exit" << std::endl;
	}
}