#pragma once

#include <simpleList.h>
#include <simpleHash.h>
#include <simpleGraph.h>
#include <dijkstrasAlgorithm.h>

#include "gridLocator.h"
#include "gridRegionGraphNode.h"
#include "gridRegionGraphEdge.h"
#include "layoutDesignRect.h"
#include "layoutConnectionData.h"
#include "layoutPartialConnectionData.h"

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;

	class layoutConnectionBuilder
	{
	public:

		layoutConnectionBuilder();
		~layoutConnectionBuilder();

		bool hasPendingWork() const;
		bool hasPendingPartial() const;
		bool hasPendingNormal() const;
		bool hasCompletedPartial() const;
		bool hasCompletedUnreconciledPartials() const;

		layoutPartialConnectionData* getNextPartial() const;
		layoutConnectionData* getNextNormal() const;

		void addConnection(layoutConnectionData* connection);
		void addConnection(layoutPartialConnectionData* connection);

		void changeConnectionToPartial(layoutConnectionData* connection, 
									   layoutDesignRect* interruptingRegion, 
									   const gridLocator& interruptingLocation);

		// Tries to complete partial connections using normal connections that
		// have already completed.
		void reconcilePartials(simpleGraph<gridRegionGraphNode<gridLocator>, gridRegionGraphEdge<gridLocator>>* roomGraph);

	private:

		simpleHash<layoutConnectionData*, layoutConnectionData*>* _connections;
	};

	layoutConnectionBuilder::layoutConnectionBuilder()
	{
		_connections = new simpleHash<layoutConnectionData*, layoutConnectionData*>();
	}
	layoutConnectionBuilder::~layoutConnectionBuilder()
	{
		delete _connections;
	}

	bool layoutConnectionBuilder::hasPendingWork() const
	{
		return hasPendingNormal() || hasPendingPartial();
	}
	bool layoutConnectionBuilder::hasPendingPartial() const
	{
		return _connections->any([] (layoutConnectionData* key, layoutConnectionData* value)
		{
			return (typeid(key) == typeid(layoutPartialConnectionData)) && !key->isComplete();
		});
	}
	bool layoutConnectionBuilder::hasPendingNormal() const
	{
		return _connections->any([] (layoutConnectionData* key, layoutConnectionData* value)
		{
			return typeid(key) == typeid(layoutConnectionData) && !key->isComplete();
		});
	}

	bool layoutConnectionBuilder::hasCompletedPartial() const
	{
		return _connections->any([] (layoutConnectionData* key, layoutConnectionData* value)
		{
			return typeid(key) == typeid(layoutPartialConnectionData) && key->isComplete();
		});
	}
	bool layoutConnectionBuilder::hasCompletedUnreconciledPartials() const
	{
		return _connections->any([] (layoutConnectionData* key, layoutConnectionData* value)
		{
			layoutPartialConnectionData* partial = (layoutPartialConnectionData*)key;

			return (typeid(key) == typeid(layoutPartialConnectionData)) && key->isComplete() && !partial->getReconciled();
		});
	}
	void layoutConnectionBuilder::addConnection(layoutConnectionData* connection)
	{
		_connections->add(connection, connection);
	}
	void layoutConnectionBuilder::addConnection(layoutPartialConnectionData* connection)
	{
		_connections->add(connection, connection);
	}
	void layoutConnectionBuilder::changeConnectionToPartial(layoutConnectionData* connection,
														    layoutDesignRect* interruptingRegion,
														    const gridLocator& interruptingLocation)
	{
		_connections->remove(connection);

		layoutPartialConnectionData* partialConnection = new layoutPartialConnectionData(connection, interruptingRegion, interruptingLocation);

		_connections->add(partialConnection, partialConnection);
	}
	layoutPartialConnectionData* layoutConnectionBuilder::getNextPartial() const
	{
		return (layoutPartialConnectionData*)_connections->firstKey([] (layoutConnectionData* key, layoutConnectionData* value)
		{
			return typeid(key) == typeid(layoutPartialConnectionData) && !key->isComplete();
		});
	}
	layoutConnectionData* layoutConnectionBuilder::getNextNormal() const
	{
		return _connections->firstKey([] (layoutConnectionData* key, layoutConnectionData* value)
		{
			return typeid(key) == typeid(layoutConnectionData) && !key->isComplete();
		});
	}

	void layoutConnectionBuilder::reconcilePartials(simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>* roomGraph)
	{
		// NOTE*** This should be run at the end of the build process

		// Normal, complete connections
		simpleList<layoutConnectionData*> normalConnections = _connections->getKeys().where([] (layoutConnectionData* connection)
		{
			return typeid(connection) == typeid(layoutConnectionData) && connection.isComplete();
		});

		if (normalConnections.count() > 0)
		{
			// Make sure each partial has been routed
			this->iteratePartials([&normalConnections, &roomGraph, &partialsReconciled] (layoutPartialConnectionData* partial)
			{
				if (!partial->isComplete())
					return iterationCallback::iterate;

				if (partial->getReconciled())
					return iterationCallback::iterate;

				// Run Dijkstra's Algorithm (graph version) on the room graph (of largest-sub-rectangle centers)
				dijkstrasAlgorithm algorithm(roomGraph);

				// Source = interrupting region node (center), Destination = original connection destination
				algorithm.initialize(partial->getInterruptingRegion()->getRegion()->getLargestSubRectangle().center(), partial->getNode2()->getNode());

				// Run Dijkstra's Algorithm (small graph)
				simpleArray<gridLocator> route = algorithm.run();

				if (route == default_value::value<simpleArray<gridLocator>>())
					return iterationCallback::iterate;

				// Validate the route with completed connections
				for (int index = 0; index < route.count() ;index++)
				{
					gridLocator currentNode = route.get(index);

					// Find connection that satisfies the route
					bool valid = normalConnections->any([&currentNode] (layoutConnectionData* connection)
					{
						if (connection->getNode1()->getNode() == currentNode)
							return true;

						else if (connection->getNode2()->getNode() == currentNode)
							return true;

						return false;
					});

					// NO ROUTE FOUND! (Must wait until other connections have finished)
					if (!valid)
						return iterationCallback::iterate;
				}

				// Valid Connection! Mark it as 'reconciled'.
				//
				partial->setReconciled(true);
			});
		}
	}

	void layoutConnectionBuilder::iteratePartials(simpleListCallback<layoutPartialConnectionData*> callback)
	{
		return _connections->iterate([&callback] (layoutConnectionData* key, layoutConnectionData* value)
		{
			if (typeid(key) == typeid(layoutConnectionData) && !key->isComplete())
				return callback((layoutPartialConnectionData*)key);

			return iterationCallback::iterate;
		});
	}
}