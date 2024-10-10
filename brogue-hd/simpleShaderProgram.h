#pragma once

#include "simplePrimitive.h"
#include "simpleUniform.h"
#include "simpleList.h"
#include "simpleException.h"

#include "gl.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
    class simpleShaderProgram
    {
    public:

        simpleShaderProgram(const simplePrimitive& vertexShader, const simplePrimitive& fragmentShader);
        ~simpleShaderProgram();

        void compile();
        void draw(int bufferIndex);
        void drawAll();

        template<typename T>
        void bindUniform(simpleUniform<T> uniform);
        void bind(bool bind);

        void declareVAO(simplePrimitive programVAO);
        void deleteProgram();

    private:

        GLuint _handle;
        bool _isCreated;
        bool _isCompiled;
        bool _isActive;

        simpleList<simplePrimitive>* _programVAOs;

        simplePrimitive _vertexShader;
        simplePrimitive _fragmentShader;
    };
}
