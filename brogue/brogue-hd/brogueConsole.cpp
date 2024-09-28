#include "brogueConsole.h"
#include "command.h"
#include "exceptionHandler.h"
#include "stringExtension.h"
#include "typeConverter.h"

using namespace brogueHd::backend::extension;

namespace brogueHd::console
{
	brogueConsole::brogueConsole(){}
	brogueConsole::~brogueConsole(){}

	brogueConsoleReturn brogueConsole::command(std::string input, ostream& stream)
	{
		std::string* cmd = stringExtension::split(input, " ");

		if (hasArgument(cmd, "--mode"))
		{
			std::string mode = getArgument<std::string>(cmd, "--mode");

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

	template<typename T>
	T brogueConsole::getArgument(const std::string args[], const char* argumentName)
	{
		for (int index = 0; index < SIZEOF(args); index++)
		{
			if (std::string(args[index]) == std::string(argumentName) &&
				index < SIZEOF(args) + 1)
			{
				if (std::is_convertible<T, char*>())
					return args[index + 1];

				else if (std::is_convertible<T, std::string>())
					return (T)std::string(args[index + 1]);

				else if (std::is_convertible<T, int>())
					return (T)stringExtension::convert<int>(args[index + 1]);

				else if (std::is_convertible<T, unsigned long>())
					return (T)stringExtension::convert<unsigned long>(args[index + 1]);

				else
					brogueException::show("Unhandled console command type:  " + std::string(typeid(T).name));
			}
		}

		return default_value<T>::value;
	}
}

