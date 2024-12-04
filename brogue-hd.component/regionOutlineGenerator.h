#pragma once

#include <simple.h>
#include <simplePolygon.h>
#include "gridDefinitions.h"
#include "gridRegion.h"
#include "gridRegionOutline.h"
#include "regionOutlineGrid.h"
#include "layoutCoordinateConverter.h"

namespace brogueHd::component
{
    using namespace simple;

    template<isGridLocator T>
	class regionOutlineGenerator
	{
	public:

        regionOutlineGenerator(layoutCoordinateConverter* coordinateConverter);
        ~regionOutlineGenerator();

        gridRegionOutline* createOutline(gridRegion<T>* region);

    private:

        simplePolygon<int>* createClockwiseOutline(const gridCellOutlineSegment& firstSegment,
                                                   brogueCompass firstTraceDirection,
                                                   bool interiorPolygon,
                                                   regionOutlineGrid<T>* algorithmGrid);

        bool isTraceCompleted(const gridCellOutlineSegment& firstSegment,
                              const gridCellOutlineSegment& lastSegment,
                              bool interiorPolygon,
                              brogueCompass lastDirection);

        gridCellOutlineSegment calculateNextTraceExterior(const gridCellOutlineSegment& lastSegment,
                                                          brogueCompass& traceDirection,
                                                          regionOutlineGrid<T>* algorithmGrid);

        gridCellOutlineSegment calculateNextTraceInterior(const gridCellOutlineSegment& lastSegment,
														  brogueCompass& traceDirection,
                                                          regionOutlineGrid<T>* algorithmGrid);

        /// <summary>
        /// Returns true if the coordinates are flipped with respect to the trace direction and interior / exterior face
        /// </summary>
        bool isSegmentFlipped(brogueCompass traceDirection);

    private:

        layoutCoordinateConverter* _coordinateConverter;
	};

    template<isGridLocator T>
    regionOutlineGenerator<T>::regionOutlineGenerator(layoutCoordinateConverter* coordinateConverter)
    {
        _coordinateConverter = coordinateConverter;
    }

    template<isGridLocator T>
    regionOutlineGenerator<T>::~regionOutlineGenerator()
    {
	    
    }

    template<isGridLocator T>
    gridRegionOutline* regionOutlineGenerator<T>::createOutline(gridRegion<T>* region)
    {
        // Initializes the grid needed to perform the outline calculation
        regionOutlineGrid<T>* algorithmGrid = new regionOutlineGrid(region, _coordinateConverter);

        // Procedure
        //
        // 1) Eliminate EXTERIOR polygon outline (there is only one)
        // 2) Take north segments and trace oriented clockwise until
        //    all segments are accounted for.
        //

        // Choose a location on the EDGE of the grid so that it's sure to be
        // an EXTERIOR north segment.
        //

        gridCellOutlineSegment lastSegment = algorithmGrid->getNorthmostSegment();
        brogueCompass lastDirection = brogueCompass::E;

        simplePolygon<int>* exteriorOutline = nullptr;
        simpleList<simplePolygon<int>*> interiorOutlines;

        do
        {
            // Removes segments from each collection; but dictionaries have to be filtered out
            simplePolygon<int>* outline = createClockwiseOutline(lastSegment, lastDirection, exteriorOutline != nullptr, algorithmGrid);

            if (exteriorOutline == nullptr)
                exteriorOutline = outline;

            else
                interiorOutlines.add(outline);

            // Choose the next NORTH segment - which should be an INTERIOR face
            if (algorithmGrid->getSegmentCount(brogueCompass::N) > 0)
            {
                // The trace will then be CLOCKWISE oriented on an
                // INTERIOR polygon face
                lastSegment = algorithmGrid->getAnySegment(brogueCompass::N);
                lastDirection = brogueCompass::W;

                // Set segment used
                algorithmGrid->removeSegment(lastSegment);
            }
            else
            {
                if (algorithmGrid->getSegmentCount(brogueCompass::S) > 0 ||
                    algorithmGrid->getSegmentCount(brogueCompass::E) > 0 ||
                    algorithmGrid->getSegmentCount(brogueCompass::W) > 0)
                    throw simpleException("Mis-handled polygon segments:  regionOutlineGenerator.h");
            }
        } while (algorithmGrid->getSegmentCount(brogueCompass::N) > 0 ||
                 algorithmGrid->getSegmentCount(brogueCompass::S) > 0 ||
                 algorithmGrid->getSegmentCount(brogueCompass::E) > 0 ||
                 algorithmGrid->getSegmentCount(brogueCompass::W) > 0);

        // (Memory!) Finished with the algorithm grid. The stacklike outline segments were transferred to the polygon.
        delete algorithmGrid;

        return new gridRegionOutline(exteriorOutline, interiorOutlines);
    }

