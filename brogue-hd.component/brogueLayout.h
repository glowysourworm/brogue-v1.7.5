#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridLayer.h"
#include "gridConnectionLayer.h"
#include "gridRegion.h"
#include "brogueRoom.h"

#include <brogueCell.h>
#include <brogueCellDisplay.h>
#include <dungeonConstants.h>

#include <simple.h>
#include <simpleList.h>
#include <simpleException.h>
#include <simpleGraph.h>
#include <simpleRect.h>
#include <functional>

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;
	using namespace brogueHd::model;

	class brogueLayout
	{
	public:
		brogueLayout(grid<brogueCell*>* mainGrid, 
					 gridConnectionLayer* connectionLayer, 
					 const simpleList<gridConnectionEdge>& roomNearestNeighbors, 
					 const simpleList<gridConnectionEdge>& roomGraph);
		~brogueLayout();

		bool isDefined(int column, int row) const;

		brogueCell* get(int column, int row) const;

		gridRect getBoundary() const;
		gridRect getParentBoundary() const;

		/// <summary>
		/// Iterates adjacent cells to satisfy the user predicate. Returns the result cell, (or null), and 
		/// will have updated the column and row, (or -1's).
		/// </summary>
		brogueCell* firstAdjacent(int column, int row, gridPredicate<brogueCell*> predicate);

		/// <summary>
		/// Iterates adjacent cells and calls the user callback
		/// </summary>
		void iterateAdjacentCells(int column, int row, gridCallbackConst<brogueCell*> callback);

		/// <summary>
		/// Iterates cells and calls the user callback
		/// </summary>
		void iterate(gridCallbackConst<brogueCell*> callback) const;

		/// <summary>
		/// Iterates cells where there is layout defined
		/// </summary>
		void iterateWhereDefined(gridCallbackConst<brogueCell*> callback) const;

		/// <summary>
		/// Gets sub-rectangle from the layout - the largest possible
		/// </summary>
		gridRect getLargestUnusedRectangle(const gridRect& minSize);

		/// <summary>
		/// Iterates the room "centroid" graph to be able to visulize it for debugging
		/// </summary>
		void iterateRoomGraph(graphSimpleEdgeIterator<gridConnectionNode, gridConnectionEdge> callback) const;

		/// <summary>
		/// Iterates all graph edges and callsback to the user code
		/// </summary>
		void iterateRoomConnections(graphSimpleEdgeIterator<gridConnectionNode, gridConnectionEdge> callback) const;

		/// <summary>
		/// Queries the connection layer (which run's a very small dijkstra iteration on the graph of
		/// connection points) for the next location -> in the directin of travel. Use this before
		/// calling dijkstra's map for the character.
		/// </summary>
		gridLocator getNextConnectionPoint(const gridLocator& source, const gridLocator& destination);

	private:

		// Builds all of the grids from the main grid
		void initialize(grid<brogueCell*>* mainGrid, gridConnectionLayer* connectionLayer);

	private:

		// Main Grid for the identity of the brogueCell*. All other layers may be derived
		// from this grid during serialization.
		grid<brogueCell*>* _mainGrid;

		// Permanent Layers (shared pointers with the main grid)
		gridLayer* _wallLayer;
		gridLayer* _wallIndestructibleLayer;
		gridLayer* _cellLayer;
		gridLayer* _chasmLayer;
		gridLayer* _terrainLayer;						// All Terrain
		gridLayer* _terrainLavaLayer;					// Lava Terrain (only)
		gridLayer* _terrainWaterLayer;					// Water Terrain (only) (permanent)
		gridLayer* _terrainCombustibleLayer;			// Combustible Terrain (only) (permanent)
		gridLayer* _alteredLayer;						// Other movement altering (permanent) cell flags

		// Temporary Layers (shared pointers with the main grid)
		gridLayer* _temporaryChasmLayer;
		gridLayer* _temporaryTerrainLayer;				// All Terrain (temporary only)
		gridLayer* _temporaryWaterTerrainLayer;			// Water Terrain (temporary only)
		gridLayer* _temporaryPoisonVineTerrainLayer;	// Poisonous Vine Terrain (temporary only)
		gridLayer* _flameLayer;							// Flame Terrain (temporary)
		gridLayer* _causticGasLayer;
		gridLayer* _nauseaGasLayer;
		gridLayer* _temporaryAlteredLayer;				// Other movement altering (temporary only) cell flags

		// Connection Layer:  Used for movement / travel (region queries)
		gridConnectionLayer* _connectionLayer;

		// DEBUG DATA
		simpleList<gridConnectionEdge*>* _roomNearestNeighbors;
		simpleList<gridConnectionEdge*>* _roomGraph;

	};

	brogueLayout::brogueLayout(grid<brogueCell*>* mainGrid, 
							   gridConnectionLayer* connectionLayer, 
							   const simpleList<gridConnectionEdge>& roomNearestNeighbors,
							   const simpleList<gridConnectionEdge>& roomGraph)
	{
		// DEBUG DATA
		_roomNearestNeighbors = new simpleList<gridConnectionEdge*>();
		_roomGraph = new simpleList<gridConnectionEdge*>();

		for (int index = 0; index < roomNearestNeighbors.count(); index++)
		{
			_roomNearestNeighbors->add(new gridConnectionEdge(roomNearestNeighbors.get(index)));
		}
		for (int index = 0; index < roomGraph.count(); index++)
		{
			_roomGraph->add(new gridConnectionEdge(roomGraph.get(index)));
		}

		_mainGrid = nullptr;

		// Permanent Layers
		_wallLayer = nullptr;
		_wallIndestructibleLayer = nullptr;
		_cellLayer = nullptr;
		_chasmLayer = nullptr;
		_terrainLayer = nullptr;
		_terrainLavaLayer = nullptr;
		_terrainWaterLayer = nullptr;
		_terrainCombustibleLayer = nullptr;
		_alteredLayer = nullptr;

		// Temporary Layers
		_temporaryChasmLayer = nullptr;
		_temporaryTerrainLayer = nullptr;
		_temporaryWaterTerrainLayer = nullptr;
		_temporaryPoisonVineTerrainLayer = nullptr;
		_flameLayer = nullptr;
		_causticGasLayer = nullptr;
		_nauseaGasLayer = nullptr;
		_temporaryAlteredLayer = nullptr;

		_connectionLayer = nullptr;

		initialize(mainGrid, connectionLayer);
	}

	brogueLayout::~brogueLayout()
	{
		_mainGrid->iterate([](int column, int row, brogueCell* cell)
		{
			delete cell;

			return iterationCallback::iterate;
		});

		delete _mainGrid;

		// Build Process Artifacts* (We're responsible for this memory afterward)
		delete _connectionLayer;
	}

	void brogueLayout::initialize(grid<brogueCell*>* mainGrid, gridConnectionLayer* connectionLayer)
	{
		_mainGrid = mainGrid;

		// Permanent Layers
		_wallLayer = nullptr;
		_wallIndestructibleLayer = nullptr;
		_cellLayer = nullptr;
		_chasmLayer = nullptr;
		_terrainLayer = nullptr;
		_terrainLavaLayer = nullptr;
		_terrainWaterLayer = nullptr;
		_terrainCombustibleLayer = nullptr;
		_alteredLayer = nullptr;

		// Temporary Layers
		_temporaryChasmLayer = nullptr;
		_temporaryTerrainLayer = nullptr;
		_temporaryWaterTerrainLayer = nullptr;
		_temporaryPoisonVineTerrainLayer = nullptr;
		_flameLayer = nullptr;
		_causticGasLayer = nullptr;
		_nauseaGasLayer = nullptr;
		_temporaryAlteredLayer = nullptr;

		_connectionLayer = connectionLayer;
	}

	gridRect brogueLayout::getBoundary() const
	{
		return _mainGrid->getRelativeBoundary();
	}

	gridRect brogueLayout::getParentBoundary() const
	{
		return _mainGrid->getParentBoundary();
	}

	bool brogueLayout::isDefined(int column, int row) const
	{
		return _mainGrid->isDefined(column, row);
	}

	brogueCell* brogueLayout::get(int column, int row) const
	{
		return _mainGrid->get(column, row);
	}

	gridRect brogueLayout::getLargestUnusedRectangle(const gridRect& minSize)
	{
		grid<brogueCell*>* mainGrid = _mainGrid;

		return _mainGrid->calculateLargestRectangle(minSize, [&mainGrid](int column, int row, brogueCell* cell)
		{
			return !mainGrid->isDefined(column, row);
		});
	}

	void brogueLayout::iterateRoomGraph(graphSimpleEdgeIterator<gridConnectionNode, gridConnectionEdge> callback) const
	{
		_roomGraph->forEach([&callback] (gridConnectionEdge* edge)
		{
			return callback(*edge);
		});
	}

	void brogueLayout::iterateRoomConnections(graphSimpleEdgeIterator<gridConnectionNode, gridConnectionEdge> callback) const
	{
		//_connectionLayer->iterateConnections(callback);
		_roomNearestNeighbors->forEach([&callback](gridConnectionEdge* edge)
		{
			return callback(*edge);
		});
	}

	void brogueLayout::iterateAdjacentCells(int column, int row, gridCallbackConst<brogueCell*> callback)
	{
		_mainGrid->iterateAdjacent(column, row, true,
		                           [&callback](int column, int row, brogueCompass direction, brogueCell* item)
		                           {
			                           return callback(column, row, item);
		                           });
	}

	void brogueLayout::iterate(gridCallbackConst<brogueCell*> callback) const
	{
		_mainGrid->iterate(callback);
	}

	void brogueLayout::iterateWhereDefined(gridCallbackConst<brogueCell*> callback) const
	{
		_mainGrid->iterate([&callback](int column, int row, brogueCell* cell)
		{
			if (cell != nullptr)
				callback(column, row, cell);

			return iterationCallback::iterate;
		});
	}

	brogueCell* brogueLayout::firstAdjacent(int column, int row, gridPredicate<brogueCell*> predicate)
	{
		brogueCell* result;

		_mainGrid->iterateAdjacent(column, row, true,
		                           [&predicate, &result](int acolumn, int arow, brogueCompass direction,
		                                                 brogueCell* cell)
		                           {
			                           if (predicate(acolumn, arow, cell))
			                           {
				                           result = cell;
				                           return iterationCallback::breakAndReturn;
			                           }

			                           return iterationCallback::iterate;
		                           });

		return result;
	}

	gridLocator brogueLayout::getNextConnectionPoint(const gridLocator& source, const gridLocator& destination)
	{
		throw simpleException("TODO:  brogueLayout");
	}
}
