#pragma once
#include "brogueUIConstants.h"
#include "simple.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
	struct brogueUITagAction : hashable
	{
		brogueUITagAction()
		{
			action = brogueUIAction::None;
			glfwHotkey = -1;
			fileName = default_value::value<simpleString>();
		}
		brogueUITagAction(brogueUIAction aaction, int hotkeyCodeGLFW, simpleString fileChosen)
		{
			action = aaction;
			glfwHotkey = hotkeyCodeGLFW;
			fileName = fileChosen;
		}
		brogueUITagAction(const brogueUITagAction& copy)
		{
			action = copy.action;
			glfwHotkey = copy.glfwHotkey;
			fileName = copy.fileName;
		}
		void operator=(const brogueUITagAction& copy)
		{
			action = copy.action;
			glfwHotkey = copy.glfwHotkey;
			fileName = copy.fileName;
		}

		/// <summary>
		/// GLFW key code for the hotkey (modifiers won't matter)
		/// </summary>
		int glfwHotkey;

		/// <summary>
		/// File name from the chosen file
		/// </summary>
		simpleString fileName;

		/// <summary>
		/// Action to perform - which is the purpose of the UI control
		/// </summary>
		brogueUIAction action;
	};
}