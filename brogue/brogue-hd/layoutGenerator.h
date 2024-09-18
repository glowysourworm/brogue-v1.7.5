#pragma once

#include "brogueLevel.h"
#include "randomGenerator.h"

namespace brogueHd
{
	class layoutGenerator
	{
	public:

		layoutGenerator(brogueLevel* level, randomGenerator* randomGenerator);
		~layoutGenerator();

		void generateDungeon(brogueLevel* level);

	private:

		void designRandomRoom(dungeonProfile roomProfile);
		void designCavern(dungeonProfile roomProfile);
		void designEntranceRoom(dungeonProfile roomProfile);
		void designCrossRoom(dungeonProfile roomProfile);
		void designSymmetricalCrossRoom(dungeonProfile roomProfile);
		void designSmallRoom(dungeonProfile roomProfile);
		void designCircularRoom(dungeonProfile roomProfile);
		void designChunkyRoom(dungeonProfile roomProfile);

	private:

		brogueLevel* _level;
		randomGenerator* _randomGenerator;
	};
}

