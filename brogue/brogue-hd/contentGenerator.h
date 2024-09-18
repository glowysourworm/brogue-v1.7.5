#pragma once

#include "brogueLevel.h"

namespace brogueHd::backend::generator
{
	class contentGenerator
	{
	public:

		contentGenerator(brogueLevel* level);
		~contentGenerator();

		void initialize();
		void generateLevelContents();

	private:

		void placeStairs();

	private:

		brogueLevel* _level;

		grid<bool>* _validLocations;	// This should be initialized from the terrain layers. Each dungeon content placed will 
										// set its location to "false".

	};
}
