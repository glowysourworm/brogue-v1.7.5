#pragma once

#include "brogueUIConstants.h"
#include "simple.h"

using namespace brogueHd::frontend;
using namespace brogueHd::simple;
using namespace brogueHd::frontend::ui;

namespace brogueHd::frontend::opengl
{
	struct programControlData : hashable
	{
		programControlData()
		{
			currentProgram = brogueUIProgram::ContainerControlledProgram;
			nextProgram = brogueUIProgram::ContainerControlledProgram;
			currentContainer = brogueUIContainer::TitleContainer;
			currentView = brogueUIView::Unnamed;
			nextView = brogueUIView::Unnamed;
			exitContainer = false;
			exitOnMouseButton = true;
			exitOnMouseClickOff = true;
			exitOnKeyboardAny = true;
			exitOnKeyboardEsc = true;
			exitOnKeyboardHotkey = false;
			hotkey = '\0';
		}
		programControlData(brogueUIContainer container, brogueUIProgram program, brogueUIProgram programNext, brogueUIView view, brogueUIView viewNext, bool containerExit, bool mouseButtonExit, bool mouseOffClickExit, bool keyboardAnyExit, bool keyboardEscExit, bool keyboardHotKeyExit, char ahotkey)
		{
			currentContainer = container;
			currentProgram = program;
			nextProgram = programNext;
			currentView = view;
			nextView = viewNext;
			exitContainer = containerExit;
			exitOnMouseButton = mouseButtonExit;
			exitOnMouseClickOff = mouseOffClickExit;
			exitOnKeyboardAny = keyboardAnyExit;
			exitOnKeyboardEsc = keyboardEscExit;
			exitOnKeyboardHotkey = keyboardHotKeyExit;
			hotkey = ahotkey;
		}
		programControlData(const programControlData& copy)
		{
			copyImpl(copy);
		}
		~programControlData() {}

		void operator=(const programControlData& copy)
		{
			copyImpl(copy);
		}

		brogueUIContainer currentContainer;
		brogueUIProgram currentProgram;
		brogueUIProgram nextProgram;
		brogueUIView currentView;
		brogueUIView nextView;
		bool exitContainer;
		bool exitOnMouseButton;
		bool exitOnMouseClickOff;
		bool exitOnKeyboardAny;
		bool exitOnKeyboardEsc;
		bool exitOnKeyboardHotkey;
		char hotkey;

	private:

		void copyImpl(const programControlData& copy)
		{
			currentContainer = copy.currentContainer;
			currentProgram = copy.currentProgram;
			nextProgram = copy.nextProgram;
			currentView = copy.currentView;
			nextView = copy.nextView;
			exitContainer = copy.exitContainer;
			exitOnMouseButton = copy.exitOnMouseButton;
			exitOnMouseClickOff = copy.exitOnMouseClickOff;
			exitOnKeyboardAny = copy.exitOnKeyboardAny;
			exitOnKeyboardEsc = copy.exitOnKeyboardEsc;
			exitOnKeyboardHotkey = copy.exitOnKeyboardHotkey;
			hotkey = copy.hotkey;
		}
	};
}