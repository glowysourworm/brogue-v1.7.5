#pragma once

#include "gridRect.h"
#include "gridDefinitions.h"
#include "randomGenerator.h"
#include <simpleException.h>

namespace brogueHd::component
{
	using namespace simple;

	class noiseGenerator
	{
	public:
		noiseGenerator(randomGenerator* randomGenerator);
		virtual ~noiseGenerator();

		virtual void run(const gridRect& parentBoundary, const gridRect& relativeBoundary, gridCallback<bool> callback);
	};

	noiseGenerator::noiseGenerator(randomGenerator* randomGenerator)
	{
	}

	noiseGenerator::~noiseGenerator()
	{
	}

	void noiseGenerator::run(const gridRect& parentBoundary, const gridRect& relativeBoundary,
	                         gridCallback<bool> callback)
	{
		throw simpleException("Run method must be overridden in child class:  noiseGenerator.h");
	}
}
