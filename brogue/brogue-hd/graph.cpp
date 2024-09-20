#pragma once 

#include "arrayExtension.h"
#include "graphDefinitions.h"
#include "graph.h"

namespace brogueHd::backend::math
{
    template<GraphTemplate>
    graph<TValue, TWeight, TNode, TEdge>::graph(const TNode* nodes, const TEdge* edges)
    {
        _nodes = new std::vector<TNode*>(nodes);
        _edgeCollection = new graphEdgeCollection(nodes, edges);

        //arrayExtension::forEach(nodes, [](TNode* node)
        //{
        //    _nodes->push_back(node);
        //});
    }
    template<GraphTemplate>
    graph<TValue, TWeight, TNode, TEdge>::~graph()
    {
        delete _nodes;
        delete _edgeCollection;
    }

    template<GraphTemplate>
    void graph<TValue, TWeight, TNode, TEdge>::addEdge(TEdge* edge)
    {
        if (!vectorExtension::contains(_nodes, edge->node1))
            _nodes->push_back(edge->node1);

        if (!vectorExtension::contains(_nodes, edge->node2))
            _nodes->push_back(edge->node2);

        _edgeCollection->addEdge(edge);
    }

    template<GraphTemplate>
    void graph<TValue, TWeight, TNode, TEdge>::modify(TEdge* existingEdge, TEdge* newEdge)
    {
        if (vectorExtension::contains(_nodes, edge->node1))
            _nodes->erase(edge->node1);

        if (vectorExtension::contains(_nodes, edge->node2))
            _nodes->erase(edge->node2);

        if (_edgeCollection->containsEdge(existingEdge))
            _edgeCollection->removeEdge(existingEdge);

        this->addEdge(newEdge);
    }

    template<GraphTemplate>
    TEdge& graph<TValue, TWeight, TNode, TEdge>::getAdjacentEdges(TNode* node)
    {
        return _edgeCollection->getAdjacentEdges(node);
    }

    template<GraphTemplate>
    TEdge& graph<TValue, TWeight, TNode, TEdge>::findEdge(TNode* node1, TNode* node2)
    {
        return _edgeCollection->findEdge(node1, node2);
    }
}