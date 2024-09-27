#pragma once

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

	template<brogueMathType TMath, std::same_as<TMath>... Args>
	class brogueMath
	{
	public:

		static TMath abs(TMath x)
		{
			if (x < 0)
				return x * -1;

			return x;
		}
		static TMath min(TMath x, TMath... args)
		{
			// Variadic args... is treated as an array
			//
			if (SIZEOF(args...) == 0)
				return x;

			if (x <= min(args...))
				return x;

			else
				return min(args...);
		}
		static TMath max(TMath x, TMath... args)
		{
			// Variadic args... is treated as an array
			//
			if (SIZEOF(args...) == 0)
				return x;

			if (x > max(args...))
				return x;

			else
				return max(args...);
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