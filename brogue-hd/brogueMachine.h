#pragma once

#include "machineConstants.h"
#include "machineTemplate.h"

using namespace brogueHd::backend::model::gameTemplate;
using namespace brogueHd::backend::model::machine;

namespace brogueHd::backend::model
{
	class brogueMachine
	{
	public:
		
		brogueMachine(machineBase generator, machineFeatureFlags featureFlags);
		~brogueMachine();

		machineBase getGenerator() const;
		machineFeatureFlags getFeatureFlags() const;

	private:

		machineBase _generator;
		machineFeatureFlags _featureFlags;

	};
}