    template<isGridLocator T>
    simplePolygon<int>* regionOutlineGenerator<T>::createClockwiseOutline(const gridCellOutlineSegment& firstSegment,
							                                              brogueCompass firstTraceDirection,
							                                              bool interiorPolygon,
							                                              regionOutlineGrid<T>* algorithmGrid)
    {
        // Setup for tracing in the specified direction
        gridCellOutlineSegment lastSegment = firstSegment;
        brogueCompass lastDirection = firstTraceDirection;

        // Careful with hashing:  Done on insert only!
        simpleHash<gridCellOutlineSegment, gridCellOutlineSegment> outline;

        // Trace the outline of the polygon using the reduced N, S, E, and W segments
        //
        while (true)
        {
            bool lastSegmentFlipped = isSegmentFlipped(lastDirection);
            gridCellOutlineSegment lastSegmentReversed = lastSegment.createReversed();

            if (lastSegment == default_value::value<gridCellOutlineSegment>())
                throw simpleException("No outline segment found on previous iteration!  regionOutlineGenerator");

            if (outline.contains(lastSegment) || outline.contains(lastSegmentReversed))
                throw simpleException("Trying to add duplicate line segment:  regionOutlineGenerator");

            // CHECK ORIENTATION!
            if (!lastSegmentFlipped)
                outline.add(lastSegment, lastSegment);

            // Happens when tracing in the negative direction
            else
                outline.add(lastSegmentReversed, lastSegmentReversed);

            // Check to see if we've connected back to the beginning!
            //
            if (outline.count() > 1 && isTraceCompleted(firstSegment, lastSegment, interiorPolygon, lastDirection))
            {
                break;
            }

            if (!interiorPolygon)
                lastSegment = calculateNextTraceExterior(lastSegment, lastDirection, algorithmGrid);

            else
                lastSegment = calculateNextTraceInterior(lastSegment, lastDirection, algorithmGrid);
        }

        // Finally, create the outline! The line segments are IN ORDER CLOCKWISE around
        // the exterior of the Polygon. These will be stored and used later on for other
        // algorithms.
        //

        return new simplePolygon<int>(outline.getKeys(), true);
    }

