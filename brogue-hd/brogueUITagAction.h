#pragma once
#include "brogueUIConstants.h"
#include "simple.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
	struct brogueUITagAction
	{
		brogueUITagAction()
		{
			action = brogueUIAction::None;
			actionView = brogueUIView::Unnamed;
			glfwHotkey = -1;
			fileName = default_value::value<simpleString>();
			deactivateAction = brogueUIAction::None;
		}
		brogueUITagAction(brogueUIAction aaction, brogueUIAction adeactivateAction, brogueUIView aview, int hotkeyCodeGLFW, simpleString fileChosen)
		{
			action = aaction;
			deactivateAction = adeactivateAction;
			actionView = aview;
			glfwHotkey = hotkeyCodeGLFW;
			fileName = fileChosen;
		}
		brogueUITagAction(const brogueUITagAction& copy)
		{
			action = copy.action;
			actionView = copy.actionView;
			deactivateAction = copy.deactivateAction;
			glfwHotkey = copy.glfwHotkey;
			fileName = copy.fileName;
		}
		void operator=(const brogueUITagAction& copy)
		{
			action = copy.action;
			actionView = copy.actionView;
			deactivateAction = copy.deactivateAction;
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
		/// View associated with the action
		/// </summary>
		brogueUIView actionView;

		/// <summary>
		/// Action to perform - which is the purpose of the UI control
		/// </summary>
		brogueUIAction action;

		/// <summary>
		/// Action to perform when the view is considered "deactivated":  Escape, or Click Off.
		/// </summary>
		brogueUIAction deactivateAction;
	};
}