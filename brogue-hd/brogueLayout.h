#pragma once

#include "brogueRoom.h"
#include "dungeonConstants.h"

#include "grid.h"
#include "simpleList.h"

#include "brogueCell.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "gridRegion.h"
#include "simple.h"
#include <functional>

using namespace brogueHd::simple;
using namespace brogueHd::component;

namespace brogueHd::backend::model
{
	class brogueLayout
	{

	public:

		brogueLayout(const gridRect& levelParentBoundary, const gridRect& levelBoundary);
		~brogueLayout();

		bool isDefined(short column, short row) const;

		brogueCell* get(short column, short row) const;

		gridRect getBoundary() const;
		gridRect getParentBoundary() const;

		/// <summary>
		/// Iterates adjacent cells to satisfy the user predicate. Returns the result cell, (or null), and 
		/// will have updated the column and row, (or -1's).
		/// </summary>
		brogueCell* firstAdjacent(short column, short row, gridPredicate<brogueCell*> predicate);

		/// <summary>
		/// Iterates adjacent cells and calls the user callback
		/// </summary>
		void iterateAdjacentCells(short column, short row, gridCallback<brogueCell*> callback);

		/// <summary>
		/// Iterates cells and calls the user callback
		/// </summary>
		void iterate(gridCallback<brogueCell*> callback) const;

		/// <summary>
		/// Ensures that the layout has cells in the specified locations.
		/// </summary>
		template<isGridLocator T>
		void createCells(gridRegion<T>* region);

		/// <summary>
		/// Ensures that cell is created for specified location
		/// </summary>
		template<isGridLocator T>
		void createCells(const T& locator);

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

		//short cost;

		//enum dungeonLayers layerFlags;					// Designate which of the layers the cell inhabits
		//unsigned long flags;							// non-terrain cell flags
		//unsigned short volume;							// quantity of gas in cell

		//unsigned char machineNumber;


		// REFACTORED VOLUME:  Separates gases
		//unsigned short poisonGasVolume;
		//unsigned short swampGasVolume;

	};

	brogueLayout::brogueLayout(const gridRect& levelParentBoundary, const gridRect& levelBoundary)
	{
		_mainGrid = new grid<brogueCell*>(levelParentBoundary, levelBoundary);
	}

	brogueLayout::~brogueLayout()
	{
		_mainGrid->iterate([] (short column, short row, brogueCell* cell)
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
	bool brogueLayout::isDefined(short column, short row) const
	{
		return _mainGrid->isDefined(column, row);
	}

	brogueCell* brogueLayout::get(short column, short row) const
	{
		return _mainGrid->get(column, row);
	}

	template<isGridLocator T>
	void brogueLayout::createCells(const T& locator)
	{
		if (!_mainGrid->isDefined(locator.column, locator.row))
			_mainGrid->set(locator.column, locator.row, new brogueCell(locator.column, locator.row), true);
	}

	template<isGridLocator T>
	void brogueLayout::createCells(gridRegion<T>* region)
	{
		grid<brogueCell*>* mainGrid = _mainGrid;

		// Check cells
		region->iterateLocations([&mainGrid] (short column, short row, const T& locator)
		{
			if (!mainGrid->isDefined(locator.column, locator.row))
				mainGrid->set(locator.column, locator.row, new brogueCell(locator.column, locator.row), true);

			return iterationCallback::iterate;
		});
	}

	void brogueLayout::iterateAdjacentCells(short column, short row, gridCallback<brogueCell*> callback)
	{
		_mainGrid->iterateAdjacent(column, row, true, [&callback] (short column, short row, brogueCompass direction, brogueCell* item)
		{
			return callback(column, row, item);
		});
	}
	void brogueLayout::iterate(gridCallback<brogueCell*> callback) const
	{
		_mainGrid->iterate(callback);
	}

	brogueCell* brogueLayout::firstAdjacent(short column, short row, gridPredicate<brogueCell*> predicate)
	{
		brogueCell* result;

		_mainGrid->iterateAdjacent(column, row, true, [&predicate, &result] (short acolumn, short arow, brogueCompass direction, brogueCell* cell)
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

