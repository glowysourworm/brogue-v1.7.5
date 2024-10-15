#pragma once

#include "simple.h"
#include "gl.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
	class simplePrimitive : public hashableObject
	{

    public:

        simplePrimitive()
        {
            this->handle = NULL;
            this->isCreated = false;
            this->isBound = false;
        }
        ~simplePrimitive()
        {}
        
        /// <summary>
        /// Function that declares the primitive on the backend (shader, program, uniform, ...). This
        /// must be inherited and extended in child classes.
        /// </summary>
        virtual void glCreate(GLuint programHandle)
        {

        }

        /// <summary>
        /// Binds / Unbinds the primitive to be the active component on the GL backend. Must bind before running
        /// program.
        /// </summary>
        virtual void bind(bool bind)
        {

        }

        /// <summary>
        /// Draws the primitive by calling the GL backend
        /// </summary>
        virtual void draw()
        {

        }

        /// <summary>
        /// Calls GL backend to remove created objects
        /// </summary>
        virtual void teardown()
        {

        }

        virtual bool hasErrors()
        {
            return false;
        }

        /// <summary>
        /// GL handle created by the GL backend
        /// </summary>
        GLuint getHandle() const
        {
            return handle;
        }

        bool getIsBound() const
        {
            return isBound;
        }

        virtual size_t getHash() const override
        {
            return 0;   // These handles are from the GL backend; and are usually 0, 1, 2, ...
        }

    protected:

        GLuint handle;

        /// <summary>
        /// True if the model is created on the GL backend
        /// </summary>
        bool isCreated;

        /// <summary>
        /// True if the primitive is bound on the backend
        /// </summary>
        bool isBound;
	};
}