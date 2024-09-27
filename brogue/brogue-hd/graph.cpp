#pragma once 

#include "graphDefinitions.h"
#include "graph.h"

namespace brogueHd::backend::math
{
    template<graphNodeType TNode, graphEdgeType TEdge>
    graph<TNode, TEdge>::graph(const TNode* nodes, const TEdge* edges)
    {
        _nodes = new simpleList<TNode*>(nodes);
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

        _nodes = new simpleList<TNode*>(nodes);
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
        if (!_nodes->contains(edge->node1))
            _nodes->push_back(edge->node1);

        if (!_nodes->contains(edge->node2))
            _nodes->push_back(edge->node2);

        _edgeCollection->addEdge(edge);
    }

    template<graphNodeType TNode, graphEdgeType TEdge>
    void graph<TNode, TEdge>::modify(TEdge* existingEdge, TEdge* newEdge)
    {
        if (_nodes->contains(edge->node1))
            _nodes->erase(edge->node1);

        if (_nodes->contains(edge->node2))
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
    void graph<TNode, TEdge>::iterateNodes(simpleListDelegates<TNode*>::callback callback)
    {
        _nodes->forEach(callback);
    }

    template<graphNodeType TNode, graphEdgeType TEdge>
    void graph<TNode, TEdge>::iterateEdges(simpleListDelegates<TEdge*>::callback callback)
    {
        _edgeCollection->getEdges()->forEach(callback);
    }
}