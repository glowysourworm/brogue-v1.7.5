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
			this->node1 = point1;
			this->node2 = point2;
		}
		simpleLine(const simpleLine<T>& copy)
		{
			this->node1 = copy.node1;
			this->node2 = copy.node2;
		}
		void operator=(const simpleLine<T>& other)
		{
			this->node1 = other.node1;
			this->node2 = other.node2;
		}
		bool operator==(const simpleLine<T>& other)
		{
			return this->node1 == other.node1 &&
				   this->node2 == other.node2;
		}
		bool operator!=(const simpleLine<T>& other)
		{
			return this->node1 != other.node1 ||
				   this->node2 != other.node2;
		}
		bool isEquivalent(const simplePoint<T>& point1, const simplePoint<T>& point2) const
		{
			return (this->node1.x == point1.x &&
					this->node1.y == point1.y &&
					this->node2.x == point2.x &&
					this->node2.y == point2.y) ||
				   (this->node1.x == point2.x &&
					this->node1.y == point2.y &&
					this->node2.x == point1.x &&
					this->node2.y == point1.y);
		}
	};
}