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
        gridCellOutline(const gridCellOutlineSegment& north,
                        const gridCellOutlineSegment& south,
                        const gridCellOutlineSegment& east,
                        const gridCellOutlineSegment& west)
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
                    return northSegment;
                case brogueCompass::S:
                    return southSegment;
                case brogueCompass::E:
                    return eastSegment;
                case brogueCompass::W:
                    return westSegment;
                default:
                    throw simpleException("Mishandled outline face direction:  gridCellOutline.h");
            }
        }

        void setSegment(const gridCellOutlineSegment& segment, brogueCompass face)
        {
	        switch (face)
	        {
	        case brogueCompass::N:
                northSegment.set(segment);
                break;
            case brogueCompass::S:
                southSegment.set(segment);
                break;
            case brogueCompass::E:
                eastSegment.set(segment);
                break;
            case brogueCompass::W:
                westSegment.set(segment);
                break;
	        default:
                throw simpleException("Mishandled outline face direction:  gridCellOutline.h");
	        }
        }

        void translate(const gridLocator& translationGrid, const simplePoint<int>& translationUI)
        {
            if (northSegment != default_value::value<gridCellOutlineSegment>())
                northSegment.translate(translationGrid, translationUI);

            if (southSegment != default_value::value<gridCellOutlineSegment>())
                southSegment.translate(translationGrid, translationUI);

            if (eastSegment != default_value::value<gridCellOutlineSegment>())
                eastSegment.translate(translationGrid, translationUI);

            if (westSegment != default_value::value<gridCellOutlineSegment>())
                westSegment.translate(translationGrid, translationUI);
        }

        size_t getHash() const override
        {
            size_t north = 0;
            size_t south = 0;
            size_t east = 0;
            size_t west = 0;

            if (northSegment != default_value::value<gridCellOutlineSegment>())
                north = hashGenerator::generateHash(northSegment);

            if (southSegment != default_value::value<gridCellOutlineSegment>())
                south = hashGenerator::generateHash(southSegment);

            if (eastSegment != default_value::value<gridCellOutlineSegment>())
                east = hashGenerator::generateHash(eastSegment);

            if (westSegment != default_value::value<gridCellOutlineSegment>())
                west = hashGenerator::generateHash(westSegment);

            return hashGenerator::combineHash(north, south, east, west);
        }

    private:

        gridCellOutlineSegment northSegment;           // UI Coordinates (typically)
        gridCellOutlineSegment southSegment;
        gridCellOutlineSegment eastSegment;
        gridCellOutlineSegment westSegment;
	};
}