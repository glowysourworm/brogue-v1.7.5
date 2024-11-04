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
using namespace brogueHd::backend::modelConstant;

namespace brogueHd::backend::model
{
	class brogueLayout
	{

	public:

		brogueLayout();
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

	brogueLayout::brogueLayout()
	{
		_mainGrid = new grid<brogueCell*>(gridRect(0, 0, DCOLS, DROWS), gridRect(0, 0, DCOLS, DROWS));
		//_scentMap = new grid<short>(DCOLS, DROWS, 0, 1);
		//_safetyMap = new grid<short>(DCOLS, DROWS, 0, 1);
		//_allySafetyMap = new grid<short>(DCOLS, DROWS, 0, 1);
		//_chokePointMap = new grid<short>(DCOLS, DROWS, 0, 1);
	}

	brogueLayout::~brogueLayout()
	{
		delete _mainGrid;

		//delete _scentMap;
		//delete _safetyMap;
		//delete _allySafetyMap;
		//delete _chokePointMap;
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

