#pragma once

#include "graphEdgeCollection.h"
#include "exceptionHandler.h"

namespace brogueHd::component
{
    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    graphEdgeCollection<TNode, TEdge>::graphEdgeCollection(TEdge* edges)
    {
        _nodes = new simpleHash<TNode, TNode>();
        _edges = new simpleHash<TEdge, TEdge>();
        _nodeAdjacentEdges = new simpleHash<TNode*, simpleHash<TEdge, TEdge>*>();

        initialize(edges, TNode[0]);
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    graphEdgeCollection<TNode, TEdge>::graphEdgeCollection(TEdge* edges, TNode* nodes)
    {
        _nodes = new simpleHash<TNode, TNode>();
        _edges = new simpleHash<TEdge, TEdge>();
        _nodeAdjacentEdges = new simpleHash<TNode*, simpleHash<TEdge, TEdge>*>();

        initialize(edges, nodes);
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    graphEdgeCollection<TNode, TEdge>::~graphEdgeCollection()
    {
        delete _nodes;
        delete _edges;
        delete _nodeAdjacentEdges;
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    void graphEdgeCollection<TNode, TEdge>::initialize(TEdge* edges, TNode* nodes)
    {
        graphEdgeCollection<TNode, TEdge>* that = this;

        // Initialize for edges and adjacent vertex lookup
        edges->forEach([&that](TEdge* edge)
        {
            that->addEdge(edge);

            return iterationCallback::iterate;
        });

        // Look for any nodes not yet in the collections - add these
        nodes->forEach([&that](TNode* node)
        {
            if (!that->hasNode(node))
                that->addNode(node);

            return iterationCallback::iterate;
        });
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    short graphEdgeCollection<TNode, TEdge>::edgeCount() const
    {
        return _edges->count();
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    short graphEdgeCollection<TNode, TEdge>::nodeCount() const
    {
        return _nodes->count();
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    void graphEdgeCollection<TNode, TEdge>::addEdge(const TEdge& edge)
    {
        if (edge.node1 == edge.node2)
            brogueException::show("Trying to add self-referencing edge to a graph:  graphEdgeCollection.addEdge");

        if (_edges->contains(edge))
            brogueException::show("Trying to insert duplicate edge key:  graphEdgeCollection.addEdge");

        _edges->add(edge, edge);

        // Nodes
        if (!_nodes->contains(edge.node1))
            _nodes->add(edge.node1, edge.node1);

        if (!_nodes->contains(edge->node2))
            _nodes->add(edge.node2, edge.node2);

        // Forward entry
        if (_nodeAdjacentEdges->contains(edge.node1))
            _nodeAdjacentEdges[edge.node1]->add(edge, edge);

        else
        {
            simpleHash<TEdge, TEdge>* adjacentEdges;

            adjacentEdges->add(edge, edge);
            
            _nodeAdjacentEdges->add(edge.node1, adjacentEdges);
        }

        // Reverse entry
        if (_nodeAdjacentEdges->contains(edge.node2))
            _nodeAdjacentEdges[edge.node2]->add(edge, edge);

        else
        {
            simpleHash<TEdge, TEdge>* adjacentEdges = new simpleHash<TEdge, TEdge>();

            adjacentEdges->add(edge, edge);

            _nodeAdjacentEdges->add(edge.node2, adjacentEdges);
        }
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    void graphEdgeCollection<TNode, TEdge>::addNode(const TNode& node)
    {
        if (!_nodes->contains(node))
            _nodes->insert(node, node);

        // Adjacent edges map
        if (!_nodeAdjacentEdges->contains(node))
            _nodeAdjacentEdges->add(node, new simpleHash<TEdge, TEdge>());
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    void graphEdgeCollection<TNode, TEdge>::removeEdge(const TEdge& edge)
    {
        _edges->remove(edge);

        if (_nodes->contains(edge.node1))
            _nodes->remove(edge.node1);

        if (_nodes->contains(edge.node2))
            _nodes->remove(edge.node2);

        // Forward entry
        if (_nodeAdjacentEdges->containsKey(edge.node1))
        {
            if (_nodeAdjacentEdges->get(edge.node1)->contains(edge))
                _nodeAdjacentEdges->get(edge.node1)->remove(edge);
        }

        // Reverse entry
        if (_nodeAdjacentEdges->containsKey(edge.node2))
        {
            if (_nodeAdjacentEdges->get(edge.node2)->contains(edge))
                _nodeAdjacentEdges->get(edge.node2)->remove(edge);
        }
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    bool graphEdgeCollection<TNode, TEdge>::containsNode(const TNode& node)
    {
        return _nodes->contains(node);
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    bool graphEdgeCollection<TNode, TEdge>::containsEdge(const TEdge& edge)
    {
        return _edges->contains(edge);
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    bool graphEdgeCollection<TNode, TEdge>::containsEdge(const TNode& node1, const TNode& node2)
    {
        return findEdge(node1, node2) != NULL;
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    TEdge graphEdgeCollection<TNode, TEdge>::findEdge(const TNode& node1, const TNode& node2)
    {
        return _edges->firstKey([&node1, &node2](TEdge key, TEdge value)
        {
            if (key.node1 == node1 && key.node2 == node2)
            {
                return key;
            }
            else if (key.node2 == node1 && key.node1 == node2)
            {
                return key;
            }
        });
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    simpleList<TEdge> graphEdgeCollection<TNode, TEdge>::getAdjacentEdges(const TNode& node)
    {
        if (!_nodeAdjacentEdges->contains(node))
            brogueException::show("No adjacent edges contained for node:  graphEdgeCollection.getAdjacentEdges");

        return _nodeAdjacentEdges->get(node)->getKeys();
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    simpleList<TEdge> graphEdgeCollection<TNode, TEdge>::getEdges() const
    {
        return _edges->getValues();
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    simpleList<TNode> graphEdgeCollection<TNode, TEdge>::getNodes() const
    {
        return _nodes->getKeys();
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    void graphEdgeCollection<TNode, TEdge>::clear()
    {
        _nodes->clear();
        _edges->clear();

        // Have to delete what was new'd
        _nodeAdjacentEdges->forEach([](TNode node, simpleHash<TEdge, TEdge>* adjacentEdges)
        {
            delete adjacentEdges;
        });

        _nodeAdjacentEdges->clear();
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    void graphEdgeCollection<TNode, TEdge>::clearEdges()
    {
        _edges->clear();

        // Have to delete what was new'd
        _nodeAdjacentEdges->forEach([](TNode node, simpleHash<TEdge, TEdge>* adjacentEdges)
        {
            delete adjacentEdges;
        });

        _nodeAdjacentEdges->clear();
    }

    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
    void graphEdgeCollection<TNode, TEdge>::clearNodes()
    {
        _nodes->clear();

        // Have to delete what was new'd
        _nodeAdjacentEdges->forEach([](TNode* node, simpleHash<TEdge, TEdge>* adjacentEdges)
        {
            delete adjacentEdges;
        });

        _nodeAdjacentEdges->clear();
    }
}