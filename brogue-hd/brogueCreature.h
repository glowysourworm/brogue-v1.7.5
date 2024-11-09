#pragma once

#include "brogueItem.h"
#include "brogueObject.h"
#include "creature.h"

namespace brogueHd::backend::model
{
	class brogueCreature : public brogueObject
	{
	public:

		creatureInfo* info;

		short currentHP;
		long turnsUntilRegen;
		short regenPerTurn;					// number of HP to regenerate every single turn
		short weaknessAmount;				// number of points of weakness that are inflicted by the weakness status
		short poisonAmount;                 // number of points of damage per turn from poison

		bool hasMutation;
		mutation mutation;

		short ticksUntilTurn;				// how long before the creature gets its next move

		//short previousHealthPoints;         // remembers what your health proportion was at the start of the turn
		short turnsSpentStationary;			// how many (subjective) turns it's been since the creature moved between tiles

		short xpxp;							// exploration experience (used to time telepathic bonding for allies)

		//short newPowerCount;                // how many more times this monster can absorb a fallen monster
		//short totalPowerCount;              // how many times has the monster been empowered? Used to recover abilities when negated.

		brogueItem* carriedItem;			// only used for monsters

	private:



	};
}

