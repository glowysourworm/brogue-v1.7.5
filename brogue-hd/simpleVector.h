#pragma once

namespace brogueHd::simple
{
    template<typename T>
    struct simpleVector
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

        T dot(simpleVector<T> vector)
        {
            return (x * vector.x) + (y * vector.y);
        }

        /// <summary>
        /// Returns the value of the cross product (casted in 3 dimensions)
        /// </summary>
        T cross(simpleVector<T> vector)
        {
            return (x * vector.y) - (vector.x * y);
        }
    };
}