#pragma once

#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleKeyboardState.h"
#include "simpleMouseState.h"

using namespace brogueHd::component;
using namespace brogueHd::frontend::ui;

namespace brogueHd::frontend::opengl
{
	class brogueProgram : hashableObject
	{
	public:

		brogueProgram(brogueUIProgram programName, bool isActive)
		{
			_programName = programName;
			_isActive = isActive;
		};
		~brogueProgram() {};

		/// <summary>
		/// Calls necessary routines to initialize the opengl program
		/// </summary>
		virtual void initialize()
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::initialize");
		}

		/// <summary>
		/// Function that deletes GL program from the backend
		/// </summary>
		virtual void teardown()
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::teardown");
		}

		virtual brogueKeyboardState calculateKeyboardState(const simpleKeyboardState& keyboard)
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::calculateKeyboardState");
		}

		virtual brogueMouseState calculateMouseState(const simpleMouseState& mouse)
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::calculateMouseState");
		}

		/// <summary>
		/// Following a similar pipeline to the brogueView, checkUpdate checks program parameters needed
		/// to update the view - also calling the view's checkUpdate method. (creating the pipeline)
		/// </summary>
		virtual void checkUpdate(const simpleKeyboardState& keyboardState,
								 const simpleMouseState& mouseState,
								 int millisecondsLapsed)
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::checkUpdate");
		}

		/// <summary>
		/// Returns true if the program tree needs to be updated
		/// </summary>
		virtual bool needsUpdate()
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::needsUpdate");
		}

		/// <summary>
		/// Clears mouse event data from the UI tree
		/// </summary>
		virtual void clearUpdate()
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::clearUpdate");
		}

		/// <summary>
		/// Clears mouse event data from the UI tree
		/// </summary>
		virtual void clearEvents()
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::clearEvents");
		}

		/// <summary>
		/// Following a similar pipeline to the brogueView, update checks program parameters needed
		/// to update the view - also calling the view's update method. (creating the pipeline)
		/// </summary>
		virtual void update(const simpleKeyboardState& keyboardState,
							const simpleMouseState& mouseState,
							int millisecondsLapsed)
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::update");
		}

		/// <summary>
		/// Runs the opengl program(s) for one draw pass
		/// </summary>
		virtual void run(int millisecondsElapsed)
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::run");
		}

		/// <summary>
		/// Outputs status for program objects to simpleLogger
		/// </summary>
		virtual void outputStatus() const
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::outputStatus");
		}

		virtual bool isCompiled() const
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::isCompiled");
		}

		/// <summary>
		/// Returns true if there are errors in the program; and the sequence must be halted.
		/// </summary>
		virtual bool hasErrors() const
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::hasErrors");
		}

		/// <summary>
		/// Returns the scene boundary in UI coordinates
		/// </summary>
		/// <returns></returns>
		virtual gridRect getSceneBoundaryUI() const
		{
			throw simpleException("Function must be overridden in the child class:  brogueProgram::getSceneBoundaryUI");
		}

		brogueUIProgram getProgramName() const
		{
			return _programName;
		}
		bool getIsActive() const
		{
			return _isActive;
		}
		void setIsActive(bool active)
		{
			_isActive = active;
		}

	private:

		brogueUIProgram _programName;
		bool _isActive;
	};
}