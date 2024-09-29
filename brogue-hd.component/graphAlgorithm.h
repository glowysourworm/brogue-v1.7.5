#pragma once

#include "graphDefinitions.h"
#include "graph.h"

namespace brogueHd::component
{
	template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
	class graphAlgorithm
	{
	public:

		graphAlgorithm(graphEdgeConstructor<TNode, TEdge> graphEdgeConstructor);
		~graphAlgorithm();

		virtual graph<TNode, TEdge>* run(const simpleList<TNode>& vertices)
		{
			return NULL;
		}

	protected:

		/// <summary>
		/// Returns FULL GRAPH:  Fully connected graph of all nodes
		/// </summary>
		graph<TNode, TEdge>* createDefaultGraph(const simpleList<TNode>& vertices);

		/// <summary>
		/// Delegate that constructs the proper edge type
		/// </summary>
		graphEdgeConstructor<TNode, TEdge> edgeConstructor;
	};
}