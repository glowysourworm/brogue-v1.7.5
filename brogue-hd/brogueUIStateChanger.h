#pragma once
#include "brogueUIConstants.h"

namespace brogueHd::frontend
{
	using namespace simple;

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
