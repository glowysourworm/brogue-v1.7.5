#pragma once

#include "brogueGlobal.h"
#include "simpleArray.h"
#include "simpleString.h"
#include <iosfwd>
#include <iostream>

namespace brogueHd::console
{
	using namespace simple;

	/// <summary>
	/// Defines different "consoles" to work with the main entry loop:  DEV, RESOURCE, GAME
	/// </summary>
	class brogueConsole
	{
	public:
		const char* BrogueVersion = BROGUE_VERSION_STRING;

		simpleString consoleName;

	public:
		brogueConsole();
		~brogueConsole();

		virtual brogueConsoleReturn command(simpleString input, std::ostream& stream);
		virtual void printHelp(std::ostream& stream);

		bool hasArgument(const simpleArray<simpleString>& args, const char* argumentName);
		int getArgumentInt(const simpleArray<simpleString>& args, const char* argumentName);
		bool getArgumentBool(const simpleArray<simpleString>& args, const char* argumentName);
		simpleString getArgument(const simpleArray<simpleString>& args, const char* argumentName);
	};

	brogueConsole::brogueConsole()
	{
		this->consoleName = "Brogue";
	}

	brogueConsole::~brogueConsole()
	{
	}

	brogueConsoleReturn brogueConsole::command(simpleString input, std::ostream& stream)
	{
		int choice = 0;

		if (!input.tryToInt(choice))
			return brogueConsoleReturn::CompletedWithError;

		switch (choice)
		{
		case 1: return brogueConsoleReturn::Completed_SetMode_Game;
		case 2: return brogueConsoleReturn::Completed_SetMode_Dev;
		case 3: return brogueConsoleReturn::Completed_SetMode_Resource;
		default:
			return brogueConsoleReturn::Exit;
		}
	}

	void brogueConsole::printHelp(std::ostream& stream)
	{
		stream << "Brogue Command Line:  Welcome to the Brogue command terminal!" << std::endl << std::endl;
		stream << "\t1) Game Console (start, open, .. game files)" << std::endl;
		stream << "\t2) Developer Console (run unit tests, debug)" << std::endl;
		stream << "\t3) Resources Console (compile, import, export resource files)" << std::endl;
		stream << "\tElse) Exit" << std::endl;
	}

	bool brogueConsole::hasArgument(const simpleArray<simpleString>& args, const char* argumentName)
	{
		for (int index = 0; index < args.count(); index++)
		{
			if (args.get(index) == simpleString(argumentName))
				return true;
		}

		return false;
	}

	simpleString brogueConsole::getArgument(const simpleArray<simpleString>& args, const char* argumentName)
	{
		for (int index = 0; index < args.count(); index++)
		{
			if (args.get(index) == simpleString(argumentName) &&
				index < args.count() + 1)
			{
				return args.get(index + 1);
			}
		}

		return "";
	}

	int brogueConsole::getArgumentInt(const simpleArray<simpleString>& args, const char* argumentName)
	{
		for (int index = 0; index < args.count(); index++)
		{
			if (args.get(index) == simpleString(argumentName) &&
				index < args.count() + 1)
			{
				return args.get(index + 1).toInt();
			}
		}

		return 0;
	}

	bool brogueConsole::getArgumentBool(const simpleArray<simpleString>& args, const char* argumentName)
	{
		for (int index = 0; index < args.count(); index++)
		{
			if (args.get(index) == simpleString(argumentName) &&
				index < args.count() + 1)
			{
				return args.get(index + 1).toBool();
			}
		}

		return false;
	}
}
