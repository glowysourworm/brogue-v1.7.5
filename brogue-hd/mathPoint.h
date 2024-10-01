#pragma once

#include "brogueGlobal.h"

namespace brogueHd::component::math
{
	template<typename T>
	struct mathPoint : hashable
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

		void operator =(const mathPoint<T>& copy)
		{
			x = copy.x;
			y = copy.y;
		}

		bool operator != (const mathPoint<T>& point)
		{
			return x != point.x || y != point.y;
		}
		bool operator == (const mathPoint<T>& point)
		{
			return x == point.x && y == point.y;
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(x, y);
		}
	};
}

MAKE_HASHABLE_STRUCT(brogueHd::component::math::mathPoint<short>);