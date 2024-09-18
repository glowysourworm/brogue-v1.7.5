#pragma once

#include "creatureBehaviorDeclaration.h"

using namespace brogueHd::backend::model::creature;

namespace brogueHd::backend::model::machine
{
	struct creatureMachineInfo
	{
		creatureBookkeepingFlags bookkeepingFlags;
		//short spawnDepth;					// keep track of the depth of the machine to which they relate (for activation monsters)
		short machineHome;                  // monsters that spawn in a machine keep track of the machine number here (for activation monsters)
	};
}