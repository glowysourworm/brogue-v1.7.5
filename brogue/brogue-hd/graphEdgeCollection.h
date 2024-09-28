#pragma once

#include "graphDefinitions.h"
#include "graphNode.h"
#include "graphEdge.h"
#include "simpleList.h"
#include "simpleHash.h"

using namespace brogueHd::component;

namespace brogueHd::backend::math
{
	/// <summary>
	/// Defines an edge collection for a graph. Type definitions are similar to the graph.
	/// </summary>
    template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	class graphEdgeCollection
	{
	public:

        graphEdgeCollection(TEdge* edges);
		graphEdgeCollection(TEdge* edges, TNode* nodes);
		~graphEdgeCollection();

        short edgeCount() const;
        short nodeCount() const;

        void addEdge(const TEdge& edge);
        void addNode(const TNode& node);

        void removeEdge(const TEdge& edge);

        bool containsNode(const TNode& node);
        bool containsEdge(const TEdge& edge);
        bool containsEdge(const TNode& node1, const TNode& node2);
        TEdge findEdge(const TNode& node1, const TNode& node2);

        simpleList<TEdge> getAdjacentEdges(const TNode& node);
        simpleList<TEdge> getEdges() const;
        simpleList<TNode> getNodes() const;

        void clear();
        void clearEdges();
        void clearNodes();

    private:

        void initialize(TEdge* edges, TNode* nodes);

    private:

        simpleHash<TNode, TNode>* _nodes;
        simpleHash<TEdge, TEdge>* _edges;
        simpleHash<TNode, simpleHash<TEdge, TEdge>*>* _nodeAdjacentEdges;
	};
}