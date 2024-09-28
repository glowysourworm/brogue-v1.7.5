#pragma once

namespace brogueHd::component::math
{
	template<typename T>
	struct mathPoint
	{
		T x;
		T y;

		mathPoint()
		{
			x = -1;
			y = -1;
		}
		mathPoint(const mathPoint<T>& copy)
		{
			x = copy.x;
			y = copy.y;
		}
		mathPoint(T ax, T ay)
		{
			x = ax;
			y = ay;
		}

		mathPoint<T>& operator =(const mathPoint<T>& copy)
		{
			x = copy.x;
			y = copy.y;

			return this;
		}

		bool operator != (const mathPoint<T>& point)
		{
			return x != point.x || y != point.y;
		}
		bool operator == (const mathPoint<T>& point)
		{
			return x == point.x && y == point.y;
		}
	};
}