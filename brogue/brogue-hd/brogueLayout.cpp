#include "brogueLayout.h"
#include "gridMethods.h"
#include "gridDefinitions.h"
#include "brogueCell.h"

namespace brogueHd::backend::model
{
	brogueLayout::brogueLayout()
	{
		//// TODO: Find better way to handle the brogueCell
		//brogueCell* maxCell = new brogueCell();
		//brogueCell* zeroCell = new brogueCell();

		//maxCell->cost = 1;
		//zeroCell->cost = 0;

		//_grid = new grid<brogueCell*>(DCOLS, DROWS, zeroCell, maxCell);
		//_scentMap = new grid<short>(DCOLS, DROWS, 0, 1);
		//_safetyMap = new grid<short>(DCOLS, DROWS, 0, 1);
		//_allySafetyMap = new grid<short>(DCOLS, DROWS, 0, 1);
		//_chokePointMap = new grid<short>(DCOLS, DROWS, 0, 1);
	}

	brogueLayout::~brogueLayout()
	{
		//delete _scentMap;
		//delete _safetyMap;
		//delete _allySafetyMap;
		//delete _chokePointMap;
	}

	gridRect brogueLayout::getBoundary() const
	{
		//return _grid->getBoundary();
	}

	bool brogueLayout::isDefined(short column, short row) const
	{
		//return !_grid->isZeroValue(column, row);
	}

	brogueCell* brogueLayout::getCell(short column, short row) const
	{
		//return _grid->get(column, row);
	}

	void brogueLayout::addCell(short column, short row)
	{
		//brogueAssert(_brogueGrid[column][row] == NULL);

		//// Be sure to clean up memory on remove...
		////
		//_grid->set(column, row, new brogueCell());
	}

	void brogueLayout::addCells(array2D<short>* region, function<bool(short)> predicate)
	{
		//brogueGrid* that = this;

		//// javascript... 
		//iterateIn(region, region->getBoundary(), [that, region, predicate](short column, short row)
		//{
		//	if (predicate(region->get(column, row)))
		//		that->addCell(column, row);
		//});
	}

	void brogueLayout::addCells(gridRect boundary)
	{
		//brogueGrid* that = this;

		//iterate(boundary, [that](short column, short row)
		//{
		//	that->addCell(column, row);
		//});
	}

	void brogueLayout::addCellsInCircle(gridRect boundary)
	{
		//brogueAssert(boundary.width == boundary.height);

		//brogueGrid* that = this;

		//// Need to use this function for creating new instances of the value. TODO: Craft a class
		//// hierarchy that includes instance factory.
		////
		//iterateInCircle(_grid, boundary, [that](short column, short row)
		//{
		//	if (!that->isDefined(column, row))
		//		that->addCell(column, row);
		//});
	}

	void brogueLayout::removeCell(short column, short row)
	{
		//if (this->isDefined(column, row))
		//{
		//	// Delete memory for the brogueCell instance
		//	//
		//	delete _grid->get(column, row);
		//}
	}

	void brogueLayout::removeCells(gridRect boundary)
	{
		//brogueGrid* that = this;

		//iterateIn(_grid, boundary, [that](short column, short row)
		//{
		//	if (that->isDefined(column, row))
		//		that->removeCell(column, row);
		//});
	}

	void brogueLayout::removeCellsInCircle(gridRect boundary)
	{
		//brogueAssert(boundary.width == boundary.height);

		//brogueGrid* that = this;

		//iterateInCircle(_grid, boundary, [that](short column, short row)
		//{
		//	if (that->isDefined(column, row))
		//		that->removeCell(column, row);
		//});
	}

	void brogueLayout::iterateAdjacentCells(short column, short row, function<bool(short, short, brogueCell*)> callback)
	{
		//grid<brogueCell*>* grid = _grid;

		//iterateAround(_grid, column, row, true, [&grid, &callback](short column, short row)
		//{
		//	callback(column, row, grid->get(column, row));
		//});
	}

	brogueCell* brogueLayout::checkAdjacentCells(short& column, short& row, function<bool(brogueCell*)> callback)
	{
		//grid<brogueCell*>* grid = _grid;
		//brogueCell* result;
		//short resultColumn = column;
		//short resultRow = row;

		//iterateAround(_grid, column, row, true, [&grid, &callback, &result, &resultColumn, &resultRow](short column, short row)
		//{
		//	if (callback(grid->get(column, row)))
		//	{
		//		result = grid->get(column, row);
		//		resultColumn = column;
		//		resultRow = row;
		//	}
		//});

		//return result;
	}
}