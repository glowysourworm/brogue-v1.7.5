#pragma once

#include "brogueRoom.h"
#include "dungeonConstants.h"

#include "grid.h"
#include "simpleList.h"

#include <functional>

using namespace std;
using namespace brogueHd::component;

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
		brogueCell* checkAdjacentCells(short& column, short& row, function<bool(brogueCell*)> predicate);

		/// <summary>
		/// Iterates adjacent cells and calls the user callback
		/// </summary>
		void iterateAdjacentCells(short column, short row, function<bool(short, short, brogueCell*)> callback);

	private:

		simpleList<brogueRoom*>* _rooms;

		//// May need to break into separate grids
		//grid<brogueCell*>* _mainGrid;

		//// Grid containing permanent dungeon layers
		//grid<permanentDungeonLayers>* _permanentGrid;

		//// Grid containing temporary dungeon layers
		//grid<temporaryDungeonLayers>* _temporaryGrid;

		//// Grid containing flag-enum for categories of features:  TODO
		//grid<dungeonFeatureCategories>* _featureCategoriesGrid;

		//short cost;

		//enum dungeonLayers layerFlags;					// Designate which of the layers the cell inhabits
		//unsigned long flags;							// non-terrain cell flags
		//unsigned short volume;							// quantity of gas in cell

		//unsigned char machineNumber;


		// REFACTORED VOLUME:  Separates gases
		//unsigned short poisonGasVolume;
		//unsigned short swampGasVolume;

	};
}

