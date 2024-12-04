#pragma once

#include <simple.h>
#include <simpleLine.h>

#include "gridCellOutlineSegment.h"

namespace brogueHd::component
{
    using namespace simple;
    using namespace simple::math;

    struct gridCellOutline : simpleStruct
    {
        gridCellOutline(gridCellOutlineSegment* north,
                        gridCellOutlineSegment* south,
                        gridCellOutlineSegment* east,
                        gridCellOutlineSegment* west)
        {
            northSegment = north;
            southSegment = south;
            eastSegment = east;
            westSegment = west;
        }

        gridCellOutlineSegment getSegment(brogueCompass face)
        {
            switch (face)
            {
                case brogueCompass::N:
                    return northSegment == nullptr ? default_value::value<gridCellOutlineSegment>() : *northSegment;
                case brogueCompass::S:
                    return southSegment == nullptr ? default_value::value<gridCellOutlineSegment>() : *southSegment;
                case brogueCompass::E:
                    return eastSegment == nullptr ? default_value::value<gridCellOutlineSegment>() : *eastSegment;
                case brogueCompass::W:
                    return westSegment == nullptr ? default_value::value<gridCellOutlineSegment>() : *westSegment;
                default:
                    throw simpleException("Mishandled outline face direction:  gridCellOutline.h");
            }
        }

        void setSegment(const gridCellOutlineSegment& segment, brogueCompass face)
        {
	        switch (face)
	        {
	        case brogueCompass::N:
                northSegment->set(segment);
                break;
            case brogueCompass::S:
                southSegment->set(segment);
                break;
            case brogueCompass::E:
                eastSegment->set(segment);
                break;
            case brogueCompass::W:
                westSegment->set(segment);
                break;
	        default:
                throw simpleException("Mishandled outline face direction:  gridCellOutline.h");
	        }
        }

        size_t getHash() const override
        {
            size_t north = 0;
            size_t south = 0;
            size_t east = 0;
            size_t west = 0;

            if (northSegment != nullptr)
                north = hashGenerator::generateHash(northSegment);

            if (southSegment != nullptr)
                south = hashGenerator::generateHash(southSegment);

            if (eastSegment != nullptr)
                east = hashGenerator::generateHash(eastSegment);

            if (westSegment != nullptr)
                west = hashGenerator::generateHash(westSegment);

            return hashGenerator::combineHash(north, south, east, west);
        }

    private:

        gridCellOutlineSegment* northSegment;           // UI Coordinates (typically)
        gridCellOutlineSegment* southSegment;
        gridCellOutlineSegment* eastSegment;
        gridCellOutlineSegment* westSegment;
	};
}