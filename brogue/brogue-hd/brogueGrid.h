#pragma once

#include "brogueCell.h"
#include "grid.h"
#include "gridCell.h"
#include "griddef.h"
#include "gridRect.h"
#include "dungeonDeclaration.h"
#include "dungeonConstants.h"
#include <functional>

using namespace std;

namespace brogueHd::backend::model::layout
{
	class brogueGrid
	{

	public:

		brogueGrid();
		~brogueGrid();

		bool isDefined(short column, short row) const;

		brogueCell* getCell(short column, short row) const;

		gridRect getBoundary() const;

		void addCell(short column, short row);
		void addCells(gridRegion<short>* grid, function<bool(short)> predicate);
		void addCells(gridRect boundary);
		void addCellsInCircle(gridRect boundary);

		void removeCell(short column, short row);
		void removeCells(gridRect boundary);
		void removeCellsInCircle(gridRect boundary);

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
}

