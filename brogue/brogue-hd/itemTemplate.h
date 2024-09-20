#pragma once

#include "mathdef.h"
#include "dungeon.h"
#include "dungeonDeclaration.h"

using namespace brogueHd::backend::math;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model::item
{
	/// <summary>
	/// Simple "blueprints" for an item: Separated these from the item data for its corresponding brogueObject
	/// </summary>
	struct itemTemplate
	{
		keyLocationProfile keyLoc[KEY_ID_MAXIMUM];
		short originDepth;
	};

	typedef struct itemTable {
		char* name;
		char* flavor;
		char callTitle[30];
		short frequency;
		short marketValue;
		short strengthRequired;
		randomRange range;
		boolean identified;
		boolean called;
		char description[1500];
	} itemTable;

}