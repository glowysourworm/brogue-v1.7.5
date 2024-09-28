#pragma once

#include "graphDefinitions.h"
#include "graph.h"

namespace brogueHd::backend::math
{
	template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
	class graphAlgorithm
	{
	public:

		graphAlgorithm(graphDelegates<TNode, TEdge>::edgeConstructor graphEdgeConstructor);
		~graphAlgorithm();

		virtual graph<TNode, TEdge>* run(const simpleList<TNode>& vertices);

	protected:

		/// <summary>
		/// Returns FULL GRAPH:  Fully connected graph of all nodes
		/// </summary>
		graph<TNode, TEdge>* createDefaultGraph(const simpleList<TNode>& vertices);

		/// <summary>
		/// Delegate that constructs the proper edge type
		/// </summary>
		graphDelegates<TNode, TEdge>::edgeConstructor graphEdgeConstructor;
	};
}