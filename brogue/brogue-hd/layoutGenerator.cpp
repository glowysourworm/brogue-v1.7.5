#include "layoutGenerator.h"
#include "randomGenerator.h"
#include "gridDefinitions.h"
#include "dungeonConstants.h"
#include "dungeon.h"
#include "vectordef.h"

namespace brogueHd
{
	layoutGenerator::layoutGenerator(brogueLevel* level, randomGenerator* randomGenerator)
	{
        brogueAssert(randomGenerator->checkId(RANDOM_GENERATOR_MAIN))

        _level = level;
		_randomGenerator = randomGenerator;
	}
	layoutGenerator::~layoutGenerator()
	{
        
	}


	void layoutGenerator::generateDungeon()
	{
		dungeonProfile profile, firstRoomProfile;

		// Select dungeon profiles
		profile = dungeonProfileCatalog[DP_BASIC];
		firstRoomProfile = dungeonProfileCatalog[DP_BASIC_FIRST_ROOM];

		// Adjust profiles for depth
		profile.adjustDungeonProfileForDepth(_level->getDepth());
		firstRoomProfile.adjustDungeonFirstRoomProfileForDepth(_level->getDepth());

		designRandomRoom(firstRoomProfile);

		if (D_INSPECT_LEVELGEN) {
			//colorOverDungeon(&darkGrayBrogue);
			//hiliteGrid(grid, &whiteBrogue, 100);
			//temporaryMessage("First room placed:", true);
		}

		attachRooms(grid, &theDP, 35, 35);
	}

	void layoutGenerator::designRandomRoom(dungeonProfile roomProfile)
	{
        switch (_randomGenerator->randWeighted(roomProfile.roomFrequencies)) 
        {
        case 0:
            designCrossRoom(grid);
            break;
        case 1:
            designSymmetricalCrossRoom(grid);
            break;
        case 2:
            designSmallRoom(grid);
            break;
        case 3:
            designCircularRoom(grid);
            break;
        case 4:
            designChunkyRoom(grid);
            break;
        case 5:
            switch (_randomGenerator->rand_range(0, 2)) {
            case 0:
                designCavern(roomProfile, 3, 12, 4, 8); // Compact cave room.
                break;
            case 1:
                designCavern(roomProfile, 3, 12, 15, DROWS - 2); // Large north-south cave room.
                break;
            case 2:
                designCavern(roomProfile, 20, DROWS - 2, 4, 8); // Large east-west cave room.
                break;
            default:
                break;
            }
            break;
        case 6:
            designCavern(roomProfile, CAVE_MIN_WIDTH, DCOLS - 2, CAVE_MIN_HEIGHT, DROWS - 2);
            break;
        case 7:
            designEntranceRoom();
            break;
        default:
            break;
        }

        if (doorSites) {
            chooseRandomDoorSites(grid, doorSites);
            if (attachHallway) {
                dir = rand_range(0, 3);
                for (i = 0; doorSites[dir][0] == -1 && i < 3; i++) {
                    dir = (dir + 1) % 4; // Each room will have at least 2 valid directions for doors.
                }
                attachHallwayTo(grid, doorSites);
            }
        }
	}

