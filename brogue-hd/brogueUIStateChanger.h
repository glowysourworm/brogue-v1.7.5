#pragma once
#include "brogueUIConstants.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	struct brogueUIStateChanger
	{
		brogueUIStateChanger(brogueUIState current)
		{
			nextState = current;
			currentState = current;
		}

		void set(brogueUIState next)
		{
			nextState = next;
		}

		void clear()
		{
			currentState = nextState;
		}

		brogueUIState getCurrentState()
		{
			return currentState;
		}

		bool isChanging() const
		{
			return nextState != currentState;
		}

	private:

		brogueUIState nextState;
		brogueUIState currentState;
	};
}