#pragma once

#include "brogueCellDisplay.h"
#include "color.h"
#include "gridLocator.h"

using namespace brogueHd::component;

namespace brogueHd::backend::model
{
	//struct pdsLink {
	//	int distance;
	//	int cost;
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
		brogueCell(int column, int row)
		{
			_display = new brogueCellDisplay(column, row);
			_display->backColor = colors::getGray(0.5);
			_display->foreColor = colors::blue();
		}
		brogueCell(int column, int row, const color& backColor, const color& foreColor, char symbol)
		{
			_display = new brogueCellDisplay(column, row);
			_display->backColor = backColor;
			_display->foreColor = foreColor;
			_display->character = symbol;
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

		//int terrainRandomValues[8];					// TODO: Figure this out!
		//bool isWayPoint;

		//char machine;
		//bool disposableHere;
	};
}

