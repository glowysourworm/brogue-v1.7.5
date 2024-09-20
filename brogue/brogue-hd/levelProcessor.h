#pragma once

#include "playerCharacter.h"
#include "brogueLevel.h"

namespace brogueHd::backend::processor
{
	class levelProcessor
	{
	public:

		levelProcessor(brogueLevel* level);
		~levelProcessor();

		void initialize();

		color messageColorFromVictim(creature* monst);

	private:

		brogueLevel* _level;

		creature* _player;
	};
}
