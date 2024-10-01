#pragma once

#include "graphDefinitions.h"
#include "graphEdgeCollection.h"
#include "simpleList.h"

namespace brogueHd::component
{
    /// <summary>
    /// Defines a graph with type constraints for graphNode, and graphEdge
    /// </summary>
    /// <typeparam name="TValue">Type for the node value</typeparam>
    /// <typeparam name="TWeight">Type for the node weight</typeparam>
    //template<GraphTemplate>

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    class graph
    {
    public:

        graph(const TNode* nodes, const TEdge* edges);
        graph(const TNode* nodes);
        ~graph();

        void addEdge(TEdge edge);
        void modify(TEdge existingEdge, TEdge newEdge);
        simpleList<TEdge> getAdjacentEdges(TNode node);
        TEdge findEdge(TNode node1, TNode node2);

        void iterateNodes(simpleListCallback<TNode> callback);
        void iterateEdges(simpleListCallback<TEdge> callback);

    private:

        simpleList<TNode>* _nodes;

        graphEdgeCollection<TNode, TEdge>* _edgeCollection;
    };

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    graph<TNode, TEdge>::graph(const TNode* nodes, const TEdge* edges)
    {
        _nodes = new simpleList<TNode>(nodes);
        _edgeCollection = new graphEdgeCollection<TNode, TEdge>(nodes, edges);

        //arrayExtension::forEach(nodes, [](TNode* node)
        //{
        //    _nodes->push_back(node);
        //});
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    graph<TNode, TEdge>::graph(const TNode* nodes)
    {
        TEdge* edges = NULL;

        _nodes = new simpleList<TNode>(nodes);
        _edgeCollection = new graphEdgeCollection<TNode, TEdge>(nodes, edges);
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    graph<TNode, TEdge>::~graph()
    {
        delete _nodes;
        delete _edgeCollection;
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    void graph<TNode, TEdge>::addEdge(TEdge edge)
    {
        if (!_nodes->contains(edge.node1))
            _nodes->add(edge.node1);

        if (!_nodes->contains(edge.node2))
            _nodes->add(edge.node2);

        _edgeCollection->addEdge(edge);
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    void graph<TNode, TEdge>::modify(TEdge existingEdge, TEdge newEdge)
    {
        if (_nodes->contains(newEdge.node1))
            _nodes->remove(newEdge.node1);

        if (_nodes->contains(newEdge.node2))
            _nodes->remove(newEdge.node2);

        if (_edgeCollection->containsEdge(existingEdge))
            _edgeCollection->removeEdge(existingEdge);

        this->addEdge(newEdge);
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    simpleList<TEdge> graph<TNode, TEdge>::getAdjacentEdges(TNode node)
    {
        return _edgeCollection->getAdjacentEdges(node);
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    TEdge graph<TNode, TEdge>::findEdge(TNode node1, TNode node2)
    {
        return _edgeCollection->findEdge(node1, node2);
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    void graph<TNode, TEdge>::iterateNodes(simpleListCallback<TNode> callback)
    {
        _nodes->forEach(callback);
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    void graph<TNode, TEdge>::iterateEdges(simpleListCallback<TEdge> callback)
    {
        _edgeCollection->getEdges().forEach(callback);
    }
}