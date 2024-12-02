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

		int currentHP;
		long turnsUntilRegen;
		int regenPerTurn;					// number of HP to regenerate every single turn
		int weaknessAmount;				// number of points of weakness that are inflicted by the weakness status
		int poisonAmount;                 // number of points of damage per turn from poison

		bool hasMutation;
		mutation mutation;

		int ticksUntilTurn;				// how long before the creature gets its next move

		//int previousHealthPoints;         // remembers what your health proportion was at the start of the turn
		int turnsSpentStationary;			// how many (subjective) turns it's been since the creature moved between tiles

		int xpxp;							// exploration experience (used to time telepathic bonding for allies)

		//int newPowerCount;                // how many more times this monster can absorb a fallen monster
		//int totalPowerCount;              // how many times has the monster been empowered? Used to recover abilities when negated.

		brogueItem* carriedItem;			// only used for monsters

	private:



	};
}

