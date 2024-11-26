#pragma once

#include "simple.h"
#include "simplePoint.h"
#include "graphEdge.h"

using namespace brogueHd::component;

namespace brogueHd::simple
{
	template<isHashable T>
	struct simpleLine : public graphEdge<simplePoint<T>>
	{
	public:

		simpleLine() 
		{
		}
		simpleLine(const simplePoint<T>& point1, const simplePoint<T>& point2)
		{
			node1 = point1;
			node2 = point2;
		}
		simpleLine(const simpleLine<T>& copy)
		{
			node1 = copy.node1;
			node2 = copy.node2;
		}
	};
}