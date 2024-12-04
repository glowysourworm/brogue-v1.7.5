#pragma once

#include <simpleMath.h>
#include <simpleException.h>
#include <simpleList.h>

#include "gridCellOutlineSegment.h"

namespace brogueHd::component
{
	using namespace simple;

    class gridCellOutlineSegmentHorizontalComparer : simpleListComparer<gridCellOutlineSegment>
    {
    public:

        gridCellOutlineSegmentHorizontalComparer(bool overlapExceptions)
	        : simpleListComparer<gridCellOutlineSegment>(
			[&overlapExceptions] (const gridCellOutlineSegment& segment1, const gridCellOutlineSegment& segment2)
	        {
				int minX1 = simpleMath::minOf(segment1.vertex1.X, segment1.vertex2.X);
				int minX2 = simpleMath::minOf(segment2.vertex1.X, segment2.vertex2.X);

				int maxX1 = simpleMath::maxOf(segment1.vertex1.X, segment1.vertex2.X);
				int maxX2 = simpleMath::maxOf(segment2.vertex1.X, segment2.vertex2.X);

	            // DON'T ALLOW OVERLAP (CAREFUL WITH END POINTS)
	            if (!(minX1 < maxX2) && !(minX2 < maxX1) && overlapExceptions)
	                throw simpleException("Overlapping outline segments:  gridCellOutlineSegmentHorizontalComparer");

	            if (minX1 < minX2)
	                return -1;

	            else if (minX2 < minX1)
	                return 1;

	            // OVERLAP EXCEPTION DOESN'T APPLY TO EQUALITY!
	            else
	                return 0;
	        })
        {
        }
    };
}
