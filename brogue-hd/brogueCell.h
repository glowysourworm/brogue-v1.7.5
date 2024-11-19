#pragma once

#include "brogueCellDisplay.h"
#include "color.h"
#include "gridLocator.h"

using namespace brogueHd::component;

namespace brogueHd::backend::model
{
	//struct pdsLink {
	//	short distance;
	//	short cost;
	//	pdsLink* left, * right;
	//};

	//struct pdsMap {
	//	boolean eightWays;

	//	pdsLink front;
	//	pdsLink links[DCOLS * DROWS];
	//};

	struct brogueCell : public gridLocator
	{
	public:

		// Sets default values (isEmpty() == true)
		brogueCell()
		{
			_display = new brogueCellDisplay(-1, -1);
			_display->backColor = colors::getGray(0.5);
			_display->foreColor = colors::blue();
		}
		brogueCell(short column, short row)
		{
			_display = new brogueCellDisplay(column, row);
			_display->backColor = colors::getGray(0.5);
			_display->foreColor = colors::blue();
		}
		~brogueCell() 
		{
			delete _display;
		};

		brogueCellDisplay getDisplay() const
		{
			return *_display;
		}

	public:

		brogueCellDisplay* _display;

		//short terrainRandomValues[8];					// TODO: Figure this out!
		//bool isWayPoint;

		//char machine;
		//bool disposableHere;
	};
}

