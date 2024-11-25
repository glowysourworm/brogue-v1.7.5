#pragma once

#include "graphDefinitions.h"
#include "graphEdgeCollection.h"
#include "simple.h"
#include "simpleArray.h"
#include "simpleList.h"
#include <functional>

namespace brogueHd::component
{
	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	using graphSimpleNodeIterator = std::function<iterationCallback(TNode node)>;

	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	using graphSimpleEdgeIterator = std::function<iterationCallback(TEdge edge)>;

	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	using graphIterator = std::function<iterationCallback(TNode node, const simpleList<TEdge>& adjacentEdges)>;

	/// <summary>
	/// Defines a graph with type constraints for graphNode, and graphEdge
	/// </summary>
	/// <typeparam name="TValue">Type for the node value</typeparam>
	/// <typeparam name="TWeight">Type for the node weight</typeparam>
	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	class graph
	{
	public:

		graph();
		graph(const simpleArray<TNode>& nodes, const simpleArray<TEdge>& edges);
		graph(const simpleArray<TNode>& nodes);
		graph(const graph<TNode, TEdge>& copy);
		~graph();

		void operator=(const graph<TNode, TEdge>& other);

		void addEdge(TEdge edge);
		void modify(TEdge existingEdge, TEdge newEdge);
		simpleList<TEdge> getAdjacentEdges(TNode node) const;
		TEdge findEdge(TNode node1, TNode node2) const;

		void iterate(graphIterator<TNode, TEdge> callback) const;
		void iterateNodes(graphSimpleNodeIterator<TNode, TEdge> callback) const;
		void iterateEdges(graphSimpleEdgeIterator<TNode, TEdge> callback) const;

	private:

		simpleList<TNode> getNodes() const
		{
			return *_nodes;
		}
		graphEdgeCollection<TNode, TEdge> getEdges() const
		{
			return *_edgeCollection;
		}

	private:

		simpleList<TNode>* _nodes;

		graphEdgeCollection<TNode, TEdge>* _edgeCollection;
	};

	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	graph<TNode, TEdge>::graph()
	{
		_nodes = new simpleList<TNode>();
		_edgeCollection = new graphEdgeCollection<TNode, TEdge>();
	}

	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	graph<TNode, TEdge>::graph(const simpleArray<TNode>& nodes, const simpleArray<TEdge>& edges)
	{
		_nodes = new simpleList<TNode>(nodes);
		_edgeCollection = new graphEdgeCollection<TNode, TEdge>(nodes, edges);
	}

	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	graph<TNode, TEdge>::graph(const simpleArray<TNode>& nodes)
	{
		_nodes = new simpleList<TNode>(nodes);
		_edgeCollection = new graphEdgeCollection<TNode, TEdge>(nodes, simpleArray<TEdge>());
	}

	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	graph<TNode, TEdge>::graph(const graph<TNode, TEdge>& copy)
	{
		_nodes = new simpleList<TNode>(copy.getNodes());
		_edgeCollection = new graphEdgeCollection(copy.getNodes(), copy.getEdges());
	}

	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	graph<TNode, TEdge>::~graph()
	{
		delete _nodes;
		delete _edgeCollection;
	}

	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	void graph<TNode, TEdge>::operator=(const graph<TNode, TEdge>& other)
	{
		delete _nodes;
		delete _edgeCollection;

		_nodes = new simpleList<TNode>(other.getNodes());
		_edgeCollection = new graphEdgeCollection(other.getNodes(), other.getEdges());
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
	simpleList<TEdge> graph<TNode, TEdge>::getAdjacentEdges(TNode node) const
	{
		return _edgeCollection->getAdjacentEdges(node);
	}

	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	TEdge graph<TNode, TEdge>::findEdge(TNode node1, TNode node2) const
	{
		return _edgeCollection->findEdge(node1, node2);
	}

	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	void graph<TNode, TEdge>::iterate(graphIterator<TNode, TEdge> callback) const
	{
		bool userBreak = false;

		for (int index = 0; index < _nodes->count() && !userBreak; index++)
		{
			TNode node = _nodes->get(index);

			simpleList<TEdge> adjacentEdges = this->getAdjacentEdges(node);

			userBreak |= (callback(node, adjacentEdges) == iterationCallback::breakAndReturn);
		}
	}

	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	void graph<TNode, TEdge>::iterateNodes(graphSimpleNodeIterator<TNode, TEdge> callback) const
	{
		_nodes->forEach(callback);
	}

	template<graphNodeType TNode, graphEdgeType<TNode> TEdge>
	void graph<TNode, TEdge>::iterateEdges(graphSimpleEdgeIterator<TEdge, TEdge> callback) const
	{
		_edgeCollection->getEdges().forEach(callback);
	}
}