    template<isGridLocator T>
    gridCellOutlineSegment regionOutlineGenerator<T>::calculateNextTraceExterior(const gridCellOutlineSegment& lastSegment,
																				 brogueCompass& traceDirection,
							                                                     regionOutlineGrid<T>* algorithmGrid)
    {
        // Create CLOCKWISE trace of exterior edge - favor CROTCH points to stay on the exterior of the region
        //
        // Keep orientation CLOCKWISE:  NOTE*** The orientation of the line segments is ALWAYS UI-POSITIVE!
        //

        gridCellOutlineSegment nextSegment = default_value::value<gridCellOutlineSegment>();

        switch (traceDirection)
        {
            case brogueCompass::N:
            {
                // NORTH on a WEST face: Turn E, or W

                // Is the current segment's endpoint a CORNER or a CROTCH point?
                int crotchColumn = lastSegment.vertex1.column - 1;
                int crotchRow = lastSegment.vertex1.row - 1;

                int cornerColumn = lastSegment.vertex1.column;
                int cornerRow = lastSegment.vertex1.row;

                // CROTCH
                if (algorithmGrid->hasSegment(crotchColumn, crotchRow, brogueCompass::S))
                {
                    // => V1 is a CROTCH point with a turn towards the WEST
                    //
                    traceDirection = brogueCompass::W;      // Tracing N -> Turn W

                    nextSegment = algorithmGrid->getSegment(crotchColumn, crotchRow, brogueCompass::S);
                }

                // CORNER
                else
                {
                    if (!algorithmGrid->hasSegment(cornerColumn, cornerRow, brogueCompass::N))
                        throw simpleException("Invalid outline winding:  regionOutlineGenerator::calculateNextTraceExterior");

                    // => Endpoint is a CORNER point which turns towards the EAST
                    //
                    traceDirection = brogueCompass::E;      // Tracing N -> Turn E

                    nextSegment = algorithmGrid->getSegment(cornerColumn, cornerRow, brogueCompass::N);
                }
            }
            break;
            case brogueCompass::S:
            {
                // Tracing SOUTH on an EAST face

                // Is the current segment's endpoint a CORNER or a CROTCH point?
                int crotchColumn = lastSegment.vertex2.column + 1;
                int crotchRow = lastSegment.vertex2.row + 1;

                int cornerColumn = lastSegment.vertex2.column;
                int cornerRow = lastSegment.vertex2.row;

                // CROTCH
                if (algorithmGrid->hasSegment(crotchColumn, crotchRow, brogueCompass::N))
                {
                    // => Endpoint is a CROTCH point with a turn towards the EAST
                    //
                    traceDirection = brogueCompass::E;      // Tracing S -> Turn E

                    nextSegment = algorithmGrid->getSegment(crotchColumn, crotchRow, brogueCompass::N);
                }

                // CORNER
                else
                {
                    if (!algorithmGrid->hasSegment(cornerColumn, cornerRow, brogueCompass::S))
                        throw simpleException("Invalid outline winding:  regionOutlineGenerator::calculateNextTraceExterior");

                    // => Endpoint is a CORNER point which turns towards the WEST
                    //
                    traceDirection = brogueCompass::W;      // Tracing S -> Turn W

                    nextSegment = algorithmGrid->getSegment(cornerColumn, cornerRow, brogueCompass::S);
                }
            }
            break;
            case brogueCompass::E:
            {
                // Tracing EAST on a NORTH face

                // Is the current segment's endpoint a CORNER or a CROTCH point?
                int crotchColumn = lastSegment.vertex2.column + 1;
                int crotchRow = lastSegment.vertex2.row - 1;

                int cornerColumn = lastSegment.vertex2.column;
                int cornerRow = lastSegment.vertex2.row;

                // CROTCH
                if (algorithmGrid->hasSegment(crotchColumn, crotchRow, brogueCompass::W))
                {
                    // => Endpoint is a CROTCH point with a turn towards the NORTH
                    //
                    traceDirection = brogueCompass::N;      // Tracing E -> Turn N

                    nextSegment = algorithmGrid->getSegment(crotchColumn, crotchRow, brogueCompass::W);
                }

                // CORNER
                else
                {
                    if (!algorithmGrid->hasSegment(cornerColumn, cornerRow, brogueCompass::E))
                        throw simpleException("Invalid outline winding:  regionOutlineGenerator::calculateNextTraceExterior");

                    // => Endpoint is a CORNER poinit which turns towards the SOUTH
                    //
                    traceDirection = brogueCompass::S;      // Tracing E -> Turn S

                    nextSegment = algorithmGrid->getSegment(cornerColumn, cornerRow, brogueCompass::E);
                }
            }
            break;
            case brogueCompass::W:
            {
                // Tracing WEST on a SOUTH face

                // Is the current segment's endpoint a CORNER or a CROTCH point?
                int crotchColumn = lastSegment.vertex1.column - 1;
                int crotchRow = lastSegment.vertex1.row + 1;

                int cornerColumn = lastSegment.vertex1.column;
                int cornerRow = lastSegment.vertex1.row;

                // CROTCH
                if (algorithmGrid->hasSegment(crotchColumn, crotchRow, brogueCompass::E))
                {
                    // => Endpoint is a CROTCH point with a turn towards the SOUTH
                    //
                    traceDirection = brogueCompass::S;      // Tracing W -> Turn S

                    nextSegment = algorithmGrid->getSegment(crotchColumn, crotchRow, brogueCompass::E);
                }

                // CORNER
                else
                {
                    if (!algorithmGrid->hasSegment(cornerColumn, cornerRow, brogueCompass::W))
                        throw simpleException("Invalid outline winding:  regionOutlineGenerator::calculateNextTraceExterior");

                    // => Endpoint is a CORNER point which turns towards the NORTH
                    //
                    traceDirection = brogueCompass::N;      // Tracing W -> Turn N

                    nextSegment = algorithmGrid->getSegment(cornerColumn, cornerRow, brogueCompass::W);
                }
            }
            break;
            case brogueCompass::NW:
            case brogueCompass::NE:
            case brogueCompass::SE:
            case brogueCompass::SW:
            case brogueCompass::None:
            default:
                throw simpleException("Unhandled Compass Direction:  regionOutlineGenerator.h");
        }

        // Update the grid
        algorithmGrid->removeSegment(nextSegment);

        return nextSegment;
    }

