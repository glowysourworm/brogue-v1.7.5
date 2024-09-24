#pragma once

#include "terrainGenerator.h"
#include "brogueMath.h"
#include "array2DExtension.h"

using namespace brogueHd::backend::math;
using namespace brogueHd::backend::model;
using namespace brogueHd::backend::model::layout;

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