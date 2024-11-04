#pragma once

#include "brogueConsole.h"
#include "brogueGlobal.h"
#include "simpleArrayTests.h"
#include "simpleComponentTests.h"
#include "simpleHashTests.h"
#include "simpleString.h"
#include <iosfwd>
#include <ostream>

using namespace brogueHd::test;

namespace brogueHd::console
{
	class developerConsole : public brogueConsole
	{
	public:
		developerConsole();
		~developerConsole();

		brogueConsoleReturn command(simpleString input, std::ostream& stream) override;
		void printHelp(std::ostream& stream) override;

		bool runUnitTests();
	};

	developerConsole::developerConsole()
	{
		this->consoleName = "Brogue Developer Console";
	}
	developerConsole::~developerConsole() {}

	brogueConsoleReturn developerConsole::command(simpleString input, std::ostream& stream)
	{
		int choice = 0;

		if (!input.tryToInt(choice))
			return brogueConsoleReturn::CompletedWithError;

		switch (choice)
		{
			case 1:
			{
				if (this->runUnitTests())
					return brogueConsoleReturn::Completed;

				else
					return brogueConsoleReturn::CompletedWithError;
			}
			break;
			default:
				return brogueConsoleReturn::Completed;
		}
	}

	void developerConsole::printHelp(std::ostream& stream)
	{
		stream << "Brogue Developer Console:" << std::endl << std::endl;
		stream << "\t1) Run Unit Tests" << std::endl;
		stream << "\tElse) Exit" << std::endl;
	}

	bool developerConsole::runUnitTests()
	{
		// Brogue Tests map output to the console using brogueLogger
		simpleArrayTests arrayTests;
		simpleHashTests hashTests;
		simpleComponentTests componentTests;

		bool result = arrayTests.run();
		result &= hashTests.run();
		result &= componentTests.run();

		return result;
	}
}