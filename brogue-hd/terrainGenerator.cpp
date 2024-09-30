#pragma once

#include "terrainGenerator.h"
#include "brogueMath.h"

using namespace brogueHd::component::math;

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