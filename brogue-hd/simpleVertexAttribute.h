#pragma once

#include <string>
#include "gl.h"

namespace brogueHd::frontend::opengl
{
    /// <summary>
    /// Describes a single vertex attribute "in" parameter for a shader
    /// </summary>
    struct simpleVertexAttribute
    {
    public:
        simpleVertexAttribute(){}
        simpleVertexAttribute(GLuint index, std::string name, int glActiveUniformType)
        {
            _index = index;
            _name = name;
            _activeUniformType = glActiveUniformType;
            
            // NO ENUM SUPPORT

            //glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
            //printf("Active Uniforms: %d\n", count);

            //for (i = 0; i < count; i++)
            //{
            //    glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);

            //    printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
            //}
        }

        GLuint getIndex() const
        {
            return _index;
        }
        std::string getName() const
        {
            return _name;
        }
        GLenum getUniformType() const
        {
            return _activeUniformType;
        }

    private:
        
        GLuint _index;
        std::string _name;

        /// <summary>
        /// Type of input data for the shader - this would be some sort of GLSL supported data type.
        /// </summary>
        GLenum _activeUniformType;
    };
}