#pragma once

#include "brogueUIConstants.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"
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

		}

		/// <summary>
		/// Following a similar pipeline to the brogueView, checkUpdate checks program parameters needed
		/// to update the view - also calling the view's checkUpdate method. (creating the pipeline)
		/// </summary>
		virtual void checkUpdate(const simpleKeyboardState& keyboardState,
								 const simpleMouseState& mouseState,
								 int millisecondsLapsed)
		{

		}

		/// <summary>
		/// Returns true if the program tree needs to be updated
		/// </summary>
		virtual bool needsUpdate()
		{
			return false;
		}

		/// <summary>
		/// Clears mouse event data from the UI tree
		/// </summary>
		virtual void clearUpdate()
		{

		}

		/// <summary>
		/// Clears mouse event data from the UI tree
		/// </summary>
		virtual void clearEvents()
		{

		}

		/// <summary>
		/// Following a similar pipeline to the brogueView, update checks program parameters needed
		/// to update the view - also calling the view's update method. (creating the pipeline)
		/// </summary>
		virtual void update(const simpleKeyboardState& keyboardState,
							const simpleMouseState& mouseState,
							int millisecondsLapsed)
		{

		}

		/// <summary>
		/// Runs the opengl program(s) for one draw pass
		/// </summary>
		virtual void run(int millisecondsElapsed)
		{

		}

		/// <summary>
		/// Outputs status for program objects to simpleLogger
		/// </summary>
		virtual void outputStatus() const
		{

		}

		virtual bool isCompiled() const
		{
			return false;
		}

		/// <summary>
		/// Returns true if there are errors in the program; and the sequence must be halted.
		/// </summary>
		virtual bool hasErrors() const
		{
			return false;
		}

		/// <summary>
		/// Returns the scene boundary in UI coordinates
		/// </summary>
		/// <returns></returns>
		virtual gridRect getSceneBoundaryUI() const
		{
			return default_value::value<gridRect>();
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