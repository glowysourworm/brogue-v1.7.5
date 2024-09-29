#pragma once

namespace brogueHd::component::math
{
	template<typename T>
	struct mathVector
	{
		T x;
		T y;

        mathVector()
        {
            x = -1;
            y = -1;
        }
        mathVector(T ax, T ay)
        {
            x = ax;
            y = ay;
        }

        T dot(mathVector<T> vector)
        {
            return (x * vector.x) + (y * vector.y);
        }

        /// <summary>
        /// Returns the value of the cross product (casted in 3 dimensions)
        /// </summary>
        T cross(mathVector<T> vector)
        {
            return (x * vector.y) - (vector.x * y);
        }
	};
}