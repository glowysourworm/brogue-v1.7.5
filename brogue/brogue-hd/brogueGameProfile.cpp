#pragma once

#include "brogueGameProfile.h"
#include "brogueMath.h"
#include "broguedef.h"

using namespace brogueHd::backend::math;

namespace brogueHd::backend::model
{
	brogueGameProfile::brogueGameProfile(randomGenerator* randomGenerator)
	{
		_randomGenerator = randomGenerator;
		_levelProfiles = new std::vector<brogueLevelProfile*>();

		this->initialize();
	}
	brogueGameProfile::~brogueGameProfile()
	{
		for (int index = 0; index < _levelProfiles->size(); index++)
		{
			delete _levelProfiles->at(index);
		}

		delete _levelProfiles;
	}
	void brogueGameProfile::initialize()
	{
		for (int levelNumber = 1; levelNumber < DEEPEST_LEVEL; levelNumber++)
		{
			if (levelNumber == 1)
				_levelProfiles->push_back(new brogueLevelProfile(_randomGenerator, levelNumber, levelTypes::DP_BASIC_FIRST_ROOM));

			// TODO:  Gather up which levels will get what profiles
			else
				_levelProfiles->push_back(new brogueLevelProfile(_randomGenerator, levelNumber, levelTypes::DP_BASIC));
		}
	}
}