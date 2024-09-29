#pragma once

#include "primsAlgorithm.h"
#include "exceptionHandler.h"
#include "simpleList.h"

namespace brogueHd::component
{
    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    primsAlgorithm<TNode, TEdge>::primsAlgorithm(graphEdgeConstructor<TNode, TEdge> graphEdgeConstructor)
    {

    }

    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    primsAlgorithm<TNode, TEdge>::~primsAlgorithm()
    {

    }

    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    graph<TNode, TEdge>* primsAlgorithm<TNode, TEdge>::run(const simpleList<TNode>& vertices)
    {
        if (vertices.count() < 3)
            return this->createDefaultGraph(vertices);

        return this->createMST(vertices);
    }

    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    graph<TNode, TEdge>* primsAlgorithm<TNode, TEdge>::createMST(const simpleList<TNode>& vertices)
    {
        // NOTE*** The MST is being created on a graph of REGIONS. This will behave differently than a
        //         graph of vertices. So, the input graph should be the FULL GRAPH to avoid issues with
        //         creating an MST of REGIONS.
        //         

        // Procedure (Prim's Algorithm)
        //
        // 1) Start the tree with a single vertex
        // 2) Calculate edges of the graph that connect NEW points not yet in the tree (P)
        //    to the existing tree points (T)
        // 3) Choose the least distant edge and add that edge to the tree
        //

        primsAlgorithm* that = this;

        simpleList<TNode> unusedVertices(vertices);
        simpleList<TNode> usedVertices;
        simpleList<TEdge> treeEdges;

        while (usedVertices.count() < vertices.count())
        {
            TNode nextVertex = NULL;
            TEdge nextEdge = NULL;

            // Initialize the tree
            if (treeEdges.count() == 0)
            {
                TNode firstVertex = unusedVertices[0];

                // Remove the first vertex - add to the used collection
                unusedVertices.remove(firstVertex);
                usedVertices.add(firstVertex);

                nextVertex = unusedVertices.withMin<short>([&firstVertex](TNode vertex)
                {
                    return firstVertex.calculateDistance(vertex);
                });

                nextEdge = this->graphEdgeConstructor(firstVertex, nextVertex);
            }

            else
            {
                // Get the next edge that connects an UNUSED vertex to a USED vertex
                unusedVertices.forEach([&treeEdges, &that](TNode vertex)
                {
                    // Edges in the current tree
                    short potentialEdgeWeight = std::numeric_limits<short>::max();
                    TNode potentialNode = NULL;

                    treeEdges.forEach([&vertex, &potentialEdgeWeight, &that](TEdge edge)
                    {
                        short distance = edge.node1.calculateDistance(vertex);

                        if (distance < potentialEdgeWeight)
                        {
                            potentialEdgeWeight = distance;
                            potentialNode = edge.node1;
                        }

                        distance = edge.node2.calculateDistance(vertex);

                        if (distance < potentialEdgeWeight)
                        {
                            potentialEdgeWeight = distance;
                            potentialNode = edge.node2;
                        }
                    });

                    // Check both potential edges for the least distant choice
                    if (nextEdge == NULL || potentialEdgeWeight < nextEdge.weight())
                    {
                        nextEdge = that->graphEdgeConstructor(potentialNode, vertex);
                        nextVertex = vertex;
                    }
                });
            }

            if (nextEdge == NULL)
                brogueException::show("No connection found between regions Minimum Spanning Tree:  primsAlgorithm.cpp");

            unusedVertices.remove(nextVertex);
            usedVertices.add(nextVertex);

            // Add next connection to the tree
            treeEdges.add(nextEdge);
        }

        return new graph<TNode, TEdge>(vertices, treeEdges);
    }
}