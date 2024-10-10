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
}