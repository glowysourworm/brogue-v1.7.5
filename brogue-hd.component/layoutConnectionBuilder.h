#pragma once

#include <dijkstrasAlgorithm.h>
#include <simpleGraph.h>
#include <simpleHash.h>
#include <simpleList.h>

#include "gridLocator.h"
#include "gridRegionGraphEdge.h"
#include "gridRegionGraphNode.h"
#include "layoutConnectionData.h"
#include "layoutPartialConnectionData.h"
#include <simple.h>
#include <simpleArray.h>

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
									   const gridRegionGraphNode& interruptingRegion,
									   const gridLocator& interruptingLocation);

		// Tries to complete partial connections using normal connections that
		// have already completed.
		void reconcilePartials(simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>* roomGraph);

		void iteratePartials(simpleListCallback<layoutPartialConnectionData*> callback);

		void iterateNormals(simpleListCallback<layoutConnectionData*> callback);

	private:

		simpleHash<layoutConnectionData*, layoutConnectionData*>* _connections;
		simpleHash<layoutPartialConnectionData*, layoutPartialConnectionData*>* _partialConnections;
	};

	layoutConnectionBuilder::layoutConnectionBuilder()
	{
		_connections = new simpleHash<layoutConnectionData*, layoutConnectionData*>();
		_partialConnections = new simpleHash<layoutPartialConnectionData*, layoutPartialConnectionData*>();
	}
	layoutConnectionBuilder::~layoutConnectionBuilder()
	{
		delete _connections;
		delete _partialConnections;
	}

	bool layoutConnectionBuilder::hasPendingWork() const
	{
		return hasPendingNormal() || hasPendingPartial();
	}
	bool layoutConnectionBuilder::hasPendingPartial() const
	{
		return _partialConnections->any([] (layoutPartialConnectionData* key, layoutPartialConnectionData* value)
		{
			return !key->isComplete() && !key->isFailed();
		});
	}
	bool layoutConnectionBuilder::hasPendingNormal() const
	{
		return _connections->any([] (layoutConnectionData* key, layoutConnectionData* value)
		{
			return !key->isComplete() && !key->isFailed();
		});
	}

	bool layoutConnectionBuilder::hasCompletedPartial() const
	{
		return _partialConnections->any([] (layoutPartialConnectionData* key, layoutPartialConnectionData* value)
		{
			return key->isComplete();
		});
	}
	bool layoutConnectionBuilder::hasCompletedUnreconciledPartials() const
	{
		return _partialConnections->any([] (layoutPartialConnectionData* key, layoutPartialConnectionData* value)
		{
			return key->isComplete() && !key->getReconciled();
		});
	}
	void layoutConnectionBuilder::addConnection(layoutConnectionData* connection)
	{
		_connections->add(connection, connection);
	}
	void layoutConnectionBuilder::addConnection(layoutPartialConnectionData* connection)
	{
		_partialConnections->add(connection, connection);
	}
	void layoutConnectionBuilder::changeConnectionToPartial(layoutConnectionData* connection,
															const gridRegionGraphNode& interruptingRegion,
															const gridLocator& interruptingLocation)
	{
		_connections->remove(connection);

		layoutPartialConnectionData* partialConnection = new layoutPartialConnectionData(connection, interruptingRegion, interruptingLocation);

		_partialConnections->add(partialConnection, partialConnection);
	}
	layoutPartialConnectionData* layoutConnectionBuilder::getNextPartial() const
	{
		return _partialConnections->firstKey([] (layoutConnectionData* key, layoutConnectionData* value)
		{
			return !key->isComplete() && !key->isFailed();
		});
	}
	layoutConnectionData* layoutConnectionBuilder::getNextNormal() const
	{
		return _connections->firstKey([] (layoutConnectionData* key, layoutConnectionData* value)
		{
			return !key->isComplete() && !key->isFailed();
		});
	}

	void layoutConnectionBuilder::reconcilePartials(simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>* roomGraph)
	{
		// NOTE*** This should be run at the end of the build process

		// Normal, complete connections
		simpleList<layoutConnectionData*> normalConnections = _connections->getKeys().where([] (layoutConnectionData* connection)
		{
			return connection->isComplete();
		});

		if (normalConnections.count() > 0)
		{
			// Make sure each partial has been routed
			this->iteratePartials([&normalConnections, &roomGraph] (layoutPartialConnectionData* partial)
			{
				if (!partial->isComplete())
					return iterationCallback::iterate;

				if (partial->getReconciled())
					return iterationCallback::iterate;

				// Run Dijkstra's Algorithm (graph version) on the room graph (of largest-sub-rectangle centers)
				dijkstrasAlgorithm<gridRegionGraphNode, gridRegionGraphEdge> algorithm(roomGraph);

				// Source = interrupting region node (center), Destination = original connection destination
				algorithm.initialize(partial->getInterruptingRegion(), partial->getNode2());

				// Run Dijkstra's Algorithm (small graph)
				simpleArray<gridRegionGraphNode> route = algorithm.run();

				if (route == default_value::value<simpleArray<gridRegionGraphNode>>())
					return iterationCallback::iterate;

				// Validate the route with completed connections
				for (int index = 0; index < route.count(); index++)
				{
					gridRegionGraphNode currentNode = route.get(index);

					// Find connection that satisfies the route
					bool valid = normalConnections.any([&currentNode] (layoutConnectionData* connection)
					{
						return connection->getNode1() == currentNode || connection->getNode2() == currentNode;
					});

					// NO ROUTE FOUND! (Must wait until other connections have finished)
					if (!valid)
						return iterationCallback::iterate;
				}

				// Valid Connection! Mark it as 'reconciled'.
				//
				partial->setReconciled(true);

				return iterationCallback::iterate;
			});
		}
	}

	void layoutConnectionBuilder::iteratePartials(simpleListCallback<layoutPartialConnectionData*> callback)
	{
		return _partialConnections->iterate([&callback] (layoutPartialConnectionData* key, layoutPartialConnectionData* value)
		{
			return callback(key);
		});
	}

	void layoutConnectionBuilder::iterateNormals(simpleListCallback<layoutConnectionData*> callback)
	{
		return _connections->iterate([&callback] (layoutConnectionData* key, layoutConnectionData* value)
		{
			return callback(key);
		});
	}
}