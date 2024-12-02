#pragma once

#include "machineConstants.h"
#include "machineTemplate.h"

namespace brogueHd::model
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

	brogueMachine::brogueMachine(machineBase generator, machineFeatureFlags featureFlags)
	{
		_generator = generator;
		_featureFlags = featureFlags;
	}

	brogueMachine::~brogueMachine()
	{
	}

	machineBase brogueMachine::getGenerator() const
	{
		return _generator;
	}

	machineFeatureFlags brogueMachine::getFeatureFlags() const
	{
		return _featureFlags;
	}
}
