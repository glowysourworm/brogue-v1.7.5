#include "resourceConsole.h"
#include "stringExtension.h"
#include "brogueColorMap.h"
#include "colorConstants.h"
#include <format>

using namespace std;

using namespace brogueHd::backend::modelConstant;
using namespace brogueHd::backend::model;
using namespace brogueHd::component;

namespace brogueHd::console
{
	resourceConsole::resourceConsole(resourceController* resourceController)
	{
		_resourceController = resourceController;
	}
	resourceConsole::~resourceConsole() {}

	brogueConsoleReturn resourceConsole::command(std::string input, ostream& stream)
	{
		std::string* cmd = stringExtension::split(input, " ");

		if (hasArgument(cmd, "--loadColors"))
		{
			char* path = getArgument<char*>(cmd, "--loadColors");

			stream << "Loading Color Definitions:  " << path << std::endl;

			brogueColorMap* colorMap = _resourceController->loadColors(path);

			if (colorMap != NULL)
			{
				stream << "Loading Successful! Printing Results..." << std::endl;

				color white = colorMap->getColor(gameColors::white);

				//stream << std::format("Color (.csv):  color({}, {}, {}, {}, {}, {})", 
				//					   white.red, white.green, white.blue, white.redRand, white.greenRand, white.blueRand) << std::endl;
			}
			else
				stream << "Loading Failed..." << std::endl;
		}

		return brogueConsoleReturn::Completed;
	}
}