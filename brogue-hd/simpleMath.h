#pragma once

#include "simple.h"
#include "simpleVector.h"
#include "simplePoint.h"
#include "simpleException.h"

namespace brogueHd::simple
{
	template<isNumber TMath>
	class simpleMath
	{
	public:
		static TMath abs(TMath x)
		{
			if (x < 0)
				return x * -1;

			return x;
		}

		template<isNumber...Args>
		static TMath minOf(TMath x, Args...args)
		{
			if (sizeof...(args) > 1)
				return minOf(x, args...);

			else if (sizeof...(args) > 0)
				return x < args;

			return x;
		}
		static TMath minOf(TMath x, TMath y, TMath z)
		{
			if (x <= y)
				return minOf(x, z);

			return minOf(y, z);
		}
		static TMath maxOf(TMath x, TMath y)
		{
			if (x > y)
				return x;

			return y;
		}
		static TMath maxOf(TMath x, TMath y, TMath z)
		{
			if (x > y)
				return maxOf(x, z);

			return maxOf(y, z);
		}
		static TMath sqrt(TMath x)
		{
			if (std::is_same<TMath, double>())
				return sqrt(static_cast<double>(x));

			else if (std::is_same<TMath, float>())
				return sqrtf(static_cast<float>(x));

			else if (std::is_convertible<TMath, int>())
				return sqrtl(x);

			else
				simpleException::showCstr("Unknown sqrt type simpleMath.h");
		}
		static TMath clamp(TMath x, TMath low, TMath high)
		{
			if (x < low)
				return low;

			else if (x > high)
				return high;

			return x;
		}

		static simpleVector<TMath> subtract(simplePoint<TMath> point2, simplePoint<TMath> point1)
		{
			return simpleVector<TMath>(point2.x - point1.x, point2.y - point1.y);
		}

		/// <summary>
		/// Returns the determinant of the orientation cross product (the sign of the cross product resulting in 
		/// crossing two difference vectors that order points 1,2, and 3 in that order). The sign of the determinant
		/// returned shows the orientation of the ordering (clockwise, counter-clockwise, or collinear)
		/// </summary>
		static TMath orientation(simplePoint<TMath> point1, simplePoint<TMath> point2, simplePoint<TMath> point3)
		{
			// 1 -> 2 -> 3 (Results from crossing the vectors 12 X 23 - where subtracting the points gives you the vector)
			simpleVector<TMath> vector12 = simpleMath<TMath>::subtract(point2, point1);
			simpleVector<TMath> vector23 = simpleMath<TMath>::subtract(point3, point2);

			return vector12.cross(vector23);
		}
	};
}