#pragma once

#include "point.h"
#include "vector.h"
#include "exceptionHandler.h"

namespace brogueHd::backend::math
{
    template<typename T>
	struct triangle
	{
        point<T> point1;
        point<T> point2;
        point<T> point3;

        triangle(){}
        triangle(point<T> apoint1, point<T> apoint2, point<T> apoint3)
        {
            point1 = apoint1;
            point2 = apoint2;
            point3 = apoint3;
        }

        bool containsEqualEdge(point<T> vertex1, point<T> vertex2)
        {
            int pointsShared = 0;

            pointsShared += ((point1 == vertex1) || (point1 == vertex2)) ? 1 : 0;

            pointsShared += ((point2 == vertex1) || (point2 == vertex2)) ? 1 : 0;

            pointsShared += ((point3 == vertex1) || (point3 == vertex2)) ? 1 : 0;

            return pointsShared > 1;
        }

        bool containsEqualPoint(point<T> point)
        {
            return (point == point1) ||
                   (point == point2) ||
                   (point == point3);
        }

        bool circumCircleContains(point<T> point)
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
            var d123 = vector<T>::orientation(point1, point2, point3);

            // 1 -> 3 -> 2
            var d132 = vector<T>::orientation(point1, point3, point2);

            // NOTE*** Must handle collinear case. This may be the incorrect way to handle this.
            if (d123 == 0 || d132 == 0)
                return false;

            // Re-number the vertices to be counter-clockwise (1 -> 2 -> 3)
            point<T> p1, p2, p3;

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

            var m00 = p1.x - point.x;
            var m10 = p1.y - point.y;
            var m20 = pow(m00, 2) + pow(m10, 2);
            var m01 = p2.x - point.x;
            var m11 = p2.y - point.y;
            var m21 = pow(m01, 2) + pow(m11, 2);
            var m02 = p3.x - point.x;
            var m12 = p3.y - point.y;
            var m22 = pow(m02, 2) + pow(m12, 2);

            var d = (m00 * ((m11 * m22) - (m21 * m12))) -
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