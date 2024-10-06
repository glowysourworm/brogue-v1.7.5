#pragma once

#include "brogueGlobal.h"
#include "simplePrimitive.h"
#include "simpleList.h"
#include "gl.h"

namespace brogueHd::frontend::opengl
{
    /// <summary>
    /// Representation of a shader shared by the GPU. This contains the shader handle and data needed to
    /// compile the shader at runtime.
    /// </summary>
    class simpleShader : public simplePrimitive
    {
    public:
        simpleShader();
        simpleShader(simpleString* source);
        ~simpleShader(){};

        void glCreate(GLuint programHandle) override;
        void bind(bool bind) override;
        void teardown() override;

    private:

        GLenum _shaderType;
        simpleString* _source;
    };

    simpleShader::simpleShader()
    {
        this->handle = NULL;
        this->isCreated = false;
        this->isBound = false;

        _shaderType = NULL;
        _source = NULL;
    }
    simpleShader::simpleShader(simpleString* source)
    {
        this->handle = NULL;
        this->isCreated = false;
        this->isBound = false;

        _source = source;
        _shaderType = NULL;
    }

    void simpleShader::teardown()
    {
        if (!this->isCreated)
            brogueException::show("simpleShader already deleted from the backend");

        // Deletes the shader object from the GL backend
        glDeleteShader(this->handle);

        this->isCreated = false;
    }

    void simpleShader::glCreate(GLuint programHandle)
    {
        if (this->isCreated)
            brogueException::show("simpleShader already created on the backend");

        // Declare the shader on the GL backend
        this->handle = glCreateShader(_shaderType);

        // (some funny casting)
        const GLchar* source = _source->c_str();
        const GLint size = _source->count();

        // Set the shader source on the GL backend
        glShaderSource(this->handle, 1, &source, &size);

        // Compile the shader and check for errors
        glCompileShader(this->handle);

        GLchar* buffer = NULL;
        GLsizei  bufferSize = 0;
        GLsizei  length = 0;

        // Check info log for the errors
        glGetShaderInfoLog(this->handle, bufferSize, &length, buffer);

        // For now, just show the exception from the shader
        if (buffer != NULL)
            brogueException::show(buffer);

        this->isCreated = true;
    }

    void simpleShader::bind(bool bind)
    {

    }
}