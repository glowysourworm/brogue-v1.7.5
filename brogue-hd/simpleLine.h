#pragma once

#include "graphEdge.h"
#include "simple.h"
#include "simpleMath.h"
#include "simplePoint.h"
#include "simpleVector.h"

using namespace brogueHd::component;

namespace brogueHd::simple
{
	template<isHashable T>
	struct simpleLine : public graphEdge<simplePoint<T>>
	{

	public:

		simpleLine() 
		{
		}
		simpleLine(const simplePoint<T>& point1, const simplePoint<T>& point2)
		{
			this->node1 = point1;
			this->node2 = point2;
		}
		simpleLine(const simpleLine<T>& copy)
		{
			this->node1 = copy.node1;
			this->node2 = copy.node2;
		}
		bool intersects(const simpleLine<float>& other, simplePoint<float>& intersection)
		{
			// Parametric form of Line Segment Intersection:
			// 
			//https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
			//
            //

			// Use the parametric form for the intersection that resolves as a
			// simple vector algebra equation:
			//
			// Let:  v(s) = v_center + (s * v_ray)      s in [0, inf]
			//
			//       v(t) = v_1 + (t * (v_2 - v_1))     t in [0, 1]
			//
			// Substitute:
			//
			//       w_1 = v_center - v_1
			//       w_2 = v_2 - v_1
			//       w_3 = { -v_ray_y, v_ray_x }        Negative perpendicular vector to the ray (NOT A UNIT VECTOR!)
			//
			// Then:
			//
			//       s = |w_2 x w_1| / w_2 * w_3        (cross and dot products)
			//
			//       t =  w_1 * w_3 / w_2 * w_3
			//

			// v_ray => rayPoint - rayCenter

			simplePoint<float> rayPoint = this->node2;
			simplePoint<float> rayCenter = this->node1;
			simplePoint<float> lineVertex1 = other.node1;
			simplePoint<float> lineVertex2 = other.node2;

			// PERFORMANCE:  Don't allocate new memory
			float dX = rayPoint.x - rayCenter.x;
			float dY = rayPoint.y - rayCenter.y;

			float w1X = rayCenter.x - lineVertex1.x;
			float w1Y = rayCenter.y - lineVertex1.y;

			float w2X = lineVertex2.x - lineVertex1.x;
			float w2Y = lineVertex2.y - lineVertex1.y;

			float w3X = -1 * dY;
			float w3Y = dX;

			simpleVector<float> w1(w1X, w1Y);
			simpleVector<float> w2(w2X, w2Y);
			simpleVector<float> w3(w3X, w3Y);

			float dot23 = w2.dot(w3);

			// Parallel
			if (dot23 == 0)
				return false;

			float cross21 = w2.cross(w1);
			float dot13 = w1.dot(w3);

			// simpleVector.cross returns the magnitude of the cross product (NOT TO BE CONFUSED WITH THE ABS VALUE OF A NUMBER!)
			float s = cross21 / dot23;
			float t = dot13 / dot23;

			// Now, check that t is in the interval [0, 1] and that s >= 0
			//

			if (t < 0 || t > 1)
				return false;

			else
			{
				if (s < 0)
					return false;

				// Gives a point along the ray where the parameter s is at the intersection point
				//
				//      intersection = v_center + (s * v_ray)
				//
				//      intersection = v_1 + (t * (v_2 - v_1))
				//

				intersection.x = lineVertex1.x + (w2X * t);
				intersection.y = lineVertex1.y + (w2Y * t);

				float otherIntersectionX = rayCenter.x + (dY * s);
				float otherIntersectionY = rayCenter.y + (dY * s);

				//// Tolerances not liked...
				//if (simpleMath::abs(t - 1) > 0.0001 ||
				//	simpleMath::abs(t) > 0.0001)
				//	return true;

				// These two parameters will align for the intersection
				if (simpleMath::abs(otherIntersectionX - intersection.x) < 0.1 &&
					simpleMath::abs(otherIntersectionY - intersection.y) < 0.1)
					return true;

				return false;
			}
		}
		void operator=(const simpleLine<T>& other)
		{
			this->node1 = other.node1;
			this->node2 = other.node2;
		}
		bool operator==(const simpleLine<T>& other)
		{
			return this->node1 == other.node1 &&
				   this->node2 == other.node2;
		}
		bool operator!=(const simpleLine<T>& other)
		{
			return this->node1 != other.node1 ||
				   this->node2 != other.node2;
		}

		bool isEquivalent(const simplePoint<T>& point1, const simplePoint<T>& point2) const
		{
			return (this->node1.x == point1.x &&
					this->node1.y == point1.y &&
					this->node2.x == point2.x &&
					this->node2.y == point2.y) ||
				   (this->node1.x == point2.x &&
					this->node1.y == point2.y &&
					this->node2.x == point1.x &&
					this->node2.y == point1.y);
		}
	};
}