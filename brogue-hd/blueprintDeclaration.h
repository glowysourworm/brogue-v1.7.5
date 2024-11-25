#pragma once

#include "brogueGlobal.h"
#include "machineTemplate.h"

namespace brogueHd::backend::model
{
	typedef struct blueprint {
		int depthRange[2];				// machine must be built between these dungeon depths
		int roomSize[2];					// machine must be generated in a room of this size
		int frequency;					// frequency (number of tickets this blueprint enters in the blueprint selection raffle)
		int featureCount;					// how many different types of features follow (max of 20)
		int dungeonProfileType;           // if BP_REDESIGN_INTERIOR is set, which dungeon profile do we use?
		unsigned long flags;				// blueprint flags
		machineFeature feature[20];			// the features themselves
	} blueprint;

	enum blueprintFlags {
		BP_ADOPT_ITEM = Fl(0),	// the machine must adopt an item (e.g. a door key)
		BP_VESTIBULE = Fl(1),    // spawns in a doorway (location must be given) and expands outward, to guard the room
		BP_PURGE_PATHING_BLOCKERS = Fl(2),	// clean out traps and other T_PATHING_BLOCKERs
		BP_PURGE_INTERIOR = Fl(3),	// clean out all of the terrain in the interior before generating the machine
		BP_PURGE_LIQUIDS = Fl(4),	// clean out all of the liquids in the interior before generating the machine
		BP_SURROUND_WITH_WALLS = Fl(5),	// fill in any impassable gaps in the perimeter (e.g. water, lava, brimstone, traps) with wall
		BP_IMPREGNABLE = Fl(6),	// impassable perimeter and interior tiles are locked; tunneling bolts will bounce off harmlessly
		BP_REWARD = Fl(7),	// metered reward machines
		BP_OPEN_INTERIOR = Fl(8),	// clear out walls in the interior, widen the interior until convex or bumps into surrounding areas
		BP_MAXIMIZE_INTERIOR = Fl(9),    // same as BP_OPEN_INTERIOR but expands the room as far as it can go, potentially surrounding the whole level.
		BP_ROOM = Fl(10),	// spawns in a dead-end room that is dominated by a chokepoint of the given size (as opposed to a random place of the given size)
		BP_TREAT_AS_BLOCKING = Fl(11),	// abort the machine if, were it filled with wall tiles, it would disrupt the level connectivity
		BP_REQUIRE_BLOCKING = Fl(12),	// abort the machine unless, were it filled with wall tiles, it would disrupt the level connectivity
		BP_NO_INTERIOR_FLAG = Fl(13),	// don't flag the area as being part of a machine
		BP_REDESIGN_INTERIOR = Fl(14),   // nuke and pave -- delete all terrain in the interior and build entirely new rooms within the bounds
	};
}

