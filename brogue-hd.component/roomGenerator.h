#pragma once

#include "grid.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "gridRegion.h"
#include "gridRegionConstructor.h"
#include "gridRegionLocator.h"

#include "cellularAutomataGenerator.h"
#include "perlinNoiseGenerator.h"
#include "randomGenerator.h"
#include "cellularAutomataParameters.h"
#include "dungeonConstants.h"
#include "mazeGenerator.h"

#include <simple.h>
#include <simpleRect.h>
#include <simpleException.h>
#include <simpleList.h>
#include <simpleMath.h>

namespace brogueHd::component
{
	using namespace simple;
	using namespace brogueHd::model;

	class roomGenerator
	{
	public:
		roomGenerator(randomGenerator* randomGenerator);
		~roomGenerator();

		gridRegion* designRoom(brogueRoomType roomType, const gridRect& designRect, const gridRect& parentBoundary);

	private:
		void designDefault(grid<gridLocator>& designGrid, bool overwrite = false);
		void designCavern(grid<gridLocator>& designGrid);
		void designEntranceRoom(grid<gridLocator>& designGrid);
		void designCrossRoom(grid<gridLocator>& designGrid);
		void designSymmetricalCrossRoom(grid<gridLocator>& designGrid);
		void designSmallRoom(grid<gridLocator>& designGrid);
		void designCircularRoom(grid<gridLocator>& designGrid);
		void designChunkyRoom(grid<gridLocator>& designGrid);

	private:
		randomGenerator* _randomGenerator;

		mazeGenerator* _mazeGenerator;
		perlinNoiseGenerator* _perlinNoiseGenerator;
		cellularAutomataGenerator* _cellularAutomataGenerator;
		cellularAutomataParameters* _cavernParameters;
	};

	roomGenerator::roomGenerator(randomGenerator* randomGenerator)
	{
		_randomGenerator = randomGenerator;

		bool polarity = true; // Make walls
		float fillRatio = 0.40f; // Fill 45% with walls
		int iterations = 4; // # iterations
		int padding = 1; // Prevent collecting cells by the wall
		cellularAutomataRule rule1(4, true, true); // Rule 1 (OR) Rule 2 (passes) -> set to [polarity = wall]
		cellularAutomataRule rule2(5, false, true); // Rule 2

		// CA Algorithm Parameters:  These are for the cavern room (Brogue v1.7.5)
		_cavernParameters = new cellularAutomataParameters(fillRatio, iterations, padding, polarity, rule1, rule2);
		_cellularAutomataGenerator = new cellularAutomataGenerator(randomGenerator);

		// Perlin Noise Generator:  Use for smoothly varying random "elevation" noise maps
		_perlinNoiseGenerator = new perlinNoiseGenerator(randomGenerator);

		// Maze Generator:  Make a maze in any room!
		_mazeGenerator = new mazeGenerator(randomGenerator);

		// Sets parameters for use
		_cellularAutomataGenerator->initialize(_cavernParameters);
	}

	roomGenerator::~roomGenerator()
	{
		delete _cavernParameters;
		delete _perlinNoiseGenerator;
		delete _cellularAutomataGenerator;
		delete _mazeGenerator;
	}

	gridRegion* roomGenerator::designRoom(brogueRoomType roomType, const gridRect& designRect, const gridRect& parentBoundary)
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
			designCavern(designGrid);
			break;

		case brogueRoomType::ChunkyRoom:
			designChunkyRoom(designGrid);
			break;

		case brogueRoomType::CircularRoom:
			designCircularRoom(designGrid);
			break;

		case brogueRoomType::CrossRoom:
			designCrossRoom(designGrid);
			break;

		case brogueRoomType::MainEntranceRoom:
			designEntranceRoom(designGrid);
			break;

		case brogueRoomType::SmallRoom:
			designSmallRoom(designGrid);
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
		simpleList<gridRegion*> regions = regionLocator.locateRegions(designGrid);

		// Create Default Region:
		if (regions.count() == 0)
		{
			designDefault(designGrid, true);

			// (MEMORY!)
			simpleList<gridRegion*> defaultRegions = regionLocator.locateRegions(designGrid);

			if (defaultRegions.count() != 1)
				throw simpleException("No default region found:  roomGenerator::designRoom");

			return defaultRegions.get(0);
		}


