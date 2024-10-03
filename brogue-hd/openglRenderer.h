#pragma once

#ifndef MYGL
#define MYGL
#include <windows.h>
#include "gl.h"
#include <GLFW/glfw3.h>
#endif

#include <thread>

#include "brogueView.h"

using namespace brogueHd::frontend::ui;

namespace brogueHd::frontend::opengl
{
	class openglRenderer
	{
	public:
		openglRenderer();
		~openglRenderer();

		// Making OpenGL / Window calls static to work with rendering controller.

		void openWindow(brogueView* view);
		void closeWindow();
		void renderWindow();

	private: 

		// OpenGL backend callbacks via GLFW

		static void errorCallback(int error, const char* message);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void resizeCallback(GLFWwindow* window, int height, int width);
		static void refreshCallback(GLFWwindow* window);
		static void windowCloseCallback(GLFWwindow* window);
		static void terminate(GLFWwindow* window);

	private:

		void thread_start();
		void thread_terminate();

	private:

		std::thread* _thread;
		brogueView* _view;
		bool _windowOpen;
	};
}

