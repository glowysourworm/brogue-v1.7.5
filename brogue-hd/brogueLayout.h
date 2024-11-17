#pragma once

#include "brogueRoom.h"
#include "dungeonConstants.h"

#include "grid.h"
#include "simpleList.h"

#include "brogueCell.h"
#include "brogueGlobal.h"
#include "gridDefinitions.h"
#include "gridRect.h"
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

		brogueCell* getCell(short column, short row) const;

		gridRect getBoundary() const;

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

		grid<brogueCell*>* grid = _mainGrid;

		levelBoundary.iterate([&grid] (short column, short row)
		{
			grid->set(column, row, new brogueCell(column, row), true);

			return iterationCallback::iterate;
		});
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

	bool brogueLayout::isDefined(short column, short row) const
	{
		return _mainGrid->isDefined(column, row);
	}

	brogueCell* brogueLayout::getCell(short column, short row) const
	{
		return _mainGrid->get(column, row);
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

