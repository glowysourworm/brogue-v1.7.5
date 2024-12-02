#pragma once

#include "color.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::backend::model
{
	struct bolt
	{
		simpleString name;
		simpleString description;
		simpleString abilityDescription;
		char theChar;
		color foreColor;
		color backColor;
		int boltEffect;
		int magnitude;
		int pathDF;
		int targetDF;
		unsigned long forbiddenMonsterFlags;
		unsigned long flags;
	};
}