    void layoutGenerator::designCavern(dungeonProfile roomProfile, short minWidth, short minHeight, short maxWidth, short maxHeight)
    {       
        float fillRatio = 0.55;                                     
        short smoothingIterations = 5;                              

        // Grid "on" value
        short fillValue = 1;

        grid<short>* blobGrid = new grid<short>(DCOLS, DROWS, 0, fillValue);

        // Boundary for cellular automata to process:  Leaves a single cell border
        gridRect boundary = gridRect(1, 1, DCOLS - 1, DROWS - 1);

        // CA Algorithm Parameters
        cellularAutomataParameters parameters = cellularAutomataParameters(boundary, fillValue, fillRatio, "ffffffttt", "ffffttttt", smoothingIterations);

        // Create Blob on the grid
        blobGrid->generateCellularAutomata(_randomGenerator, parameters);

        // Locate regions using flood fill method
        std::vector<gridRegion<short>*> regions = blobGrid->locateRegions([fillValue](short value)
        {
            return value == fillValue;

        }, fillValue);

        // Filter regions to comply to size constraints
        std::vector<gridRegion<short>*> validRegions = collection::where(regions, [minWidth, minHeight, maxWidth, maxHeight](bool(gridRegion<short>* region))
        {
            return region->getBoundary().width >= minWidth &&
                   region->getBoundary().width <= maxWidth &&
                   region->getBoundary().height >= minHeight &&
                   region->getBoundary().height <= maxHeight;
        });

        // Select largest area region from valid regions
        gridRegion<short>* maxRegion = collection::max(validRegions, [](gridRegion<short>* region)
        {
            return region->getBoundary().area();
        });

        // Position the new cave in the middle of the grid...
        gridRect blobBoundary = maxRegion->getBoundary();
        gridRect caveBoundary = gridRect(maxWidth - blobBoundary.width / 2,
                                         maxHeight - blobBoundary.height / 2,
                                         blobBoundary.width,
                                         blobBoundary.height);


        // ...and copy it to the master grid.
        _level->levelGrid->addCells(maxRegion, [fillValue](short value)
        {
            return value == fillValue;
        });
    }

    // This is a special room that appears at the entrance to the dungeon on depth 1.
    void layoutGenerator::designEntranceRoom(dungeonProfile roomProfile)
    {
        short roomWidth, roomHeight, roomWidth2, roomHeight2, roomX, roomY, roomX2, roomY2;

        roomWidth = 8;
        roomHeight = 10;
        roomWidth2 = 20;
        roomHeight2 = 5;
        roomX = DCOLS / 2 - roomWidth / 2 - 1;
        roomY = DROWS - roomHeight - 2;
        roomX2 = DCOLS / 2 - roomWidth2 / 2 - 1;
        roomY2 = DROWS - roomHeight2 - 2;

        //drawRectangleOnGrid(grid, roomX, roomY, roomWidth, roomHeight, 1);
        //drawRectangleOnGrid(grid, roomX2, roomY2, roomWidth2, roomHeight2, 1);

        _level->levelGrid->addCells(gridRect(roomX, roomY, roomWidth, roomHeight));
        _level->levelGrid->addCells(gridRect(roomX2, roomY2, roomWidth2, roomHeight2));
    }

    void layoutGenerator::designCrossRoom(dungeonProfile roomProfile)
    {
        short roomWidth, roomHeight, roomWidth2, roomHeight2, roomX, roomY, roomX2, roomY2;

        roomWidth = _randomGenerator->rand_range(3, 12);
        roomX = _randomGenerator->rand_range(max(0, DCOLS / 2 - (roomWidth - 1)), min(DCOLS, DCOLS / 2));
        roomWidth2 = _randomGenerator->rand_range(4, 20);
        roomX2 = (roomX + (roomWidth / 2) + _randomGenerator->rand_range(0, 2) + _randomGenerator->rand_range(0, 2) - 3) - (roomWidth2 / 2);

        roomHeight = _randomGenerator->rand_range(3, 7);
        roomY = (DROWS / 2 - roomHeight);

        roomHeight2 = _randomGenerator->rand_range(2, 5);
        roomY2 = (DROWS / 2 - roomHeight2 - (_randomGenerator->rand_range(0, 2) + _randomGenerator->rand_range(0, 1)));

        //drawRectangleOnGrid(grid, roomX - 5, roomY + 5, roomWidth, roomHeight, 1);
        //drawRectangleOnGrid(grid, roomX2 - 5, roomY2 + 5, roomWidth2, roomHeight2, 1);

        _level->levelGrid->addCells(gridRect(roomX - 5, roomY + 5, roomWidth, roomHeight));
        _level->levelGrid->addCells(gridRect(roomX2 - 5, roomY2 + 5, roomWidth2, roomHeight2));
    }

