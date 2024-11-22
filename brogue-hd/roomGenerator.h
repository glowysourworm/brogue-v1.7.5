#pragma once

#include "brogueDesignRect.h"
#include "brogueRoomTemplate.h"
#include "brogueUIBuilder.h"
#include "cellularAutomataParameters.h"
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

		gridRegion<gridLocator>* designRoom(const brogueDesignRect& designRect, const gridRect& parentBoundary);

	private:

		void designDefault(grid<gridLocator>& designGrid);
		void designCavern(grid<gridLocator>& designGrid, const gridRect& minSize);
		void designEntranceRoom(grid<gridLocator>& designGrid);
		void designCrossRoom(grid<gridLocator>& designGrid, const gridRect& minSize);
		void designSymmetricalCrossRoom(grid<gridLocator>& designGrid);
		void designSmallRoom(grid<gridLocator>& designGrid, const gridRect& minSize);
		void designCircularRoom(grid<gridLocator>& designGrid, const gridRect& minSize);
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

	gridRegion<gridLocator>* roomGenerator::designRoom(const brogueDesignRect& designRect, const gridRect& parentBoundary)
	{
		grid<gridLocator> designGrid(parentBoundary, designRect.getBoundary().createPadded(designRect.getPadding()));

		switch (designRect.getConfiguration().getRoomType())
		{
			case brogueRoomType::Default:
				designDefault(designGrid);
				break;

			case brogueRoomType::CaveCompact:
			case brogueRoomType::CaveLargeNS:
			case brogueRoomType::CaveLargeEW:
			case brogueRoomType::Cavern:
				designCavern(designGrid, designRect.getMinSize());
				break;

			case brogueRoomType::ChunkyRoom:
				designChunkyRoom(designGrid);
				break;

			case brogueRoomType::CircularRoom:
				designCircularRoom(designGrid, designRect.getMinSize());
				break;

			case brogueRoomType::CrossRoom:
				designCrossRoom(designGrid, designRect.getMinSize());
				break;

			case brogueRoomType::MainEntranceRoom:
				designEntranceRoom(designGrid);
				break;

			case brogueRoomType::SmallRoom:
				designSmallRoom(designGrid, designRect.getMinSize());
				break;

			case brogueRoomType::SmallSymmetricalCrossRoom:
				designSymmetricalCrossRoom(designGrid);
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

		// TODO: Formalize this validation (add constraints to the entire configuration)
		//if (region->getBoundary().area() < 4)
		//	throw simpleException("Invalid room created by roomGenerator:  Must fall within size constraints");

		return region;
	}

	void roomGenerator::designDefault(grid<gridLocator>& designGrid)
	{
		designGrid.iterate([&designGrid] (short column, short row, const gridLocator& locator)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designCavern(grid<gridLocator>& designGrid, const gridRect& minSize)
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
		validRegions = regions.where([&minSize] (gridRegion<gridLocator>* region)
		{
			return region->getBoundary().width >= minSize.width &&
				region->getBoundary().height >= minSize.height;
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
			// Overwrite = true. (just set the main grid to the max region)
			if (!maxRegion->isDefined(column, row))
				designGrid.set(column, row, default_value::value<gridLocator>(), true);
			else
				designGrid.set(column, row, gridLocator(column, row), true);

			return iterationCallback::iterate;
		});

		// (MEMORY) Clean up heap memory
		for (int index = 0; index < regions.count(); index++)
		{
			delete regions.get(index);
		}
	}

	void roomGenerator::designEntranceRoom(grid<gridLocator>& designGrid)
	{
		gridRect boundary = designGrid.getRelativeBoundary();
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

		int offsetX1 = boundary.column + ((boundary.width - roomWidth) / 2);
		int offsetY1 = boundary.row;
		int offsetX2 = boundary.column + ((boundary.width - roomWidth2) / 2);
		int offsetY2 = boundary.bottom() - roomHeight2 + 1;

		gridRect room1(offsetX1, offsetY1, roomWidth, roomHeight);
		gridRect room2(offsetX2, offsetY2, roomWidth2, roomHeight2);

		// Add Room 1
		room1.iterate([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Add Room 2
		room2.iterate([&designGrid, &room1] (short column, short row)
		{
			if (!room1.contains(column, row))
				designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designCrossRoom(grid<gridLocator>& designGrid, const gridRect& minSize)
	{
		gridRect boundary = designGrid.getRelativeBoundary();

		// Room 1:  75% of the total size (randomly)
		short roomWidth = _randomGenerator->randomRangeInclusive(minSize.width, 0.75f * boundary.width);
		short roomX = _randomGenerator->randomRangeInclusive(0, boundary.width - roomWidth, randomVariable::Gaussian);

		short roomHeight = _randomGenerator->randomRangeInclusive(minSize.height, 0.75f * boundary.height);
		short roomY = _randomGenerator->randomRangeInclusive(0, boundary.height - roomHeight, randomVariable::Gaussian);

		// Room 2:  100% of the total size (randomly)
		short roomWidth2 = _randomGenerator->randomRangeInclusive(minSize.width, boundary.width);
		short roomX2 = _randomGenerator->randomRangeInclusive(0, boundary.width - roomWidth2, randomVariable::Gaussian);

		short roomHeight2 = _randomGenerator->randomRangeInclusive(minSize.height, boundary.height);
		short roomY2 = _randomGenerator->randomRangeInclusive(0, boundary.height - roomHeight2, randomVariable::Gaussian);


		gridRect room1(boundary.column + roomX, boundary.row + roomY, roomWidth, roomHeight);
		gridRect room2(boundary.column + roomX2, boundary.row + roomY2, roomWidth2, roomHeight2);

		// Add Room 1
		room1.iterate([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Add Room 2
		room2.iterate([&designGrid, &room1] (short column, short row)
		{
			if (!room1.contains(column, row))
				designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designSymmetricalCrossRoom(grid<gridLocator>& designGrid)
	{
		short majorWidth, majorHeight, minorWidth, minorHeight;

		gridRect boundary = designGrid.getRelativeBoundary();

		majorWidth = _randomGenerator->randomRangeInclusive(boundary.width / 2.0f, boundary.width);
		majorHeight = _randomGenerator->randomRangeInclusive(boundary.height / 2.0f, boundary.height);

		minorWidth = _randomGenerator->randomRangeInclusive(2, majorWidth);
		minorHeight = _randomGenerator->randomRangeInclusive(2, majorHeight);

		gridRect roomNS(boundary.center().column - simpleMath::floor(minorWidth / 2.0f), boundary.center().row - simpleMath::floor(majorHeight / 2.0f), minorWidth, majorHeight);
		gridRect roomEW(boundary.center().column - simpleMath::floor(majorWidth / 2.0f), boundary.center().row - simpleMath::floor(minorHeight / 2.0f), majorWidth, minorHeight);

		// Add Room 1
		roomNS.iterate([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Add Room 2
		roomEW.iterate([&designGrid, &roomNS] (short column, short row)
		{
			if (!roomNS.contains(column, row))
				designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designSmallRoom(grid<gridLocator>& designGrid, const gridRect& minSize)
	{
		gridRect boundary = designGrid.getRelativeBoundary();

		short width = _randomGenerator->randomRangeInclusive(minSize.width, boundary.width);
		short height = _randomGenerator->randomRangeInclusive(minSize.height, boundary.height);

		gridRect room(boundary.column, boundary.row, width, height);

		// Add Room
		room.iterate([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designCircularRoom(grid<gridLocator>& designGrid, const gridRect& minSize)
	{
		gridRect boundary = designGrid.getRelativeBoundary();

		short diameterX, diameterY;

		// 5% Chance (Full Sized)
		if (_randomGenerator->next() >= 0.05)
		{
			diameterX = _randomGenerator->randomRangeInclusive(minSize.width, boundary.width);
			diameterY = _randomGenerator->randomRangeInclusive(minSize.height, boundary.height);
		}
		// 95% Chance (Less-Than-Min Size / Meets bare-minimum size)
		else
		{
			diameterX = _randomGenerator->randomRangeInclusive(2, minSize.width);
			diameterY = _randomGenerator->randomRangeInclusive(2, minSize.height);
		}

		gridRect minRect(boundary.column, boundary.row, diameterX, diameterY);

		minRect.iterateInCircle([&designGrid] (short column, short row)
		{
			designGrid.set(column, row, gridLocator(column, row));

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
		gridRect boundary = designGrid.getRelativeBoundary();
		gridRect minRect(boundary.column, boundary.row, 2, 2);

		short chunkCount = _randomGenerator->randomRangeInclusive(2, 8);

		for (short i = 0; i < chunkCount; i++)
		{
			// Brogue v1.7.5 Design:  Create chunks with 2x2 size within the layout range
			int offsetX = _randomGenerator->randomRangeExclusive(0, boundary.width - 2);
			int offsetY = _randomGenerator->randomRangeExclusive(0, boundary.height - 2);

			gridRect rect = minRect + gridLocator(offsetX, offsetY);

			rect.iterateInCircle([&designGrid] (short column, short row)
			{
				designGrid.set(column, row, gridLocator(column, row), true);
				return iterationCallback::iterate;
			});
		}
	}
}

