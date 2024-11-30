#pragma once

#include "graphNode.h"
#include "simple.h"

using namespace brogueHd::component;

namespace brogueHd::simple
{
	template<isHashable T>
	struct simplePoint : graphNode
	{
		T x;
		T y;

		simplePoint()
		{
			x = default_value::value<T>();
			y = default_value::value<T>();
		}
		simplePoint(const simplePoint<T>& copy)
		{
			x = copy.x;
			y = copy.y;
		}
		simplePoint(const T& ax, const T& ay)
		{
			x = ax;
			y = ay;
		}

		void operator=(const simplePoint<T>& copy)
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

		double distance(const simplePoint<T>& point) const
		{
			T dx = point.x - x;
			T dy = point.y - y;

			return sqrtf((dx * dx) + (dy * dy));
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(x, y);
		}
	};
}
