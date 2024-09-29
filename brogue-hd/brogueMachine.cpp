#include "brogueMachine.h"

using namespace brogueHd::backend::model::machine;

namespace brogueHd::backend::model
{
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