		// Take region with max location count
		gridRegion* region = regions.withMax<int>([](gridRegion* region)
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
		designGrid.iterate([&designGrid, &overwrite](int column, int row, const gridLocator& locator)
		{
			designGrid.set(column, row, gridLocator(column, row), overwrite);

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designCavern(grid<gridLocator>& designGrid)
	{
		gridRegionLocator<gridLocator> regionLocator;

		// Create cavern using cellular automata generator
		_cellularAutomataGenerator->run(designGrid.getParentBoundary(), designGrid.getRelativeBoundary(),
		                                [&designGrid](int column, int row, bool value)
		                                {
			                                if (value)
				                                designGrid.set(column, row, gridLocator(column, row), true);

			                                return iterationCallback::iterate;
		                                });

		// Locate regions using flood fill method:  Find the max area region
		gridRegion* maxRegion;
		simpleList<gridRegion*> validRegions;
		bool defaultRegion = false;

		// (MEMORY) Locate Regions
		simpleList<gridRegion*> regions = regionLocator.locateRegions(designGrid);

		// Filter regions to comply to size constraints
		validRegions = regions.where([](gridRegion* region)
		{
			return region->getLocationCount() > 0;
		});

		// Default
		if (regions.count() == 0 || validRegions.count() == 0)
		{
			gridRegionConstructor constructor(designGrid.getParentBoundary(), true);

			designGrid.getRelativeBoundary().iterate([&constructor](int column, int row)
			{
				constructor.add(column, row, gridLocator(column, row));

				return iterationCallback::iterate;
			});

			// (MEMORY!) This will be the default region
			maxRegion = constructor.complete();

			defaultRegion = true;
		}
		// Otherwise, take the largest region
		else
		{
			maxRegion = regions.withMax<int>([](gridRegion* region)
			{
				return region->getLocationCount();
			});
		}

		// ...and copy it to the master grid. (>_<)
		designGrid.iterate([&designGrid, &maxRegion](int column, int row, const gridLocator& item)
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

		if (defaultRegion)
			delete maxRegion;
	}

	void roomGenerator::designEntranceRoom(grid<gridLocator>& designGrid)
	{
		gridRect boundary = designGrid.getRelativeBoundary();

		int roomWidth = (int)(boundary.width / 2.0f);
		int roomHeight = boundary.height;
		int roomWidth2 = boundary.width;
		int roomHeight2 = (int)(boundary.height / 2.0f);

		int offsetX1 = boundary.column + ((boundary.width - roomWidth) / 2);
		int offsetY1 = boundary.row;
		int offsetX2 = boundary.column + ((boundary.width - roomWidth2) / 2);
		int offsetY2 = boundary.bottom() - roomHeight2 + 1;

		gridRect room1(offsetX1, offsetY1, roomWidth, roomHeight);
		gridRect room2(offsetX2, offsetY2, roomWidth2, roomHeight2);

		// Add Room 1
		room1.iterate([&designGrid](int column, int row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Add Room 2
		room2.iterate([&designGrid, &room1](int column, int row)
		{
			if (!room1.contains(column, row))
				designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designCrossRoom(grid<gridLocator>& designGrid)
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

		gridRect roomNS(boundary.column + simpleMath::ceiling((boundary.width - minorWidth) / 2.0f),
		                boundary.row + simpleMath::ceiling((boundary.height - majorHeight) / 2.0f), minorWidth,
		                majorHeight);
		gridRect roomEW(boundary.row + simpleMath::ceiling((boundary.width - majorWidth) / 2.0f),
		                boundary.row + simpleMath::ceiling((boundary.height - minorHeight) / 2.0f), majorWidth,
		                minorHeight);

		roomNS.clamp(boundary);
		roomEW.clamp(boundary);

		// Add Room 1
		roomNS.iterate([&designGrid](int column, int row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});

		// Add Room 2
		roomEW.iterate([&designGrid, &roomNS](int column, int row)
		{
			if (!roomNS.contains(column, row))
				designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designSmallRoom(grid<gridLocator>& designGrid)
	{
		gridRect boundary = designGrid.getRelativeBoundary();

		int width = _randomGenerator->randomRangeInclusive(2, boundary.width);
		int height = _randomGenerator->randomRangeInclusive(2, boundary.height);

		gridRect room(boundary.column, boundary.row, width, height);

		// Add Room
		room.iterate([&designGrid](int column, int row)
		{
			designGrid.set(column, row, gridLocator(column, row));

			return iterationCallback::iterate;
		});
	}

	void roomGenerator::designCircularRoom(grid<gridLocator>& designGrid)
	{
		gridRect boundary = designGrid.getRelativeBoundary();

		boundary.iterateInCircle([&designGrid](int column, int row)
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

		int chunkCount = _randomGenerator->randomRangeInclusive(3, 8);

		gridRect rect;
		gridLocator offset;

		for (int i = 0; i < chunkCount; i++)
		{
			int randomWidth = _randomGenerator->randomRangeInclusive(2, simpleMath::highLimit(4, boundary.width));
			int randomHeight = _randomGenerator->randomRangeInclusive(2, simpleMath::highLimit(4, boundary.height));

			// Brogue v1.7.5 Design:  Create chunks with 2x2 size within the layout range
			offset.column = _randomGenerator->randomRangeExclusive(0, boundary.width - randomWidth);
			offset.row = _randomGenerator->randomRangeExclusive(0, boundary.height - randomHeight);

			rect = gridRect(boundary.column + offset.column, boundary.row + offset.row, randomWidth, randomHeight);

			rect.iterate([&designGrid] (int column, int row)
			{
				designGrid.set(column, row, gridLocator(column, row), true);
				return iterationCallback::iterate;
			});
		}
	}
}
