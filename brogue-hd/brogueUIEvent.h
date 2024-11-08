#pragma once

#include "brogueEvent.h"
#include "brogueUIConstants.h"
#include "brogueUITagAction.h"

using namespace brogueHd::frontend;
using namespace brogueHd::frontend::opengl;
using namespace brogueHd::backend::controller;

namespace brogueHd::backend::controller
{
	class brogueUIEvent : public brogueEventBase<brogueUIProgram, brogueUITagAction>
	{

	};
}