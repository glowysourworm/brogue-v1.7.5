#pragma once
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueProgramSignature.h"
#include "simple.h"

namespace brogueHd::frontend::opengl
{
	struct brogueProgramState
	{
		brogueProgramState()
		{
			mouseState = default_value::value<brogueMouseState>();
			keyboardState = default_value::value<brogueKeyboardState>();
			signature = default_value::value<brogueProgramSignature>();
		}
		brogueProgramState(const brogueMouseState& mouse,
						   const brogueKeyboardState& keyboard,
						   const brogueProgramSignature& state)
		{
			mouseState = mouse;
			keyboardState = keyboard;
			signature = state;
		}
		brogueProgramState(const brogueProgramState& copy)
		{
			copyImpl(copy);
		}

		brogueMouseState mouseState;
		brogueKeyboardState keyboardState;
		brogueProgramSignature signature;

	private:

		void copyImpl(const brogueProgramState& copy)
		{
			mouseState = copy.mouseState;
			keyboardState = copy.keyboardState;
			signature = copy.signature;
		}
	};
}