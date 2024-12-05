#pragma once

#include <simplePoint.h>

#include "gridCell.h"
#include "gridLocator.h"

namespace brogueHd::component
{
	struct gridCellOutlineSegment : simpleStruct
	{
		gridCellOutlineSegment()
		{
			
		}
		gridCellOutlineSegment(const simplePoint<int>& v1, const simplePoint<int>& v2, const gridLocator& loc1, const gridLocator& loc2)
		{
			vertex1 = v1;
			vertex2 = v2;
			location1 = loc1;
			location2 = loc2;
		}
		gridCellOutlineSegment(const gridCellOutlineSegment& copy)
		{
			copyImpl(copy);
		}
		gridCellOutlineSegment createReversed() const
		{
			return gridCellOutlineSegment(vertex2, vertex1, location2, location1);
		}
		void translate(const gridLocator& translationGrid, const simplePoint<int>& translationUI)
		{
			location1.translate(translationGrid.column, translationGrid.row);
			location2.translate(translationGrid.column, translationGrid.row);
			vertex1.translate(translationUI.x, translationUI.y);
			vertex2.translate(translationUI.x, translationUI.y);
		}
		void set(const gridCellOutlineSegment& other)
		{
			copyImpl(other);
		}
		void operator=(const gridCellOutlineSegment& copy)
		{
			copyImpl(copy);
		}
		bool operator==(const gridCellOutlineSegment& other)
		{
			return compare(other);
		}
		bool operator!=(const gridCellOutlineSegment& other)
		{
			return !compare(other);
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(vertex1, vertex2, location1, location2);
		}

		simplePoint<int> vertex1;
		simplePoint<int> vertex2;

		gridLocator location1;
		gridLocator location2;

	private:

		void copyImpl(const gridCellOutlineSegment& copy)
		{
			vertex1 = copy.vertex1;
			vertex2 = copy.vertex2;
			location1 = copy.location1;
			location2 = copy.location2;
		}

		bool compare(const gridCellOutlineSegment& other)
		{
			return vertex1 == other.vertex1 &&
				vertex2 == other.vertex2 &&
				location1 == other.location1 &&
				location2 == other.location2;
		}
	};
}