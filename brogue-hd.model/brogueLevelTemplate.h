#pragma once
#include "dungeonConstants.h"
#include "randomGenerator.h"
#include "simple.h"

namespace brogueHd::model
{
	class brogueLevelTemplate : public simpleObject
	{
	public:
		brogueLevelTemplate() : brogueLevelTemplate(1, brogueLevelTilingType::MainEntrance, 1.0f)
		{

		}
		brogueLevelTemplate(int depth, brogueLevelTilingType tilingType, float extraCorridorProbability)
		{
			_roomTemplates = new simpleList<brogueRoomTemplate*>();
			_mainEntrance = new brogueRoomTemplate();
			_depth = depth;
			_extraCorridorProbability = extraCorridorProbability;
			_tilingType = tilingType;
		}
		~brogueLevelTemplate() override
		{
			for (int index = 0; index < _roomTemplates->count(); index++)
			{
				delete _roomTemplates->get(index);
			}

			delete _mainEntrance;
			delete _roomTemplates;
		}

		void addRoom(const brogueRoomTemplate& roomTemplate)
		{
			brogueRoomTemplate* room = new brogueRoomTemplate(roomTemplate);

			_roomTemplates->add(room);
		}

		void setMainEntrance(const brogueRoomTemplate& roomTemplate)
		{
			delete _mainEntrance;

			_mainEntrance = new brogueRoomTemplate(roomTemplate);
		}

		brogueRoomTemplate* getRoom(randomGenerator* randomGenerator) const
		{
			int randomIndex = randomGenerator->randomRangeExclusive(0, _roomTemplates->count());

			return _roomTemplates->get(randomIndex);
		}

		brogueRoomTemplate* getMainEntrance() const
		{
			return _mainEntrance;
		}

		int getDepth() const
		{
			return _depth;
		}
		float getExtraCorridorProbability() const
		{
			return _extraCorridorProbability;
		}
		brogueLevelTilingType getTilingType() const
		{
			return _tilingType;
		}

	private:

		simpleList<brogueRoomTemplate*>* _roomTemplates;

		// Specific main entrance room
		brogueRoomTemplate* _mainEntrance;

		int _depth;
		float _extraCorridorProbability;
		brogueLevelTilingType _tilingType;
	};
}
