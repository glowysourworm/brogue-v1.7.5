#pragma once

#include "exceptionHandler.h"
#include <concepts>
#include <functional>

using namespace std;

namespace brogueHd::backend::math
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
		static TMath min(TMath x, TMath y)
		{
			if (x <= y)
				return x;

			return y;
		}
		static TMath min(TMath x, TMath y, TMath z)
		{
			if (x <= y)
				return min(x, z);

			return min(y, z);
		}
		static TMath max(TMath x, TMath y)
		{
			if (x > y)
				return x;

			return y;
		}
		static TMath max(TMath x, TMath y, TMath z)
		{
			if (x > y)
				return max(x, z);

			return max(y, z);
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
	};
}