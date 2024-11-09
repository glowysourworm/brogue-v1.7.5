#pragma once

namespace brogueHd::backend::model
{
	enum boltEffects
	{
		BE_NONE,
		BE_ATTACK,
		BE_TELEPORT,
		BE_SLOW,
		BE_POLYMORPH,
		BE_NEGATION,
		BE_DOMINATION,
		BE_BECKONING,
		BE_PLENTY,
		BE_INVISIBILITY,
		BE_EMPOWERMENT,
		BE_DAMAGE,
		BE_POISON,
		BE_TUNNELING,
		BE_BLINKING,
		BE_ENTRANCEMENT,
		BE_OBSTRUCTION,
		BE_DISCORD,
		BE_CONJURATION,
		BE_HEALING,
		BE_HASTE,
		BE_SHIELDING,
	};

	enum boltFlags
	{
		BF_PASSES_THRU_CREATURES = Fl(0),	// Bolt continues through creatures (e.g. lightning and tunneling)
		BF_HALTS_BEFORE_OBSTRUCTION = Fl(1),    // Bolt takes effect the space before it terminates (e.g. conjuration, obstruction, blinking)
		BF_TARGET_ALLIES = Fl(2),    // Staffs/wands/creatures that shoot this bolt will auto-target allies.
		BF_TARGET_ENEMIES = Fl(3),    // Staffs/wands/creatures that shoot this bolt will auto-target enemies.
		BF_FIERY = Fl(4),    // Bolt will light flammable terrain on fire as it passes, and will ignite monsters hit.
		BF_NEVER_REFLECTS = Fl(6),    // Bolt will never reflect (e.g. spiderweb, arrows).
		BF_NOT_LEARNABLE = Fl(7),    // This technique cannot be absorbed by empowered allies.
		BF_NOT_NEGATABLE = Fl(8),    // Won't be erased by negation.
		BF_ELECTRIC = Fl(9),    // Activates terrain that has TM_PROMOTES_ON_ELECTRICITY
		BF_DISPLAY_CHAR_ALONG_LENGTH = Fl(10),   // Display the character along the entire length of the bolt instead of just at the front.
	};
}