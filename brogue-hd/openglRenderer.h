#pragma once

#ifndef MYGL
#define MYGL
#include <windows.h>
#include "gl.h"
#include <GLFW/glfw3.h>
#endif

#include <mutex>
#include <thread>

#include "simpleShaderProgram.h"
#include "simpleLogger.h"
#include "simpleException.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
	class openglRenderer
	{
	public:
		openglRenderer();
		~openglRenderer();

		// Making OpenGL / Window calls static to work with rendering controller.

		/// <summary>
		/// Initializes OpenGL using static calls to backend
		/// </summary>
		void initializeOpenGL();

		/// <summary>
		/// Sets the program pointer for use
		/// </summary>
		void setProgram(simpleShaderProgram* program);

		/// <summary>
		/// Starts program rendering thread, and opens window using GLFW
		/// </summary>
		void startProgram();

		/// <summary>
		/// Terminates rendering thread; and destroys program memory - calling GL functions; and 
		/// calling delete.
		/// </summary>
		void terminateProgram();

		bool isInitializedGL() const
		{
			return _initializedGL;
		}

	private: 

		void destroyGL();

		// OpenGL backend callbacks via GLFW

		static void errorCallback(int error, const char* message);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void resizeCallback(GLFWwindow* window, int height, int width);
		static void refreshCallback(GLFWwindow* window);
		static void windowCloseCallback(GLFWwindow* window);

		static void forceLastErrorGLFW();

		// GL Debug Enable
		static void debugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
								 const GLchar* message, const void* userParam);

		const char* getGLErrorString(GLenum error);

	private:

		void thread_start();

	private:

		simpleShaderProgram* _glProgram;

		bool _initializedGL;

		// Create Lock Mechanism
		std::mutex* _threadLock;
		std::thread* _thread;
	};

	openglRenderer::openglRenderer()
	{
		_initializedGL = false;
		_glProgram = nullptr;
		_thread = nullptr;
		_threadLock = new std::mutex();
	}
	openglRenderer::~openglRenderer()
	{
		if (_thread != nullptr)
		{
			_thread->join();

			delete _thread;
			delete _threadLock;
		}

		if (_glProgram != nullptr)
		{
			terminateProgram();
		}

		destroyGL();

		_initializedGL = false;
	}

#pragma region GLFW Callbacks
	void openglRenderer::errorCallback(int error, const char* message)
	{
		// THREAD SAFETY:  The std::cout is the only shared static object on this execution stack. Pretty sure it is thread safe.
		//
		simpleLogger::logColor(brogueConsoleColor::Red, "GLFW Error {} {}", simpleExt::toString(error), message);
	}
	void openglRenderer::forceLastErrorGLFW()
	{
		int errorCode = glfwGetError(NULL);

		if (errorCode)
			errorCallback(errorCode, "No Message");
	}
	void openglRenderer::debugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
									  const GLchar* message, const void* userParam)
	{
		simpleLogger::log("GL Debug:  Source: {} Type: {} Id: {} Severity:{} Message:   {}", source, type, id, severity, message);
	}

	void openglRenderer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	void openglRenderer::resizeCallback(GLFWwindow* window, int height, int width)
	{
		glViewport(0, 0, width, height);
	}
	void openglRenderer::refreshCallback(GLFWwindow* window)
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	void openglRenderer::windowCloseCallback(GLFWwindow* window)
	{
		glfwDestroyWindow(window);
	}
