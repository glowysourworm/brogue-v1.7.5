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
        simpleVertexAttribute()
        {
            _index = 0;
            _name = default_value<simpleString>::value;
            _activeUniformType = 0;
        }
        simpleVertexAttribute(const simpleVertexAttribute& copy)
        {
            copyImpl(copy);
        }
        simpleVertexAttribute(GLuint index, const simpleString& name, int glActiveUniformType)
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
        simpleString getName() const
        {
            return _name;
        }
        GLenum getUniformType() const
        {
            return _activeUniformType;
        }

    private:

        void copyImpl(const simpleVertexAttribute& copy)
        {
            _index = copy.getIndex();
            _name = copy.getName();
            _activeUniformType = copy.getUniformType();
        }

    private:
        
        GLuint _index;
        simpleString _name;

        /// <summary>
        /// Type of input data for the shader - this would be some sort of GLSL supported data type.
        /// </summary>
        GLenum _activeUniformType;
    };
}