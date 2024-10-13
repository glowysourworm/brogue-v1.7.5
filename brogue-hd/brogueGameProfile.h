#pragma once

#include "brogueLevelProfile.h"
#include "simpleList.h"

using namespace brogueHd::simple;

namespace brogueHd::backend::model
{
	// brogueCell
	//typedef struct keyLocationProfile 
	//{
	//	short x;
	//	short y;
	//	short machine;
	//	bool disposableHere;

	//} keyLocationProfile;

	// Level profiles, affecting what rooms get chosen and how they're connected:
	class brogueGameProfile
	{
	public:
		
		brogueGameProfile(randomGenerator* randomGenerator);
		~brogueGameProfile();

	private:

		// Creates level profiles 1 -> DEEPEST_LEVEL
		void initialize();

	private:

		randomGenerator* _randomGenerator;

		simpleList<brogueLevelProfile*>* _levelProfiles;

	};

	brogueGameProfile::brogueGameProfile(randomGenerator* randomGenerator)
	{
		_randomGenerator = randomGenerator;
		_levelProfiles = new simpleList<brogueLevelProfile*>();

		this->initialize();
	}
	brogueGameProfile::~brogueGameProfile()
	{
		for (int index = 0; index < _levelProfiles->count(); index++)
		{
			delete _levelProfiles->get(index);
		}

		delete _levelProfiles;
	}
	void brogueGameProfile::initialize()
	{
		for (int levelNumber = 1; levelNumber < DEEPEST_LEVEL; levelNumber++)
		{
			if (levelNumber == 1)
				_levelProfiles->add(new brogueLevelProfile(_randomGenerator, levelNumber, levelTypes::DP_BASIC_FIRST_ROOM));

			// TODO:  Gather up which levels will get what profiles
			else
				_levelProfiles->add(new brogueLevelProfile(_randomGenerator, levelNumber, levelTypes::DP_BASIC));
		}
	}
}