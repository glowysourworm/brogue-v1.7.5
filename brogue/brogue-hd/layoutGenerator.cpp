#include "layoutGenerator.h"
#include "randomGenerator.h"
#include "gridDefinitions.h"
#include "dungeonConstants.h"
#include "dungeon.h"
#include "delaunayAlgorithm.h"
#include "primsAlgorithm.h"
#include "vectorExtension.h"
#include "exceptionHandler.h"

namespace brogueHd::backend::generator
{
	layoutGenerator::layoutGenerator(randomGenerator* randomGenerator)
	{
        if(!randomGenerator->checkId(RANDOM_GENERATOR_MAIN))
            brogueException::show("Improper use of random number generator:  layoutGenerator.cpp");

        _roomTiles = new std::vector<accretionTile>();
		_randomGenerator = randomGenerator;
        _initialized = false;
	}
	layoutGenerator::~layoutGenerator()
	{
        this->clear();

        delete _roomTiles;

        _initialized = false;
	}

    void layoutGenerator::clear()
    {
        _roomTiles->clear();
        
        delete _grid;

        _initialized = false;
    }

    void layoutGenerator::initialize(const brogueLevelProfile& profile)
    {        
        this->clear();

        gridRect levelBoundary = this->getPaddedBoundary(0);
        gridRect levelPaddedBoundary = this->getPaddedBoundary(1);

        _profile = profile;
        _grid = new grid<gridLocator>(levelBoundary, levelPaddedBoundary);

        _initialized = true;
    }

    gridRect layoutGenerator::getPaddedBoundary(short padding) const
    {
        if (padding >= 10)
            brogueException::show("Invalid level padding:  must be <= 10");

        return gridRect(padding, padding, DCOLS - padding, DROWS - padding);
    }

    brogueLayout* layoutGenerator::generateLayout()
	{
        if (!_initialized)
            brogueException::show("Layout generator not properly intiialized:  call initialize() before generateLayout()");

        // Procedure
        //
        // 1) Create Rooms:
        //      -> Start with entrance
        //      -> Create room desried connection points using edge nearest-neighbors 
        //          ** These will be used later
        //          ** Check for Brogue v1.7.5 "attachHallway" (only in special case)
        //      -> Accrete (bolt-on) the rooms using the desired connection points
        //      -> Repeat for 35 attempts (Brogue v1.7.5)
        // 
        // 2) Designate Machine Rooms
        //      -> TODO: Choose based on room based on the above
        //
        // 3) Triangulate Rooms
        //      -> Create Delaunay Triangulation of connection points
        //      -> Remove Uniform[0,1] random corridors back towards
        //         Minimum Spanning Tree (MST)
        //      -> Creates Room Graph
        //
        // 4) Connect Rooms
        //      -> Use Dijkstra's map to complete corridors (fixes any gaps)
        //
        // 5) Create Terrain (including chasms)
        //      -> Create all terrain using terrain constructor
        //      -> (Honor masks, leave passage lanes around the edges)
        //
        // 6) Create Distance Map
        //      -> Starting at stairwell
        //      -> Store data for querying location cost
        //         (Stairs down will be in the highest room cost)
        // 
        // 7) Create Passage Map
        //      -> Use Dijkstra's map to move between rooms
        //          -> Complete machines
        //          -> Mark number of steps on each tile (choke points)
        //          -> Store the results (adds to difficulty rating of cells)
        //
       
        // Create Rooms
        this->createRooms();

        // Machine Rooms

        // Triangulate Rooms:  Creates Delaunay Triangulation of the connection point vertices
        graph<gridLocatorNode, gridLocatorEdge> delaunayGraph = this->triangulateRooms();


	}

