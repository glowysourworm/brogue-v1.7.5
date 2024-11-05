#pragma once
#include "brogueUIConstants.h"
#include "simple.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
	struct brogueProgramData
	{
		brogueProgramData()
		{
			glfwHotkey = -1;
			fileName = default_value::value<simpleString>();
		}
		brogueProgramData(int hotkeyCodeGLFW, simpleString fileChosen)
		{
			glfwHotkey = hotkeyCodeGLFW;
			fileName = fileChosen;
		}
		brogueProgramData(const brogueProgramData& copy)
		{
			glfwHotkey = copy.glfwHotkey;
			fileName = copy.fileName;
		}
		void operator=(const brogueProgramData& copy)
		{
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
	};
}