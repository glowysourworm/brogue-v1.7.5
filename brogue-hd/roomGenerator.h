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

		gridRegion<gridLocator>* designRoom(brogueRoomType roomType, const gridRect& designRect, const gridRect& minSize, const gridRect& parentBoundary);

	private:

		void designDefault(grid<gridLocator>& designGrid, bool overwrite = false);
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

		bool polarity = true;							// Make walls
		float fillRatio = 0.55f;						// Fill 45% with walls
		int iterations = 4;							// # iterations
		cellularAutomataRule rule1(4, true, true);		// Rule 1 (OR) Rule 2 (passes) -> set to [polarity = wall]
		cellularAutomataRule rule2(5, false, true);		// Rule 2
		

		// CA Algorithm Parameters:  These are for the cavern room (Brogue v1.7.5)
		_cavernParameters = new cellularAutomataParameters(fillRatio, iterations, polarity, rule1, rule2);
	}

	roomGenerator::~roomGenerator()
	{
		delete _cavernParameters;
	}

	gridRegion<gridLocator>* roomGenerator::designRoom(brogueRoomType roomType, const gridRect& designRect, const gridRect& minSize, const gridRect& parentBoundary)
	{
		grid<gridLocator> designGrid(parentBoundary, designRect);

		switch (roomType)
		{
			case brogueRoomType::Default:
				designDefault(designGrid);
				break;

			case brogueRoomType::CaveCompact:
			case brogueRoomType::CaveLargeNS:
			case brogueRoomType::CaveLargeEW:
			case brogueRoomType::Cavern:
				designCavern(designGrid, minSize);
				break;

			case brogueRoomType::ChunkyRoom:
				designChunkyRoom(designGrid);
				break;

			case brogueRoomType::CircularRoom:
				designCircularRoom(designGrid, minSize);
				break;

			case brogueRoomType::CrossRoom:
				designCrossRoom(designGrid, minSize);
				break;

			case brogueRoomType::MainEntranceRoom:
				designEntranceRoom(designGrid);
				break;

			case brogueRoomType::SmallRoom:
				designSmallRoom(designGrid, minSize);
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

		// Create Default Region:
		if (regions.count() == 0)
		{
			designDefault(designGrid, true);

			// (MEMORY!)
			simpleList<gridRegion<gridLocator>*> defaultRegions = regionLocator.locateRegions(designGrid);

			if (defaultRegions.count() != 1)
				throw simpleException("No default region found:  roomGenerator::designRoom");

			return defaultRegions.get(0);
		}


		// Take region with max location count
		gridRegion<gridLocator>* region = regions.withMax<int>([] (gridRegion<gridLocator>* region)
		{
			return region->getLocationCount();
		});

		// TODO: Formalize this validation (add constraints to the entire configuration)
		//if (region->getBoundary().width < minSize.width ||
		//	region->getBoundary().height < minSize.height)
		//	throw simpleException("Invalid room created by roomGenerator:  Must fall within size constraints");

		// Clean up memory
		for (int index = 0; index < regions.count(); index++)
		{
			if (regions.get(index) != region)
				delete regions.get(index);
		}

		return region;
	}

	void roomGenerator::designDefault(grid<gridLocator>& designGrid, bool overwrite)
	{
		designGrid.iterate([&designGrid, &overwrite] (int column, int row, const gridLocator& locator)
		{
			designGrid.set(column, row, gridLocator(column, row), overwrite);

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
		[&designGrid] (int column, int row, bool result)
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
			maxRegion = validRegions.withMax<int>([] (gridRegion<gridLocator>* region)
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

			defaultRect.iterate([&constructor] (int column, int row)
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
		designGrid.iterate([&designGrid, &maxRegion] (int column, int row, const gridLocator& item)
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

		int roomWidth = 8;
		int roomHeight = 10;
		int roomWidth2 = 20;
		int roomHeight2 = 5;

		int offsetX1 = boundary.column + ((boundary.width - roomWidth) / 2);
		int offsetY1 = boundary.row;
		int offsetX2 = boundary.column + ((boundary.width - roomWidth2) / 2);
		int offsetY2 = boundary.bottom() - roomHeight2 + 1;

		gridRect room1(offsetX1, offsetY1, roomWidth, roomHeight);
		gridRect room2(offsetX2, offsetY2, roomWidth2, roomHeight2);

		// Add Room 1
		room1.iterate([&designGrid] (int column, int row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Add Room 2
		room2.iterate([&designGrid, &room1] (int column, int row)
		{
			if (!room1.contains(column, row))
				designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designCrossRoom(grid<gridLocator>& designGrid, const gridRect& minSize)
	{
		return designSymmetricalCrossRoom(designGrid);

		//gridRect boundary = designGrid.getRelativeBoundary();

		//// Room 1:  75% of the total size (randomly)
		//int roomWidth = simpleMath::nearestEvenFloor(_randomGenerator->randomRangeInclusive(minSize.width, 0.75f * boundary.width));
		//int roomX = _randomGenerator->randomRangeInclusive(0, boundary.width - roomWidth, randomVariable::Gaussian);

		//int roomHeight = simpleMath::nearestEvenFloor(_randomGenerator->randomRangeInclusive(minSize.height, 0.75f * boundary.height));
		//int roomY = _randomGenerator->randomRangeInclusive(0, boundary.height - roomHeight, randomVariable::Gaussian);

		//// Room 2:  100% of the total size (randomly)
		//int roomWidth2 = simpleMath::nearestEvenFloor(_randomGenerator->randomRangeInclusive(minSize.width, boundary.width));
		//int roomX2 = _randomGenerator->randomRangeInclusive(0, boundary.width - roomWidth2, randomVariable::Gaussian);

		//int roomHeight2 = simpleMath::nearestEvenFloor(_randomGenerator->randomRangeInclusive(minSize.height, boundary.height));
		//int roomY2 = _randomGenerator->randomRangeInclusive(0, boundary.height - roomHeight2, randomVariable::Gaussian);

		//// Cross the sizes
		//gridRect room1(boundary.column + roomX, boundary.row + roomY, roomWidth, roomHeight);
		//gridRect room2(boundary.column + roomX2, boundary.row + roomY2, roomWidth2, roomHeight2);

		//// Add Room 1
		//room1.iterate([&designGrid] (int column, int row)
		//{
		//	designGrid.set(column, row, gridLocator(column, row));

		//	return iterationCallback::iterate;
		//});

		//// Add Room 2
		//room2.iterate([&designGrid, &room1] (int column, int row)
		//{
		//	if (!room1.contains(column, row))
		//		designGrid.set(column, row, gridLocator(column, row));

		//	return iterationCallback::iterate;
		//});
	}

	void roomGenerator::designSymmetricalCrossRoom(grid<gridLocator>& designGrid)
	{
		int majorWidth, majorHeight, minorWidth, minorHeight;

		gridRect boundary = designGrid.getRelativeBoundary();

		// Let's try making this the full width / height; and randomizing the constainer. Probably easier.

		majorWidth = boundary.width;
		majorHeight = boundary.height;

		if (boundary.width % 2 == 0)
			minorWidth = boundary.width * 0.66;
		else
			minorWidth = boundary.width * 0.6;

		if (boundary.height % 2 == 0)
			minorHeight = boundary.height * 0.66;
		else
			minorHeight = boundary.height * 0.6;

		gridRect roomNS(boundary.column + simpleMath::ceiling((boundary.width - minorWidth) / 2.0f), boundary.row + simpleMath::ceiling((boundary.height - majorHeight) / 2.0f), minorWidth, majorHeight);
		gridRect roomEW(boundary.row + simpleMath::ceiling((boundary.width - majorWidth) / 2.0f), boundary.row + simpleMath::ceiling((boundary.height - minorHeight) / 2.0f), majorWidth, minorHeight);

		roomNS.clamp(boundary);
		roomEW.clamp(boundary);

		// Add Room 1
		roomNS.iterate([&designGrid] (int column, int row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Add Room 2
		roomEW.iterate([&designGrid, &roomNS] (int column, int row)
		{
			if (!roomNS.contains(column, row))
				designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designSmallRoom(grid<gridLocator>& designGrid, const gridRect& minSize)
	{
		gridRect boundary = designGrid.getRelativeBoundary();

		int width = _randomGenerator->randomRangeInclusive(minSize.width, boundary.width);
		int height = _randomGenerator->randomRangeInclusive(minSize.height, boundary.height);

		gridRect room(boundary.column, boundary.row, width, height);

		// Add Room
		room.iterate([&designGrid] (int column, int row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designCircularRoom(grid<gridLocator>& designGrid, const gridRect& minSize)
	{
		gridRect boundary = designGrid.getRelativeBoundary();

		boundary.iterateInCircle([&designGrid] (int column, int row)
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

		//	int removeRadius = _randomGenerator->randomRange(3, radius - 3);
		//	gridRect removeRect = gridRect::fromCircle((DCOLS / 2) + removeRadius, (DCOLS / 2) + removeRadius, removeRadius, removeRadius);

		//	setRect.iterateInCircle([&designGrid] (int column, int row)
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

		int chunkCount = _randomGenerator->randomRangeInclusive(2, 8);

		for (int i = 0; i < chunkCount; i++)
		{
			// Brogue v1.7.5 Design:  Create chunks with 2x2 size within the layout range
			int offsetX = _randomGenerator->randomRangeExclusive(0, boundary.width - 2);
			int offsetY = _randomGenerator->randomRangeExclusive(0, boundary.height - 2);

			gridRect rect = minRect + gridLocator(offsetX, offsetY);

			rect.iterateInCircle([&designGrid] (int column, int row)
			{
				designGrid.set(column, row, gridLocator(column, row), true);
				return iterationCallback::iterate;
			});
		}
	}
}

