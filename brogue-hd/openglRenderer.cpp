#include "openglRenderer.h"
#include "simpleLogger.h"
#include "simpleString.h"
#include <mutex>

#include <thread>
#include <chrono>

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
	openglRenderer::openglRenderer()
	{
		_windowOpen = false;
	}
	openglRenderer::~openglRenderer()
	{

	}


	void openglRenderer::errorCallback(int error, const char* message)
	{
		simpleLogger::logColor(brogueConsoleColor::Red, "GLFW Error {} {}", simpleExt::toString(error), message);
	}
	void openglRenderer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	void openglRenderer::resizeCallback(GLFWwindow* window, int height, int width)
	{

	}
	void openglRenderer::refreshCallback(GLFWwindow* window)
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void openglRenderer::openWindow(simpleShaderProgram* program)
	{
		if (_windowOpen)
			return;

		// Shared Program Pointer (the program is acutally built, separating it from the brogueView)
		_glProgram = program;
		_windowOpen = true;

		// GLFW Errors
		glfwSetErrorCallback(&openglRenderer::errorCallback);

		// GLFW Init
		// 
		// https://www.glfw.org/docs/latest/quick.html#quick_steps
		//
		if (glfwInit() == GLFW_TRUE)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			// Get full screen window details
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			// Hook anything specific to the window on the other thread
			//
			_thread = new std::thread(&openglRenderer::thread_start, this);
		}
		else
		{
			// error
		}

		//glutInit(0, NULL);
		//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		//glutInitWindowSize(1024, 768);
		//glutInitWindowPosition(100, 100);
		//glutCreateWindow("Brogue Game Window");
		//glutDisplayFunc(&gameController::renderWindow);
		//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		//glutMainLoop();
		//glClear(GL_COLOR_BUFFER_BIT);
		//glutSwapBuffers();
	}

	void openglRenderer::thread_start()
	{
		// Create Lock Mechanism
		std::mutex threadLock;

		// Full Screen Mode, Primary Monitor
		//window = glfwCreateWindow(mode->width, mode->height, "Brogue v1.7.5", monitor, NULL);

		// Windowed Mode
		GLFWwindow* window = glfwCreateWindow(640, 480, "Brogue v1.7.5", NULL, NULL);

		// Open GL Context
		glfwMakeContextCurrent(window);

		if (!gladLoadGL(glfwGetProcAddress))
		{
			simpleLogger::logColor(brogueConsoleColor::Red, "Error calling gladLoadGL");

			glfwDestroyWindow(window);
			glfwTerminate();
			return;
		}

		glfwSwapInterval(1);

		// Keyboard Handler
		glfwSetKeyCallback(window, &openglRenderer::keyCallback);

		// Window Resize Callback
		glfwSetFramebufferSizeCallback(window, &openglRenderer::resizeCallback);

		//// Window Refresh Callback
		//glfwSetWindowRefreshCallback(window, &openglRenderer::refreshCallback);

		// Window Close Callback (NOT NEEDED! This is a callback to perform before window closes)
		//glfwSetWindowCloseCallback(window, &openglRenderer::windowCloseCallback);

		// *** Compile our program for the main loop:

		_glProgram->compile();		// Declares program and program pieces on GL backend
		_glProgram->bind(true);		// Activates program and program pieces on GL backend

		// Main Rendering Loop
		while (!glfwWindowShouldClose(window))
		{
			// Update from main thread's brogueView*
			threadLock.lock();

			_glProgram->drawAll();

			threadLock.unlock();

			glfwSwapBuffers(window);
			glfwPollEvents();

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	void openglRenderer::thread_terminate()
	{

	}

	void openglRenderer::closeWindow()
	{
		
	}
	void openglRenderer::renderWindow()
	{
		
	}
}