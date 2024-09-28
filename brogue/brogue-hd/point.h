#pragma once

#include "vector.h"

namespace brogueHd::backend::math
{
	template<typename T>
	struct point
	{
		T x;
		T y;

		point()
		{
			x = -1;
			y = -1;
		}
		point(const point<T>& copy)
		{
			x = copy.x;
			y = copy.y;
		}
		point(T ax, T ay)
		{
			x = ax;
			y = ay;
		}

		point<T>& operator =(const point<T>& copy)
		{
			x = copy.x;
			y = copy.y;

			return this;
		}

		bool operator != (const point<T>& point)
		{
			return x != point.x || y != point.y;
		}
		bool operator == (const point<T>& point)
		{
			return x == point.x && y == point.y;
		}

		static vector<T> subtract(point<T> point2, point<T> point1)
		{
			return vector<T>(point2.x - point1.x, point2.y - point1.y);
		}
	};
}