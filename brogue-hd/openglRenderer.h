#pragma once

#ifndef MYGL
#define MYGL
#include "gl.h"
#include <GLFW/glfw3.h>
#endif

#include <mutex>
#include <thread>
#include <chrono>
#include <functional>

#include "simpleKeyboardState.h"
#include "simpleMouseState.h"
#include "simpleLogger.h"
#include "simpleException.h"
#include "gridRect.h"
#include "brogueProgramContainer.h"
#include "brogueUIConstants.h"
#include "simpleExt.h"
#include "eventController.h"
#include "brogueUITagAction.h"
#include "simplePeriodCounter.h"
#include "brogueGlobal.h"
#include "brogueMouseState.h"
#include "brogueKeyboardState.h"
#include "simple.h"
#include "brogueUIStateChanger.h"

using namespace brogueHd::simple;
using namespace brogueHd::component;

namespace brogueHd::frontend
{
	// Contended Resources
	static simpleKeyboardState* KeyState;
	static simpleMouseState* MouseState;

	class openglRenderer
	{
	public:
		
		openglRenderer(eventController* eventController);
		~openglRenderer();

		/// <summary>
		/// Sets the program pointer for use
		/// </summary>
		void setProgram(brogueProgramContainer* program, BrogueGameMode gameMode);

		/// <summary>
		/// Game mode will handle the context switching for the program. So, there is two-way
		/// communication using the game mode. The render thread will check for a new one inside
		/// the thread lock. So, this will be set when the render routine is done for one cycle.
		/// </summary>
		void setGameMode(BrogueGameMode gameMode);

		/// <summary>
		/// Starts program rendering thread, and opens window using GLFW
		/// </summary>
		void startProgram();

		/// <summary>
		/// Terminates rendering thread; and destroys program memory - calling GL functions; and 
		/// calling delete.
		/// </summary>
		void terminateProgram();

		// Shared resources
		BrogueGameMode getRequestedMode() const;
		brogueKeyboardState getKeyboardState() const;
		brogueMouseState getMouseState() const;

	private:

		bool isInitializedGL() const;
		void initializeOpenGL(const gridRect& sceneBoundaryUI);

	private:

		void destroyGL();

		// OpenGL backend callbacks via GLFW

		static void errorCallback(int error, const char* message);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

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

	public:

		void thread_brogueUIClickEvent(brogueUIProgram sender, const brogueUITagAction& response);

	private:

		void thread_start();

	private:

		eventController* _eventController;
		int _clickToken;

		brogueProgramContainer* _program;
		simplePeriodCounter* _uiEventDebouncer;
		brogueUIStateChanger* _uiStateChanger;

		// Two way communication for the rendering context
		BrogueGameMode _gameMode;
		BrogueGameMode _gameModeIn;
		BrogueGameMode _gameModeOut;
		
		GLFWwindow* _window;
		bool _initializedGL;

		// Create Lock Mechanism
		std::mutex* _threadLock;
		std::thread* _thread;
	};

	openglRenderer::openglRenderer(eventController* eventController)
	{
		_initializedGL = false;
		_program = nullptr;
		_thread = nullptr;
		_threadLock = new std::mutex();
		_eventController = eventController;
		_uiEventDebouncer = new simplePeriodCounter(2);
		_uiStateChanger = new brogueUIStateChanger(brogueUIState::MainMenu);

		_clickToken = eventController->getUIClickEvent()->subscribe(std::bind(&openglRenderer::thread_brogueUIClickEvent, this, std::placeholders::_1, std::placeholders::_2));
		_gameModeOut = BrogueGameMode::Title;
		_gameModeIn = BrogueGameMode::Title;
		_gameMode = BrogueGameMode::Title;
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

		// Unhook Events
		if (_clickToken > 0)
			_eventController->getUIClickEvent()->unSubscribe(_clickToken);

		delete _uiEventDebouncer;
		delete _uiStateChanger;
	}

#pragma region GLFW Callbacks
	void openglRenderer::errorCallback(int error, const char* message)
	{
		// THREAD SAFETY:  The std::cout is the only shared static object on this execution stack. Pretty sure it is thread safe.
		//
		simpleLogger::logColor(brogueConsoleColor::Red, "GLFW Error {} {}", simpleExt::toString(error).c_str(), message);
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
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			if (!brogueHd::frontend::KeyState->hasKey(key))
				brogueHd::frontend::KeyState->setKey(key);
		}
		else if (action == GLFW_RELEASE)
		{
			brogueHd::frontend::KeyState->clearKey(key);
		}
		else
			simpleException::show("Unknown GLFW key callback action {}:  openglRenderer::keyCallback", action);

