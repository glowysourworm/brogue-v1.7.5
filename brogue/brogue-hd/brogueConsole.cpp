#include "brogueConsole.h"
#include "command.h"
#include "exceptionHandler.h"
#include "stringExtension.h"
#include "typeConverter.h"

using namespace brogueHd::component;

namespace brogueHd::console
{
	brogueConsole::brogueConsole(){}
	brogueConsole::~brogueConsole(){}

	brogueConsoleReturn brogueConsole::command(std::string input, ostream& stream)
	{
		std::string* cmd = stringExtension::split(input, " ");

		if (hasArgument(cmd, "--mode"))
		{
			std::string mode = getArgument(cmd, "--mode");

			if (stringExtension::toUpper(mode) == "GAME")
				return brogueConsoleReturn::Completed_SetMode_Game;

			if (stringExtension::toUpper(mode) == "DEV")
				return brogueConsoleReturn::Completed_SetMode_Dev;

			if (stringExtension::toUpper(mode) == "RESOURCE")
				return brogueConsoleReturn::Completed_SetMode_Resource;
		}

		else if (hasArgument(cmd, "--help"))
			printHelp(stream);

		return brogueConsoleReturn::Completed;
	}
	void brogueConsole::printHelp(ostream& stream)
	{
		stream << "Brogue Command Line:  Welcome to the Brogue command terminal!" << std::endl << std::endl;
		stream << "Options:" << std::endl;
		stream << "\t\t--help\tPrints this help menu" << std::endl;
		stream << "\t\t--mode\t[GAME, DEV, RESOURCE] (GAME is set by default)" << std::endl;
	}

	bool brogueConsole::hasArgument(const std::string args[], const char* argumentName)
	{
		for (int index = 0; index < SIZEOF(args); index++)
		{
			if (args[index] == std::string(argumentName))
				return true;
		}

		return false;
	}

	std::string brogueConsole::getArgument(const std::string args[], const char* argumentName)
	{
		for (int index = 0; index < SIZEOF(args); index++)
		{
			if (std::string(args[index]) == std::string(argumentName) &&
				index < SIZEOF(args) + 1)
			{
				return args[index + 1];
			}
		}

		return "";
	}

	int brogueConsole::getArgumentInt(const std::string args[], const char* argumentName)
	{
		for (int index = 0; index < SIZEOF(args); index++)
		{
			if (std::string(args[index]) == std::string(argumentName) &&
				index < SIZEOF(args) + 1)
			{
				return typeConverter::stringToint(args[index + 1]);
			}
		}

		return 0;
	}

	bool brogueConsole::getArgumentBool(const std::string args[], const char* argumentName)
	{
		for (int index = 0; index < SIZEOF(args); index++)
		{
			if (std::string(args[index]) == std::string(argumentName) &&
				index < SIZEOF(args) + 1)
			{
				return typeConverter::stringToBool(args[index + 1]);
			}
		}

		return false;
	}
}