    template<isGridLocator T>
    gridCellOutlineSegment regionOutlineGenerator<T>::calculateNextTraceInterior(const gridCellOutlineSegment& lastSegment,
																				 brogueCompass& traceDirection,
							                                                     regionOutlineGrid<T>* algorithmGrid)
    {
        // Create a CLOCKWISE trace of an interior edge favoring CROTCH points. This will separate locations
        // that touch at the corners. 
        //

        gridCellOutlineSegment nextSegment = default_value::value<gridCellOutlineSegment>();

        switch (traceDirection)
        {
            case brogueCompass::N:
            {
                // Tracing NORTH on a EAST face

                // Is the current segment's endpoint a CORNER or a CROTCH point?
                int crotchColumn = lastSegment.vertex1.column + 1;
                int crotchRow = lastSegment.vertex1.row - 1;

                int cornerColumn = lastSegment.vertex1.column;
                int cornerRow = lastSegment.vertex1.row;

                // CROTCH
                if (algorithmGrid->hasSegment(crotchColumn, crotchRow, brogueCompass::S))
                {
                    // => Endpoint is a CROTCH point with a turn towards the EAST
                    //
                    traceDirection = brogueCompass::E;      // Tracing N -> Turn E

                    // DIRECTION:  N -> E turn on a CROTCH point => lastSegment.V1 (Equals) segment.V1
                    //
                    nextSegment = algorithmGrid->getSegment(crotchColumn, crotchRow, brogueCompass::S);
                }
                // CORNER
                else
                {
                    if (!algorithmGrid->hasSegment(cornerColumn, cornerRow, brogueCompass::N))
                        throw simpleException("Invalid outline winding:  regionOutlineGenerator::calculateNextTraceInterior");

                    // => Endpoint is a CORNER point which turns towards the WEST
                    //
                    traceDirection = brogueCompass::W;      // Tracing N -> Turn W

                    // DIRECTION:  N -> W turn on a CORNER point => lastSegment.V1 (Equals) segment.V2
                    //
                    nextSegment = algorithmGrid->getSegment(cornerColumn, cornerRow, brogueCompass::N);
                }
            }
            break;
            case brogueCompass::S:
            {
                // Tracing SOUTH on a WEST face

                // Is the current segment's endpoint a CORNER or a CROTCH point?
                int crotchColumn = lastSegment.vertex2.column - 1;
                int crotchRow = lastSegment.vertex2.row + 1;

                int cornerColumn = lastSegment.vertex2.column;
                int cornerRow = lastSegment.vertex2.row;

                // CROTCH
                if (algorithmGrid->hasSegment(crotchColumn, crotchRow, brogueCompass::N))
                {
                    // => Endpoint is a CROTCH point with a turn towards the WEST
                    //
                    traceDirection = brogueCompass::W;      // Tracing N -> Turn W

                    // DIRECTION:  S -> W turn on a CROTCH point => lastSegment.V2 (Equals) segment.V2
                    //
                    nextSegment = algorithmGrid->getSegment(crotchColumn, crotchRow, brogueCompass::N);
                }
                // CORNER
                else
                {
                    if (!algorithmGrid->hasSegment(cornerColumn, cornerRow, brogueCompass::S))
                        throw simpleException("Invalid outline winding:  regionOutlineGenerator::calculateNextTraceInterior");

                    // => Endpoint is a CORNER point which turns towards the EAST
                    //
                    traceDirection = brogueCompass::E;      // Tracing S -> Turn E

                    // DIRECTION:  S -> E turn on a CORNER point => lastSegment.V2 (Equals) segment.V1
                    //
                    nextSegment = algorithmGrid->getSegment(cornerColumn, cornerRow, brogueCompass::S);
                }
            }
            break;
            case brogueCompass::E:
            {
                // Tracing EAST on a SOUTH face

                // Is the current segment's endpoint a CORNER or a CROTCH point?
                int crotchColumn = lastSegment.vertex2.column + 1;
                int crotchRow = lastSegment.vertex2.row + 1;

                int cornerColumn = lastSegment.vertex2.column;
                int cornerRow = lastSegment.vertex2.row;

                // CROTCH
                if (algorithmGrid->hasSegment(crotchColumn, crotchRow, brogueCompass::W))
                {
                    // => Endpoint is a CROTCH point with a turn towards the SOUTH
                    //
                    traceDirection = brogueCompass::S;      // Tracing E -> Turn S

                    // DIRECTION:  E -> S turn on a CROTCH point => lastSegment.V2 (Equals) segment.V1
                    //
                    nextSegment = algorithmGrid->getSegment(crotchColumn, crotchRow, brogueCompass::W);
                }
                // CORNER
                else
                {
                    if (!algorithmGrid->hasSegment(cornerColumn, cornerRow, brogueCompass::E))
                        throw simpleException("Invalid outline winding:  regionOutlineGenerator::calculateNextTraceInterior");

                    // => Endpoint is a CORNER point which turns towards the NORTH
                    //
                    traceDirection = brogueCompass::N;      // Tracing E -> Turn N

                    // DIRECTION:  E -> N turn on a CORNER point => lastSegment.V2 (Equals) segment.V2
                    //
                    nextSegment = algorithmGrid->getSegment(cornerColumn, cornerRow, brogueCompass::E);
                }
            }
            break;
            case brogueCompass::W:
            {
                // Tracing WEST on a NORTH face

                // Is the current segment's endpoint a CORNER or a CROTCH point?
                int crotchColumn = lastSegment.vertex1.column - 1;
                int crotchRow = lastSegment.vertex1.row - 1;

                int cornerColumn = lastSegment.vertex1.column;
                int cornerRow = lastSegment.vertex1.row;

                // CROTCH
                if (algorithmGrid->hasSegment(crotchColumn, crotchRow, brogueCompass::E))
                {
                    // => Endpoint is a CROTCH point with a turn towards the NORTH
                    //
                    traceDirection = brogueCompass::N;      // Tracing W -> Turn N

                    // DIRECTION:  W -> N turn on a CROTCH point => lastSegment.V1 (Equals) segment.V2
                    //
                    nextSegment = algorithmGrid->getSegment(crotchColumn, crotchRow, brogueCompass::E);
                }
                // CORNER
                else
                {
                    if (!algorithmGrid->hasSegment(cornerColumn, cornerRow, brogueCompass::W))
                        throw simpleException("Invalid outline winding:  regionOutlineGenerator::calculateNextTraceInterior");

                    // => Endpoint is a CORNER point which turns towards the SOUTH
                    //
                    traceDirection = brogueCompass::S;      // Tracing W -> Turn S

                    // DIRECTION:  W -> S turn on a CORNER point => lastSegment.V1 (Equals) segment.V1
                    //
                    nextSegment = algorithmGrid->getSegment(cornerColumn, cornerRow, brogueCompass::W);
                }
            }
            break;
            case brogueCompass::NW:
            case brogueCompass::NE:
            case brogueCompass::SE:
            case brogueCompass::SW:
			case brogueCompass::None:
            default:
                throw simpleException("Invalid trace direction:  regionOutlineGenerator::calculateNextTraceInterior");
        }

        // Update the grid
        algorithmGrid->removeSegment(nextSegment);

        return nextSegment;
    }

