#pragma once

#include "dungeonDeclaration.h"
#include "brogueCellDisplay.h"

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

	class brogueCell : public gridCell
	{

	public:

		brogueCell(){}
		brogueCell(short column, short row) 
		{ 
			this->column = column; 
			this->row = row; 
		};
		~brogueCell() {};

		static bool operator >=(const brogueCell& cell1, const brogueCell& cell2)
		{
			return cell1.tagValue >= cell2.tagValue;
		}
		static bool operator <=(const brogueCell& cell1, const brogueCell& cell2)
		{
			return cell1.tagValue <= cell2.tagValue;
		}
		static bool operator >(const brogueCell& cell1, const brogueCell& cell2)
		{
			return cell1.tagValue > cell2.tagValue;
		}
		static bool operator <(const brogueCell& cell1, const brogueCell& cell2)
		{
			return cell1.tagValue < cell2.tagValue;
		}
		static bool operator ==(const brogueCell& cell1, const brogueCell& cell2)
		{
			return cell1.tagValue == cell2.tagValue;
		}

	public:

		short terrainRandomValues[8];					// TODO: Figure this out!
		bool isWayPoint;

		char machine;
		bool disposableHere;

		// Dummy variable:  Leaving a tag here for use; but it's just for compatability with dijkstra's map
		short tagValue;
	};
}

