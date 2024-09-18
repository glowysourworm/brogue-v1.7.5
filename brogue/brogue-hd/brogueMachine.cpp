#include "brogueMachine.h"

using namespace brogueHd::backend::model::machine;

namespace brogueHd::backend::model
{
	brogueMachine::brogueMachine(machineGenerator generator, machineFeatureFlags featureFlags)
	{
		_generator = generator;
		_featureFlags = featureFlags;
	}
	brogueMachine::~brogueMachine()
	{

	}

	machineGenerator brogueMachine::getGenerator() const
	{
		return _generator;
	}
	machineFeatureFlags brogueMachine::getFeatureFlags() const
	{
		return _featureFlags;
	}
}