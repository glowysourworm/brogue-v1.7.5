#pragma once

#include "graphEdgeCollection.h"
#include "extensionDefinitions.h"
#include "vectorExtension.h"
#include "mapExtension.h"
#include "exceptionHandler.h"

using namespace brogueHd::backend::extension;

namespace brogueHd::backend::math
{
    template<GraphEdgeCollectionTemplate>
    graphEdgeCollection<TWeight, TValue, TNode, TEdge>::graphEdgeCollection(const TEdge* edges[])
    {
        _nodes = new std::map<TNode*, TNode*>();
        _edges = new std::map<TNode*, TNode*>();
        _nodeAdjacentEdges = new std::map<TNode*, std::map<TEdge*, TEdge*>>();

        initialize(edges, TNode*[0]);
    }

    template<GraphEdgeCollectionTemplate>
    graphEdgeCollection<TWeight, TValue, TNode, TEdge>::graphEdgeCollection(const TEdge* edges[], const TNode* nodes[])
    {
        _nodes = new std::map<TNode*, TNode*>();
        _edges = new std::map<TNode*, TNode*>();
        _nodeAdjacentEdges = new std::map<TNode*, std::map<TEdge*, TEdge*>>();

        initialize(edges, nodes);
    }

    template<GraphEdgeCollectionTemplate>
    graphEdgeCollection<TWeight, TValue, TNode, TEdge>::~graphEdgeCollection()
    {
        delete _nodes;
        delete _edges;
        delete _nodeAdjacentEdges;
    }

