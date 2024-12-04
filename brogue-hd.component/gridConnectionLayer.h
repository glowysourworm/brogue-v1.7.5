#pragma once

#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridLayer.h"

#include <simpleGraph.h>

namespace brogueHd::component
{
	template<typename T>
	class gridConnectionLayer
	{
	public:
		gridConnectionLayer(simpleGraph<gridLocator, gridLocatorEdge>* connectionGraph, gridLayer<T>* cellLayer);
		~gridConnectionLayer();

	private:

		simpleGraph<gridLocator, gridLocatorEdge>* _connectionGraph;

		// Permanent Cell Layer (shared pointer)(this may change during a topology change)
		gridLayer<T>* _cellLayer;
	};
}