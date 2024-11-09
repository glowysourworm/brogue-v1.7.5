#pragma once

namespace brogueHd::backend::model
{
	enum DFFlags
	{
		DFF_EVACUATE_CREATURES_FIRST = Fl(0),	// Creatures in the DF area get moved outside of it
		DFF_SUBSEQ_EVERYWHERE = Fl(1),	// Subsequent DF spawns in every cell that this DF spawns in, instead of only the origin
		DFF_TREAT_AS_BLOCKING = Fl(2),	// If filling the footprint of this DF with walls would disrupt level connectivity, then abort.
		DFF_PERMIT_BLOCKING = Fl(3),	// Generate this DF without regard to level connectivity.
		DFF_ACTIVATE_DORMANT_MONSTER = Fl(4),	// Dormant monsters on this tile will appear -- e.g. when a statue bursts to reveal a monster.
		DFF_CLEAR_OTHER_TERRAIN = Fl(5),	// Erase other terrain in the footprint of this DF.
		DFF_BLOCKED_BY_OTHER_LAYERS = Fl(6),	// Will not propagate into a cell if any layer in that cell has a superior priority.
		DFF_SUPERPRIORITY = Fl(7),	// Will overwrite terrain of a superior priority.
		DFF_AGGRAVATES_MONSTERS = Fl(8),    // Will act as though an aggravate monster scroll of effectRadius radius had been read at that point.
		DFF_RESURRECT_ALLY = Fl(9),    // Will bring back to life your most recently deceased ally.
	};
}