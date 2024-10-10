#pragma once

#include "terrainGenerator.h"
#include "simpleMath.h"

using namespace brogueHd::simple;

namespace brogueHd::backend::generator
{
	terrainGenerator::terrainGenerator(randomGenerator* randomGenerator)
	{
        _randomGenerator = randomGenerator;
	}

	terrainGenerator::~terrainGenerator()
	{

	}
}