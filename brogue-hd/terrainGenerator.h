#pragma once

#include "brogueLayout.h"
#include "randomGenerator.h"

namespace brogueHd::backend
{
	class terrainGenerator
	{
	public:

		terrainGenerator(randomGenerator* randomGenerator);
		~terrainGenerator();

		void generateTerrain(brogueLayout* layout);

	private:

		randomGenerator* _randomGenerator;
	};

	terrainGenerator::terrainGenerator(randomGenerator* randomGenerator)
	{
		_randomGenerator = randomGenerator;
	}

	terrainGenerator::~terrainGenerator()
	{

	}

	void terrainGenerator::generateTerrain(brogueLayout* layout)
	{

	}
}