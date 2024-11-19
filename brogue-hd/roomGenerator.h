#pragma once

#include "brogueGlobal.h"
#include "brogueUIBuilder.h"
#include "cellularAutomataParameters.h"
#include "dungeon.h"
#include "dungeonConstants.h"
#include "grid.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "gridRegion.h"
#include "gridRegionConstructor.h"
#include "gridRegionLocator.h"
#include "noiseGenerator.h"
#include "randomGenerator.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleList.h"
#include "simpleMath.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model;

namespace brogueHd::backend
{
	class roomGenerator
	{
	public:

		roomGenerator(brogueUIBuilder* uiBuilder, noiseGenerator* noiseGenerator, randomGenerator* randomGenerator);
		~roomGenerator();

		gridRegion<gridLocator>* designRoom(const brogueRoomInfo& configuration,
											const gridRect& parentBoundary,
											const gridRect& relativeBoundary);

	private:
		void designCavern(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& maxSize);
		void designEntranceRoom(grid<gridLocator>& designGrid);
		void designCrossRoom(grid<gridLocator>& designGrid);
		void designSymmetricalCrossRoom(grid<gridLocator>& designGrid);
		void designSmallRoom(grid<gridLocator>& designGrid);
		void designCircularRoom(grid<gridLocator>& designGrid);
		void designChunkyRoom(grid<gridLocator>& designGrid);

	private:

		noiseGenerator* _noiseGenerator;
		randomGenerator* _randomGenerator;
		brogueUIBuilder* _uiBuilder;

		cellularAutomataParameters* _cavernParameters;
	};

	roomGenerator::roomGenerator(brogueUIBuilder* uiBuilder, noiseGenerator* noiseGenerator, randomGenerator* randomGenerator)
	{
		_uiBuilder = uiBuilder;
		_randomGenerator = randomGenerator;
		_noiseGenerator = noiseGenerator;

		// Brogue v1.7.5
		float fillRatio = 0.55f;
		short birthCount = 6;
		short survivalCount = 4;
		short smoothingIterations = 5;	// This was a change:  Probably can get away with fewer iterations (was >= 10)

		// CA Algorithm Parameters:  These are for the cavern room (Brogue v1.7.5)
		_cavernParameters = new cellularAutomataParameters(fillRatio, birthCount, survivalCount, smoothingIterations);
	}

	roomGenerator::~roomGenerator()
	{
		delete _cavernParameters;
	}

	gridRegion<gridLocator>* roomGenerator::designRoom(const brogueRoomInfo& configuration,
														const gridRect& parentBoundary,
														const gridRect& relativeBoundary)
	{
		// Need a padded boundary for connection points
		gridRect paddedBoundary = _uiBuilder->getPaddedBoundary(relativeBoundary, 1);

		if (paddedBoundary.area() < 4)
			throw simpleException("Trying to design a room with less than the minimum required space of 4 tiles");

		grid<gridLocator> designGrid(parentBoundary, paddedBoundary);

		gridRect minSize = default_value::value<gridRect>();
		gridRect maxSize = default_value::value<gridRect>();

		switch (configuration.type)
		{
			case roomTypes::Cave:
			{
				switch (_randomGenerator->randomRange(0, 2))
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
						maxSize.height = paddedBoundary.height - 2;
						break;
					case 2:
						// Large east-west cave room.
						minSize.width = 20;
						minSize.height = 4;
						maxSize.width = paddedBoundary.height - 2;
						maxSize.height = 8;
						break;
					default:
						break;
				}

				designCavern(designGrid, minSize, maxSize);
			}
			break;
			case roomTypes::Cavern:
			{
				minSize.width = simpleMath::minOf(CAVE_MIN_WIDTH, paddedBoundary.width);
				minSize.height = simpleMath::minOf(CAVE_MIN_HEIGHT, paddedBoundary.height);
				maxSize.width = paddedBoundary.width;
				maxSize.height = paddedBoundary.height;

				designCavern(designGrid, minSize, maxSize);
			}
			break;
			case roomTypes::ChunkyRoom:
				designChunkyRoom(designGrid);
				break;
			case roomTypes::CircularRoom:
				designCircularRoom(designGrid);
				break;
			case roomTypes::CrossRoom:
				designCrossRoom(designGrid);
				break;
			case roomTypes::MainEntranceRoom:
				designEntranceRoom(designGrid);
				break;
			case roomTypes::SmallRoom:
				designSmallRoom(designGrid);
				break;
			case roomTypes::SmallSymmetricalCrossRoom:
				designCrossRoom(designGrid);
				break;
			default:
				throw simpleException("Unhandled room type:  roomGenerator::designRoom");
		}

