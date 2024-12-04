#pragma once

#include <simpleMath.h>
#include <simpleException.h>
#include <simpleList.h>

#include "gridCellOutlineSegment.h"

namespace brogueHd::component
{
    using namespace simple;

	class gridCellOutlineSegmentVerticalComparer : simpleListComparer<gridCellOutlineSegment>
	{
	public:

		gridCellOutlineSegmentHorizontalComparer(bool overlapExceptions)
			: simpleListComparer<gridCellOutlineSegment>(
			[&overlapExceptions] (const gridCellOutlineSegment& segment1, const gridCellOutlineSegment& segment2)
		{
            int minY1 = simpleMath::minOf(segment1.vertex1.Y, segment1.vertex2.Y);
            int minY2 = simpleMath::minOf(segment2.vertex1.Y, segment2.vertex2.Y);

            int maxY1 = simpleMath::maxOf(segment1.vertex1.Y, segment1.vertex2.Y);
            int maxY2 = simpleMath::maxOf(segment2.vertex1.Y, segment2.vertex2.Y);

            // DON'T ALLOW OVERLAP (CAREFUL WITH END POINTS)
            if (!(minY1 < maxY2) && !(minY2 < maxY1) && overlapExceptions)
                throw simpleException("Overlapping outline segments:  gridCellOutlineSegmentVerticalComparer");

            if (minY1 < minY2)
                return -1;

            else if (minY2 < minY1)
                return 1;

            // OVERLAP EXCEPTION DOESN'T APPLY TO EQUALITY!
            else
                return 0;
		})
		{
		}
	};
}
