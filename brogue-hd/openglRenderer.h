#pragma once

#ifndef MYGL
#define MYGL
#include <windows.h>
#include "gl.h"
#include <GLFW/glfw3.h>
#endif

#include <mutex>
#include <thread>
#include <functional>

#include "brogueProgram.h"
#include "simpleQueue.h"
#include "simpleShaderProgram.h"
#include "simpleLogger.h"
#include "simpleException.h"
#include "simpleBoundingBox.h"
#include "gridRect.h"

using namespace brogueHd::simple;
using namespace brogueHd::component;

namespace brogueHd::frontend::opengl
{
	/// <summary>
	/// Function that must return a pointer to a new data stream for re-buffering
	/// </summary>
	//template<typename T>
	//using openglRebufferHandler = std::function<simpleDataStream<T>*(int millisecondsLapsed);

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
		void setProgram(brogueProgram* program);

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

		static const char* getGLErrorString(GLenum error);
		static const char* getGLTypeString(GLenum errorType);
		static const char* getGLSourceString(GLenum errorSource);

	private:

		void thread_start();

	private:

		brogueProgram* _program;

		bool _initializedGL;

		// Create Lock Mechanism
		std::mutex* _threadLock;
		std::thread* _thread;
	};

	openglRenderer::openglRenderer()
	{
		_initializedGL = false;
		_program = nullptr;
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

		if (_program != nullptr)
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
		simpleLogger::log("GL Debug:  Source: {} Type: {} Id: {} Severity:{} Message:   {}", openglRenderer::getGLSourceString(source), openglRenderer::getGLTypeString(type), id, severity, message);
	}

	void openglRenderer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	void openglRenderer::resizeCallback(GLFWwindow* window, int height, int width)
	{	
		// This will reset the coordinate system. Probably only needed once if the 
		// size of the game display or aspect ratio is changed by the user. The window
		// doesn't matter to the GL backend.
		// 
		//glViewport(0, 0, width, height);
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
	void openglRenderer::setProgram(brogueProgram* program)
	{
		if (_program != nullptr)
			simpleException::show("Trying to set a new program to the opengl renderer before terminating the old program.");

		// Shared Program Pointer (the program is acutally built, separating it from the brogueView)
		_program = program;
	}
	void openglRenderer::startProgram()
	{
		if (_program == nullptr)
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

		delete _program;

		_program = nullptr;
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

		// THREAD:  LOCK TO INITIALIZE
		_threadLock->lock();

		gridRect sceneBoundaryUI = _program->getSceneBoundaryUI();

		// Windowed Mode
		GLFWwindow* window = glfwCreateWindow(sceneBoundaryUI.width, sceneBoundaryUI.height, "Brogue v1.7.5", NULL, NULL);

		// Open GL Context
		glfwMakeContextCurrent(window);

		int version = gladLoadGL(glfwGetProcAddress);

		if (!version)
		{
			simpleLogger::logColor(brogueConsoleColor::Red, "Error calling gladLoadGL");

			glfwDestroyWindow(window);
			//glfwTerminate();

			// THREAD:  UNLOCK TO RETURN
			_threadLock->unlock();
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
		_program->compile();		// Declares program and program pieces on GL backend
		_program->bind(true);		// Activates program and program pieces on GL backend

		if (_program->hasErrors())
		{
			// THREAD:  UNLOCK TO RETURN
			_threadLock->unlock();
			return;
		}

		// Initialize the viewport
		//int surfaceWidth, surfaceHeight;
		//glfwGetFramebufferSize(window, &surfaceWidth, &surfaceHeight);

		// Khronos Group:       glViewport(x, y, width, height)
		// Window Coordinates:  (x_w, y_w), (x, y) - position offsets, (width, height) - window size
		// GL Coordinates:		(x_nd, y_nd)

		// x_w = (x_nd + 1)(width / 2) + x
		// y_w = (y_nd + 1)(height / 2) + y

		//float viewportX = (_sceneBoundaryGL.x + 1) * (windowWidth / 2.0f);
		//float viewportY = (_sceneBoundaryGL.y + 1) * (windowHeight / 2.0f);
		//float viewportWidth = (_sceneBoundaryGL.width + 1) * (windowWidth / 2.0f);
		//float viewportHeight = (_sceneBoundaryGL.height + 1) * (windowHeight / 2.0f);

		//glViewport(_sceneBoundaryUI.left(), _sceneBoundaryUI.top(), _sceneBoundaryUI.width, _sceneBoundaryUI.height);
		glViewport(0, 0, sceneBoundaryUI.width, sceneBoundaryUI.height);

		// Initialize Program:  compile() -> bind(true)
		_program->initialize();
		_program->hasErrors();						// log errors

		// THREAD:  UNLOCK TO ENTER PRIMARY LOOP
		_threadLock->unlock();

		int intervalMilliseconds = 100;

		// Main Rendering Loop
		while (!glfwWindowShouldClose(window))
		{
			// Update from main thread's brogueView*
			_threadLock->lock();

			_program->update(intervalMilliseconds);						// Updates program buffers from the UI view
			_program->run();							// Run() -> Draws the buffers
			_program->hasErrors();						// log errors

			GLenum error = glGetError();

			if (error)
				simpleLogger::log(getGLErrorString(error));

			_threadLock->unlock();

			glfwSwapBuffers(window);
			glfwPollEvents();

			std::this_thread::sleep_for(std::chrono::milliseconds(intervalMilliseconds));
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
	const char* openglRenderer::getGLTypeString(GLenum errorType)
	{
		switch (errorType)
		{
		case GL_DEBUG_TYPE_ERROR:				return "GL_DEBUG_TYPE_ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY:			return "GL_DEBUG_TYPE_PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE:			return "GL_DEBUG_TYPE_PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER:				return "GL_DEBUG_TYPE_MARKER";
		case GL_DEBUG_TYPE_PUSH_GROUP:			return "GL_DEBUG_TYPE_PUSH_GROUP";
		case GL_DEBUG_TYPE_POP_GROUP:			return "GL_DEBUG_TYPE_POP_GROUP";
		case GL_DEBUG_TYPE_OTHER:				return "GL_DEBUG_TYPE_OTHER";
		default:
			return "Unknown Error Type";
		}
	}
	const char* openglRenderer::getGLSourceString(GLenum errorSource)
	{
		switch (errorSource)
		{
		case GL_DEBUG_SOURCE_API:				return "GL_DEBUG_SOURCE_API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "GL_DEBUG_SOURCE_SHADER_COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY:		return "GL_DEBUG_SOURCE_THIRD_PARTY";
		case GL_DEBUG_SOURCE_APPLICATION:		return "GL_DEBUG_SOURCE_APPLICATION";
		case GL_DEBUG_SOURCE_OTHER:				return "GL_DEBUG_SOURCE_OTHER";
		default:
			return "Unknown Source Type";
		}
	}
}

