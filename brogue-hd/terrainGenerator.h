#pragma once

#include "randomGenerator.h"

namespace brogueHd::backend::generator
{
	class terrainGenerator
	{
	public:

		terrainGenerator(randomGenerator* randomGenerator);
		~terrainGenerator();

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
}