#pragma endregion

	void openglRenderer::initializeOpenGL()
	{
		if (_initializedGL)
			return;

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

			switch (glfwGetPlatform())
			{
				case GLFW_PLATFORM_WIN32:
					simpleLogger::logColor(brogueConsoleColor::Yellow, "glfwGetPlatform() == GLFW_PLATFORM_WIN32");
					break;
				case GLFW_PLATFORM_COCOA:
					simpleLogger::logColor(brogueConsoleColor::Yellow, "glfwGetPlatform() == GLFW_PLATFORM_WIN32");
					break;
				case GLFW_PLATFORM_ERROR:
					simpleLogger::logColor(brogueConsoleColor::Red, "glfwGetPlatform() == GLFW_PLATFORM_ERROR");
					break;
				case GLFW_PLATFORM_NULL:
					simpleLogger::logColor(brogueConsoleColor::Red, "glfwGetPlatform() == GLFW_PLATFORM_NULL");
					break;
				default:
					simpleLogger::logColor(brogueConsoleColor::Red, "glfwGetPlatform() not setup properly. Showing some other platform openglRenderer.h");
					break;
			}

			// Get full screen window details
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			simpleLogger::logColor(brogueConsoleColor::Yellow, glfwGetVersionString());

			_initializedGL = true;
		}
		else
		{
			// error
			simpleException::show("Initialization of GLFW failed! Cannot render graphics!");
		}
	}
	void openglRenderer::setProgram(simpleShaderProgram* program)
	{
		if (_glProgram != nullptr)
			simpleException::show("Trying to set a new program to the opengl renderer before terminating the old program.");

		// Shared Program Pointer (the program is acutally built, separating it from the brogueView)
		_glProgram = program;
	}
	void openglRenderer::startProgram()
	{
		if (_glProgram == nullptr)
			simpleException::show("Trying to start program before setting it. Please call setProgram(..)");

		if (_thread != nullptr)
			simpleException::show("Trying to start program on opengl renderer - which is currently running. Must call terminateProgram() first.");

		// Hook anything specific to the window on the other thread
		//
		_thread = new std::thread(&openglRenderer::thread_start, this);
	}
	void openglRenderer::terminateProgram()
	{
		if (_thread == nullptr)
			return;

		_thread->join();
		_thread = nullptr;

		delete _glProgram;

		_glProgram = nullptr;
	}
	void openglRenderer::destroyGL()
	{
		if (_thread != nullptr)
			simpleException::show("Trying to terminate GL backend before shutting down rendering thread. Please call terminateProgram() first.");

		glfwTerminate();

		_initializedGL = false;
	}

	void openglRenderer::thread_start()
	{
		// Full Screen Mode, Primary Monitor
		//window = glfwCreateWindow(mode->width, mode->height, "Brogue v1.7.5", monitor, NULL);

		int windowWidth = 640;
		int windowHeight = 480;

		// Windowed Mode
		GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Brogue v1.7.5", NULL, NULL);

		// Open GL Context
		glfwMakeContextCurrent(window);

		int version = gladLoadGL(glfwGetProcAddress);

		if (!version)
		{
			simpleLogger::logColor(brogueConsoleColor::Red, "Error calling gladLoadGL");

			glfwDestroyWindow(window);
			//glfwTerminate();
			return;
		}
		else
		{
			// Version
			simpleLogger::logColor(brogueConsoleColor::Yellow, "Glad Version:  {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
			simpleLogger::logColor(brogueConsoleColor::Yellow, "GL Shading Language:  {}", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
			simpleLogger::logColor(brogueConsoleColor::Yellow, "GL Version:  {}", (char*)glGetString(GL_VERSION));
			simpleLogger::logColor(brogueConsoleColor::Yellow, "GL Vendor:  {}", (char*)glGetString(GL_VENDOR));
			simpleLogger::logColor(brogueConsoleColor::Yellow, "GL Renderer:  {}", (char*)glGetString(GL_RENDERER));

			// Print out extension(s)
			GLint numExtensions;
			glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

			for (GLint index = 0; index < numExtensions; index++)
			{
				simpleLogger::logColor(brogueConsoleColor::Yellow, "GL Extension (loaded):  {}", (char*)glGetStringi(GL_EXTENSIONS, index));
			}
		}

		glfwSwapInterval(1);

		// Keyboard Handler
		glfwSetKeyCallback(window, &openglRenderer::keyCallback);

		// Window Resize Callback
		glfwSetFramebufferSizeCallback(window, &openglRenderer::resizeCallback);

		//// Window Refresh Callback
		//glfwSetWindowRefreshCallback(window, &openglRenderer::refreshCallback);

		// Window Close Callback (NOT NEEDED! This is a callback to perform before window closes)
		glfwSetWindowCloseCallback(window, &openglRenderer::windowCloseCallback);

		// GL Enable Debug Output:
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(&openglRenderer::debugMessage, NULL);

		// (Not sure)
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

		// *** Compile our program for the main loop: HOPING THIS IS OK WHEN DONE FROM THE RENDERING THREAD
		//
		// GL Functions must be called after calling glfwMakeContextCurrent.
		//
		_glProgram->compile();		// Declares program and program pieces on GL backend
		_glProgram->bind(true);		// Activates program and program pieces on GL backend

		if (_glProgram->hasErrors())
			return;

		// Initialize the viewport
		int surfaceWidth, surfaceHeight;
		glfwGetFramebufferSize(window, &surfaceWidth, &surfaceHeight);
		glViewport(0, 0, surfaceWidth, surfaceHeight);

		// Main Rendering Loop
		while (!glfwWindowShouldClose(window))
		{
			// Update from main thread's brogueView*
			_threadLock->lock();

			_glProgram->run();
			_glProgram->hasErrors();	// log errors

			GLenum error = glGetError();

			if (error)
				simpleLogger::log(getGLErrorString(error));

			_threadLock->unlock();

			glfwSwapBuffers(window);
			glfwPollEvents();

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		// Window could've been destroyed already
		//
		//glfwDestroyWindow(window);
	}

	const char* openglRenderer::getGLErrorString(GLenum error)
	{
		switch (error)
		{
		case GL_NO_ERROR:          return "No Error";
		case GL_INVALID_ENUM:      return "Invalid Enum";
		case GL_INVALID_VALUE:     return "Invalid Value";
		case GL_INVALID_OPERATION: return "Invalid Operation";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid Framebuffer Operation";
		case GL_OUT_OF_MEMORY:     return "Out of Memory";
		case GL_STACK_UNDERFLOW:   return "Stack Underflow";
		case GL_STACK_OVERFLOW:    return "Stack Overflow";
		case GL_CONTEXT_LOST:      return "Context Lost";
		default:                   return "Unknown Error";
		}
	}
}

