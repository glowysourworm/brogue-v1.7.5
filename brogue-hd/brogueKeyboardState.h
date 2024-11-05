#pragma once

namespace brogueHd::frontend::opengl
{
	struct brogueKeyboardState
	{
		/*
			GLFW Keyboard Space:  https://www.glfw.org/docs/latest/group__keys.html

			See macro definitions for key press. The hotkeys will be w/o modifiers.
		*/

		brogueKeyboardState()
		{
			_glfwHotkey = -1;
			_asciiHotkey = -1;
		}
		brogueKeyboardState(int glfwHotkey, int asciiHotkey)
		{
			_glfwHotkey = glfwHotkey;
			_asciiHotkey = asciiHotkey;
		}
		brogueKeyboardState(const brogueKeyboardState& copy)
		{
			_glfwHotkey = copy.getGlfwHotkey();
			_asciiHotkey = copy.getAsciiHotkey();
		}
		void operator=(const brogueKeyboardState& copy)
		{
			_glfwHotkey = copy.getGlfwHotkey();
			_asciiHotkey = copy.getAsciiHotkey();
		}

		int getGlfwHotkey() const
		{
			return _glfwHotkey;
		}
		int getAsciiHotkey() const
		{
			return _asciiHotkey;
		}

	private:

		int _glfwHotkey;
		int _asciiHotkey;

	};
}