    void layoutGenerator::createRooms()
    {
        // Brogue v1.7.5 Creates up to 35 rooms using accretion (bolt-on) to fill up the space
        //

        short maxAttempts = 35;
        short interRoomPadding = 1;
        std::vector<accretionTile> attemptRegions(maxAttempts);

        // 1) Create 35 room (up front), 2) Attempt bolt-on until there are no more fits
        //
        for (short index = 0; index < maxAttempts; index++)
        {
            // MEMORY!
            gridRegion<gridLocator>* nextRegion = index == 0 ? this->createRoom(_profile.getEntranceRoom()) :
                                                               this->createRoom(_profile.getRandomRoomInfo());

            // Connection Points
            std::vector<gridLocator> northEdge = nextRegion->getBoundaryEdges(brogueCompass::N);
            std::vector<gridLocator> southEdge = nextRegion->getBoundaryEdges(brogueCompass::S);
            std::vector<gridLocator> eastEdge = nextRegion->getBoundaryEdges(brogueCompass::E);
            std::vector<gridLocator> westEdge = nextRegion->getBoundaryEdges(brogueCompass::W);

            int randN = _randomGenerator->rand_range(0, northEdge.size());
            int randS = _randomGenerator->rand_range(0, southEdge.size());
            int randE = _randomGenerator->rand_range(0, eastEdge.size());
            int randW = _randomGenerator->rand_range(0, westEdge.size());

            accretionTile nextRoom;

            nextRoom.region = nextRegion;
            nextRoom.connectionPointN = northEdge[randN];
            nextRoom.connectionPointS = southEdge[randS];
            nextRoom.connectionPointE = eastEdge[randE];
            nextRoom.connectionPointW = westEdge[randW];

            attemptRegions.push_back(nextRoom);
        }

        // Bolt-on
        for (int index = 0; index < attemptRegions.size(); index++)
        {
            // First Room
            if (index == 0)
            {
                // NOTE:  Rooms are generated in their supposed 1st-attempt location. This may
                //        have been by design (Brogue v1.7.5); but probably only matters for
                //        the first level.
                //
                _roomTiles->push_back(attemptRegions[index]);

                // Set final room locations
                _grid->setFromRegion(attemptRegions[index].region);
            }

            // Accrete rooms - translating the attempt room into place (modifies attempt region)
            else if (attemptConnection(attemptRegions[index], _grid->getParentBoundary(), interRoomPadding))
            {
                _roomTiles->push_back(attemptRegions[index]);

                // Set final room locations
                _grid->setFromRegion(attemptRegions[index].region);
            }
        }


        //if (doorSites) {
        //    chooseRandomDoorSites(grid, doorSites);
        //    if (attachHallway) {
        //        dir = rand_range(0, 3);
        //        for (i = 0; doorSites[dir][0] == -1 && i < 3; i++) {
        //            dir = (dir + 1) % 4; // Each room will have at least 2 valid directions for doors.
        //        }
        //        attachHallwayTo(grid, doorSites);
        //    }
        //}

        //attachRooms(grid, &theDP, 35, 35);
    }

    bool layoutGenerator::attemptConnection(accretionTile& roomTile, const gridRect& attemptRect, short interRoomPadding) const
    {
        // Procedure
        //
        // 1) Combinatorically attempt connection points
        //      -> Start at beginning of room tile vector
        //      -> Test each possibility
        //          1) fits inside attemptRect
        //          2) no overlapping region locations
        // 
        // Success: Update room tile; Update attemptRect
        // 
        // Fail:    (TODO) Try positioning the tile using
        //                 padding.
        //
        
        for (int index = 0; index < _roomTiles->size(); index++)
        {
            accretionTile tile = _roomTiles->at(index);

            bool northAttempt = false;
            bool southAttempt = false;
            bool eastAttempt = false;
            bool westAttempt = false;

            for (int tileAttempts = 1; tileAttempts <= 4; tileAttempts++)
            {
                gridLocator translation = gridLocator::getEmpty();

                // Left, Top, Right, Bottom
                if (!roomTile.hasEastConnection && !tile.hasWestConnection && !westAttempt)
                {
                    translation = tile.connectionPointW.subtract(roomTile.connectionPointE);

                    // Adjust the connection point by one cell + padding
                    translation.column -= (1 + interRoomPadding);

                    westAttempt = true;
                }

                else if (!roomTile.hasNorthConnection && !tile.hasSouthConnection && !northAttempt)
                {
                    translation = tile.connectionPointN.subtract(roomTile.connectionPointS);

                    // Adjust the connection point by one cell + padding
                    translation.row -= (1 + interRoomPadding);

                    northAttempt = true;
                }

                else if (!roomTile.hasEastConnection && !tile.hasWestConnection && !eastAttempt)
                {
                    translation = tile.connectionPointE.subtract(roomTile.connectionPointW);

                    // Adjust the connection point by one cell + padding
                    translation.column += (1 + interRoomPadding);

                    eastAttempt = true;
                }

                else if (!roomTile.hasSouthConnection && !tile.hasNorthConnection && !southAttempt)
                {
                    translation = tile.connectionPointS.subtract(roomTile.connectionPointN);

                    // Adjust the connection point by one cell + padding
                    translation.row += (1 + interRoomPadding);

                    southAttempt = true;
                }
                else
                    break;

                // Create copy to translate (USE HEAP IN CASE IT WORKS)
                gridRegion<gridLocator>* translatedRegion = new gridRegion(*roomTile.region);

                // Translate the region into position
                translatedRegion->translate(translation);

                // Check the boundary:  1) outer gridRect first to reduce iteration, and 2) the grid overlap per cell
                if (attemptRect.contains(translatedRegion->getBoundary()))
                {
                    if (tile.region->overlaps(*translatedRegion))
                    {
                        delete translatedRegion;
                        continue;
                    }

                    bool gridOverlaps = false;
                    grid<gridLocator>* grid = _grid;

                    // Double check the already-set rooms
                    translatedRegion->iterateLocations([&grid, &gridOverlaps](gridLocator item)
                    {
                        if (grid->isDefined(item.column, item.row))
                        {
                            gridOverlaps = true;
                            return iterationCallback::breakAndReturn;
                        }
                    });

                    if (gridOverlaps)
                    {
                        delete translatedRegion;
                        continue;
                    }

                    roomTile.region = translatedRegion;

                    // Set connection point data (should be using a direction iterator) [1, 4] -> [W,N,E,S]
                    if (index == 1)
                    {
                        roomTile.connectionPointE = tile.connectionPointW;
                        roomTile.hasEastConnection = true;
                    }
                    else if (index == 2)
                    {
                        roomTile.connectionPointS= tile.connectionPointN;
                        roomTile.hasSouthConnection = true;
                    }
                    else if (index == 3)
                    {
                        roomTile.connectionPointW = tile.connectionPointE;
                        roomTile.hasWestConnection = true;
                    }
                    else
                    {
                        roomTile.connectionPointN = tile.connectionPointS;
                        roomTile.hasNorthConnection = true;
                    }

                    return true;
                }
            }
        }
    }

