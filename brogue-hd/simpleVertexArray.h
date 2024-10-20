#pragma once

#include "simplePrimitive.h"
#include "simpleVertexBuffer.h"
#include "simpleException.h"
#include "gl.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
    /// <summary>
    /// Corresponds to the VAO objects in OpenGL. These are used to manage the VBO Vertex Buffer objects
    /// that are stored on the GL backend
    /// </summary>
    template<typename T>
    class simpleVertexArray : public simplePrimitive
    {
    public:
        simpleVertexArray();
        simpleVertexArray(const GLenum& primitiveType, simpleVertexBuffer<T>* vertexBuffer);
        ~simpleVertexArray();

        void glCreate(GLuint programHandle) override;
        void bind(bool bind) override;
        void teardown() override;
        void draw() override;

        /// <summary>
        /// Rebuffers data on the specified VBO (see simpleShaderProgram)
        /// </summary>
        void reBuffer(simpleDataStream<T>* stream);

        size_t getHash() const override
        {
            return _vertexBuffer->getHash();
        }

    private:

        // Primary VBO vertex buffer objects indexed by the OpenGL BufferIndex
        GLenum _primitiveType;
        simpleVertexBuffer<T>* _vertexBuffer;
    };

    template<typename T>
    simpleVertexArray<T>::simpleVertexArray()
    {
        _vertexBuffer = new simpleVertexBuffer<T>();
    }

    template<typename T>
    simpleVertexArray<T>::simpleVertexArray(const GLenum& primitiveType, simpleVertexBuffer<T>* vertexBuffer)
    {
        _primitiveType = primitiveType;
        _vertexBuffer = vertexBuffer;

        this->isBound = false;
        this->isCreated = false;
    }

    template<typename T>
    simpleVertexArray<T>::~simpleVertexArray()
    {
        // THESE ARE (MAY BE) CREATED BY THE SCENE BUILDER!
        delete _vertexBuffer;
    }

    template<typename T>
    void simpleVertexArray<T>::glCreate(GLuint programHandle)
    {
        if (this->isCreated)
            simpleException::showCstr("simpleVertexArray already created in the backend");

        // Procedure:  Create / Draw / Teardown several vertex buffers
        //
        // 0) Create the vertex array
        // 1) Bind the vertex array handle (starts editing)
        // 2) Calculate the total buffer draw length and store

        // Generate the vertex array for THIS instance
        glGenVertexArrays(1, &this->handle);

        // BIND THIS INSTANCE TO BE THE CURRENT VERTEX ARRAY
        glBindVertexArray(this->handle);

        // NOTE: The buffer indices are assigned prior to passing into this instance. Order
        //       has already been indicated by the calling IGLModel or IGLProgram
        //       
        //       Simply call IGLPrimitve.Create() to initialize the vertex buffer
        //
        _vertexBuffer->glCreate(programHandle);

        this->isCreated = true;
        this->isBound = true;
    }

    template<typename T>
    void simpleVertexArray<T>::reBuffer(simpleDataStream<T>* stream)
    {
        if (!this->isCreated)
            simpleException::show("simpleVertexArray already deleted from the backend");

        if (this->isBound)
            simpleException::show("simpleVertexArray must be un-bound before calling rebuffer()");

        _vertexBuffer->reBuffer(stream, true);
    }

    template<typename T>
    void simpleVertexArray<T>::teardown()
    {
        if (!this->isCreated)
            simpleException::show("simpleVertexArray already deleted from the backend");

        // Teardown vertex buffers
        _vertexBuffer->teardown();

        // Delete this vertex array
        glDeleteVertexArrays(1, &this->handle);

        this->isCreated = false;
        this->isBound = false;
        this->handle = NULL;
    }

    template<typename T>
    void simpleVertexArray<T>::draw()
    {
        if (!this->isCreated)
            simpleException::show("simpleVertexArray already deleted from the backend");

        if (!this->isBound)
            simpleException::show("simpleVertexArray must be bound before calling Draw()");

        // Draw Buffer
        glDrawArrays(_primitiveType, 0, _vertexBuffer->getBufferLength());
    }

    template<typename T>
    void simpleVertexArray<T>::bind(bool bind)
    {
        if (!this->isCreated)
            simpleException::show("simpleVertexArray already deleted from the backend");

        // Bind VAO before using
        if (bind)
        {
            glBindVertexArray(this->handle);
        }

        else
        {
            glBindVertexArray(NULL);
        }

        this->isBound = bind;
    }
}