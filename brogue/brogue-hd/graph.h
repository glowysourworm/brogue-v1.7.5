#pragma once

#include "graphDefinitions.h"
#include "graphEdgeCollection.h"
#include <vector>

using namespace std;

namespace brogueHd::backend::math
{
    /// <summary>
    /// Defines a graph with type constraints for graphNode, and graphEdge
    /// </summary>
    /// <typeparam name="TValue">Type for the node value</typeparam>
    /// <typeparam name="TWeight">Type for the node weight</typeparam>
    template<GraphTemplate>
    class graph
    {
    public:

        graph(const TNode* nodes, const TEdge* edges);
        ~graph();

        void addEdge(TEdge* edge);
        void modify(TEdge* existingEdge, TEdge* newEdge);
        TEdge& getAdjacentEdges(TNode* node);
        TEdge& findEdge(TNode* node1, TNode* node2);

    private:

        std::vector<TNode*> _nodes;

        graphEdgeCollection* _edgeCollection;
    }
}