    void layoutGenerator::designSymmetricalCrossRoom(brogueLevel* level, dungeonProfile roomProfile) 
    {
        short majorWidth, majorHeight, minorWidth, minorHeight;

        majorWidth = _randomGenerator->rand_range(4, 8);
        majorHeight = _randomGenerator->rand_range(4, 5);

        minorWidth = _randomGenerator->rand_range(3, 4);
        if (majorHeight % 2 == 0) {
            minorWidth -= 1;
        }
        minorHeight = 3;//rand_range(2, 3);
        if (majorWidth % 2 == 0) {
            minorHeight -= 1;
        }

        //drawRectangleOnGrid(grid, (DCOLS - majorWidth) / 2, (DROWS - minorHeight) / 2, majorWidth, minorHeight, 1);
        //drawRectangleOnGrid(grid, (DCOLS - minorWidth) / 2, (DROWS - majorHeight) / 2, minorWidth, majorHeight, 1);

        level->levelGrid->addCells(gridRect((DCOLS - majorWidth) / 2, (DROWS - minorHeight) / 2, majorWidth, minorHeight));
        level->levelGrid->addCells(gridRect((DCOLS - minorWidth) / 2, (DROWS - majorHeight) / 2, minorWidth, majorHeight));
    }

    void layoutGenerator::designSmallRoom(brogueLevel* level, dungeonProfile roomProfile) 
    {
        short width, height;

        width = _randomGenerator->rand_range(3, 6);
        height = _randomGenerator->rand_range(2, 4);
        
        //drawRectangleOnGrid(grid, (DCOLS - width) / 2, (DROWS - height) / 2, width, height, 1);

        level->levelGrid->addCells(gridRect((DCOLS - width) / 2, (DROWS - height) / 2, width, height));
    }

    void layoutGenerator::designCircularRoom(brogueLevel* level, dungeonProfile roomProfile) 
    {
        short radius;

        if (_randomGenerator->rand_percent(5))
        {
            radius = _randomGenerator->rand_range(4, 10);
        }
        else 
        {
            radius = _randomGenerator->rand_range(2, 4);
        }

         //drawCircleOnGrid(grid, DCOLS / 2, DROWS / 2, radius, 1);

         level->levelGrid->addCellsInCircle(gridRect(0, 0, DCOLS / 2, DROWS / 2));

        //if (radius > 6
        //    && rand_percent(50)) {
        //    drawCircleOnGrid(grid, DCOLS / 2, DROWS / 2, rand_range(3, radius - 3), 0);
        //}

         if (radius > 6 && _randomGenerator->rand_percent(50))
         {
             // TODO:  FIGURE OUT ZERO VALUE! (last parameter from above. probably chasm)

             short removeRadius = _randomGenerator->rand_range(3, radius - 3);

             level->levelGrid->removeCellsInCircle(gridRect(DCOLS / 2, DROWS / 2, removeRadius, removeRadius));
         }
    }

    void layoutGenerator::designChunkyRoom(brogueLevel* level, dungeonProfile roomProfile)
    {
        short x, y;
        short minX, maxX, minY, maxY;
        short chunkCount = _randomGenerator->rand_range(2, 8);

        
        //drawCircleOnGrid(grid, DCOLS / 2, DROWS / 2, 2, 1);
        level->levelGrid->addCellsInCircle(gridRect(0, 0, 2, 2));

        minX = DCOLS / 2 - 3;
        maxX = DCOLS / 2 + 3;
        minY = DROWS / 2 - 3;
        maxY = DROWS / 2 + 3;

        for (short i = 0; i < chunkCount;i++) 
        {
            x = _randomGenerator->rand_range(minX, maxX);
            y = _randomGenerator->rand_range(minY, maxY);

            if (level->levelGrid->isDefined(x, y))
            {
                //            colorOverDungeon(&darkGray);
                //            hiliteGrid(grid, &white, 100);

                //drawCircleOnGrid(grid, x, y, 2, 1);
                level->levelGrid->addCellsInCircle(gridRect(x, y, 2, 2));

                minX = max(1, min(x - 3, minX));
                maxX = min(DCOLS - 2, max(x + 3, maxX));
                minY = max(1, min(y - 3, minY));
                maxY = min(DROWS - 2, max(y + 3, maxY));

                //            hiliteGrid(grid, &green, 50);
                //            temporaryMessage("Added a chunk:", true);
            }
        }
    }
}