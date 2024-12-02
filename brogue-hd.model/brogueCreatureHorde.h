#pragma once

#include "brogueCreature.h"

namespace brogueHd::model
{
	/// <summary>
	/// Contains data about a horde of brogueCreature types. This would include specifics
	/// for several types of hordes:  friendlies, vampire bats, other specifics...
	/// </summary>
	class brogueCreatureHorde
	{
	public:
		brogueCreatureHorde()
		{
		};

		~brogueCreatureHorde()
		{
		};

	private:
		brogueCreature* _leader; // only if monster is a follower
		brogueCreature* _carriedMonster;
		// when vampires turn into bats, one of the bats restores the vampire when it dies
		brogueCreature* _nextCreature;
	};
}
