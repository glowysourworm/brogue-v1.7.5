#pragma once

#include "simplePrimitive.h"

namespace brogueHd::frontend::opengl
{
    template<typename T>
    class simpleUniform : simplePrimitive
    {
    public:

        simpleUniform(){}

        /// <summary>
        /// Data to be passed as a uniform to the shader code
        /// </summary>
        T data;
    };
}