#pragma once

#include "gridLocator.h"
#include "gridLocatorNode.h"
#include <simple.h>
#include <simplePoint.h>
#include <simpleGraphEdge.h>

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;

	template<isHashable T>
	struct gridLocatorEdge : public simpleGraphEdge<gridLocatorNode<T>>
	{
	public:
		gridLocatorEdge()
		{
			this->node1 = default_value::value<gridLocatorNode<T>>();
			this->node2 = default_value::value<gridLocatorNode<T>>();
		}

		gridLocatorEdge(const gridLocatorEdge<T>& copy)
		{
			this->node1 = copy.node1;
			this->node2 = copy.node2;
		}

		gridLocatorEdge(const gridLocatorNode<T>& node1, const gridLocatorNode<T>& node2)
		{
			this->node1 = node1;
			this->node2 = node2;
		}

		void operator=(const gridLocatorEdge<T>& copy)
		{
			this->node1 = copy.node1;
			this->node2 = copy.node2;
		}

		bool operator==(const gridLocatorEdge<T>& edge) const
		{
			return compare(edge);
		}

		bool operator!=(const gridLocatorEdge<T>& edge) const
		{
			return !compare(edge);
		}

		/// <summary>
		/// Returns the graph weight (distance) for this edge
		/// </summary>
		float weight() const override
		{
			return this->node1.getLocator().distance(this->node2.getLocator());
		}

		template <typename T>
		bool isEquivalent(const simplePoint<T>& point1, const simplePoint<T>& point2) const
		{
			return (this->node1.getLocator().column == point1.x &&
					this->node1.getLocator().row == point1.y &&
					this->node2.getLocator().column == point2.x &&
					this->node2.getLocator().row == point2.y) ||
				   (this->node1.getLocator().column == point2.x &&
					this->node1.getLocator().row == point2.y &&
					this->node2.getLocator().column == point1.x &&
					this->node2.getLocator().row == point1.y);
		}

		virtual size_t getHash() const override
		{
			return hashGenerator::generateHash(this->node1, this->node2);
		}

	private:

		bool compare(const gridLocatorEdge<T>& other)
		{
			return (this->node1 == other.node1 && this->node2 == other.node2) ||
				   (this->node1 == other.node2 && this->node2 == other.node1);
		}
	};
}
