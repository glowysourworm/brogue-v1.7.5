#pragma once

#include <GLFW/glfw3.h>

namespace brogueHd::frontend
{
	struct simpleKeyboardState
	{
		/*
			GLFW Keyboard Space:  https://www.glfw.org/docs/latest/group__keys.html

			See macro definitions for key press. The hotkeys will be w/o modifiers.

			GLFW Space:  32 (space) -> 348 (menu). There are some blanks in between
						 but you can figure out what you need from there. There's 
						 probably some translator code online.

						 The GLFW callback has two integers: keys and modifiers. So,
						 This array will track those; but it's in a static array form
						 for performance on the main loop.
		*/

	public:

		static const int GLFWLength = GLFW_KEY_MENU - GLFW_KEY_SPACE;
		static const int GLFWOffset = GLFW_KEY_SPACE;

	public:

		simpleKeyboardState()
		{

		}
		simpleKeyboardState(const simpleKeyboardState& copy)
		{
			for (int index = 0; index < GLFWLength; index++)
			{
				keys[index] = copy.keys[index];
				modifiers[index] = copy.modifiers[index];
			}
		}
		~simpleKeyboardState()
		{
		}

		void setKey(int key)
		{
			keys[key - GLFWOffset] = true;
		}
		void clearKey(int key)
		{
			keys[key - GLFWOffset] = false;
		}
		bool hasKey(int key)
		{
			return keys[key - GLFWOffset];
		}

		void setModifier(int key)
		{
			modifiers[key - GLFWOffset] = true;
		}
		void clearModifier(int key)
		{
			modifiers[key - GLFWOffset] = false;
		}
		bool hasModifier(int key)
		{
			return modifiers[key - GLFWOffset];
		}

	protected:

		bool keys[GLFWLength];
		bool modifiers[GLFWLength];

	};
}