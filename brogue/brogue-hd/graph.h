#pragma once

#include "graphDefinitions.h"
#include "graphEdgeCollection.h"
#include "vectorExtension.h"
#include <vector>

using namespace std;

namespace brogueHd::backend::math
{
    /// <summary>
    /// Defines a graph with type constraints for graphNode, and graphEdge
    /// </summary>
    /// <typeparam name="TValue">Type for the node value</typeparam>
    /// <typeparam name="TWeight">Type for the node weight</typeparam>
    //template<GraphTemplate>

    template<graphNodeType TNode, graphEdgeType TEdge>
    class graph
    {
    public:

        graph(const TNode* nodes, const TEdge* edges);
        graph(const TNode* nodes);
        ~graph();

        void addEdge(TEdge* edge);
        void modify(TEdge* existingEdge, TEdge* newEdge);
        TEdge& getAdjacentEdges(TNode* node);
        TEdge& findEdge(TNode* node1, TNode* node2);

        void iterateNodes(vectorDelegates<TNode>::callback callback);
        void iterateEdges(vectorDelegates<TEdge>::callback callback);

    private:

        std::vector<TNode*>* _nodes;

        graphEdgeCollection* _edgeCollection;
    }
}