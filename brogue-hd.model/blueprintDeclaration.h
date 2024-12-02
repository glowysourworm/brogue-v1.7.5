#pragma once

#include <simpleMacros.h>
#include "machineTemplate.h"

namespace brogueHd::model
{
	using namespace simple;

	typedef struct blueprint
	{
		int depthRange[2]; // machine must be built between these dungeon depths
		int roomSize[2]; // machine must be generated in a room of this size
		int frequency; // frequency (number of tickets this blueprint enters in the blueprint selection raffle)
		int featureCount; // how many different types of features follow (max of 20)
		int dungeonProfileType; // if BP_REDESIGN_INTERIOR is set, which dungeon profile do we use?
		unsigned long flags; // blueprint flags
		machineFeature feature[20]; // the features themselves
	} blueprint;

	enum blueprintFlags
	{
		BP_ADOPT_ITEM = EnumFlag(0), // the machine must adopt an item (e.g. a door key)
		BP_VESTIBULE = EnumFlag(1),
		// spawns in a doorway (location must be given) and expands outward, to guard the room
		BP_PURGE_PATHING_BLOCKERS = EnumFlag(2), // clean out traps and other T_PATHING_BLOCKERs
		BP_PURGE_INTERIOR = EnumFlag(3), // clean out all of the terrain in the interior before generating the machine
		BP_PURGE_LIQUIDS = EnumFlag(4), // clean out all of the liquids in the interior before generating the machine
		BP_SURROUND_WITH_WALLS = EnumFlag(5),
		// fill in any impassable gaps in the perimeter (e.g. water, lava, brimstone, traps) with wall
		BP_IMPREGNABLE = EnumFlag(6),
		// impassable perimeter and interior tiles are locked; tunneling bolts will bounce off harmlessly
		BP_REWARD = EnumFlag(7), // metered reward machines
		BP_OPEN_INTERIOR = EnumFlag(8),
		// clear out walls in the interior, widen the interior until convex or bumps into surrounding areas
		BP_MAXIMIZE_INTERIOR = EnumFlag(9),
		// same as BP_OPEN_INTERIOR but expands the room as far as it can go, potentially surrounding the whole level.
		BP_ROOM = EnumFlag(10),
		// spawns in a dead-end room that is dominated by a chokepoint of the given size (as opposed to a random place of the given size)
		BP_TREAT_AS_BLOCKING = EnumFlag(11),
		// abort the machine if, were it filled with wall tiles, it would disrupt the level connectivity
		BP_REQUIRE_BLOCKING = EnumFlag(12),
		// abort the machine unless, were it filled with wall tiles, it would disrupt the level connectivity
		BP_NO_INTERIOR_FLAG = EnumFlag(13), // don't flag the area as being part of a machine
		BP_REDESIGN_INTERIOR = EnumFlag(14),
		// nuke and pave -- delete all terrain in the interior and build entirely new rooms within the bounds
	};
}
