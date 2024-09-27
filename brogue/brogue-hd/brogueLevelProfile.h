#pragma once

#include "dungeon.h"
#include "randomGenerator.h"
#include "simpleHash.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::generator;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model
{
	class brogueLevelProfile
	{
	public:

		brogueLevelProfile(const randomGenerator* randomGenerator, short depth, levelTypes type);
		~brogueLevelProfile();

		brogueRoomInfo getRandomRoomInfo();
		brogueRoomInfo getEntranceRoom();

	private:

		void initialize();
		brogueRoomInfo createRoomInfo(roomTypes roomType);

	private:

		randomGenerator* _randomGenerator;

		simpleHash<roomTypes, brogueRoomInfo>* _roomInfo;

		// Room Frequency static data
		simpleHash<levelTypes, simpleHash<roomTypes, short>*>* _roomFrequencyMap;

		// Corridor static data
		simpleHash<levelTypes, short>* _corridorFrequencyMap;

		short _depth;
		levelTypes _levelType;
	};
}