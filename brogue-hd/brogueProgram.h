#pragma once

#include "simple.h"
#include "simpleGlObject.h"
#include "gridRect.h"

using namespace brogueHd::component;

namespace brogueHd::frontend::opengl
{
	class brogueProgram : hashableObject
	{
	public:

		brogueProgram(){};
		~brogueProgram(){};

		/// <summary>
		/// Calls necessary routines to initialize the opengl program
		/// </summary>
		virtual void initialize()
		{

		}

		/// <summary>
		/// Updates the opengl program by calling the necessary routines to stream / buffer data
		/// </summary>
		/// <param name="millisecondsLapsed">Number of milliseconds lapsed since the last update</param>
		virtual void update(int millisecondsLapsed)
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
	};
}