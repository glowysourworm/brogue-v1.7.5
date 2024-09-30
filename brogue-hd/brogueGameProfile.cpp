#pragma once

#include "brogueGameProfile.h"
#include "brogueMath.h"
#include "brogueGlobal.h"

using namespace brogueHd::component::math;

namespace brogueHd::backend::model
{
	brogueGameProfile::brogueGameProfile(randomGenerator* randomGenerator)
	{
		//_randomGenerator = randomGenerator;
		//_levelProfiles = new simpleList<brogueLevelProfile*>();

		//this->initialize();
	}
	brogueGameProfile::~brogueGameProfile()
	{
		//for (int index = 0; index < _levelProfiles->count(); index++)
		//{
		//	delete _levelProfiles->get(index);
		//}

		//delete _levelProfiles;
	}
	void brogueGameProfile::initialize()
	{
		//for (int levelNumber = 1; levelNumber < DEEPEST_LEVEL; levelNumber++)
		//{
		//	if (levelNumber == 1)
		//		_levelProfiles->add(new brogueLevelProfile(_randomGenerator, levelNumber, levelTypes::DP_BASIC_FIRST_ROOM));

		//	// TODO:  Gather up which levels will get what profiles
		//	else
		//		_levelProfiles->add(new brogueLevelProfile(_randomGenerator, levelNumber, levelTypes::DP_BASIC));
		//}
	}
}