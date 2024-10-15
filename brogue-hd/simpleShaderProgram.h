#pragma once

#include "simplePrimitive.h"
#include "simpleUniform.h"
#include "simpleList.h"
#include "simpleString.h"
#include "simpleShader.h"
#include "simpleLogger.h"
#include "simpleException.h"

#include "gl.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
    class simpleShaderProgram : public simplePrimitive
    {
    public:

        simpleShaderProgram(const simpleShader& vertexShader, const simpleShader& fragmentShader);
        ~simpleShaderProgram();

        void compile();
        void draw(int bufferIndex);
        void drawAll();

        template<typename T>
        void bindUniform(simpleUniform<T> uniform);
        void bind(bool bind);

        void declareVAO(simplePrimitive* programVAO);
        void deleteProgram();

        bool hasErrors() override;

    private:

        void checkStatus(const char* statusName, GLenum status, bool logOutput = true) const;

    private:

        bool _isCompiled;
        bool _isActive;

        simpleList<simplePrimitive*>* _programVAOs;

        simpleShader _vertexShader;
        simpleShader _fragmentShader;
    };

    simpleShaderProgram::simpleShaderProgram(const simpleShader& vertexShader, const simpleShader& fragmentShader)
    {
        _programVAOs = new simpleList<simplePrimitive*>();
        _vertexShader = vertexShader;
        _fragmentShader = fragmentShader;
        _isCompiled = false;
        _isActive = false;
    }
    simpleShaderProgram::~simpleShaderProgram()
    {
        delete _programVAOs;
    }

    void simpleShaderProgram::compile()
    {
        if (_isCompiled)
            simpleException::showCstr("Already called simpleShaderProgram::compile");

        if (_programVAOs->count() == 0)
            simpleException::showCstr("simpleShaderProgram must have a VAO attached before compiling");

        // Procedure
        //
        // 0) Declare the program 
        // 1) Create, compile, attach, and link the shaders
        // 2) Call the simplePrimitive Create() methods
        //

        // Declare the GL program
        this->handle = glCreateProgram();

        // Creates shader on the backend
        _vertexShader.glCreate(this->handle);
        _fragmentShader.glCreate(this->handle);

        // Attach shaders in the order they were declared
        glAttachShader(this->handle, _vertexShader.getHandle());
        glAttachShader(this->handle, _fragmentShader.getHandle());

        // Link the program
        glLinkProgram(this->handle);

        GLuint handle = this->handle;

        // Declare: VAO -> VBO
        //
        _programVAOs->forEach([&handle] (simplePrimitive* vao)
        {
            vao->glCreate(handle);

            return iterationCallback::iterate;
        });

        // "Installs the program object as part of the current rendering state"
        glUseProgram(handle);

        _isCompiled = true;
        _isActive = true;
    }

    void simpleShaderProgram::checkStatus(const char* statusName, GLenum status, bool logOutput) const
    {
        GLint result;

        glGetProgramiv(this->handle, status, &result);

        if (result == GL_FALSE)
            simpleLogger::logColor(brogueConsoleColor::Red, "{}:  {}", statusName, "Error!");

        else if (result == GL_TRUE)
            simpleLogger::log("{}:  {}", statusName, "Ok!");

        else
            simpleException::show("Unknown status return from glGetProgramiv:  simpleShaderProgram.h");
    }

    bool simpleShaderProgram::hasErrors()
    {
        GLchar* buffer = new GLchar[10000];
        GLsizei  length = 0;

        // Check info log for the errors
        glGetProgramInfoLog(this->handle, 10000, &length, buffer);

        // For now, just show the exception from the shader
        if (length > 0)
            simpleLogger::logColor(brogueConsoleColor::Red, buffer);

        delete[] buffer;

        // (Outputs all error logs)
        bool vertexShaderError = _vertexShader.hasErrors();
        bool fragmentShaderError = _fragmentShader.hasErrors();

        return length > 0 || vertexShaderError || fragmentShaderError;
    }

    void simpleShaderProgram::draw(int bufferIndex)
    {
        if (!_isCompiled)
            simpleException::showCstr("Must first call IGLProgram.Compile() before using the GL program");

        if (!_isActive)
            simpleException::showCstr("Must first call Bind to set the program active");

        simplePrimitive* programVAO = _programVAOs->get(bufferIndex);
        programVAO->bind(true);
        programVAO->draw();
    }

    void simpleShaderProgram::drawAll()
    {
        if (!_isCompiled)
            simpleException::showCstr("Must first call compile before using the GL program");

        if (!_isActive)
            simpleException::showCstr("Must first call Bind to set the program active");

        _programVAOs->forEach([] (simplePrimitive* vao)
        {
            if (!vao->getIsBound())
                vao->bind(true);

            vao->draw();
            return iterationCallback::iterate;
        });
    }

    void simpleShaderProgram::bind(bool bind)
    {
        if (!_isCompiled)
            simpleException::showCstr("Must first call compile before using the GL program");

        if (bind)
        {
            glUseProgram(this->handle);
        }
        else
        {
            glUseProgram(0);
        }

        _isActive = bind;
        this->isBound = bind;
    }

    void simpleShaderProgram::declareVAO(simplePrimitive* programVAO)
    {
        if (_isCompiled)
            simpleException::showCstr("Must add texture before compiling:  simpleShaderProgram");

        _programVAOs->add(programVAO);
    }

    template<typename T>
    void simpleShaderProgram::bindUniform(simpleUniform<T> uniform)
    {
        //var uniformLocation = GL.GetUniformLocation(this.Handle, uniform.Name);

        //switch (uniform.Type)
        //{
        //case ActiveUniformType.FloatVec2:
        //    var vector2 = (Vector2)Convert.ChangeType(uniform.Value, typeof(Vector2));
        //    GL.Uniform2(uniformLocation, vector2);
        //    break;
        //case ActiveUniformType.FloatVec4:
        //    var vector4 = (Vector4)Convert.ChangeType(uniform.Value, typeof(Vector4));
        //    GL.Uniform4(uniformLocation, vector4);
        //    break;

        //    // Not sure about sampler declaration
        //case ActiveUniformType.Sampler2D:
        //    var textureIndex = (int)Convert.ChangeType(uniform.Value, typeof(int));
        //    GL.Uniform1(uniformLocation, textureIndex);
        //    break;
        //case ActiveUniformType.Float:
        //    var floatValue = (float)Convert.ChangeType(uniform.Value, typeof(float));
        //    GL.Uniform1(uniformLocation, floatValue);
        //    break;
        //default:
        //    throw new GLException("Unhandled Active Uniform Type:  GLShaderProgram.cs");
        //}
    }

    void simpleShaderProgram::deleteProgram()
    {
        if (!_isCompiled)
            simpleException::showCstr("Must first call compile before using the GL program");

        // Procedure
        //
        // 0) Detach the shaders
        // 1) Call Teardown on the IGLModel entities
        // 2) Deactivate the program by setting GL.UseProgram to null
        // 3) Reset handle and flag for this instance
        //

        // Detach the shaders
        glDetachShader(this->handle, _vertexShader.getHandle());
        glDetachShader(this->handle, _fragmentShader.getHandle());

        // Teardown all shader entities
        _vertexShader.teardown();
        _fragmentShader.teardown();

        // Teardown the VAO
        _programVAOs->forEach([] (simplePrimitive* vao)
        {
            vao->teardown();
            return iterationCallback::iterate;
        });

        // Deactivate the program on the backend before deleting
        glUseProgram(NULL);

        // Delete this prgram from the backend
        glDeleteProgram(this->handle);

        _isCompiled = false;
        _isActive = false;
    }
}
