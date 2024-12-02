#pragma once

#include <simpleMacros.h>

namespace brogueHd::model
{
	using namespace simple;

	enum DFFlags
	{
		DFF_EVACUATE_CREATURES_FIRST = EnumFlag(0), // Creatures in the DF area get moved outside of it
		DFF_SUBSEQ_EVERYWHERE = EnumFlag(1),
		// Subsequent DF spawns in every cell that this DF spawns in, instead of only the origin
		DFF_TREAT_AS_BLOCKING = EnumFlag(2),
		// If filling the footprint of this DF with walls would disrupt level connectivity, then abort.
		DFF_PERMIT_BLOCKING = EnumFlag(3), // Generate this DF without regard to level connectivity.
		DFF_ACTIVATE_DORMANT_MONSTER = EnumFlag(4),
		// Dormant monsters on this tile will appear -- e.g. when a statue bursts to reveal a monster.
		DFF_CLEAR_OTHER_TERRAIN = EnumFlag(5), // Erase other terrain in the footprint of this DF.
		DFF_BLOCKED_BY_OTHER_LAYERS = EnumFlag(6),
		// Will not propagate into a cell if any layer in that cell has a superior priority.
		DFF_SUPERPRIORITY = EnumFlag(7), // Will overwrite terrain of a superior priority.
		DFF_AGGRAVATES_MONSTERS = EnumFlag(8),
		// Will act as though an aggravate monster scroll of effectRadius radius had been read at that point.
		DFF_RESURRECT_ALLY = EnumFlag(9), // Will bring back to life your most recently deceased ally.
	};
}
