#pragma once 

#include "arrayExtension.h"
#include "graphDefinitions.h"
#include "graph.h"
#include "vectorExtension.h"

namespace brogueHd::backend::math
{
    template<graphNodeType TNode, graphEdgeType TEdge>
    graph<TNode, TEdge>::graph(const TNode* nodes, const TEdge* edges)
    {
        _nodes = new std::vector<TNode*>(nodes);
        _edgeCollection = new graphEdgeCollection(nodes, edges);

        //arrayExtension::forEach(nodes, [](TNode* node)
        //{
        //    _nodes->push_back(node);
        //});
    }

    template<graphNodeType TNode, graphEdgeType TEdge>
    graph<TNode, TEdge>::graph(const TNode* nodes)
    {
        TEdge edges[0];

        _nodes = new std::vector<TNode*>(nodes);
        _edgeCollection = new graphEdgeCollection(nodes, &edges);
    }

    template<graphNodeType TNode, graphEdgeType TEdge>
    graph<TNode, TEdge>::~graph()
    {
        delete _nodes;
        delete _edgeCollection;
    }

    template<graphNodeType TNode, graphEdgeType TEdge>
    void graph<TNode, TEdge>::addEdge(TEdge* edge)
    {
        if (!vectorExtension::contains(_nodes, edge->node1))
            _nodes->push_back(edge->node1);

        if (!vectorExtension::contains(_nodes, edge->node2))
            _nodes->push_back(edge->node2);

        _edgeCollection->addEdge(edge);
    }

    template<graphNodeType TNode, graphEdgeType TEdge>
    void graph<TNode, TEdge>::modify(TEdge* existingEdge, TEdge* newEdge)
    {
        if (vectorExtension::contains(_nodes, edge->node1))
            _nodes->erase(edge->node1);

        if (vectorExtension::contains(_nodes, edge->node2))
            _nodes->erase(edge->node2);

        if (_edgeCollection->containsEdge(existingEdge))
            _edgeCollection->removeEdge(existingEdge);

        this->addEdge(newEdge);
    }

    template<graphNodeType TNode, graphEdgeType TEdge>
    TEdge& graph<TNode, TEdge>::getAdjacentEdges(TNode* node)
    {
        return _edgeCollection->getAdjacentEdges(node);
    }

    template<graphNodeType TNode, graphEdgeType TEdge>
    TEdge& graph<TNode, TEdge>::findEdge(TNode* node1, TNode* node2)
    {
        return _edgeCollection->findEdge(node1, node2);
    }

    template<graphNodeType TNode, graphEdgeType TEdge>
    void graph<TNode, TEdge>::iterateNodes(vectorDelegates<TNode>::callback callback)
    {
        vectorExtension<TNode>::forEach(_nodes, [](TNode node)
        {
            callback(node);
        });
    }

    template<graphNodeType TNode, graphEdgeType TEdge>
    void graph<TNode, TEdge>::iterateEdges(vectorDelegates<TEdge>::callback callback)
    {
        vectorExtension<TEdge>::forEach(_edgeCollection->getEdges(), [](TEdge edge)
        {
            callback(edge);
        });
    }
}