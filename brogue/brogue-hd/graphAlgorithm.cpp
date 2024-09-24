#pragma once

#include "graphAlgorithm.h"
#include "exceptionHandler.h"
#include "vectorExtension.h"
#include <vector>

using namespace std;
using namespace brogueHd::backend::extension;

namespace brogueHd::backend::math
{
    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    graphAlgorithm<TNode, TEdge>::graphAlgorithm(graphDelegates<TNode, TEdge>::edgeConstructor graphEdgeConstructor)
    {
        this->graphEdgeConstructor = graphEdgeConstructor;
    }

    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    graphAlgorithm<TNode, TEdge>::~graphAlgorithm()
    {

    }

    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    graph<TNode, TEdge> run(const std::vector<TNode>& vertices)
    {
        // Overridden in child class
    }

    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    graph<TNode, TEdge>* graphAlgorithm<TNode, TEdge>::createDefaultGraph(const std::vector<TNode>& vertices)
    {
        if (vertices.count() == 0)
            brogueException::show("Trying to make a graph with zero vertices: delaunay.h");

        else if (vertices.count() == 1)
        {
            return new graph<TNode, TEdge>(vertices);
        }

        else
        {
            std::vector<TEdge> edges;

            vectorExtension<TNode>::distinctPairs(vertices, vertices, [&edges](TNode node1, TNode node2)
            {
                edges.push_back(this->graphEdgeConstructor(node1, node2));
            });

            return new graph<TNode, TEdge>(vertices, edges.data());
        }
    }
}