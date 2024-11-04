#pragma once

#include "brogueUIConstants.h"

namespace brogueHd::frontend::ui
{
	struct brogueUIResponseData
	{
		brogueUIResponseData()
		{
			program = brogueUIProgram::ContainerControlledProgram;
			sender = brogueUIView::Unnamed;
			hotkey = '\0';
			hotkeyUsed = false;
			mouseLeft = false;
			mouseHover = false;
			mouseScroll = false;
			mouseUsed = false;
			shouldUpdate = false;
		}
		brogueUIResponseData(brogueUIProgram aprogram, brogueUIView asender, bool ashouldUpdate)
		{
			program = aprogram;
			sender = asender;
			shouldUpdate = ashouldUpdate;
		}
		brogueUIResponseData(const brogueUIResponseData& copy)
		{
			copyImpl(copy);
		}
		void operator=(const brogueUIResponseData& copy)
		{
			copyImpl(copy);
		}

		brogueUIProgram program;
		brogueUIView sender;
		char hotkey;
		bool hotkeyUsed;		// Hotkey consumed and used by the focused view
		bool mouseLeft;
		bool mouseHover;
		bool mouseScroll;
		bool mouseUsed;			// Mouse data consumed and used by the focused view

		bool shouldUpdate;

	private:

		void copyImpl(const brogueUIResponseData& copy)
		{
			program = copy.program;
			sender = copy.sender;
			hotkey = copy.hotkey;
			hotkeyUsed = copy.hotkeyUsed;
			mouseLeft = copy.mouseLeft;
			mouseHover = copy.mouseHover;
			mouseScroll = copy.mouseScroll;
			mouseUsed = copy.mouseUsed;
			shouldUpdate = copy.shouldUpdate;
		}
	};
}