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
		brogueLayout(grid<brogueCell*>* mainGrid, gridConnectionLayer<brogueCell*>* connectionLayer);
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
		void iterateAdjacentCells(int column, int row, gridCallback<brogueCell*> callback);

		/// <summary>
		/// Iterates cells and calls the user callback
		/// </summary>
		void iterate(gridCallback<brogueCell*> callback) const;

		/// <summary>
		/// Iterates cells where there is layout defined
		/// </summary>
		void iterateWhereDefined(gridCallback<brogueCell*> callback) const;

		/// <summary>
		/// Gets sub-rectangle from the layout - the largest possible
		/// </summary>
		gridRect getLargestUnusedRectangle(const gridRect& minSize);

		/// <summary>
		/// Iterates all graph edges and callsback to the user code
		/// </summary>
		void iterateRoomConnections(graphSimpleEdgeIterator<gridLocator, gridLocatorEdge> callback);

		/// <summary>
		/// Queries the connection layer (which run's a very small dijkstra iteration on the graph of
		/// connection points) for the next location -> in the directin of travel. Use this before
		/// calling dijkstra's map for the character.
		/// </summary>
		gridLocator getNextConnectionPoint(const gridLocator& source, const gridLocator& destination);

	private:

		// Builds all of the grids from the main grid
		void initialize(grid<brogueCell*>* mainGrid, gridConnectionLayer<brogueCell*>* connectionLayer);

	private:

		// Main Grid for the identity of the brogueCell*. All other layers may be derived
		// from this grid during serialization.
		grid<brogueCell*>* _mainGrid;

		// Permanent Layers (shared pointers with the main grid)
		gridLayer<brogueCell*>* _wallLayer;
		gridLayer<brogueCell*>* _wallIndestructibleLayer;
		gridLayer<brogueCell*>* _cellLayer;
		gridLayer<brogueCell*>* _chasmLayer;
		gridLayer<brogueCell*>* _terrainLayer;						// All Terrain
		gridLayer<brogueCell*>* _terrainLavaLayer;					// Lava Terrain (only)
		gridLayer<brogueCell*>* _terrainWaterLayer;					// Water Terrain (only) (permanent)
		gridLayer<brogueCell*>* _terrainCombustibleLayer;			// Combustible Terrain (only) (permanent)
		gridLayer<brogueCell*>* _alteredLayer;						// Other movement altering (permanent) cell flags

		// Temporary Layers (shared pointers with the main grid)
		gridLayer<brogueCell*>* _temporaryChasmLayer;
		gridLayer<brogueCell*>* _temporaryTerrainLayer;				// All Terrain (temporary only)
		gridLayer<brogueCell*>* _temporaryWaterTerrainLayer;		// Water Terrain (temporary only)
		gridLayer<brogueCell*>* _temporaryPoisonVineTerrainLayer;	// Poisonous Vine Terrain (temporary only)
		gridLayer<brogueCell*>* _flameLayer;						// Flame Terrain (temporary)
		gridLayer<brogueCell*>* _causticGasLayer;
		gridLayer<brogueCell*>* _nauseaGasLayer;
		gridLayer<brogueCell*>* _temporaryAlteredLayer;				// Other movement altering (temporary only) cell flags

		// Connection Layer:  Used for movement / travel (region queries)
		gridConnectionLayer<brogueCell*>* _connectionLayer;

	};

	brogueLayout::brogueLayout(grid<brogueCell*>* mainGrid, gridConnectionLayer<brogueCell*>* connectionLayer)
	{
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
	}

	void brogueLayout::initialize(grid<brogueCell*>* mainGrid, gridConnectionLayer<brogueCell*>* connectionLayer)
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

	void brogueLayout::iterateRoomConnections(graphSimpleEdgeIterator<gridLocator, gridLocatorEdge> callback)
	{
		//if (_connectionGraph == nullptr)
		//	throw simpleException("Trying to iterate room graph before setting it");

		//_connectionGraph->iterateEdges(callback);
	}

	void brogueLayout::iterateAdjacentCells(int column, int row, gridCallback<brogueCell*> callback)
	{
		_mainGrid->iterateAdjacent(column, row, true,
		                           [&callback](int column, int row, brogueCompass direction, brogueCell* item)
		                           {
			                           return callback(column, row, item);
		                           });
	}

	void brogueLayout::iterate(gridCallback<brogueCell*> callback) const
	{
		_mainGrid->iterate(callback);
	}

	void brogueLayout::iterateWhereDefined(gridCallback<brogueCell*> callback) const
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
