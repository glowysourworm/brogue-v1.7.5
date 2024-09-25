#pragma once

#include "broguedef.h"
#include "creatureDeclaration.h"

namespace brogueHd::backend::model::creature
{
	const char monsterBehaviorFlagDescriptions[32][COLS] = {
		"is invisible",								// MONST_INVISIBLE
		"is an inanimate object",					// MONST_INANIMATE
		"cannot move",								// MONST_IMMOBILE
		"",                                         // MONST_CARRY_ITEM_100
		"",                                         // MONST_CARRY_ITEM_25
		"",                                         // MONST_ALWAYS_HUNTING
		"flees at low health",						// MONST_FLEES_NEAR_DEATH
		"",											// MONST_ATTACKABLE_THRU_WALLS
		"corrodes weapons when hit",				// MONST_DEFEND_DEGRADE_WEAPON
		"is immune to weapon damage",				// MONST_IMMUNE_TO_WEAPONS
		"flies",									// MONST_FLIES
		"moves erratically",						// MONST_FLITS
		"is immune to fire",						// MONST_IMMUNE_TO_FIRE
		"",											// MONST_CAST_SPELLS_SLOWLY
		"cannot be entangled",						// MONST_IMMUNE_TO_WEBS
		"can reflect magic spells",                 // MONST_REFLECT_4
		"never sleeps",								// MONST_NEVER_SLEEPS
		"burns unceasingly",						// MONST_FIERY
		"is invulnerable",                          // MONST_INVULNERABLE
		"is at home in water",						// MONST_IMMUNE_TO_WATER
		"cannot venture onto dry land",				// MONST_RESTRICTED_TO_LIQUID
		"submerges",								// MONST_SUBMERGES
		"keeps $HISHER distance",					// MONST_MAINTAINS_DISTANCE
		"",											// MONST_WILL_NOT_USE_STAIRS
		"is animated purely by magic",				// MONST_DIES_IF_NEGATED
		"",                                         // MONST_MALE
		"",                                         // MONST_FEMALE
		"",                                         // MONST_NOT_LISTED_IN_SIDEBAR
		"moves only when activated",                // MONST_GETS_TURN_ON_ACTIVATION
	};

	const char monsterAbilityFlagDescriptions[32][COLS] = {
		"can induce hallucinations",				// MA_HIT_HALLUCINATE
		"can steal items",							// MA_HIT_STEAL_FLEE
		"lights enemies on fire when $HESHE hits",  // MA_HIT_BURN
		"can possess $HISHER summoned allies",		// MA_ENTER_SUMMONS
		"corrodes armor when $HESHE hits",			// MA_HIT_DEGRADE_ARMOR
		"can summon allies",						// MA_CAST_SUMMON
		"immobilizes $HISHER prey",					// MA_SEIZES
		"injects poison when $HESHE hits",			// MA_POISONS
		"",											// MA_DF_ON_DEATH
		"divides in two when struck",				// MA_CLONE_SELF_ON_DEFEND
		"dies when $HESHE attacks",					// MA_KAMIKAZE
		"recovers health when $HESHE inflicts damage",// MA_TRANSFERENCE
		"saps strength when $HESHE inflicts damage",// MA_CAUSE_WEAKNESS

		"attacks up to two opponents in a line",    // MA_ATTACKS_PENETRATE
		"attacks all adjacent opponents at once",   // MA_ATTACKS_ALL_ADJACENT
		"attacks with a whip",                      // MA_ATTACKS_EXTEND
		"pushes opponents backward when $HESHE hits", // MA_ATTACKS_STAGGER
		"avoids attacking in corridors in a group", // MA_AVOID_CORRIDORS
	};

	const char monsterBookkeepingFlagDescriptions[32][COLS] = {
		"",											// MB_WAS_VISIBLE
		"is telepathically bonded with you",		// MB_TELEPATHICALLY_REVEALED
		"",											// MB_PREPLACED
		"",											// MB_APPROACHING_UPSTAIRS
		"",											// MB_APPROACHING_DOWNSTAIRS
		"",											// MB_APPROACHING_PIT
		"",											// MB_LEADER
		"",											// MB_FOLLOWER
		"",											// MB_CAPTIVE
		"has been immobilized",						// MB_SEIZED
		"is currently holding $HISHER prey immobile",// MB_SEIZING
		"is submerged",								// MB_SUBMERGED
		"",											// MB_JUST_SUMMONED
		"",											// MB_WILL_FLASH
		"is anchored to reality by $HISHER summoner",// MB_BOUND_TO_LEADER
		"is marked for demonic sacrifice",          // MB_MARKED_FOR_SACRIFICE
	};
}