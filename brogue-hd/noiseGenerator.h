#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "randomGenerator.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model;

namespace brogueHd::backend
{
	class noiseGenerator
	{
	public:
		noiseGenerator(randomGenerator* randomGenerator);
		~noiseGenerator();

		virtual void run(const gridRect& parentBoundary, const gridRect& relativeBoundary, gridCallback<bool> callback);
	};

	noiseGenerator::noiseGenerator(randomGenerator* randomGenerator)
	{
	}

	noiseGenerator::~noiseGenerator()
	{
	}

	void noiseGenerator::run(const gridRect& parentBoundary, const gridRect& relativeBoundary, gridCallback<bool> callback)
	{
		throw simpleException("Run method must be overridden in child class:  noiseGenerator.h");
	}
}