    template<isGridLocator T>
    bool regionOutlineGenerator<T>::isSegmentFlipped(brogueCompass traceDirection)
    {
        // Check trace direction considering CLOCKWISE orientation
        //
        switch (traceDirection)
        {
            case brogueCompass::N:

                // CLOCKWISE:  Treat V1 as though it is V2
                return true;

            case brogueCompass::S:

                // CLOCKWISE:  Normal vertex orientation (V1 -> V2)
                return false;

            case brogueCompass::E:

                // CLOCKWISE:  Normal vertex orientation (V1 -> V2)
                return false;

            case brogueCompass::W:

                // CLOCKWISE:  Treat V1 as though it is V2
                return true;

            case brogueCompass::NW:
            case brogueCompass::NE:
            case brogueCompass::SE:
            case brogueCompass::SW:
            case brogueCompass::None:
            default:
                throw simpleException("Unhandled Compass direction:  regionOutlineGenerator.h");
        }
    }

    template<isGridLocator T>
    bool regionOutlineGenerator<T>::isTraceCompleted(const gridCellOutlineSegment& firstSegment, 
                                                     const gridCellOutlineSegment& lastSegment, 
                                                     bool interiorPolygon, 
                                                     brogueCompass lastDirection)
    {
        switch (lastDirection)
        {
            case brogueCompass::N:
            {
                if (interiorPolygon)
                {
                    return firstSegment.vertex2 == lastSegment.vertex1;
                }
                else
                {
                    return firstSegment.vertex1 == lastSegment.vertex1;
                }
            }
            case brogueCompass::S:
            {
                if (interiorPolygon)
                {
                    return firstSegment.vertex2 == lastSegment.vertex2;
                }
                else
                {
                    return firstSegment.vertex1 == lastSegment.vertex2;
                }
            }
            case brogueCompass::E:
            case brogueCompass::W:

                // If you try these on a piece of paper you'll see they're impossible
                //
                return false;
            default:
                throw simpleException("Invalid trace direction:  regionOutlineGenerator::isTraceCompleted");
        }
    }
}
