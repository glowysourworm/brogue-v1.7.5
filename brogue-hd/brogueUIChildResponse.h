#pragma once

#include "brogueProgramData.h"
#include "simple.h"

namespace brogueHd::frontend::opengl
{
	struct brogueUIChildResponse
	{
		brogueUIChildResponse()
		{
			data = default_value::value<brogueProgramData>();
			mouseLeftRegistered = false;
			mouseHoverRegistered = false;
			mouseScrollRegistered = false;
			shouldUpdate = false;
		}
		brogueUIChildResponse(const brogueProgramData& responseData, bool mouseLeft, bool mouseHover, bool mouseScroll, bool update)
		{
			data = responseData;
			mouseLeftRegistered = mouseLeft;
			mouseHoverRegistered = mouseHover;
			mouseScrollRegistered = mouseScroll;
			shouldUpdate = update;
		}
		brogueUIChildResponse(const brogueUIChildResponse& copy)
		{
			copyImpl(copy);
		}
		void operator=(const brogueUIChildResponse& copy)
		{
			copyImpl(copy);
		}

		brogueProgramData data;

		bool mouseLeftRegistered;
		bool mouseHoverRegistered;
		bool mouseScrollRegistered;

		bool shouldUpdate;

	private:

		void copyImpl(const brogueUIChildResponse& copy)
		{
			data = copy.data;
			mouseLeftRegistered = copy.mouseLeftRegistered;
			mouseHoverRegistered = copy.mouseHoverRegistered;
			mouseScrollRegistered = copy.mouseScrollRegistered;
			shouldUpdate = copy.shouldUpdate;
		}
	};
}