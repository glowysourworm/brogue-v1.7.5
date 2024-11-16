#pragma once

#include "simple.h"
#include "simpleException.h"
#include "simplePoint.h"
#include "simpleVector.h"

#include <cmath>
#include <numbers>

namespace brogueHd::simple
{
	class simpleMath
	{
	public:

		static constexpr float Pi = std::numbers::pi;

	public:

		template<isNumber TMath>
		static TMath abs(TMath x)
		{
			if (x < 0)
				return x * -1;

			return x;
		}

		template<isNumber TMath, isNumber...Args>
		static TMath minOf(const TMath x, const Args...args)
		{
			if (sizeof...(args) > 0)
			{
				TMath result = minOf(args...);

				return x < result ? x : result;
			}

			return x;
		}

		template<isNumber TMath, isNumber...Args>
		static TMath maxOf(const TMath x, const Args...args)
		{
			if (sizeof...(args) > 0)
			{
				TMath result = maxOf(args...);

				return x > result ? x : result;
			}

			return x;
		}

		// Cheat sheet:  https://easings.net/

		template<isNumber TMath>
		static TMath easeInCubic(const TMath x)
		{
			return powerImpl(x, 3);
		}

		template<isNumber TMath>
		static TMath easeOutCubic(const TMath x)
		{
			return 1 - powerImpl(1 - x, 3);
		}

		template<isNumber TMath>
		static TMath power(const TMath x, const TMath raisedTo)
		{
			return powerImpl(x, raisedTo);
		}

		template<isNumber TMath>
		static TMath sqrt(TMath x)
		{
			if (isFloatLike<TMath>)
				return sqrtf(static_cast<float>(x));

			else if (isIntLike<TMath>)
				return sqrt(x);

			else
				simpleException::showCstr("Unknown sqrt type simpleMath.h");
		}

		template<isNumber TMath>
		static TMath naturalLog(TMath x)
		{
			if (isFloatLike<TMath>)
				return log(static_cast<float>(x));

			else if (isIntLike<TMath>)
				return log(x);

			else
				simpleException::showCstr("Unknown sqrt type simpleMath.h");
		}

		template<isNumber TMath>
		static TMath sin(TMath x)
		{
			if (isFloatLike<TMath>)
				return sinf(static_cast<float>(x));

			else if (isIntLike<TMath>)
				return sin(x);

			else
				simpleException::showCstr("Unknown sqrt type simpleMath.h");
		}

		template<isNumber TMath>
		static TMath exponential(TMath x)
		{
			if (isFloatLike<TMath>)
				return expf(static_cast<float>(x));

			else if (isIntLike<TMath>)
				return exp(x);

			else
				simpleException::showCstr("Unknown sqrt type simpleMath.h");
		}

		template<isNumber TMath>
		static TMath floor(TMath x)
		{
			if (isFloatLike<TMath>)
				return floorf(static_cast<float>(x));

			else if (isIntLike<TMath>)
				return floor(x);

			else
				simpleException::showCstr("Unknown sqrt type simpleMath.h");
		}

		template<isNumber TMath>
		static TMath clamp(TMath x, TMath low, TMath high)
		{
			if (x < low)
				return low;

			else if (x > high)
				return high;

			return x;
		}

		template<isNumber TMath>
		static simpleVector<TMath> subtract(simplePoint<TMath> point2, simplePoint<TMath> point1)
		{
			return simpleVector<TMath>(point2.x - point1.x, point2.y - point1.y);
		}

		/// <summary>
		/// Returns the determinant of the orientation cross product (the sign of the cross product resulting in 
		/// crossing two difference vectors that order points 1,2, and 3 in that order). The sign of the determinant
		/// returned shows the orientation of the ordering (clockwise, counter-clockwise, or collinear)
		/// </summary>
		template<isNumber TMath>
		static TMath orientation(simplePoint<TMath> point1, simplePoint<TMath> point2, simplePoint<TMath> point3)
		{
			// 1 -> 2 -> 3 (Results from crossing the vectors 12 X 23 - where subtracting the points gives you the vector)
			simpleVector<TMath> vector12 = simpleMath::subtract(point2, point1);
			simpleVector<TMath> vector23 = simpleMath::subtract(point3, point2);

			return vector12.cross(vector23);
		}

	private:

		template<isNumber TMath>
		static TMath minOf(const TMath x)
		{
			return x;
		}

		template<isNumber TMath>
		static TMath maxOf(const TMath x)
		{
			return x;
		}

		template<isNumber TMath>
		static TMath powerImpl(const TMath x, const int raisedTo)
		{
			return pow(x, raisedTo);
		}

		template<isNumber TMath>
		static TMath powerImpl(const TMath x, const float raisedTo)
		{
			return pow(x, raisedTo);
		}
	};
}