    gridRegion<gridLocator>* layoutGenerator::createRoom(gridRect levelBoundary, gridRect levelPaddedBoundary, const brogueRoomInfo& roomInfo)
    {
        gridRect minSize, maxSize;

        // Tracks the cell additions to the grid
        //
        gridRegionConstructor<gridLocator> constructor(levelBoundary, [](short column, short row, gridLocator locator)
        {
            // Inclusion Predicate
            return true;
        });

        switch (roomInfo.type)
        {
        case roomTypes::CrossRoom:
            _roomGenerator->designCrossRoom(constructor);
            break;
        case roomTypes::SmallSymmetricalCrossRoom:
            _roomGenerator->designSymmetricalCrossRoom(constructor);
            break;
        case roomTypes::SmallRoom:
            _roomGenerator->designSmallRoom(constructor);
            break;
        case roomTypes::CircularRoom:
            _roomGenerator->designCircularRoom(constructor, levelBoundary, levelPaddedBoundary);
            break;
        case roomTypes::ChunkyRoom:
            _roomGenerator->designChunkyRoom(constructor, levelBoundary, levelPaddedBoundary);
            break;
        case roomTypes::Cave:

            switch (_randomGenerator->rand_range(0, 2))
            {
            case 0:
                // Compact cave room.
                minSize.width = 3;
                minSize.height = 4;
                maxSize.width = 12;
                maxSize.height = 8;
                break;
            case 1:
                // Large north-south cave room.
                minSize.width = 3;
                minSize.height = 15;
                maxSize.width = 12;
                maxSize.height = DROWS - 2;
                break;
            case 2:
                // Large east-west cave room.
                minSize.width = 20;
                minSize.height = 4;
                maxSize.width = DROWS - 2;
                maxSize.height = 8;
                break;
            default:
                break;
            }
            break;

            _roomGenerator->designCavern(constructor, levelBoundary, levelPaddedBoundary, minSize, maxSize);

        case roomTypes::Cavern:

            minSize.width = CAVE_MIN_WIDTH;
            minSize.height = CAVE_MIN_HEIGHT;
            maxSize.width = DCOLS - 2;
            maxSize.height = DROWS - 2;

            _roomGenerator->designCavern(constructor, levelBoundary, levelPaddedBoundary, minSize, maxSize);

            break;
        case roomTypes::MainEntranceRoom:
            _roomGenerator->designEntranceRoom(constructor);
            break;
        default:
            break;
        }

        return constructor.complete();
    }

    graph<gridLocatorNode, gridLocatorEdge> layoutGenerator::triangulateRooms()
    {
        // Create delaunay triangulator with graph edge constructor
        delaunayAlgorithm<gridLocatorNode, gridLocatorEdge> triangulator([](gridLocatorNode node1, gridLocatorNode node2)
        {
            return gridLocatorEdge(node1, node2);
        });

        // Create connection point vertices
        std::vector<gridLocatorNode> connectionNodes;
        
        vectorExtension<accretionTile>::forEach(*_roomTiles, [&connectionNodes](accretionTile item)
        {
            if (item.hasEastConnection)
                connectionNodes.push_back(gridLocatorNode(item.connectionPointE));

            if (item.hasNorthConnection)
                connectionNodes.push_back(gridLocatorNode(item.connectionPointN));

            if (item.hasSouthConnection)
                connectionNodes.push_back(gridLocatorNode(item.connectionPointS));

            if (item.hasWestConnection)
                connectionNodes.push_back(gridLocatorNode(item.connectionPointW));
        });

        return triangulator.run(connectionNodes);
    }
}