		// Locate Regions:
		gridRegionLocator<gridLocator> regionLocator;

		// (MEMORY!)
		simpleList<gridRegion<gridLocator>*> regions = regionLocator.locateRegions(designGrid);

		if (regions.count() == 0)
			throw simpleException("Invalid room created by roomGenerator:  Must have at least one valid region");

		// Take region with max location count
		gridRegion<gridLocator>* region = regions.withMax<int>([] (gridRegion<gridLocator>* region)
		{
			return region->getLocationCount();
		});

		if (minSize != default_value::value<gridRect>() &&
			maxSize != default_value::value<gridRect>())
		{
			if (region->getBoundary().area() < minSize.area() ||
				region->getBoundary().area() > maxSize.area())
				throw simpleException("Invalid room created by roomGenerator:  Must fall within size constraints");
		}

		return region;
	}


	void roomGenerator::designCavern(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& maxSize)
	{
		gridRegionLocator<gridLocator> regionLocator;

		// Create cellular automata using cavern parameters
		_noiseGenerator->cellularAutomata(designGrid.getParentBoundary(), 
										  designGrid.getRelativeBoundary(), 
										  *_cavernParameters, 
		[&designGrid] (short column, short row, bool result)
		{
			if (result)
				designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Locate regions using flood fill method:  Find the max area region
		gridRegion<gridLocator>* maxRegion;
		simpleList<gridRegion<gridLocator>*> validRegions;

		// (MEMORY) Locate Regions
		simpleList<gridRegion<gridLocator>*> regions = regionLocator.locateRegions(designGrid);

		// Filter regions to comply to size constraints
		validRegions = regions.where([&minSize, &maxSize] (gridRegion<gridLocator>* region)
		{
			return region->getBoundary().width >= minSize.width &&
				region->getBoundary().width <= maxSize.width &&
				region->getBoundary().height >= minSize.height &&
				region->getBoundary().height <= maxSize.height;
		});

		// Valid CA result
		if (validRegions.count() > 0)
		{
			// Select largest area region from valid regions
			maxRegion = validRegions.withMax<short>([] (gridRegion<gridLocator>* region)
			{
				return region->getBoundary().area();
			});
		}
		else
		{
			gridRegionConstructor<gridLocator> constructor(designGrid.getParentBoundary(), true);
			gridRect defaultRect(((designGrid.getRelativeBoundary().width - minSize.width) / 2) + designGrid.getRelativeBoundary().column,
								 ((designGrid.getRelativeBoundary().height - minSize.height) / 2) + designGrid.getRelativeBoundary().row,
								 minSize.width,
								 minSize.height);
			
			defaultRect.iterate([&constructor] (short column, short row) 
			{
				constructor.add(column, row, gridLocator(column, row));

				return iterationCallback::iterate;
			});

			// (MEMORY!) This will be the default region
			maxRegion = constructor.complete();
		}

		// Position the new cave in the middle of the grid...
		gridRect blobBoundary = maxRegion->getBoundary();
		gridRect caveBoundary = gridRect(((designGrid.getRelativeBoundary().width - blobBoundary.width) / 2) + designGrid.getRelativeBoundary().column,
										 ((designGrid.getRelativeBoundary().height - blobBoundary.height) / 2) + designGrid.getRelativeBoundary().row,
										  blobBoundary.width,
										  blobBoundary.height);

		gridLocator translation(caveBoundary.column - blobBoundary.column, caveBoundary.row - blobBoundary.row);

		// Translates the region to a new location 
		maxRegion->translate_StackLike(translation.column, translation.row);

		// ...and copy it to the master grid.
		designGrid.iterate([&designGrid, &maxRegion] (short column, short row, const gridLocator& item)
		{
			if (!maxRegion->isDefined(column, row))
				designGrid.set(column, row, default_value::value<gridLocator>(), true);

			// Just force it to be defined (but should already be set from the CA algorithm)
			else
				designGrid.set(column, row, gridLocator(column, row), true);

			return iterationCallback::iterate;
		});

		// (MEMORY) Clean up heap memory
		for (int index = 0; index < regions.count(); index++)
		{
			if (maxRegion != regions.get(index))
				delete regions.get(index);
		}

		delete maxRegion;
	}

	void roomGenerator::designEntranceRoom(grid<gridLocator>& designGrid)
	{
		short roomWidth, roomHeight, roomWidth2, roomHeight2, roomX, roomY, roomX2, roomY2;

		roomWidth = 8;
		roomHeight = 10;
		roomWidth2 = 20;
		roomHeight2 = 5;
		roomX = (DCOLS / 2) - (roomWidth / 2) - 1;
		roomY = DROWS - roomHeight - 2;
		roomX2 = (DCOLS / 2) - (roomWidth2 / 2) - 1;
		roomY2 = DROWS - roomHeight2 - 2;

		gridRect room1(roomX, roomY, roomWidth, roomHeight);
		gridRect room2(roomX2, roomY2, roomWidth2, roomHeight2);

		// Add Room 1
		room1.iterate([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Add Room 2
		room2.iterate([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row), true);

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designCrossRoom(grid<gridLocator>& designGrid)
	{
		gridRect boundary = designGrid.getRelativeBoundary();

		short roomWidth, roomHeight, roomWidth2, roomHeight2, roomX, roomY, roomX2, roomY2;

		roomWidth = _randomGenerator->randomRange(3, 12);
		roomX = _randomGenerator->randomRange(simpleMath::maxOf(0, boundary.centerX() - roomWidth - 1), boundary.centerX());
		roomWidth2 = _randomGenerator->randomRange(4, 20);
		roomX2 = (roomX + (roomWidth / 2) + _randomGenerator->randomRange(0, 2) + _randomGenerator->randomRange(0, 2) - 3) - (roomWidth2 / 2);

		roomHeight = _randomGenerator->randomRange(3, 7);
		roomY = (boundary.centerY() - roomHeight);

		roomHeight2 = _randomGenerator->randomRange(2, 5);
		roomY2 = (boundary.centerY() - roomHeight2 - (_randomGenerator->randomRange(0, 2) + _randomGenerator->randomRange(0, 1)));

		gridRect room1(roomX - 5, roomY + 5, roomWidth, roomHeight);
		gridRect room2(roomX2 - 5, roomY2 + 5, roomWidth2, roomHeight2);

		// Add Room 1
		room1.iterate([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Add Room 2
		room2.iterate([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row), true);

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designSymmetricalCrossRoom(grid<gridLocator>& designGrid)
	{
		short majorWidth, majorHeight, minorWidth, minorHeight;

		majorWidth = _randomGenerator->randomRange(4, 8);
		majorHeight = _randomGenerator->randomRange(4, 5);

		minorWidth = _randomGenerator->randomRange(3, 4);
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
		room1.iterate([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Add Room 2
		room2.iterate([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row), true);

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designSmallRoom(grid<gridLocator>& designGrid)
	{
		short width, height;

		width = _randomGenerator->randomRange(3, 6);
		height = _randomGenerator->randomRange(2, 4);

		gridRect room((DCOLS - width) / 2, (DROWS - height) / 2, width, height);

		// Add Room
		room.iterate([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designCircularRoom(grid<gridLocator>& designGrid)
	{
		short radius;

		// 5% Chance
		if (_randomGenerator->next() >= 0.05)
		{
			radius = _randomGenerator->randomRange(4, 10);
		}
		else
		{
			radius = _randomGenerator->randomRange(2, 4);
		}

		gridRect minRect(designGrid.getRelativeBoundary().column,
						 designGrid.getRelativeBoundary().row,
						 radius, radius);

		minRect.iterateInCircle([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row), true);

			return iterationCallback::iterate;
		});

		//if (radius > 6
		//    && rand_percent(50)) {
		//    drawCircleOnGrid(grid, DCOLS / 2, DROWS / 2, rand_range(3, radius - 3), 0);
		//}

		//// 50%
		//if (radius > 6 && _randomGenerator->next() >= 0.5)
		//{
		//	// TODO:  FIGURE OUT ZERO VALUE! (last parameter from above. probably chasm)

		//	short removeRadius = _randomGenerator->randomRange(3, radius - 3);
		//	gridRect removeRect = gridRect::fromCircle((DCOLS / 2) + removeRadius, (DCOLS / 2) + removeRadius, removeRadius, removeRadius);

		//	setRect.iterateInCircle([&designGrid] (short column, short row)
		//	{
		//		designGrid.set(column, row, default_value::value<gridLocator>(), true);

		//		return iterationCallback::iterate;
		//	});
		//}
	}

	void roomGenerator::designChunkyRoom(grid<gridLocator>& designGrid)
	{
		gridRect minRect(designGrid.getRelativeBoundary().column,
						 designGrid.getRelativeBoundary().row,
						 2, 2);

		short chunkCount = _randomGenerator->randomRange(2, 8);

		for (short i = 0; i < chunkCount; i++)
		{
			int offsetX = _randomGenerator->randomRange(0, 5);
			int offsetY = _randomGenerator->randomRange(0, 5);

			gridRect rect = minRect + gridLocator(offsetX, offsetY);

			rect.iterateInCircle([&designGrid] (short column, short row)
			{
				designGrid.set(column, row, gridLocator(column, row), true);
				return iterationCallback::iterate;
			});
		}
	}
}

