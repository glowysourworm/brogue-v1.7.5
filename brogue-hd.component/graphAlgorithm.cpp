#pragma once

#include "graphDefinitions.h"
#include "graphAlgorithm.h"
#include "exceptionHandler.h"
#include "simpleListExtension.h"

namespace brogueHd::component
{
    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    graphAlgorithm<TNode, TEdge>::graphAlgorithm(graphEdgeConstructor<TNode, TEdge> edgeConstructor)
    {
        this->edgeConstructor = edgeConstructor;
    }

    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    graphAlgorithm<TNode, TEdge>::~graphAlgorithm()
    {

    }

    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    graph<TNode, TEdge>* run(const simpleList<TNode>& vertices)
    {
        // Overridden in child class
    }

    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    graph<TNode, TEdge>* graphAlgorithm<TNode, TEdge>::createDefaultGraph(const simpleList<TNode>& vertices)
    {
        if (vertices.count() == 0)
            brogueException::show("Trying to make a graph with zero vertices: delaunay.h");

        else if (vertices.count() == 1)
        {
            return new graph<TNode, TEdge>(vertices);
        }

        else
        {
            simpleList<TEdge> edges;
            graphAlgorithm<TNode, TEdge>* that = this;

            simpleListExtension<TNode>::distinctPairs(vertices, vertices, [&edges, &that](TNode node1, TNode node2)
            {
                edges.add(that->edgeConstructor(node1, node2));
            });

            return new graph<TNode, TEdge>(vertices, edges.data());
        }
    }
}