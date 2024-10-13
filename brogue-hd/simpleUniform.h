#pragma once

#include "simple.h"
#include "simplePrimitive.h"

namespace brogueHd::frontend::opengl
{
    template<isHashable T>
    class simpleUniform : simplePrimitive
    {
    public:

        simpleUniform(){}

        /// <summary>
        /// Data to be passed as a uniform to the shader code
        /// </summary>
        T data;

        size_t getHash() const override
        {
            return hashGenerator::generateHash(data);
        }
    };
}