#pragma once

#include "graphDefinitions.h"
#include "simple.h"
#include "simpleArray.h"
#include "simpleException.h"
#include "simpleHash.h"
#include "simpleList.h"

namespace brogueHd::component
{
	/// <summary>
	/// Defines an edge collection for a graph. Type definitions are similar to the graph.
	/// </summary>
	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	class graphEdgeCollection
	{
	public:

		graphEdgeCollection();
		graphEdgeCollection(const simpleArray<TEdge>& edges);
		graphEdgeCollection(const simpleArray<TNode>& nodes, const simpleArray<TEdge>& edges);
		graphEdgeCollection(const graphEdgeCollection& copy);
		~graphEdgeCollection();

		int edgeCount() const;
		int nodeCount() const;

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

		void initialize(const simpleArray<TNode>& nodes, const simpleArray<TEdge>& edges);

	private:

		simpleHash<TNode, TNode>* _nodes;
		simpleHash<TEdge, TEdge>* _edges;
		simpleHash<TNode, simpleHash<TEdge, TEdge>*>* _nodeAdjacentEdges;
	};

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	graphEdgeCollection<TNode, TEdge>::graphEdgeCollection()
	{
		_nodes = new simpleHash<TNode, TNode>();
		_edges = new simpleHash<TEdge, TEdge>();
		_nodeAdjacentEdges = new simpleHash<TNode, simpleHash<TEdge, TEdge>*>();
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	graphEdgeCollection<TNode, TEdge>::graphEdgeCollection(const simpleArray<TEdge>& edges)
	{
		_nodes = new simpleHash<TNode, TNode>();
		_edges = new simpleHash<TEdge, TEdge>();
		_nodeAdjacentEdges = new simpleHash<TNode*, simpleHash<TEdge, TEdge>*>();

		initialize(simpleArray<TNode>(), edges);
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	graphEdgeCollection<TNode, TEdge>::graphEdgeCollection(const simpleArray<TNode>& nodes, const simpleArray<TEdge>& edges)
	{
		_nodes = new simpleHash<TNode, TNode>();
		_edges = new simpleHash<TEdge, TEdge>();
		_nodeAdjacentEdges = new simpleHash<TNode, simpleHash<TEdge, TEdge>*>();

		initialize(nodes, edges);
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	graphEdgeCollection<TNode, TEdge>::graphEdgeCollection(const graphEdgeCollection& copy)
	{
		// Use copy contrustors! Keep the two node / edge hash table copies; and construct our own nodeAdjacentEdges.
		_nodes = new simpleHash<TNode, TNode>();
		_edges = new simpleHash<TEdge, TEdge>();
		_nodeAdjacentEdges = new simpleHash<TNode, simpleHash<TEdge, TEdge>*>();

		initialize(copy.getNodes(), copy.getEdges());
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	graphEdgeCollection<TNode, TEdge>::~graphEdgeCollection()
	{
		this->clear(); // ~simpleHash<...>  (our adjacent edges hash tables)

		delete _nodes;
		delete _edges;
		delete _nodeAdjacentEdges;
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	void graphEdgeCollection<TNode, TEdge>::initialize(const simpleArray<TNode>& nodes, const simpleArray<TEdge>& edges)
	{
		// Initialize for edges and adjacent vertex lookup
		for (int index = 0; index < edges.count(); index++)
		{
			this->addEdge(edges.get(index));
		}

		// Look for any nodes not yet in the collections - add these
		for (int index = 0; index < nodes.count(); index++)
		{
			if (!this->containsNode(nodes.get(index)))
				this->addNode(nodes.get(index));
		}
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	int graphEdgeCollection<TNode, TEdge>::edgeCount() const
	{
		return _edges->count();
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	int graphEdgeCollection<TNode, TEdge>::nodeCount() const
	{
		return _nodes->count();
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	void graphEdgeCollection<TNode, TEdge>::addEdge(const TEdge& edge)
	{
		if (edge.node1 == edge.node2)
			throw simpleException("Trying to add self-referencing edge to a graph:  graphEdgeCollection.addEdge");

		if (_edges->contains(edge))
			throw simpleException("Trying to insert duplicate edge key:  graphEdgeCollection.addEdge");

		_edges->add(edge, edge);

		// Nodes
		if (!_nodes->contains(edge.node1))
			_nodes->add(edge.node1, edge.node1);

		if (!_nodes->contains(edge.node2))
			_nodes->add(edge.node2, edge.node2);

		// Forward entry
		if (_nodeAdjacentEdges->contains(edge.node1))
			_nodeAdjacentEdges->get(edge.node1)->add(edge, edge);

		else
		{
			simpleHash<TEdge, TEdge>* adjacentEdges = new simpleHash<TEdge, TEdge>();

			adjacentEdges->add(edge, edge);

			_nodeAdjacentEdges->add(edge.node1, adjacentEdges);
		}

		// Reverse entry
		if (_nodeAdjacentEdges->contains(edge.node2))
			_nodeAdjacentEdges->get(edge.node2)->add(edge, edge);

		else
		{
			simpleHash<TEdge, TEdge>* adjacentEdges = new simpleHash<TEdge, TEdge>();

			adjacentEdges->add(edge, edge);

			_nodeAdjacentEdges->add(edge.node2, adjacentEdges);
		}
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	void graphEdgeCollection<TNode, TEdge>::addNode(const TNode& node)
	{
		if (!_nodes->contains(node))
			_nodes->add(node, node);

		// Adjacent edges map
		if (!_nodeAdjacentEdges->contains(node))
			_nodeAdjacentEdges->add(node, new simpleHash<TEdge, TEdge>());
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
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

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	bool graphEdgeCollection<TNode, TEdge>::containsNode(const TNode& node)
	{
		return _nodes->contains(node);
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	bool graphEdgeCollection<TNode, TEdge>::containsEdge(const TEdge& edge)
	{
		return _edges->contains(edge);
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	bool graphEdgeCollection<TNode, TEdge>::containsEdge(const TNode& node1, const TNode& node2)
	{
		return findEdge(node1, node2) != default_value::value<TEdge>();
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	TEdge graphEdgeCollection<TNode, TEdge>::findEdge(const TNode& node1, const TNode& node2)
	{
		return _edges->firstOrDefaultKey([&node1, &node2] (TEdge key, TEdge value)
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

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	simpleList<TEdge> graphEdgeCollection<TNode, TEdge>::getAdjacentEdges(const TNode& node)
	{
		if (!_nodeAdjacentEdges->contains(node))
			throw simpleException("No adjacent edges contained for node:  graphEdgeCollection.getAdjacentEdges");

		return _nodeAdjacentEdges->get(node)->getKeys();
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	simpleList<TEdge> graphEdgeCollection<TNode, TEdge>::getEdges() const
	{
		return _edges->getValues();
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	simpleList<TNode> graphEdgeCollection<TNode, TEdge>::getNodes() const
	{
		return _nodes->getKeys();
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	void graphEdgeCollection<TNode, TEdge>::clear()
	{
		_nodes->clear();
		_edges->clear();

		// Have to delete what was new'd
		_nodeAdjacentEdges->forEach([] (TNode node, simpleHash<TEdge, TEdge>* adjacentEdges)
		{
			delete adjacentEdges;

			return iterationCallback::iterate;
		});

		_nodeAdjacentEdges->clear();
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	void graphEdgeCollection<TNode, TEdge>::clearEdges()
	{
		_edges->clear();

		// Have to delete what was new'd
		_nodeAdjacentEdges->forEach([] (TNode node, simpleHash<TEdge, TEdge>* adjacentEdges)
		{
			delete adjacentEdges;

			return iterationCallback::iterate;
		});

		_nodeAdjacentEdges->clear();
	}

	template<isGraphNode TNode, isGraphEdge<TNode> TEdge>
	void graphEdgeCollection<TNode, TEdge>::clearNodes()
	{
		_nodes->clear();

		// Have to delete what was new'd
		_nodeAdjacentEdges->forEach([] (TNode* node, simpleHash<TEdge, TEdge>* adjacentEdges)
		{
			delete adjacentEdges;

			return iterationCallback::iterate;
		});

		_nodeAdjacentEdges->clear();
	}
}