		brogueHd::frontend::KeyState->setModifier(mods);
	}
	void openglRenderer::mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		brogueHd::frontend::MouseState->updatePosition(xpos, ypos);
	}
	void openglRenderer::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		brogueHd::frontend::MouseState->updateButtons(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT),
										  glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT),
										  glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE));
	}
	void openglRenderer::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		brogueHd::frontend::MouseState->updateScroll(xoffset, yoffset);
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

	void openglRenderer::initializeOpenGL(const gridRect& sceneBoundaryUI)
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

		// Windowed Mode
		_window = glfwCreateWindow(sceneBoundaryUI.width, sceneBoundaryUI.height, "Brogue v1.7.5", NULL, NULL);

		// Open GL Context
		glfwMakeContextCurrent(_window);

		int version = gladLoadGL(glfwGetProcAddress);

		if (!version)
		{
			simpleLogger::logColor(brogueConsoleColor::Red, "Error calling gladLoadGL");

			glfwDestroyWindow(_window);
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
		glfwSetKeyCallback(_window, &openglRenderer::keyCallback);

		// Mouse Handlers
		glfwSetCursorPosCallback(_window, mousePositionCallback);
		glfwSetMouseButtonCallback(_window, mouseButtonCallback);
		glfwSetScrollCallback(_window, mouseScrollCallback);

		// Window Resize Callback
		glfwSetFramebufferSizeCallback(_window, &openglRenderer::resizeCallback);

		//// Window Refresh Callback
		//glfwSetWindowRefreshCallback(window, &openglRenderer::refreshCallback);

		// Window Close Callback (NOT NEEDED! This is a callback to perform before window closes)
		glfwSetWindowCloseCallback(_window, &openglRenderer::windowCloseCallback);

		// GL Enable Debug Output:
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(&openglRenderer::debugMessage, NULL);

		// (Not sure)
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

		// Khronos Group:       glViewport(x, y, width, height)
		// Window Coordinates:  (x_w, y_w), (x, y) - position offsets, (width, height) - window size
		// GL Coordinates:		(x_nd, y_nd)

		// x_w = (x_nd + 1)(width / 2) + x
		// y_w = (y_nd + 1)(height / 2) + y

		// Initialize the viewport
		glViewport(0, 0, sceneBoundaryUI.width, sceneBoundaryUI.height);
	}
	void openglRenderer::setProgram(brogueProgramContainer* program, BrogueGameMode gameMode)
	{
		// (CRITICAL!)  The program pointer is shared. So, this set function is called during 
		//				initialization, but still before GL is initialized. The thread_start
		//				method will take care of everything else for the backend. So, the program
		//			    container is not yet compiled. (for all of its parts)

		_program = program;
		_gameMode = gameMode;
		_gameModeIn = gameMode;
		_gameModeOut = gameMode;
	}
	void openglRenderer::setGameMode(BrogueGameMode gameMode)
	{
		_threadLock->lock();
		_gameModeIn = gameMode;
		_threadLock->unlock();
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
		delete _thread;
		_thread = nullptr;

		// (MEMORY!) All other pieces of memory have been deleted. The program container is managed
		//			 by the rendering controller. These are nulled here for reference; and to guard
		//			 other methods during a program change.
		//
		brogueHd::frontend::KeyState = nullptr;
		brogueHd::frontend::MouseState = nullptr;
		_program = nullptr;
	}
	void openglRenderer::destroyGL()
	{
		if (_thread != nullptr)
			simpleException::show("Trying to terminate GL backend before shutting down rendering thread. Please call terminateProgram() first.");

		glfwTerminate();

		_initializedGL = false;
	}

	BrogueGameMode openglRenderer::getRequestedMode() const
	{
		BrogueGameMode mode;

		_threadLock->lock();
		mode = _gameModeOut;
		_threadLock->unlock();

		return mode;
	}
	brogueKeyboardState openglRenderer::getKeyboardState() const
	{
		brogueKeyboardState keyboard;

		_threadLock->lock();

		if (brogueHd::frontend::KeyState == nullptr)
			keyboard = default_value::value<brogueKeyboardState>();

		else
			keyboard = _program->getKeyboardState(*brogueHd::frontend::KeyState);

		_threadLock->unlock();

		return keyboard;
	}
	brogueMouseState openglRenderer::getMouseState() const
	{
		brogueMouseState mouse;

		_threadLock->lock();

		if (brogueHd::frontend::MouseState == nullptr)
			mouse = default_value::value<brogueMouseState>();

		else
			mouse = _program->getMouseState(*brogueHd::frontend::MouseState);

		_threadLock->unlock();

		return mouse;
	}
	bool openglRenderer::isInitializedGL() const
	{
		return _initializedGL;
	}

	void openglRenderer::thread_brogueUIClickEvent(brogueUIProgram sender, const brogueUITagAction& tagAction)
	{
		// Thread Mutex Lock (Still Active)

		if (_gameModeOut != _gameMode)
			return;

		switch (tagAction.action)
		{
			// Click registered by a background program (typically)
			case brogueUIAction::None:
			{
				if (sender == brogueUIProgram::FlameMenuProgram)
				{
					_program->deactivateUIAll();
					_program->activateUIProgram(brogueUIProgram::MainMenuProgram);
					_program->clearEvents();
				}
			}
			break;

			case brogueUIAction::StateChange:
			{
				switch (tagAction.desiredState)
				{
					case brogueUIState::MainMenu:
					{
						_program->deactivateUIAll();
						_program->activateUIProgram(brogueUIProgram::MainMenuProgram);
					}
					break;
					case brogueUIState::OpenMenu:
					{
						_program->deactivateUIAll();
						_program->activateUIProgram(brogueUIProgram::OpenMenuBackgroundProgram);
						_program->activateUIProgram(brogueUIProgram::OpenMenuProgram);
					}
					break;
					case brogueUIState::PlaybackMenu:
					{
						_program->deactivateUIAll();
						_program->activateUIProgram(brogueUIProgram::PlaybackMenuBackgroundProgram);
						_program->activateUIProgram(brogueUIProgram::PlaybackMenuProgram);
					}
					break;
					case brogueUIState::HighScores:
					{
						_program->deactivateUIAll();
						_program->activateUIProgram(brogueUIProgram::HighScoresProgram);
					}
					break;
					case brogueUIState::GameNormal:
					{

					}
					break;
					case brogueUIState::GameLogOpen:
					{
						
					}
					break;
					case brogueUIState::GameMenuOpen:
					case brogueUIState::GameInventoryOpen:
					case brogueUIState::GameDiscoveredItemsOpen:
					case brogueUIState::GameHelpOpen:
					case brogueUIState::Modal:
					default:
						break;
				}

				// Clear out mouse events (action handled)
				_program->clearEvents();

				// Initiate a state change signal to the view tree
				_program->initiateStateChange(_uiStateChanger->getCurrentState(), tagAction.desiredState);

				// Keep a record of the state change for use in the render loop
				_uiStateChanger->set(tagAction.desiredState);
			}
			break;

			case brogueUIAction::NewGame:
				_gameModeOut = BrogueGameMode::Game;
				break;

			case brogueUIAction::OpenGame:
			{
			}
			break;

			case brogueUIAction::QuitGame:
			{

			}
			break;
			default:
				break;
		}
	}
	void openglRenderer::thread_start()
	{
		int intervalMilliseconds = 5;

		// THREAD:  LOCK TO INITIALIZE
		_threadLock->lock();

		// Get the calculated scene boundary from the UI view components
		gridRect sceneBoundaryUI = _program->getSceneBoundaryUI(); 

		// Initialize GL
		if (!_initializedGL)
			initializeOpenGL(sceneBoundaryUI);

		// *** Compile our program for the main loop:  GL Functions must be called after calling glfwMakeContextCurrent.
		//
		_program->initialize();
		_program->setMode(_gameMode);	// TODO: Make this part of the initialize call

		if (_program->hasErrors())
		{
			_program->showErrors();

			// THREAD:  UNLOCK TO RETURN
			_threadLock->unlock();
			return;
		}

		// Keyboard / Mouse State
		brogueHd::frontend::KeyState = new simpleKeyboardState();
		brogueHd::frontend::MouseState = new simpleMouseState();

		// Update the viewport for the current program
		glViewport(0, 0, sceneBoundaryUI.width, sceneBoundaryUI.height);

		// THREAD:  UNLOCK TO ENTER PRIMARY LOOP
		_threadLock->unlock();

		// Main Rendering Loop (haven't seen any issues with this on the other thread.)
		while (!glfwWindowShouldClose(_window) /* && (_gameModeRequest == _gameMode) */)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(intervalMilliseconds));

			// Update from main thread's brogueView*
			_threadLock->lock();

			// Procedure: (CRITICAL SECTION)
			//
			// 0) Copy mouse and keyboard state onto the stack
			// 1) Run checkUpdate (sets UI data on the tree)
			// 2) Run queries: Was Deactivated? Was Action Met? NeedsUpdate?
			// 3) Action Met -> Change View Programs
			// 4) Deactivated -> (May) Change View Programs
			// 5) Needs Update -> View needs re-buffered
			// 6) Run the drawing program(s)
			// 7) Log Errors, Reset Mouse State, finish draw pass.
			//

			simpleMouseState mouseState(*brogueHd::frontend::MouseState);
			simpleKeyboardState keyboardState(*brogueHd::frontend::KeyState);
			bool gameModeChangeThisIteration = _gameModeIn != _gameMode;

			// Program Context Change (sent from main thread)
			if (gameModeChangeThisIteration)
			{
				_program->setMode(_gameModeIn);
				_gameMode = _gameModeIn;
			}

			// UI Animations
			if (_uiStateChanger->isChanging())
			{
				// Are animations still running on the UI tree?
				if (!_program->checkStateChange())
				{
					// Animations are finished -> clear UI tree.
					_program->clearStateChange();

					// Reset the state changer
					_uiStateChanger->clear();
				}
			}

			// Check Update(s) -> (Click Events) (can signal view change) (animations will also be aware for state changes)
			_program->checkUpdate(keyboardState, mouseState, intervalMilliseconds);

			// Updates Program Buffers:  brogueViewBase* -> simpleDataStream (gets sent to GPU)
			//							 Check normal program update (view state change) to force 
			//							 update (to active programs only!)
			//
			if (_program->needsUpdate() || gameModeChangeThisIteration)
				_program->update(keyboardState, mouseState, intervalMilliseconds, gameModeChangeThisIteration);

			// Run drawing program
			_program->run(intervalMilliseconds);										// Run() -> Draws the buffers
			_program->showErrors();														// Log Errors to simpleLogger -> std::cout
			_program->clearUpdate();													// Clear Update Flags

			GLenum error = glGetError();

			if (error)
				simpleLogger::log(getGLErrorString(error));

			// Scroll data has already been consumed by the view tree
			brogueHd::frontend::MouseState->resetScroll();

			_threadLock->unlock();

			glfwSwapBuffers(_window);
			glfwPollEvents();
		}

		// Keyboard / Mouse State (cleanup)
		delete brogueHd::frontend::KeyState;
		delete brogueHd::frontend::MouseState;

		//_program->teardown();

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

