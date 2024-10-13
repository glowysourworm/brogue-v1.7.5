#pragma once

#include "gridLocator.h"

using namespace brogueHd::component;

namespace brogueHd::backend::model::layout
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
		}
		brogueCell(short column, short row)
		{
			// Base struct
		}
		~brogueCell() {};

	public:

		short terrainRandomValues[8];					// TODO: Figure this out!
		bool isWayPoint;

		char machine;
		bool disposableHere;
	};
}

