#pragma once
#include "simpleShaderProgram.h"

namespace brogueHd::frontend::opengl
{
    simpleShaderProgram::simpleShaderProgram(const simplePrimitive& vertexShader, const simplePrimitive& fragmentShader)
    {
        _vertexShader = vertexShader;
        _fragmentShader = fragmentShader;
        _handle = NULL;
    }
    simpleShaderProgram::~simpleShaderProgram()
    {

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
        _handle = glCreateProgram();

        // Creates shader on the backend
        _vertexShader.glCreate(_handle);
        _fragmentShader.glCreate(_handle);

        // Attach shaders in the order they were declared
        glAttachShader(_handle, _vertexShader.getHandle());
        glAttachShader(_handle, _fragmentShader.getHandle());

        // Link the program
        glLinkProgram(_handle);

        GLuint handle = _handle;

        // Declare: VAO -> VBO
        //
        _programVAOs->forEach([&handle](simplePrimitive vao)
        {
            vao.glCreate(handle);
            return iterationCallback::iterate;
        });

        // "Installs the program object as part of the current rendering state"
        glUseProgram(handle);

        _isCompiled = true;
        _isActive = true;
    }

    void simpleShaderProgram::draw(int bufferIndex)
    {
        if (!_isCompiled)
            simpleException::showCstr("Must first call IGLProgram.Compile() before using the GL program");

        if (!_isActive)
            simpleException::showCstr("Must first call Bind to set the program active");

        simplePrimitive programVAO = _programVAOs->get(bufferIndex);
        programVAO.bind(true);
        programVAO.draw();
    }

    void simpleShaderProgram::drawAll()
    {
        if (!_isCompiled)
            simpleException::showCstr("Must first call compile before using the GL program");

        if (!_isActive)
            simpleException::showCstr("Must first call Bind to set the program active");

        _programVAOs->forEach([](simplePrimitive vao)
        {
            vao.bind(true);
            vao.draw();
            return iterationCallback::iterate;
        });
    }

    void simpleShaderProgram::bind(bool bind)
    {
        if (!_isCompiled)
            simpleException::showCstr("Must first call compile before using the GL program");

        if (bind)
        {
            glUseProgram(_handle);
        }
        else
        {
            glUseProgram(0);
        }

        _isActive = bind;
    }

    void simpleShaderProgram::declareVAO(simplePrimitive programVAO)
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
        glDetachShader(_handle, _vertexShader.getHandle());
        glDetachShader(_handle, _fragmentShader.getHandle());

        // Teardown all shader entities
        _vertexShader.teardown();
        _fragmentShader.teardown();

        // Teardown the VAO
        _programVAOs->forEach([](simplePrimitive vao)
        {
            vao.teardown();
            return iterationCallback::iterate;
        });

        // Deactivate the program on the backend before deleting
        glUseProgram(NULL);

        // Delete this prgram from the backend
        glDeleteProgram(_handle);

        _isCompiled = false;
        _isActive = false;
    }
}