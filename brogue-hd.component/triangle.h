#pragma once

#include "mathPoint.h"
#include "mathVector.h"
#include "brogueMath.h"
#include "exceptionHandler.h"

namespace brogueHd::component::math
{
    template<typename T>
	struct triangle
	{
        mathPoint<T> point1;
        mathPoint<T> point2;
        mathPoint<T> point3;

        triangle(){}
        triangle(mathPoint<T> apoint1, mathPoint<T> apoint2, mathPoint<T> apoint3)
        {
            point1 = apoint1;
            point2 = apoint2;
            point3 = apoint3;
        }

        bool operator==(const triangle<T>& triangle)
        {
            return point1 == triangle.point1 &&
                   point2 == triangle.point2 &&
                   point3 == triangle.point3;
        }

        bool operator!=(const triangle<T>& triangle)
        {
            return point1 != triangle.point1 ||
                   point2 != triangle.point2 ||
                   point3 != triangle.point3;
        }

        bool containsEqualEdge(mathPoint<T> vertex1, mathPoint<T> vertex2)
        {
            int pointsShared = 0;

            pointsShared += ((point1 == vertex1) || (point1 == vertex2)) ? 1 : 0;

            pointsShared += ((point2 == vertex1) || (point2 == vertex2)) ? 1 : 0;

            pointsShared += ((point3 == vertex1) || (point3 == vertex2)) ? 1 : 0;

            return pointsShared > 1;
        }

        bool containsEqualPoint(mathPoint<T> point)
        {
            return (point == point1) ||
                   (point == point2) ||
                   (point == point3);
        }

        bool circumCircleContains(mathPoint<T> point)
        {
            // Procedure
            //
            // 1) Calculate determinant of the three vertices
            //      d > 0  (clock-wise)
            //      d = 0  (collinear)
            //      d < 0  (counter-clock-wise)
            //
            // 2) Calculate this same determinant for all configurations of vertices
            //    to find the counter-clockwise orientation. Label these point1, point2, point3.
            //
            // 3) Solve 3x3 determinant described here https://en.wikipedia.org/wiki/Delaunay_triangulation
            //      d > 0  (implies point "D" is inside the circum-circle)
            //

            // Double Check:  There are only 2 possible orderings of the points

            // 1 -> 2 -> 3 (Results from crossing the vectors 12 X 23 - where subtracting the points gives you the vector)
            T d123 = brogueMath<T>::orientation(point1, point2, point3);

            // 1 -> 3 -> 2
            T d132 = brogueMath<T>::orientation(point1, point3, point2);

            // NOTE*** Must handle collinear case. This may be the incorrect way to handle this.
            if (d123 == 0 || d132 == 0)
                return false;

            // Re-number the vertices to be counter-clockwise (1 -> 2 -> 3)
            mathPoint<T> p1;
            mathPoint<T> p2;
            mathPoint<T> p3;

            // NOTE*** Was unsure of sign because of UI coordinates. It appears that UI coordinates
            //         end up following convention. I'm thinking because the "handedness" is the same
            //         if you flip the 2D plane upward.
            //
            //         Negative => Clockwise
            //         Positive => Counter-Clockwise
            if (d123 > 0)
            {
                p1 = point1;
                p2 = point2;
                p3 = point3;
            }
            else if (d132 > 0)
            {
                p1 = point1;
                p2 = point3;
                p3 = point2;
            }
            else
                brogueException::show("Improper use of circum-circle algorithm");

            // 3) Solve the circum-circle interior determinant
            //

            T m00 = p1.x - point.x;
            T m10 = p1.y - point.y;
            T m20 = pow(m00, 2) + pow(m10, 2);
            T m01 = p2.x - point.x;
            T m11 = p2.y - point.y;
            T m21 = pow(m01, 2) + pow(m11, 2);
            T m02 = p3.x - point.x;
            T m12 = p3.y - point.y;
            T m22 = pow(m02, 2) + pow(m12, 2);

            T d = (m00 * ((m11 * m22) - (m21 * m12))) -
                  (m10 * ((m01 * m22) - (m21 * m02))) +
                  (m20 * ((m01 * m12) - (m11 * m02)));

            // Theorem:  Point lies in the circum-circle iff d > 0 (When 1 -> 2 -> 3 are sorted counter-clockwise)
            //

            // NOTE*** (Be sure to check this because of UI coordinates (y -> -y))
            //
            return d > 0;
        }
	};
}