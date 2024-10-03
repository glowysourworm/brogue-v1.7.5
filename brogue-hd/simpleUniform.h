#pragma once

#include "simplePrimitive.h"

namespace brogueHd::frontend::opengl
{
    template<typename T>
    struct simpleUniform : simplePrimitive
    {
        /// <summary>
        /// Data to be passed as a uniform to the shader code
        /// </summary>
        T data;
    };
}