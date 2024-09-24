#pragma once

#include "vector.h"

namespace brogueHd::backend::math
{
	template<typename T>
	struct point
	{
		T x;
		T y;

		point(){}
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

		static point<T>& operator =(const point<T>& copy)
		{
			x = copy.x;
			y = copy.y;

			return this;
		}

		static bool operator != (const point<T>& point1, const point<T>& point2)
		{
			return point1.x != point2.x || point1.y != point2.y;
		}
		static bool operator == (const point<T>& point1, const point<T>& point2)
		{
			return point1.x == point2.x && point1.y == point2.y;
		}

		static vector<T> subtract(point<T> point2, point<T> point1)
		{
			return vector<T>(point2.x - point1.x, point2.y - point1.y);
		}
	};
}