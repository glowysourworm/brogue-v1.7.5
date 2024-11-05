#pragma once

#include "brogueUITagAction.h"
#include "simple.h"

namespace brogueHd::frontend::opengl
{
	struct brogueUIChildResponse
	{
		brogueUIChildResponse()
		{
			tag = default_value::value<brogueUITagAction>();
			actionMet = false;
			needsUpdate = false;
		}
		brogueUIChildResponse(const brogueUITagAction& responseData, bool aactionMet, bool aneedsUpdate)
		{
			tag = responseData;
			actionMet = aactionMet;
			needsUpdate = aneedsUpdate;
		}
		brogueUIChildResponse(const brogueUIChildResponse& copy)
		{
			copyImpl(copy);
		}
		void operator=(const brogueUIChildResponse& copy)
		{
			copyImpl(copy);
		}

		brogueUITagAction tag;

		bool actionMet;
		bool needsUpdate;

	private:

		void copyImpl(const brogueUIChildResponse& copy)
		{
			tag = copy.tag;
			actionMet = copy.actionMet;
			needsUpdate = copy.needsUpdate;
		}
	};
}