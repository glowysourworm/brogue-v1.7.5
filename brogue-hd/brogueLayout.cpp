#include "brogueLayout.h"
#include "gridDefinitions.h"
#include "brogueCell.h"

using namespace brogueHd::component;

namespace brogueHd::backend::model
{
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
		_mainGrid->iterateAround(column, row, true, [&callback](short column, short row, brogueCell* item)
		{
			return callback(column, row, item);
		});
	}

	brogueCell* brogueLayout::firstAdjacent(short column, short row, gridPredicate<brogueCell*> predicate)
	{
		brogueCell* result;

		_mainGrid->iterateAround(column, row, true, [&predicate, &result](short acolumn, short arow, brogueCell* cell)
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