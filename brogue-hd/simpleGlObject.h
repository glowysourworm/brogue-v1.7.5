#pragma once

#include "gl.h"
#include "simple.h"
#include "simpleLogger.h"
#include <limits>
#include <execution>

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	class simpleGlObject : public simpleObject
	{
	public:

		/// <summary>
		/// Constant for an empty handle for the backend. 
		/// </summary>
		static const GLuint HandleNull = std::numeric_limits<GLuint>::infinity();

	public:

		simpleGlObject()
		{
			this->handle = simpleGlObject::HandleNull;
			this->programHandle = simpleGlObject::HandleNull;
		}
		~simpleGlObject()
		{}

		/// <summary>
		/// Function that declares the primitive on the backend (shader, program, uniform, ...). This
		/// must be inherited and extended in child classes.
		/// </summary>
		virtual void glCreate(GLuint programHandle)
		{
			this->programHandle = programHandle;

			simpleLogger::logColor(brogueConsoleColor::Red, "Override glCreate not complete for child class");
		}

		/// <summary>
		/// Binds the GL object to be the active component on the GL backend.
		/// </summary>
		virtual void bind()
		{
			simpleLogger::logColor(brogueConsoleColor::Red, "Override bind not complete for child class");
		}

		/// <summary>
		/// "Un-Binds" the GL object. This is used for certain components only:  FrameBuffer
		/// </summary>
		virtual void unBind()
		{
			simpleLogger::logColor(brogueConsoleColor::Red, "Override unBind not complete for child class");
		}

		virtual bool isBound() const
		{
			simpleLogger::logColor(brogueConsoleColor::Red, "Override isBound not complete for child class");

			return false;
		}

		virtual bool isCreated() const
		{
			simpleLogger::logColor(brogueConsoleColor::Red, "Override isCreated not complete for child class");

			return false;
		}

		/// <summary>
		/// Calls GL backend to remove created objects
		/// </summary>
		virtual void teardown()
		{
			simpleLogger::logColor(brogueConsoleColor::Red, "Override teardown not complete for child class");
		}

		virtual bool hasErrors() const
		{
			simpleLogger::logColor(brogueConsoleColor::Red, "Override hasErrors not complete for child class");

			return false;
		}

		virtual void showErrors() const
		{
			simpleLogger::logColor(brogueConsoleColor::Red, "Override showErrors not complete for child class");
		}

		/// <summary>
		/// GL handle created by the GL backend
		/// </summary>
		virtual GLuint getHandle() const
		{
			return handle;
		}

		/// <summary>
		/// GL program handle created by the GL backend
		/// </summary>
		virtual GLuint getProgramHandle() const
		{
			return programHandle;
		}

		virtual size_t getHash() const override
		{
			simpleLogger::logColor(brogueConsoleColor::Red, "Override getHash not complete for child class");

			return 0;   // These handles are from the GL backend; and are usually 0, 1, 2, ...
		}

	protected:

		GLuint handle;
		GLuint programHandle;
	};
}