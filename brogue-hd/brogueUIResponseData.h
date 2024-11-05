#pragma once

#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueProgramSignature.h"
#include "brogueUIChildResponse.h"
#include "simple.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
	struct brogueUIResponseData
	{

	public:

		brogueUIResponseData()
		{
			signature = default_value::value<brogueProgramSignature>();
			mouse = default_value::value<brogueMouseState>();
			keyboard = default_value::value<brogueKeyboardState>();
			response = default_value::value<brogueUIChildResponse>();
		}
		brogueUIResponseData(const brogueProgramSignature& programSignature, const brogueMouseState& mouseState, const brogueKeyboardState& keyboardState)
		{
			signature = programSignature;
			mouse = mouseState;
			keyboard = keyboardState;
		}
		brogueUIResponseData(const brogueUIResponseData& copy)
		{
			copyImpl(copy);
		}
		void operator=(const brogueUIResponseData& copy)
		{
			copyImpl(copy);
		}

		brogueProgramSignature signature;
		brogueMouseState mouse;
		brogueKeyboardState keyboard;

		/// <summary>
		/// Data sent from the leaf child ui element back to the parent (this will be from a child program / view)
		/// </summary>
		brogueUIChildResponse response;

	private:

		void copyImpl(const brogueUIResponseData& copy)
		{
			signature = copy.signature;
			mouse = copy.mouse;
			keyboard = copy.keyboard;
			response = copy.response;
		}
	};
}