#pragma once

#include "brogueGlobal.h"

namespace brogueHd::backend::modelConstant
{
	enum creatureStates 
	{
		MONSTER_SLEEPING,
		MONSTER_TRACKING_SCENT,
		MONSTER_WANDERING,
		MONSTER_FLEEING,
		MONSTER_ALLY,
	};

	enum creatureModes 
	{
		MODE_NORMAL,
		MODE_PERM_FLEEING
	};

	enum hordeFlags 
	{
		HORDE_DIES_ON_LEADER_DEATH = Fl(0),	// if the leader dies, the horde will die instead of electing new leader
		HORDE_IS_SUMMONED = Fl(1),	// minions summoned when any creature is the same species as the leader and casts summon
		HORDE_SUMMONED_AT_DISTANCE = Fl(2),    // summons will appear across the level, and will naturally path back to the leader
		HORDE_LEADER_CAPTIVE = Fl(3),	// the leader is in chains and the followers are guards
		HORDE_NO_PERIODIC_SPAWN = Fl(4),	// can spawn only when the level begins -- not afterwards
		HORDE_ALLIED_WITH_PLAYER = Fl(5),

		HORDE_MACHINE_BOSS = Fl(6),	// used in machines for a boss challenge
		HORDE_MACHINE_WATER_MONSTER = Fl(7),	// used in machines where the room floods with shallow water
		HORDE_MACHINE_CAPTIVE = Fl(8),	// powerful captive monsters without any captors
		HORDE_MACHINE_STATUE = Fl(9),	// the kinds of monsters that make sense in a statue
		HORDE_MACHINE_TURRET = Fl(10),	// turrets, for hiding in walls
		HORDE_MACHINE_MUD = Fl(11),	// bog monsters, for hiding in mud
		HORDE_MACHINE_KENNEL = Fl(12),	// monsters that can appear in cages in kennels
		HORDE_VAMPIRE_FODDER = Fl(13),	// monsters that are prone to capture and farming by vampires
		HORDE_MACHINE_LEGENDARY_ALLY = Fl(14),	// legendary allies
		HORDE_NEVER_OOD = Fl(15),   // Horde cannot be generated out of depth
		HORDE_MACHINE_THIEF = Fl(16),   // monsters that can be generated in the key thief area machines
		HORDE_MACHINE_GOBLIN_WARREN = Fl(17),   // can spawn in goblin warrens
		HORDE_SACRIFICE_TARGET = Fl(18),   // can be the target of an assassination challenge; leader will get scary light.

		HORDE_MACHINE_ONLY = (HORDE_MACHINE_BOSS | HORDE_MACHINE_WATER_MONSTER
		| HORDE_MACHINE_CAPTIVE | HORDE_MACHINE_STATUE
			| HORDE_MACHINE_TURRET | HORDE_MACHINE_MUD
			| HORDE_MACHINE_KENNEL | HORDE_VAMPIRE_FODDER
			| HORDE_MACHINE_LEGENDARY_ALLY | HORDE_MACHINE_THIEF
			| HORDE_MACHINE_GOBLIN_WARREN
			| HORDE_SACRIFICE_TARGET),
	};

