#pragma once

#include "brogueUIConstants.h"
#include "brogueUIResponseData.h"
#include "gridRect.h"
#include "simple.h"
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
		/// Returns true if the program requires update.
		/// </summary>
		/// <param name="millisecondsLapsed">Number of milliseconds lapsed since the last update</param>
		virtual brogueUIResponseData& checkUpdate(const simpleMouseState& mouseState, int millisecondsLapsed)
		{
			brogueUIResponseData defaultResponse;

			defaultResponse.program = _programName;
			defaultResponse.sender = brogueUIView::Unnamed;
			defaultResponse.shouldUpdate = false;

			return defaultResponse;
		}

		/// <summary>
		/// Updates the opengl program by calling the necessary routines to stream / buffer data
		/// </summary>
		/// <param name="millisecondsLapsed">Number of milliseconds lapsed since the last update</param>
		virtual void update(const simpleMouseState& mouseState, int millisecondsLapsed)
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