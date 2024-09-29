#pragma once

#include "playerCharacter.h"
#include "brogueLevel.h"

using namespace brogueHd::backend::model;

namespace brogueHd::backend::processor
{
	class levelProcessor
	{
	public:

		levelProcessor(brogueLevel* level);
		~levelProcessor();

		void initialize();

		//color messageColorFromVictim(creatureInfo* monst);

	private:

		brogueLevel* _level;

		creatureInfo* _player;
	};
}