	enum creatureBehaviorFlags 
	{
		MONST_INVISIBLE = Fl(0),	// monster is invisible
		MONST_INANIMATE = Fl(1),	// monster has abbreviated stat bar display and is immune to many things
		MONST_IMMOBILE = Fl(2),	// monster won't move or perform melee attacks
		MONST_CARRY_ITEM_100 = Fl(3),	// monster carries an item 100% of the time
		MONST_CARRY_ITEM_25 = Fl(4),	// monster carries an item 25% of the time
		MONST_ALWAYS_HUNTING = Fl(5),	// monster is never asleep or in wandering mode
		MONST_FLEES_NEAR_DEATH = Fl(6),	// monster flees when under 25% health and re-engages when over 75%
		MONST_ATTACKABLE_THRU_WALLS = Fl(7),	// can be attacked when embedded in a wall
		MONST_DEFEND_DEGRADE_WEAPON = Fl(8),	// hitting the monster damages the weapon
		MONST_IMMUNE_TO_WEAPONS = Fl(9),	// weapons ineffective
		MONST_FLIES = Fl(10),	// permanent levitation
		MONST_FLITS = Fl(11),	// moves randomly a third of the time
		MONST_IMMUNE_TO_FIRE = Fl(12),	// won't burn, won't die in lava
		MONST_CAST_SPELLS_SLOWLY = Fl(13),	// takes twice the attack duration to cast a spell
		MONST_IMMUNE_TO_WEBS = Fl(14),	// monster passes freely through webs
		MONST_REFLECT_4 = Fl(15),	// monster reflects projectiles as though wearing +4 armor of reflection
		MONST_NEVER_SLEEPS = Fl(16),	// monster is always awake
		MONST_FIERY = Fl(17),	// monster carries an aura of flame (but no automatic fire light)
		MONST_INVULNERABLE = Fl(18),	// monster is immune to absolutely everything
		MONST_IMMUNE_TO_WATER = Fl(19),	// monster moves at full speed in deep water and (if player) doesn't drop items
		MONST_RESTRICTED_TO_LIQUID = Fl(20),	// monster can move only on tiles that allow submersion
		MONST_SUBMERGES = Fl(21),	// monster can submerge in appropriate terrain
		MONST_MAINTAINS_DISTANCE = Fl(22),	// monster tries to keep a distance of 3 tiles between it and player
		MONST_WILL_NOT_USE_STAIRS = Fl(23),	// monster won't chase the player between levels
		MONST_DIES_IF_NEGATED = Fl(24),	// monster will die if exposed to negation magic
		MONST_MALE = Fl(25),	// monster is male (or 50% likely to be male if also has MONST_FEMALE)
		MONST_FEMALE = Fl(26),	// monster is female (or 50% likely to be female if also has MONST_MALE)
		MONST_NOT_LISTED_IN_SIDEBAR = Fl(27),   // monster doesn't show up in the sidebar
		MONST_GETS_TURN_ON_ACTIVATION = Fl(28),   // monster never gets a turn, except when its machine is activated
		MONST_ALWAYS_USE_ABILITY = Fl(29),   // monster will never fail to use special ability if eligible (no random factor)
		MONST_NO_POLYMORPH = Fl(30),   // monster cannot result from a polymorph spell (liches, phoenixes and Warden of Yendor)

		NEGATABLE_TRAITS = (MONST_INVISIBLE | MONST_DEFEND_DEGRADE_WEAPON | MONST_IMMUNE_TO_WEAPONS | MONST_FLIES
		| MONST_FLITS | MONST_IMMUNE_TO_FIRE | MONST_REFLECT_4 | MONST_FIERY | MONST_MAINTAINS_DISTANCE),
		MONST_TURRET = (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE |
			MONST_ATTACKABLE_THRU_WALLS | MONST_WILL_NOT_USE_STAIRS),
		LEARNABLE_BEHAVIORS = (MONST_INVISIBLE | MONST_FLIES | MONST_IMMUNE_TO_FIRE | MONST_REFLECT_4),
		MONST_NEVER_VORPAL_ENEMY = (MONST_INANIMATE | MONST_INVULNERABLE | MONST_IMMOBILE | MONST_RESTRICTED_TO_LIQUID | MONST_GETS_TURN_ON_ACTIVATION | MONST_MAINTAINS_DISTANCE),
		MONST_NEVER_MUTATED = (MONST_INVISIBLE | MONST_INANIMATE | MONST_IMMOBILE | MONST_INVULNERABLE),
	};

