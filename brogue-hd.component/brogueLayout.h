#pragma once

#include "brogueRoom.h"
#include "dungeonConstants.h"

#include "grid.h"
#include "simpleList.h"

#include "brogueCell.h"
#include "brogueCellDisplay.h"
#include "graph.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include <simpleGridRect.h>
#include "gridRegion.h"
#include "simple.h"
#include "simpleException.h"
#include <functional>

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;

	class brogueLayout
	{
	public:
		brogueLayout(const simpleGridRect& levelParentBoundary, const simpleGridRect& levelBoundary);
		~brogueLayout();

		bool isDefined(int column, int row) const;

		brogueCell* get(int column, int row) const;

		simpleGridRect getBoundary() const;
		simpleGridRect getParentBoundary() const;

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
		/// Ensures that the layout has cells in the specified locations.
		/// </summary>
		template <isGridLocator T>
		void createCells(gridRegion<T>* region, const brogueCell& prototype, bool overwrite = false);

		/// <summary>
		/// Ensures that cell is created for specified location
		/// </summary>
		template <isGridLocator T>
		void createCells(const T& locator, const brogueCell& prototype, bool overwrite = false);

		/// <summary>
		/// Gets sub-rectangle from the layout - the largest possible
		/// </summary>
		gridRect getLargestUnusedRectangle(const gridRect& minSize);

		/// <summary>
		/// Sets graph of rooms for the layout
		/// </summary>
		void setRoomConnectionGraph(graph<gridLocator, gridLocatorEdge>* connectionGraph);

		void setCorridorConnections(const simpleList<gridLocatorEdge>& connections);

		simpleList<gridLocatorEdge> getCorridorConnections()
		{
			return *_corridorConnections;
		}

		/// <summary>
		/// Iterates all graph edges and callsback to the user code
		/// </summary>
		void iterateRoomConnections(graphSimpleEdgeIterator<gridLocator, gridLocatorEdge> callback);

	private:
		simpleList<brogueRoom*>* _rooms;

		// May need to break into separate grids
		grid<brogueCell*>* _mainGrid;

		// Grid containing permanent dungeon layers
		grid<permanentDungeonLayers>* _permanentGrid;

		// Grid containing temporary dungeon layers
		grid<temporaryDungeonLayers>* _temporaryGrid;

		// Grid containing flag-enum for categories of features:  TODO
		grid<dungeonFeatureCategories>* _featureCategoriesGrid;

		graph<gridLocator, gridLocatorEdge>* _connectionGraph;

		simpleList<gridLocatorEdge>* _corridorConnections;

		//int cost;

		//enum dungeonLayers layerFlags;					// Designate which of the layers the cell inhabits
		//unsigned long flags;							// non-terrain cell flags
		//unsigned int volume;							// quantity of gas in cell

		//unsigned char machineNumber;


		// REFACTORED VOLUME:  Separates gases
		//unsigned int poisonGasVolume;
		//unsigned int swampGasVolume;
	};

	brogueLayout::brogueLayout(const gridRect& levelParentBoundary, const gridRect& levelBoundary)
	{
		_mainGrid = new grid<brogueCell*>(levelParentBoundary, levelBoundary);
		_connectionGraph = nullptr;
		_corridorConnections = new simpleList<gridLocatorEdge>();
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

	template <isGridLocator T>
	void brogueLayout::createCells(const T& locator, const brogueCell& prototype, bool overwrite)
	{
		brogueCellDisplay display = prototype.getDisplay();

		_mainGrid->set(locator.column,
		               locator.row,
		               new brogueCell(locator.column, locator.row, display.backColor, display.foreColor,
		                              display.character),
		               overwrite);
	}

	template <isGridLocator T>
	void brogueLayout::createCells(gridRegion<T>* region, const brogueCell& prototype, bool overwrite)
	{
		grid<brogueCell*>* mainGrid = _mainGrid;
		brogueCellDisplay display = prototype.getDisplay();

		// Check cells
		region->iterateLocations([&mainGrid, &display, &overwrite](int column, int row, const T& locator)
		{
			mainGrid->set(locator.column,
			              locator.row,
			              new brogueCell(locator.column, locator.row, display.backColor, display.foreColor,
			                             display.character),
			              overwrite);

			return iterationCallback::iterate;
		});
	}

	void brogueLayout::setRoomConnectionGraph(graph<gridLocator, gridLocatorEdge>* connectionGraph)
	{
		_connectionGraph = connectionGraph;
	}

	void brogueLayout::setCorridorConnections(const simpleList<gridLocatorEdge>& connections)
	{
		_corridorConnections->clear();
		_corridorConnections->addRange(connections);
	}

	void brogueLayout::iterateRoomConnections(graphSimpleEdgeIterator<gridLocator, gridLocatorEdge> callback)
	{
		if (_connectionGraph == nullptr)
			throw simpleException("Trying to iterate room graph before setting it");

		_connectionGraph->iterateEdges(callback);
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
}
