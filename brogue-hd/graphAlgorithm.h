#pragma once

#include "simpleListExtension.h"
#include "graphDefinitions.h"
#include "graph.h"

namespace brogueHd::component
{
	template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
	class graphAlgorithm
	{
	public:
        graphAlgorithm(){};
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
        /// Delegate that constructs the proper node type
        /// </summary>
        //graphNodeConstructor<TNode, TEdge> nodeConstructor;

		/// <summary>
		/// Delegate that constructs the proper edge type
		/// </summary>
		graphEdgeConstructor<TNode, TEdge> edgeConstructor;
	};

    template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
    graphAlgorithm<TNode, TEdge>::graphAlgorithm(graphEdgeConstructor<TNode, TEdge> edgeConstructor)
    {
        this->edgeConstructor = edgeConstructor;
    }

    template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
    graphAlgorithm<TNode, TEdge>::~graphAlgorithm()
    {

    }

    template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
    graph<TNode, TEdge>* run(const simpleList<TNode>& vertices)
    {
        // Overridden in child class
    }

    template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
    graph<TNode, TEdge>* graphAlgorithm<TNode, TEdge>::createDefaultGraph(const simpleList<TNode>& vertices)
    {
        if (vertices.count() == 0)
            brogueException::show("Trying to make a graph with zero vertices: delaunay.h");

        else if (vertices.count() == 1)
        {
            return new graph<TNode, TEdge>(vertices.toArray());
        }

        else
        {
            simpleList<TEdge> edges;
            graphAlgorithm<TNode, TEdge>* that = this;

            simpleListExtension<TNode>::distinctPairs(vertices, vertices, [&edges, &that](TNode node1, TNode node2)
            {
                edges.add(that->edgeConstructor(node1, node2));
            });

            return new graph<TNode, TEdge>(vertices.toArray(), edges.toArray());
        }
    }
}