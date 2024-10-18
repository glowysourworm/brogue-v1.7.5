#pragma once

#include "simplePrimitive.h"
#include "gridRect.h"

using namespace brogueHd::component;

namespace brogueHd::frontend::opengl
{
	class brogueProgram : simplePrimitive
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
		/// Runs the opengl program for one draw pass
		/// </summary>
		virtual void run()
		{

		}

		/// <summary>
		/// Returns the scene boundary in UI coordinates
		/// </summary>
		/// <returns></returns>
		virtual gridRect getSceneBoundaryUI() const
		{
			return default_value::value<gridRect>();
		}

    public:  // simplePrimitive

        /// <summary>
        /// Function that declares the primitive on the backend (shader, program, uniform, ...). This
        /// must be inherited and extended in child classes.
        /// </summary>
		virtual void glCreate(GLuint programHandle) override
		{

		}

		virtual void compile()
		{

		}

        /// <summary>
        /// Binds / Unbinds the primitive to be the active component on the GL backend. Must bind before running
        /// program.
        /// </summary>
		virtual void bind(bool bind) override
		{

		}

        /// <summary>
        /// Draws the primitive by calling the GL backend
        /// </summary>
		virtual void draw() override
		{

		}

        /// <summary>
        /// Calls GL backend to remove created objects
        /// </summary>
		virtual void teardown() override
		{

		}

        /// <summary>
        /// Checks GL backend for errors; and forwards them on.
        /// </summary>
        /// <returns></returns>
		virtual bool hasErrors() override
		{
			return false;
		}

        /// <summary>
        /// GL handle created by the GL backend
        /// </summary>
		virtual GLuint getHandle() const override
		{
			return 0;
		}

		virtual bool getIsBound() const override
		{
			return false;
		}

		virtual size_t getHash() const override
		{
			return 0;
		}

	protected:

		bool isCompiled;
	};
}