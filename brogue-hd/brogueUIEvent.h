#pragma once

#include "brogueEvent.h"
#include "brogueUIConstants.h"
#include "brogueUITagAction.h"

namespace brogueHd::backend
{
	using namespace brogueHd::frontend;

	class brogueUIEvent : public brogueEventBase<brogueUIProgram, brogueUITagAction>
	{
	};
}
