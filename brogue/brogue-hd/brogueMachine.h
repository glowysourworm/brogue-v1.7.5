#pragma once

#include "machineConstants.h"
#include "machineFeature.h"
#include "machineGenerator.h"
#include "generationParameters.h"

using namespace brogueHd::backend::model::machine;

namespace brogueHd::backend::model
{
	class brogueMachine
	{
	public:
		
		brogueMachine(machineGenerator generator, machineFeatureFlags featureFlags);
		~brogueMachine();

		machineGenerator getGenerator() const;
		machineFeatureFlags getFeatureFlags() const;

	private:

		machineGenerator _generator;
		machineFeatureFlags _featureFlags;

	};
}

