#pragma once

#include "simple.h"
#include "simpleMath.h"

namespace brogueHd::simple
{
    template<isNumber T>
    struct simpleVector : simpleStruct
    {
        T x;
        T y;

        simpleVector()
        {
            x = -1;
            y = -1;
        }
        simpleVector(T ax, T ay)
        {
            x = ax;
            y = ay;
        }
        simpleVector(const simpleVector<T>& copy)
        {
            x = copy.x;
            y = copy.y;
        }
        void operator=(const simpleVector<T>& copy)
        {
            x = copy.x;
            y = copy.y;
        }
        bool operator==(const simpleVector<T>& other)
        {
            return x == other.x &&
                y == other.y;
        }
        bool operator!=(const simpleVector<T>& other)
        {
            return x != other.x ||
                y != other.y;
        }

        void set(T ax, T ay)
        {
            x = ax;
            y = ay;
        }
        void set(const simpleVector<T>& copy)
        {
            x = copy.x;
            y = copy.y;
        }

        friend simpleVector<T> operator+(const simpleVector<T>& vector1, const simpleVector<T>& vector2)
        {
            return simpleVector<T>(vector1.x + vector2.x, vector1.y + vector2.y);
        }

        T magnitude()
        {
            return sqrt(x * x + y * y);
        }

        T dot(const simpleVector<T>& vector)
        {
            return (x * vector.x) + (y * vector.y);
        }

        T slope() const
        {
            return y / x;
        }

        void add(T ax, T ay)
        {
            x += ax;
            y += ay;
        }

        /// <summary>
        /// Returns the value of the cross product (casted in 3 dimensions)
        /// </summary>
        T cross(const simpleVector<T>& vector)
        {
            return (x * vector.y) - (vector.x * y);
        }

        size_t getHash() const override
        {
            return hashGenerator::generateHash(x, y);
        }
    };
}