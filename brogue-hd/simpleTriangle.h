#pragma once

#include "simple.h"
#include "simpleException.h"
#include "simpleMath.h"
#include "simplePoint.h"

namespace brogueHd::simple
{
	template<isHashable T>
	struct simpleTriangle : simpleStruct
	{
		simplePoint<T> point1;
		simplePoint<T> point2;
		simplePoint<T> point3;

		simpleTriangle() 
		{
			point1 = default_value::value<simplePoint<T>>();
			point2 = default_value::value<simplePoint<T>>();
			point3 = default_value::value<simplePoint<T>>();
		}
		simpleTriangle(const simpleTriangle<T>& copy) 
		{
			point1 = copy.point1;
			point2 = copy.point2;
			point3 = copy.point3;
		}
		simpleTriangle(const simplePoint<T>& apoint1, const simplePoint<T>& apoint2, const simplePoint<T>& apoint3)
		{
			point1 = apoint1;
			point2 = apoint2;
			point3 = apoint3;
		}

		void operator=(const simpleTriangle<T>& copy)
		{
			point1 = copy.point1;
			point2 = copy.point2;
			point3 = copy.point3;
		}

		bool operator==(const simpleTriangle<T>& triangle) const
		{
			return point1 == triangle.point1 &&
				point2 == triangle.point2 &&
				point3 == triangle.point3;
		}

		bool operator!=(const simpleTriangle<T>& triangle) const
		{
			return point1 != triangle.point1 ||
					point2 != triangle.point2 ||
					point3 != triangle.point3;
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(point1.getHash(), point2.getHash(), point3.getHash());
		}

		T area() const
		{
			return simpleMath::abs((point1.x * (point2.y - point3.y) + 
									point2.x * (point3.y - point1.y) + 
									point3.x * (point1.y - point2.y)) / 2.0f);
		}

		bool isPointInTriangle(const simplePoint<T>& other) const
		{
			simpleTriangle<T> subTriangle1(point1, point2, other);
			simpleTriangle<T> subTriangle2(point2, point3, other);
			simpleTriangle<T> subTriangle3(point3, point1, other);

			return area() == (subTriangle1.area() + subTriangle2.area() + subTriangle3.area());
		}

		bool isEquivalentTo(const simpleTriangle<T>& other) const
		{
			return containsEqualPoint(other.point1) &&
				   containsEqualPoint(other.point2) &&
				   containsEqualPoint(other.point3);
		}

		bool containsEqualEdge(const simplePoint<T>& vertex1, const simplePoint<T>& vertex2) const
		{
			int pointsShared = 0;

			pointsShared += ((point1 == vertex1) || (point1 == vertex2)) ? 1 : 0;

			pointsShared += ((point2 == vertex1) || (point2 == vertex2)) ? 1 : 0;

			pointsShared += ((point3 == vertex1) || (point3 == vertex2)) ? 1 : 0;

			return pointsShared > 1;
		}

		bool containsEqualPoint(const simplePoint<T>& point) const
		{
			return (point == point1) ||
					(point == point2) ||
					(point == point3);
		}

		bool circumCircleContains(const simplePoint<T>& point) const
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
			T d123 = simpleMath::orientation(point1, point2, point3);

			// 1 -> 3 -> 2
			T d132 = simpleMath::orientation(point1, point3, point2);

			// NOTE*** Must handle collinear case. This may be the incorrect way to handle this.
			if (d123 == 0 || d132 == 0)
				return false;

			// Re-number the vertices to be counter-clockwise (1 -> 2 -> 3)
			simplePoint<T> p1;
			simplePoint<T> p2;
			simplePoint<T> p3;

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
				throw simpleException("Improper use of circum-circle algorithm");

			// 3) Solve the circum-circle interior determinant
			//

			T m00 = p1.x - point.x;
			T m10 = p1.y - point.y;
			T m20 = simpleMath::power<float>(m00, 2) + simpleMath::power<float>(m10, 2);
			T m01 = p2.x - point.x;
			T m11 = p2.y - point.y;
			T m21 = simpleMath::power<float>(m01, 2) + simpleMath::power<float>(m11, 2);
			T m02 = p3.x - point.x;
			T m12 = p3.y - point.y;
			T m22 = simpleMath::power<float>(m02, 2) + simpleMath::power<float>(m12, 2);

			T d = (m00 * ((m11 * m22) - (m21 * m12))) -
				  (m10 * ((m01 * m22) - (m21 * m02))) +
				  (m20 * ((m01 * m12) - (m11 * m02)));

			// Theorem:  Point lies in the circum-circle iff d > 0 (When 1 -> 2 -> 3 are sorted counter-clockwise)
			//

			// NOTE*** (Be sure to check this because of UI coordinates (y -> -y)). This class should be operating
			//		   in real numbers; not UI coordinates.
			//
			return d > 0;
		}
	};
}