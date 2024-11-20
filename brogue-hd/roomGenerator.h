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
											const gridRect& relativeBoundary,
											int padding);

		gridRect getRoomDesiredSize(const brogueRoomInfo& configuration, const gridRect& constraint);
		gridRect getRoomMinSize(const brogueRoomInfo& configuration, const gridRect& constraint);

	private:

		void designCavern(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary);
		void designEntranceRoom(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary);
		void designCrossRoom(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary);
		void designSymmetricalCrossRoom(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary);
		void designSmallRoom(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary);
		void designCircularRoom(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary);
		void designChunkyRoom(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary);

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

	gridRect roomGenerator::getRoomDesiredSize(const brogueRoomInfo& configuration, const gridRect& constraint)
	{
		gridRect result = default_value::value<gridRect>();

		switch (configuration.type)
		{
			case brogueRoomType::CaveCompact:
				result = gridRect(0, 0, 12, 8); break;

			case brogueRoomType::CaveLargeNS:
				result = gridRect(0, 0, 12, constraint.height); break;

			case brogueRoomType::CaveLargeEW:
				result = gridRect(0, 0, constraint.width, 8); break;

			case brogueRoomType::Cavern:
				result = constraint; break;

			case brogueRoomType::ChunkyRoom:
				result = gridRect(0, 0, 7, 7); break;

			case brogueRoomType::CircularRoom:
				result = gridRect(0, 0, 20, 20); break;

			case brogueRoomType::CrossRoom:
				result = gridRect(0, 0, 20, 7); break;

			case brogueRoomType::MainEntranceRoom:
				result = gridRect(0, 0, 20, 10); break;

			case brogueRoomType::SmallRoom:
				result = gridRect(0, 0, 6, 4); break;

			case brogueRoomType::SmallSymmetricalCrossRoom:
				result = gridRect(0, 0, 8, 5); break;

			default:
				throw simpleException("Unhandled room type:  roomGenerator::designRoom");
		}

		if (result.width > constraint.width ||
			result.height > constraint.height)
			return default_value::value<gridRect>();

		return result;
	}

	gridRect roomGenerator::getRoomMinSize(const brogueRoomInfo& configuration, const gridRect& constraint)
	{
		gridRect result = default_value::value<gridRect>();

		switch (configuration.type)
		{
			case brogueRoomType::CaveCompact:
				result = gridRect(0, 0, 3, 4); break;

			case brogueRoomType::CaveLargeNS:
				result = gridRect(0, 0, 3, 15); break;

			case brogueRoomType::CaveLargeEW:
				result = gridRect(0, 0, 20, 4); break;

			case brogueRoomType::Cavern:
				result = gridRect(0, 0, CAVE_MIN_WIDTH, CAVE_MIN_HEIGHT); break;

			case brogueRoomType::ChunkyRoom:
				result = gridRect(0, 0, 2, 2); break;

			case brogueRoomType::CircularRoom:
				result = gridRect(0, 0, 4, 4); break;

			case brogueRoomType::CrossRoom:
				result = gridRect(0, 0, 4, 3); break;

			case brogueRoomType::MainEntranceRoom:
				result = gridRect(0, 0, 20, 10); break;

			case brogueRoomType::SmallRoom:
				result = gridRect(0, 0, 3, 2); break;

			case brogueRoomType::SmallSymmetricalCrossRoom:
				result = gridRect(0, 0, 4, 4); break;

			default:
				throw simpleException("Unhandled room type:  roomGenerator::designRoom");
		}

		if (result.width > constraint.width ||
			result.height > constraint.height)
			return default_value::value<gridRect>();

		return result;
	}

	gridRegion<gridLocator>* roomGenerator::designRoom(const brogueRoomInfo& configuration,
													   const gridRect& parentBoundary,
													   const gridRect& roomBoundary,
													   int padding)
	{
		gridRect paddedBoundary = _uiBuilder->getPaddedBoundary(roomBoundary, padding);
		gridRect desiredBoundary = getRoomDesiredSize(configuration, paddedBoundary);
		gridRect minBoundary = getRoomMinSize(configuration, paddedBoundary);

		if (desiredBoundary.width > paddedBoundary.width ||
			desiredBoundary.height > paddedBoundary.height)
			throw simpleException("Trying to design a room with less than the minimum required space");

		grid<gridLocator> designGrid(parentBoundary, paddedBoundary);

		switch (configuration.type)
		{
			case brogueRoomType::CaveCompact:
			case brogueRoomType::CaveLargeNS:
			case brogueRoomType::CaveLargeEW:
			case brogueRoomType::Cavern:
				designCavern(designGrid, minBoundary, paddedBoundary);
				break;

			case brogueRoomType::ChunkyRoom:
				designChunkyRoom(designGrid, minBoundary, paddedBoundary);
				break;

			case brogueRoomType::CircularRoom:
				designCircularRoom(designGrid, minBoundary, paddedBoundary);
				break;

			case brogueRoomType::CrossRoom:
				designCrossRoom(designGrid, minBoundary, paddedBoundary);
				break;

			case brogueRoomType::MainEntranceRoom:
				designEntranceRoom(designGrid, minBoundary, paddedBoundary);
				break;

			case brogueRoomType::SmallRoom:
				designSmallRoom(designGrid, minBoundary, paddedBoundary);
				break;

			case brogueRoomType::SmallSymmetricalCrossRoom:
				designSymmetricalCrossRoom(designGrid, minBoundary, paddedBoundary);
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

		if (region->getBoundary().width < minBoundary.width ||
			region->getBoundary().height < minBoundary.height ||
			region->getBoundary().width > desiredBoundary.width ||
			region->getBoundary().height > desiredBoundary.height)
			throw simpleException("Invalid room created by roomGenerator:  Must fall within size constraints");

		return region;
	}


	void roomGenerator::designCavern(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary)
	{
		gridRegionLocator<gridLocator> regionLocator;

		// Create cellular automata using cavern parameters
		_noiseGenerator->cellularAutomata(designGrid.getParentBoundary(),
										  desiredBoundary,
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
		validRegions = regions.where([&minSize, &desiredBoundary] (gridRegion<gridLocator>* region)
		{
			return region->getBoundary().width >= minSize.width &&
				region->getBoundary().width <= desiredBoundary.width &&
				region->getBoundary().height >= minSize.height &&
				region->getBoundary().height <= desiredBoundary.height;
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

	void roomGenerator::designEntranceRoom(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary)
	{
		short roomWidth, roomHeight, roomWidth2, roomHeight2, roomX, roomY, roomX2, roomY2;

		roomWidth = 8;
		roomHeight = 10;
		roomWidth2 = 20;
		roomHeight2 = 5;
		//roomX = (DCOLS / 2) - (roomWidth / 2) - 1;
		//roomY = DROWS - roomHeight - 2;
		//roomX2 = (DCOLS / 2) - (roomWidth2 / 2) - 1;
		//roomY2 = DROWS - roomHeight2 - 2;

		//gridRect room1(roomX, roomY, roomWidth, roomHeight);
		//gridRect room2(roomX2, roomY2, roomWidth2, roomHeight2);

		int offsetX1 = desiredBoundary.column + ((desiredBoundary.width - roomWidth) / 2);
		int offsetY1 = desiredBoundary.row;
		int offsetX2 = desiredBoundary.column + ((desiredBoundary.width - roomWidth2) / 2);
		int offsetY2 = (desiredBoundary.bottom() - roomHeight2) + 1;

		gridRect room1(offsetX1, offsetY1, roomWidth, roomHeight);
		gridRect room2(offsetX2, offsetY2, roomWidth2, roomHeight2);

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

	void roomGenerator::designCrossRoom(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary)
	{
		short roomWidth, roomHeight, roomWidth2, roomHeight2, roomX, roomY, roomX2, roomY2;

		roomWidth = _randomGenerator->randomRange(3, 12);
		roomX = _randomGenerator->randomRange(simpleMath::maxOf(0, desiredBoundary.centerX() - roomWidth - 1), desiredBoundary.centerX());
		roomWidth2 = _randomGenerator->randomRange(4, 20);
		roomX2 = (roomX + (roomWidth / 2) + _randomGenerator->randomRange(0, 2) + _randomGenerator->randomRange(0, 2) - 3) - (roomWidth2 / 2);

		roomHeight = _randomGenerator->randomRange(3, 7);
		roomY = (desiredBoundary.centerY() - roomHeight);

		roomHeight2 = _randomGenerator->randomRange(2, 5);
		roomY2 = (desiredBoundary.centerY() - roomHeight2 - (_randomGenerator->randomRange(0, 2) + _randomGenerator->randomRange(0, 1)));

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

	void roomGenerator::designSymmetricalCrossRoom(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary)
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

		gridRect room1(desiredBoundary.centerX() - (majorWidth / 2), desiredBoundary.centerY() - (minorHeight / 2), majorWidth, minorHeight);
		gridRect room2(desiredBoundary.centerX() - (minorWidth / 2), desiredBoundary.centerY() - (majorHeight / 2), minorWidth, majorHeight);

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

	void roomGenerator::designSmallRoom(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary)
	{
		short width, height;

		width = _randomGenerator->randomRange(3, 6);
		height = _randomGenerator->randomRange(2, 4);

		gridRect room(desiredBoundary.column, desiredBoundary.row, width, height);

		// Add Room
		room.iterate([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designCircularRoom(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary)
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
						 radius * 2, radius * 2);

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

	void roomGenerator::designChunkyRoom(grid<gridLocator>& designGrid, const gridRect& minSize, const gridRect& desiredBoundary)
	{
		gridRect minRect(desiredBoundary.column,
						 desiredBoundary.row,
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

