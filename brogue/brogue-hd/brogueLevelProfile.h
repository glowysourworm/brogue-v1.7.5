#pragma once

#include "dungeonDeclaration.h"
#include "randomGenerator.h"
#include <map>

using namespace std;

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

		std::map<roomTypes, brogueRoomInfo>* _roomInfo;

		// Room Frequency static data
		std::map<levelTypes, std::map<roomTypes, short>> _roomFrequencyMap;

		// Corridor static data
		std::map<levelTypes, short> _corridorFrequencyMap;

		short _depth;
		levelTypes _levelType;
	};
}