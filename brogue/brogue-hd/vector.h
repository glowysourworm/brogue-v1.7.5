#pragma once

#include "point.h"

namespace brogueHd::backend::math
{
	template<typename T>
	struct vector
	{
		T x;
		T y;

        T dot(vector<T> vector)
        {
            return (x * vector.x) + (y * vector.y);
        }

        /// <summary>
        /// Returns the value of the cross product (casted in 3 dimensions)
        /// </summary>
        T cross(vector<T> vector)
        {
            return (x * vector.y) - (vector.x * y);
        }

        /// <summary>
        /// Returns the determinant of the orientation cross product (the sign of the cross product resulting in 
        /// crossing two difference vectors that order points 1,2, and 3 in that order). The sign of the determinant
        /// returned shows the orientation of the ordering (clockwise, counter-clockwise, or collinear)
        /// </summary>
        template<typename T>
        static T orientation(point<T> point1, point<T> point2, point<T> point3)
        {
            // 1 -> 2 -> 3 (Results from crossing the vectors 12 X 23 - where subtracting the points gives you the vector)
            vector<T> vector12 = point<T>::subtract(point2, point1);
            vector<T> vector23 = point<T>::subtract(point3, point2);

            return vector12.cross(vector23);
        }
	};
}