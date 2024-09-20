#pragma once

#include "brogueLayout.h"
#include "brogueLevel.h"
#include "randomGenerator.h"

using namespace brogueHd::backend::model;

namespace brogueHd::backend::generator
{
	class layoutGenerator
	{
	public:

		/// <summary>
		/// Creates the base layout, terrain, and machine terrain for the leve
		/// </summary>
		/// <param name="profile">The dungeon profile:  must contain all parameters</param>
		layoutGenerator();
		~layoutGenerator();

		void initialize(const dungeonProfile& profile);
		void clear();

		brogueLayout& generateLayout();

	private:

		void createRooms();
		void designateMachineRooms();
		void triangulateRooms();
		void connectRooms();
		void createTerrain();

	private:



	private:

		brogueLevel* _level;
		randomGenerator* _randomGenerator;
	};
}

