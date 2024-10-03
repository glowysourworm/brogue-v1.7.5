#pragma once

#include "roomGenerator.h"
#include "brogueCell.h"

#include "brogueMath.h"
#include "gridDefinitions.h"
#include "gridRegion.h"
#include "gridRegionConstructor.h"
#include "simpleList.h"

using namespace brogueHd::component;
using namespace brogueHd::component::math;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::generator
{
	roomGenerator::roomGenerator(noiseGenerator* noiseGenerator, randomGenerator* randomGenerator)
	{
		_randomGenerator = randomGenerator;
		_noiseGenerator = noiseGenerator;

		// This gets changed during generation
		gridRect paddedBoundary = gridRect(1, 1, DCOLS - 1, DROWS - 1);

		// Brogue v1.7.5
		float fillRatio = 0.55f;
		short birthCount = 6;
		short survivalCount = 4;
		short smoothingIterations = 5;	// This was a change:  Probably can get away with fewer iterations (was >= 10)

		// CA Algorithm Parameters:  These are for the cavern room (Brogue v1.7.5)
		_cavernParameters = new cellularAutomataParameters(paddedBoundary, fillRatio, birthCount, survivalCount, smoothingIterations);
	}

	roomGenerator::~roomGenerator()
	{
		delete _cavernParameters;
	}

	void roomGenerator::designCavern(gridRegionConstructor<gridLocator>& regionConstructor,
										const gridRect& parentBoundary,
										const gridRect& relativeBoundary,
										const gridRect& minSize,
										const gridRect& maxSize)
	{
		grid<gridLocator> designGrid(parentBoundary, relativeBoundary);
		gridRegionLocator<gridLocator> regionLocator;

		// Create cellular automata using cavern parameters
		_noiseGenerator->cellularAutomata(*_cavernParameters, [&designGrid](short column, short row, bool result)
		{			
			if (result)
				designGrid.set(column, row, brogueCell(column, row));

			return iterationCallback::iterate;
		});

		// Locate regions using flood fill method:  Find the max area region
		gridRegion<gridLocator>* maxRegion;
		gridRegion<gridLocator>* translatedRegion;
		simpleList<gridRegion<gridLocator>*> validRegions;

		// (MEMORY) Locate Regions
		simpleList<gridRegion<gridLocator>*> regions = regionLocator.locateRegions(designGrid);

		// Filter regions to comply to size constraints
		validRegions = regions.where([&minSize, &maxSize](gridRegion<gridLocator>* region)
		{
			return region->getBoundary().width >= minSize.width &&
				   region->getBoundary().width <= maxSize.width &&
				   region->getBoundary().height >= minSize.height &&
				   region->getBoundary().height <= maxSize.height;
		});

		// Select largest area region from valid regions
		maxRegion = validRegions.withMax<short>([](gridRegion<gridLocator>* region)
		{
			return region->getBoundary().area();
		});

		// Position the new cave in the middle of the grid...
		gridRect blobBoundary = maxRegion->getBoundary();
		gridRect caveBoundary = gridRect(maxSize.width - blobBoundary.width / 2,
										 maxSize.height - blobBoundary.height / 2,
										 blobBoundary.width,
										 blobBoundary.height);

		gridLocator translation(caveBoundary.column - blobBoundary.column, caveBoundary.row - blobBoundary.row);

		// Translates the region to a NEW REGION (MEMORY!)
		translatedRegion = gridRegionConstructor<gridLocator>::translate(*maxRegion, translation);
		
		// ...and copy it to the master grid.
		translatedRegion->iterateLocations([&regionConstructor](short column, short row, gridLocator cell)
		{
			regionConstructor.add(column, row, cell);

			return iterationCallback::iterate;
		});

		// (MEMORY) Don't need the region containers. Data for locators is alive in the regionConstructor.
		regions.forEach([](gridRegion<gridLocator>* region)
		{
			delete region;

			return iterationCallback::iterate;
		});

		// (MEMORY) Don't forget the region created by the translation!
		delete translatedRegion;
	}

	void roomGenerator::designEntranceRoom(gridRegionConstructor<gridLocator>& regionConstructor)
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

		gridRect room1(roomX, roomY, roomWidth, roomHeight);
		gridRect room2(roomX2, roomY2, roomWidth2, roomHeight2);

		// Add Room 1
		room1.iterate([&regionConstructor](short column, short row)
		{
			regionConstructor.add(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Add Room 2
		room2.iterate([&regionConstructor](short column, short row)
		{
			regionConstructor.add(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designCrossRoom(gridRegionConstructor<gridLocator>& regionConstructor)
	{
		short roomWidth, roomHeight, roomWidth2, roomHeight2, roomX, roomY, roomX2, roomY2;

		roomWidth = _randomGenerator->rand_range(3, 12);
		roomX = _randomGenerator->rand_range(brogueMath<short>::max(0, DCOLS / 2 - (roomWidth - 1)), brogueMath<short>::min(DCOLS, DCOLS / 2));
		roomWidth2 = _randomGenerator->rand_range(4, 20);
		roomX2 = (roomX + (roomWidth / 2) + _randomGenerator->rand_range(0, 2) + _randomGenerator->rand_range(0, 2) - 3) - (roomWidth2 / 2);

		roomHeight = _randomGenerator->rand_range(3, 7);
		roomY = (DROWS / 2 - roomHeight);

		roomHeight2 = _randomGenerator->rand_range(2, 5);
		roomY2 = (DROWS / 2 - roomHeight2 - (_randomGenerator->rand_range(0, 2) + _randomGenerator->rand_range(0, 1)));

		gridRect room1(roomX - 5, roomY + 5, roomWidth, roomHeight);
		gridRect room2(roomX2 - 5, roomY2 + 5, roomWidth2, roomHeight2);

		// Add Room 1
		room1.iterate([&regionConstructor](short column, short row)
		{
			regionConstructor.add(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Add Room 2
		room2.iterate([&regionConstructor](short column, short row)
		{
			regionConstructor.add(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designSymmetricalCrossRoom(gridRegionConstructor<gridLocator>& regionConstructor)
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

		gridRect room1((DCOLS - majorWidth) / 2, (DROWS - minorHeight) / 2, majorWidth, minorHeight);
		gridRect room2((DCOLS - minorWidth) / 2, (DROWS - majorHeight) / 2, minorWidth, majorHeight);

		// Add Room 1
		room1.iterate([&regionConstructor](short column, short row)
		{
			regionConstructor.add(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Add Room 2
		room2.iterate([&regionConstructor](short column, short row)
		{
			regionConstructor.add(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designSmallRoom(gridRegionConstructor<gridLocator>& regionConstructor)
	{
		short width, height;

		width = _randomGenerator->rand_range(3, 6);
		height = _randomGenerator->rand_range(2, 4);

		gridRect room((DCOLS - width) / 2, (DROWS - height) / 2, width, height);

		// Add Room
		room.iterate([&regionConstructor](short column, short row)
		{
			regionConstructor.add(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designCircularRoom(gridRegionConstructor<gridLocator>& regionConstructor, const gridRect& parentBoundary, const gridRect& relativeBoundary)
	{
		grid<bool> setGrid(parentBoundary, relativeBoundary);
		short radius;

		// 5% Chance
		if (_randomGenerator->next() >= 0.05)
		{
			radius = _randomGenerator->rand_range(4, 10);
		}
		else
		{
			radius = _randomGenerator->rand_range(2, 4);
		}

		//drawCircleOnGrid(grid, DCOLS / 2, DROWS / 2, radius, 1);

		gridRect setRect = gridRect::fromCircle(DCOLS / 2, DROWS / 2, radius, radius);

		setRect.iterateInCircle([& setGrid](short column, short row)
		{
			setGrid.set(column, row, true);

			return iterationCallback::iterate;
		});

		//if (radius > 6
		//    && rand_percent(50)) {
		//    drawCircleOnGrid(grid, DCOLS / 2, DROWS / 2, rand_range(3, radius - 3), 0);
		//}

		// 50%
		if (radius > 6 && _randomGenerator->next() >= 0.5)
		{
			// TODO:  FIGURE OUT ZERO VALUE! (last parameter from above. probably chasm)

			short removeRadius = _randomGenerator->rand_range(3, radius - 3);
			gridRect removeRect = gridRect::fromCircle(DCOLS / 2 - removeRadius, DCOLS / 2 - removeRadius, removeRadius, removeRadius);

			setRect.iterateInCircle([&setGrid](short column, short row)
			{
				setGrid.set(column, row, false);

				return iterationCallback::iterate;
			});
		}
	}

	void roomGenerator::designChunkyRoom(gridRegionConstructor<gridLocator>& regionConstructor, const gridRect& parentBoundary, const gridRect& relativeBoundary)
	{
		short x, y;
		short minX, maxX, minY, maxY;
		short chunkCount = _randomGenerator->rand_range(2, 8);

		grid<bool> setGrid(parentBoundary, parentBoundary);

		// Set small room
		gridRect(0, 0, 2, 2).iterateInCircle([&setGrid, &regionConstructor](short column, short row)
		{
			setGrid.set(column, row, true);
			regionConstructor.add(column, row, gridLocator(column, row));
			return iterationCallback::iterate;
		});

		minX = DCOLS / 2 - 3;
		maxX = DCOLS / 2 + 3;
		minY = DROWS / 2 - 3;
		maxY = DROWS / 2 + 3;

		for (short i = 0; i < chunkCount; i++)
		{
			x = _randomGenerator->rand_range(minX, maxX);
			y = _randomGenerator->rand_range(minY, maxY);

			if (setGrid.isDefined(x, y))
			{
				//            colorOverDungeon(&darkGray);
				//            hiliteGrid(grid, &white, 100);

				gridRect(x, y, 2, 2).iterateInCircle([&setGrid, &regionConstructor](short column, short row)
				{
					setGrid.set(column, row, true);
					regionConstructor.add(column, row, gridLocator(column, row));
					return iterationCallback::iterate;
				});

				minX = brogueMath<short>::max(1, brogueMath<short>::min(x - 3, minX));
				maxX = brogueMath<short>::min(DCOLS - 2, brogueMath<short>::max(x + 3, maxX));
				minY = brogueMath<short>::max(1, brogueMath<short>::min(y - 3, minY));
				maxY = brogueMath<short>::min(DROWS - 2, brogueMath<short>::max(y + 3, maxY));

				//            hiliteGrid(grid, &green, 50);
				//            temporaryMessage("Added a chunk:", true);
			}
		}
	}
}