    template<GraphEdgeCollectionTemplate>
    void graphEdgeCollection<TWeight, TValue, TNode, TEdge>::initialize(TEdge* edges[], TNode* nodes[])
    {
        // Initialize for edges and adjacent vertex lookup
        mapExtension::forEach(edges, [](TEdge* edge)
        {
            this->addEdge(edge);
        });

        // Look for any nodes not yet in the collections - add these
        mapExtension::forEach(nodes, [](TNode* node)
        {
            if (!this->hasNode(node))
                this->addNode(node);
        }
    }

    template<GraphEdgeCollectionTemplate>
    long graphEdgeCollection<TWeight, TValue, TNode, TEdge>::createDirectionalHashCode(TNode* node1, TNode* node2)
    {

    }

    template<GraphEdgeCollectionTemplate>
    long graphEdgeCollection<TWeight, TValue, TNode, TEdge>::createDirectionalHashCode(long oldHashCode1, long oldHashCode2)
    {

    }


    template<GraphEdgeCollectionTemplate>
    short graphEdgeCollection<TWeight, TValue, TNode, TEdge>::edgeCount() const
    {
        return _edges->count();
    }

    template<GraphEdgeCollectionTemplate>
    short graphEdgeCollection<TWeight, TValue, TNode, TEdge>::nodeCount() const
    {
        return _nodes->count();
    }

    template<GraphEdgeCollectionTemplate>
    void graphEdgeCollection<TWeight, TValue, TNode, TEdge>::addEdge(const TEdge* edge)
    {
        if (edge->node1 == edge->node2)
            brogueException::show("Trying to add self-referencing edge to a graph:  graphEdgeCollection.addEdge");

        if (_edges->contains(edge))
            brogueException::show("Trying to insert duplicate edge key:  graphEdgeCollection.addEdge");

        _edges->insert(edge, edge);

        // Nodes
        if (!_nodes->contains(edge->node1))
            _nodes->insert(edge->node1, edge->node1);

        if (!_nodes->contains(edge->node2))
            _nodes.Add(edge->node2, edge->node2);

        // Forward entry
        if (_nodeAdjacentEdges->contains(edge->node1))
            _nodeAdjacentEdges[edge->node1]->insert(edge, edge);

        else
        {
            std::map<TEdge*, TEdge*> adjacentEdges;

            adjacentEdges.insert(edge, edge);
            
            _nodeAdjacentEdges->insert(edge->node1, adjacentEdges);
        }

        // Reverse entry
        if (_nodeAdjacentEdges->contains(edge->node2))
            _nodeAdjacentEdges[edge->node2]->insert(edge, edge);

        else
        {
            std::map<TEdge*, TEdge*> adjacentEdges;

            adjacentEdges.insert(edge, edge);

            _nodeAdjacentEdges->insert(edge->node2, adjacentEdges);
        }
    }

    template<GraphEdgeCollectionTemplate>
    void graphEdgeCollection<TWeight, TValue, TNode, TEdge>::addNode(const TNode* node)
    {
        if (!_nodes->contains(node))
            _nodes->insert(node, node);

        // Adjacent edges map
        if (!_nodeAdjacentEdges->contains(node.GetHashCode()))
            _nodeAdjacentEdges->insert(node, new std::map<TEdge*, TEdge*>());
    }

    template<GraphEdgeCollectionTemplate>
    void graphEdgeCollection<TWeight, TValue, TNode, TEdge>::removeEdge(const TEdge* edge)
    {
        _edges->remove(edge);

        if (_nodes->contains(edge->node1))
            _nodes->remove(edge->node1);

        if (_nodes->contains(edge->node2))
            _nodes->remove(edge->node2);

        // Forward entry
        if (_nodeAdjacentEdges.ContainsKey(edge.Node.GetHashCode()))
        {
            if (_nodeAdjacentEdges[edge->node1]->contains(edge))
                _nodeAdjacentEdges[edge->node1]->remove(edge);
        }

        // Reverse entry
        if (_nodeAdjacentEdges.ContainsKey(edge.AdjacentNode.GetHashCode()))
        {
            if (_nodeAdjacentEdges[edge->node2]->contains(edge))
                _nodeAdjacentEdges[edge->node2]->remove(edge);
        }
    }

    template<GraphEdgeCollectionTemplate>
    bool graphEdgeCollection<TWeight, TValue, TNode, TEdge>::containsNode(const TNode* node)
    {
        return _nodes->contains(node);
    }

    template<GraphEdgeCollectionTemplate>
    bool graphEdgeCollection<TWeight, TValue, TNode, TEdge>::containsEdge(const TEdge* edge)
    {
        return _edges->contains(edge);
    }

    template<GraphEdgeCollectionTemplate>
    bool graphEdgeCollection<TWeight, TValue, TNode, TEdge>::containsEdge(const TNode* node1, const TNode* node2)
    {
        return findEdge(node1, node2) != NULL;
    }

    template<GraphEdgeCollectionTemplate>
    TEdge& graphEdgeCollection<TWeight, TValue, TNode, TEdge>::findEdge(const TNode* node1, const TNode* node2)
    {
        return &mapExtension::firstKey(_edges, [&node1, &node2](TEdge* key, TEdge* value)
        {
            if (key->node1 == node1 && key->node2 == node2)
            {
                return key;
            }
            else if (key->node2 == node1 && key->node1 == node2)
            {
                return key;
            }
        });
    }

    template<GraphEdgeCollectionTemplate>
    std::vector<TEdge*> graphEdgeCollection<TWeight, TValue, TNode, TEdge>::getAdjacentEdges(const TNode* node)
    {
        if (!_nodeAdjacentEdges->contains(node))
            brogueException::show("No adjacent edges contained for node:  graphEdgeCollection.getAdjacentEdges");

        return mapExtension::getKeys(_nodeAdjacentEdges[node]);
    }

    template<GraphEdgeCollectionTemplate>
    std::vector<TEdge*> graphEdgeCollection<TWeight, TValue, TNode, TEdge>::getEdges() const
    {
        return mapExtension::getKeys(_edges);
    }

    template<GraphEdgeCollectionTemplate>
    std::vector<TNode*> graphEdgeCollection<TWeight, TValue, TNode, TEdge>::getNodes() const
    {
        return mapExtension::getKeys(_nodes);
    }

    template<GraphEdgeCollectionTemplate>
    void graphEdgeCollection<TWeight, TValue, TNode, TEdge>::clear()
    {
        _nodes->clear();
        _edges->clear();

        // Have to delete what was new'd
        mapExtension::forEach(_nodeAdjacentEdges, [](TNode* node, std::map<TEdge*, TEdge*>* adjacentEdges)
        {
            delete adjacentEdges;
        });

        _nodeAdjacentEdges->clear();
    }

    template<GraphEdgeCollectionTemplate>
    void graphEdgeCollection<TWeight, TValue, TNode, TEdge>::clearEdges()
    {
        _edges->clear();

        // Have to delete what was new'd
        mapExtension::forEach(_nodeAdjacentEdges, [](TNode* node, std::map<TEdge*, TEdge*>* adjacentEdges)
        {
            delete adjacentEdges;
        });

        _nodeAdjacentEdges->clear();
    }

    template<GraphEdgeCollectionTemplate>
    void graphEdgeCollection<TWeight, TValue, TNode, TEdge>::clearNodes()
    {
        _nodes->clear();

        // Have to delete what was new'd
        mapExtension::forEach(_nodeAdjacentEdges, [](TNode* node, std::map<TEdge*, TEdge*>* adjacentEdges)
        {
            delete adjacentEdges;
        });

        _nodeAdjacentEdges->clear();
    }
}