#pragma once

#include "brogueEvent.h"
#include "brogueUIConstants.h"
#include "brogueUITagAction.h"

using namespace brogueHd::frontend;

namespace brogueHd::backend
{
	class brogueUIEvent : public brogueEventBase<brogueUIProgram, brogueUITagAction>
	{

	};
}