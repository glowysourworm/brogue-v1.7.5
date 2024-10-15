#pragma once

#include "brogueGlobal.h"
#include "simplePrimitive.h"
#include "simpleList.h"
#include "simpleString.h"
#include "shaderData.h"
#include "simpleLogger.h"
#include "gl.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
    /// <summary>
    /// Representation of a shader shared by the GPU. This contains the shader handle and data needed to
    /// compile the shader at runtime.
    /// </summary>
    struct simpleShader : public simplePrimitive
    {
    public:
        simpleShader();
        simpleShader(const simpleShader& copy);
        simpleShader(const shaderData& data);
        ~simpleShader(){};

        void operator=(const simpleShader& copy);

        void glCreate(GLuint programHandle) override;
        void bind(bool bind) override;
        void teardown() override;

        simpleString getSource() const
        {
            return _source;
        }
        GLenum getShaderType() const
        {
            return _shaderType;
        }

        size_t getHash() const override
        {
            return _source.getHash();
        }

        bool hasErrors() override
        {
            GLchar* buffer = new GLchar[10000];
            GLsizei  length = 0;

            // Check info log for the errors
            glGetShaderInfoLog(this->handle, 10000, &length, buffer);

            // For now, just show the exception from the shader
            if (length > 0)
                simpleLogger::logColor(brogueConsoleColor::Red, buffer);

            delete[] buffer;

            return length > 0;
        }

    private:

        GLenum _shaderType;
        simpleString _source;
    };

    simpleShader::simpleShader()
    {
        this->handle = NULL;
        this->isCreated = false;
        this->isBound = false;

        _shaderType = NULL;
    }
    simpleShader::simpleShader(const simpleShader& copy)
    {
        this->handle = NULL;
        this->isCreated = false;
        this->isBound = false;

        _shaderType = copy.getShaderType();
        _source = copy.getSource();
    }
    simpleShader::simpleShader(const shaderData& data)
    {
        this->handle = NULL;
        this->isCreated = false;
        this->isBound = false;

        _source = *(data.source);
        _shaderType = data.type;
    }

    void simpleShader::operator=(const simpleShader& copy)
    {
        _shaderType = copy.getShaderType();
        _source = copy.getSource();
    }

    void simpleShader::teardown()
    {
        if (!this->isCreated)
            simpleException::showCstr("simpleShader already deleted from the backend");

        // Deletes the shader object from the GL backend
        glDeleteShader(this->handle);

        this->isCreated = false;
    }

    void simpleShader::glCreate(GLuint programHandle)
    {
        if (this->isCreated)
            simpleException::showCstr("simpleShader already created on the backend");

        // Declare the shader on the GL backend
        this->handle = glCreateShader(_shaderType);

        // (some funny casting)
        const GLchar* source = _source.c_str();
        const GLint size = _source.count();

        // Set the shader source on the GL backend
        glShaderSource(this->handle, 1, &source, &size);

        // Compile the shader and check for errors
        glCompileShader(this->handle);

        this->isCreated = true;
    }

    void simpleShader::bind(bool bind)
    {

    }
}