	enum creatureAbilityFlags 
	{
		MA_HIT_HALLUCINATE = Fl(0),	// monster can hit to cause hallucinations
		MA_HIT_STEAL_FLEE = Fl(1),	// monster can steal an item and then run away
		MA_HIT_BURN = Fl(2),    // monster can hit to set you on fire
		MA_ENTER_SUMMONS = Fl(3),	// monster will "become" its summoned leader, reappearing when that leader is defeated
		MA_HIT_DEGRADE_ARMOR = Fl(4),	// monster damages armor
		MA_CAST_SUMMON = Fl(5),	// requires that there be one or more summon hordes with this monster type as the leader
		MA_SEIZES = Fl(6),	// monster seizes enemies before attacking
		MA_POISONS = Fl(7),	// monster's damage is dealt in the form of poison
		MA_DF_ON_DEATH = Fl(8),	// monster spawns its DF when it dies
		MA_CLONE_SELF_ON_DEFEND = Fl(9),	// monster splits in two when struck
		MA_KAMIKAZE = Fl(10),	// monster dies instead of attacking
		MA_TRANSFERENCE = Fl(11),	// monster recovers 40 or 90% of the damage that it inflicts as health
		MA_CAUSES_WEAKNESS = Fl(12),	// monster attacks cause weakness status in target
		MA_ATTACKS_PENETRATE = Fl(13),   // monster attacks all adjacent enemies, like an axe
		MA_ATTACKS_ALL_ADJACENT = Fl(14),   // monster attacks penetrate one layer of enemies, like a spear
		MA_ATTACKS_EXTEND = Fl(15),   // monster attacks from a distance in a cardinal direction, like a whip
		MA_ATTACKS_STAGGER = Fl(16),   // monster attacks will push the player backward by one space if there is room
		MA_AVOID_CORRIDORS = Fl(17),   // monster will avoid corridors when hunting

		SPECIAL_HIT = (MA_HIT_HALLUCINATE | MA_HIT_STEAL_FLEE | MA_HIT_DEGRADE_ARMOR | MA_POISONS
		| MA_TRANSFERENCE | MA_CAUSES_WEAKNESS | MA_HIT_BURN | MA_ATTACKS_STAGGER),
		LEARNABLE_ABILITIES = (MA_TRANSFERENCE | MA_CAUSES_WEAKNESS),

		MA_NON_NEGATABLE_ABILITIES = (MA_ATTACKS_PENETRATE | MA_ATTACKS_ALL_ADJACENT | MA_ATTACKS_EXTEND | MA_ATTACKS_STAGGER),
		MA_NEVER_VORPAL_ENEMY = (MA_KAMIKAZE),
		MA_NEVER_MUTATED = (MA_KAMIKAZE),
	};

	enum creatureBookkeepingFlags
	{
		MB_WAS_VISIBLE = Fl(0),	// monster was visible to player last turn
		MB_TELEPATHICALLY_REVEALED = Fl(1),    // player can magically see monster and adjacent cells
		MB_PREPLACED = Fl(2),	// monster dropped onto the level and requires post-processing
		MB_APPROACHING_UPSTAIRS = Fl(3),	// following the player up the stairs
		MB_APPROACHING_DOWNSTAIRS = Fl(4),	// following the player down the stairs
		MB_APPROACHING_PIT = Fl(5),	// following the player down a pit
		MB_LEADER = Fl(6),	// monster is the leader of a horde
		MB_FOLLOWER = Fl(7),	// monster is a member of a horde
		MB_CAPTIVE = Fl(8),	// monster is all tied up
		MB_SEIZED = Fl(9),	// monster is being held
		MB_SEIZING = Fl(10),	// monster is holding another creature immobile
		MB_SUBMERGED = Fl(11),	// monster is currently submerged and hence invisible until it attacks
		MB_JUST_SUMMONED = Fl(12),	// used to mark summons so they can be post-processed
		MB_WILL_FLASH = Fl(13),	// this monster will flash as soon as control is returned to the player
		MB_BOUND_TO_LEADER = Fl(14),	// monster will die if the leader dies or becomes separated from the leader
		MB_MARKED_FOR_SACRIFICE = Fl(15),   // scary glow, monster can be sacrificed in the appropriate machine
		MB_ABSORBING = Fl(16),	// currently learning a skill by absorbing an enemy corpse
		MB_DOES_NOT_TRACK_LEADER = Fl(17),	// monster will not follow its leader around
		MB_IS_FALLING = Fl(18),	// monster is plunging downward at the end of the turn
		MB_IS_DYING = Fl(19),	// monster has already been killed and is awaiting the end-of-turn graveyard sweep (or in purgatory)
		MB_GIVEN_UP_ON_SCENT = Fl(20),	// to help the monster remember that the scent map is a dead end
		MB_IS_DORMANT = Fl(21),	// lurking, waiting to burst out
		MB_HAS_SOUL = Fl(22),   // slaying the monster will count toward weapon auto-ID
		MB_ALREADY_SEEN = Fl(23),   // seeing this monster won't interrupt exploration
	};
}