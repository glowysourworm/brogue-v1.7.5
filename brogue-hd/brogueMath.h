#pragma once

#include "mathPoint.h"
#include "mathVector.h"
#include "brogueGlobal.h"
#include <concepts>
#include <functional>

namespace brogueHd::component::math
{
	template<typename T>
	concept brogueMathType = requires(T numberType)
	{
		{ numberType } -> std::convertible_to<short>;
		{ numberType } -> std::convertible_to<long>;
		{ numberType } -> std::convertible_to<int>;
		{ numberType } -> std::convertible_to<float>;
		{ numberType } -> std::convertible_to<double>;
	};

	template<brogueMathType TMath>
	class brogueMath
	{
	public:

		static TMath abs(TMath x)
		{
			if (x < 0)
				return x * -1;

			return x;
		}
		static TMath minOf(TMath x, TMath y)
		{
			if (x <= y)
				return x;

			return y;
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
				brogueException::show("Unknown sqrt type brogueMath.h");
		}
		static TMath clamp(TMath x, TMath low, TMath high)
		{
			if (x < low)
				return low;

			else if (x > high)
				return high;

			return x;
		}

		static mathVector<TMath> subtract(mathPoint<TMath> point2, mathPoint<TMath> point1)
		{
			return mathVector<TMath>(point2.x - point1.x, point2.y - point1.y);
		}

		/// <summary>
		/// Returns the determinant of the orientation cross product (the sign of the cross product resulting in 
		/// crossing two difference vectors that order points 1,2, and 3 in that order). The sign of the determinant
		/// returned shows the orientation of the ordering (clockwise, counter-clockwise, or collinear)
		/// </summary>
		static TMath orientation(mathPoint<TMath> point1, mathPoint<TMath> point2, mathPoint<TMath> point3)
		{
			// 1 -> 2 -> 3 (Results from crossing the vectors 12 X 23 - where subtracting the points gives you the vector)
			mathVector<TMath> vector12 = brogueMath<TMath>::subtract(point2, point1);
			mathVector<TMath> vector23 = brogueMath<TMath>::subtract(point3, point2);

			return vector12.cross(vector23);
		}
	};
}