#pragma once

#include "brogueGlobal.h"
#include "simplePrimitive.h"
#include "simpleGlData.h"
#include "simpleUniform.h"
#include "simpleArray.h"
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
        simpleShader(shaderData* data);
        ~simpleShader();

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

        template<isOpenGlUniform T>
        uniformData getUniform(const simpleString& name)
        {
            for (int index = 0; index < _uniforms1i->count(); index++)
            {
                if (_uniforms1i->get(index).name == name)
                    return uniformData(_uniforms1i->get(index));
            }

            for (int index = 0; index < _uniforms1->count(); index++)
            {
                if (_uniforms1->get(index).name == name)
                    return uniformData(_uniforms1->get(index));
            }

            for (int index = 0; index < _uniforms2->count(); index++)
            {
                if (_uniforms2->get(index).name == name)
                    return uniformData(_uniforms2->get(index));
            }

            for (int index = 0; index < _uniforms4->count(); index++)
            {
                if (_uniforms4->get(index).name == name)
                    return uniformData(_uniforms4->get(index));
            }

            return nullptr;
        }

        template<isOpenGlUniform T>
        uniformData getUniform(int index)
        {
            if (std::same_as<T, int>)
                return uniformData(_uniforms1i->get(index));

            else if (std::same_as<T, float>)
                return uniformData(_uniforms1->get(index));

            else if (std::same_as<T, vec2>)
                return uniformData(_uniforms2->get(index));

            else if (std::same_as<T, vec4>)
                return uniformData(_uniforms4->get(index));

            else
                simpleException::show("Unhandled uniform type:  simpleShader.h");
        }

        template<isOpenGlUniform T>
        int getUniformCount()
        {
            if (std::same_as<T, int>)
                return _uniforms1i->count();

            else if (std::same_as<T, float>)
                return _uniforms1->count();

            else if (std::same_as<T, vec2>)
                return _uniforms2->count();

            else if (std::same_as<T, vec4>)
                return _uniforms4->count();

            else
                simpleException::show("Unhandled uniform type:  simpleShader.h");
        }

    public:

        simpleArray<simpleUniform<int>> getUniforms1i() const
        {
            return *_uniforms1i;
        }
        simpleArray<simpleUniform<float>> getUniforms1() const
        {
            return *_uniforms1;
        }
        simpleArray<simpleUniform<vec2>> getUniforms2() const
        {
            return *_uniforms2;
        }
        simpleArray<simpleUniform<vec4>> getUniforms4() const
        {
            return *_uniforms4;
        }

    private:

        void copyImpl(const simpleShader& copy);

    private:

        GLenum _shaderType;
        simpleString _source;

        simpleArray<simpleUniform<int>>*    _uniforms1i;
        simpleArray<simpleUniform<float>>*  _uniforms1;
        simpleArray<simpleUniform<vec2>>*   _uniforms2;
        simpleArray<simpleUniform<vec4>>*   _uniforms4;
    };

    simpleShader::simpleShader()
    {
        this->handle = NULL;
        this->isCreated = false;
        this->isBound = false;

        _uniforms1i = new simpleArray<simpleUniform<int>>();
        _uniforms1 = new simpleArray<simpleUniform<float>>();
        _uniforms2 = new simpleArray<simpleUniform<vec2>>();
        _uniforms4 = new simpleArray<simpleUniform<vec4>>();

        _shaderType = NULL;
    }
    simpleShader::~simpleShader()
    {
        delete _uniforms1i;
        delete _uniforms1;
        delete _uniforms2;
        delete _uniforms4;
    }
    simpleShader::simpleShader(const simpleShader& copy)
    {
        copyImpl(copy);
    }
    simpleShader::simpleShader(shaderData* data)
    {
        this->handle = NULL;
        this->isCreated = false;
        this->isBound = false;

        _uniforms1i = new simpleArray<simpleUniform<int>>(data->uniforms1i.toArray());
        _uniforms1 = new simpleArray<simpleUniform<float>>(data->uniforms1.toArray());
        _uniforms2 = new simpleArray<simpleUniform<vec2>>(data->uniforms2.toArray());
        _uniforms4 = new simpleArray<simpleUniform<vec4>>(data->uniforms4.toArray());

        _source = data->source;
        _shaderType = data->type;
    }

    void simpleShader::copyImpl(const simpleShader& copy)
    {
        this->handle = NULL;
        this->isCreated = false;
        this->isBound = false;

        _shaderType = copy.getShaderType();
        _source = copy.getSource();

        _uniforms1i = new simpleArray<simpleUniform<int>>(copy.getUniforms1i());
        _uniforms1 = new simpleArray<simpleUniform<float>>(copy.getUniforms1());
        _uniforms2 = new simpleArray<simpleUniform<vec2>>(copy.getUniforms2());
        _uniforms4 = new simpleArray<simpleUniform<vec4>>(copy.getUniforms4());
    }

    void simpleShader::operator=(const simpleShader& copy)
    {
        copyImpl(copy);
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