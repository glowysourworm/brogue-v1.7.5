#pragma once

#include "simple.h"

namespace brogueHd::simple
{
	template<isHashable T>
	struct simplePoint : hashable
	{
		T x;
		T y;

		simplePoint()
		{
			x = -1;
			y = -1;
		}
		simplePoint(const simplePoint<T>& copy)
		{
			x = copy.x;
			y = copy.y;
		}
		simplePoint(T ax, T ay)
		{
			x = ax;
			y = ay;
		}

		void operator =(const simplePoint<T>& copy)
		{
			x = copy.x;
			y = copy.y;
		}

		bool operator != (const simplePoint<T>& point) const
		{
			return x != point.x || y != point.y;
		}
		bool operator == (const simplePoint<T>& point) const
		{
			return x == point.x && y == point.y;
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(x, y);
		}
	};
}
