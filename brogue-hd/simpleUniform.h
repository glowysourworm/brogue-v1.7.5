#pragma once

#include "simple.h"
#include "simplePrimitive.h"

namespace brogueHd::frontend::opengl
{
    template<isHashable T>
    struct simpleUniform : simplePrimitive
    {
    public:

        simpleUniform()
        {
            data = default_value<T>::value;
        }
        simpleUniform(const simpleUniform<T>& copy)
        {
            data = copy.data;
        }

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