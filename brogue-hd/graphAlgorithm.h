#pragma once

#include "graph.h"
#include "graphDefinitions.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleList.h"
#include "simpleListExtension.h"

using namespace brogueHd::simple;

namespace brogueHd::component
{
	template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
	class graphAlgorithm
	{
	public:
		graphAlgorithm(const graphEdgeConstructor<TNode, TEdge>& graphEdgeConstructor);
		~graphAlgorithm();

		virtual graph<TNode, TEdge>* run(const simpleList<TNode>& vertices);

		graph<TNode, TEdge>* createFullGraph(const simpleList<TNode>& vertices);

	protected:

		/// <summary>
		/// Returns FULL GRAPH:  Fully connected graph of all nodes
		/// </summary>
		graph<TNode, TEdge>* createDefaultGraph(const simpleList<TNode>& vertices);

		/// <summary>
		/// Delegate that constructs the proper node type
		/// </summary>
		//graphNodeConstructor<TNode, TEdge> nodeConstructor;

		/// <summary>
		/// Delegate that constructs the proper edge type
		/// </summary>
		graphEdgeConstructor<TNode, TEdge> edgeConstructor;
	};

	template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
	graphAlgorithm<TNode, TEdge>::graphAlgorithm(const graphEdgeConstructor<TNode, TEdge>& edgeConstructor)
	{
		this->edgeConstructor = edgeConstructor;
	}

	template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
	graphAlgorithm<TNode, TEdge>::~graphAlgorithm()
	{
	}

	template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
	graph<TNode, TEdge>* graphAlgorithm<TNode, TEdge>::run(const simpleList<TNode>& vertices)
	{
		throw simpleException("Must override the run function in a child class:  graphAlgorithm.h");
	}

	template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
	graph<TNode, TEdge>* graphAlgorithm<TNode, TEdge>::createFullGraph(const simpleList<TNode>& vertices)
	{
		return createDefaultGraph(vertices);
	}

	template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
	graph<TNode, TEdge>* graphAlgorithm<TNode, TEdge>::createDefaultGraph(const simpleList<TNode>& vertices)
	{
		if (vertices.count() == 0)
			throw simpleException("Trying to make a graph with zero vertices: delaunay.h");

		else if (vertices.count() == 1)
		{
			return new graph<TNode, TEdge>(vertices.toArray());
		}

		else
		{
			simpleList<TEdge> edges;
			graphAlgorithm<TNode, TEdge>* that = this;

			simpleListExtension<TNode>::distinctPairs(vertices, vertices, [&edges, &that] (const TNode& node1, const TNode& node2)
			{
				edges.add(that->edgeConstructor(node1, node2));
			});

			return new graph<TNode, TEdge>(vertices.toArray(), edges.toArray());
		}
	}
}