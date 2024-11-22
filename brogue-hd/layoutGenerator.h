#pragma once

#include "brogueDesignRect.h"
#include "brogueLayout.h"
#include "brogueLevelProfile.h"
#include "randomGenerator.h"
#include "roomGenerator.h"

#include "brogueRoomTemplate.h"
#include "brogueUIBuilder.h"
#include "graph.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRect.h"
#include "gridRegion.h"
#include "noiseGenerator.h"
#include "rectanglePackingAlgorithm.h"
#include "simple.h"
#include "simpleList.h"

using namespace brogueHd::simple;
using namespace brogueHd::component;
using namespace brogueHd::backend::model;

namespace brogueHd::backend
{
	class layoutGenerator
	{

	public:

		/// <summary>
		/// Creates the base layout, terrain, and machine terrain for the level.
		/// </summary>
		layoutGenerator(brogueUIBuilder* uiBuilder, randomGenerator* randomGenerator, noiseGenerator* noiseGenerator);
		~layoutGenerator();

		brogueLayout* generateLayout(brogueLevelProfile* profile);

		/// <summary>
		/// Clears the layout generation data out for another use. Must set up once per level.
		/// </summary>
		void clear();

	private:

		void initialize(brogueLevelProfile* profile);

		void createRooms();

		//bool attemptConnection(brogueDesignRect* roomTile, const gridRect& attemptRect, short interRoomPadding) const;

		//void designateMachineRooms();

		void triangulateRooms();

		void connectRooms();

	private:

		roomGenerator* _roomGenerator;
		randomGenerator* _randomGenerator;
		brogueUIBuilder* _uiBuilder;

		// Generation Data Stores:
		//
		brogueLayout* _layout;
		brogueLevelProfile* _profile;
		simpleList<brogueDesignRect*>* _roomTiles;
		graph<gridLocator, gridLocatorEdge>* _delaunayGraph;
	};


	layoutGenerator::layoutGenerator(brogueUIBuilder* uiBuilder, randomGenerator* randomGenerator, noiseGenerator* noiseGenerator)
	{
		_uiBuilder = uiBuilder;
		_randomGenerator = randomGenerator;
		_roomGenerator = new roomGenerator(uiBuilder, noiseGenerator, randomGenerator);

		// Layout Data
		_layout = nullptr;
		_profile = nullptr;
		_roomTiles = nullptr;
		_delaunayGraph = nullptr;
	}

	layoutGenerator::~layoutGenerator()
	{
		// Accretion tiles can now be deleted from the previous run
		for (int index = 0; index < _roomTiles->count(); index++)
		{
			// gridRegion instances handled separately
			delete _roomTiles->get(index);
		}

		delete _roomTiles;

		delete _roomGenerator;
	}

	void layoutGenerator::initialize(brogueLevelProfile* profile)
	{
		// Accretion tiles can now be deleted from the previous run
		if (_roomTiles != nullptr)
		{
			for (int index = 0; index < _roomTiles->count(); index++)
			{
				// gridRegion instances handled separately
				delete _roomTiles->get(index);
			}
			delete _roomTiles;
		}

		gridRect levelBoundary = _uiBuilder->getBrogueGameBoundary();
		gridRect levelPaddedBoundary = _uiBuilder->getPaddedBoundary(levelBoundary, 1);

		_layout = new brogueLayout(levelBoundary, levelPaddedBoundary);
		_profile = profile;
		_roomTiles = new simpleList<brogueDesignRect*>();
		_delaunayGraph = nullptr;
	}

	brogueLayout* layoutGenerator::generateLayout(brogueLevelProfile* profile)
	{
		// Setup memory for the next layout generation
		initialize(profile);

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
		//      -> Create Minimum Spanning Tree (MST) using Prim's Algorithm
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

		// Create Rooms (new _roomTiles)
		createRooms();

		// Machine Rooms

		// Triangulate Rooms:  Creates Delaunay Triangulation of the connection point vertices (new _delaunayGraph)
		//triangulateRooms();

		// Connect Rooms:  Create cells in the grid for the delaunay triangulation of the 
		//                 connection points of the room tiles. Runs dijkstra's algorithm to
		//				   create corridor cells inside the brogueLayout*
		//connectRooms();

		return _layout;
	}

	void layoutGenerator::createRooms()
	{
		// Brogue v1.7.5 Creates up to 35 rooms using accretion (bolt-on) to fill up the space
		//
		//
		// Update:  (see backlog.txt) This will now be done using a tiling method.
		//

		//rectanglePackingAlgorithm algorithm(_randomGenerator, _layout->getBoundary());

		simpleList<brogueDesignRect*> tiling;

		int maxIterations = 600;
		int iteration = 0;

		while (iteration < maxIterations)
		{
			brogueRoomTemplate configuration = (iteration == 0) ? _profile->getEntranceRoom(_randomGenerator) : _profile->getDefaultRoom();

			// Entrance Room Selection
			//
			if (iteration == 0)
			{
				brogueDesignRect* designRect = new brogueDesignRect(configuration, _layout->getBoundary(), 0);
				gridLocator topLeft = gridLocator(_layout->getBoundary().centerX() - (designRect->getBoundary().width / 2),
												  _layout->getBoundary().bottom() - designRect->getBoundary().height);
				designRect->setOffset(topLeft);

				// Initialize Rectangle Packing Algorithm
				//
				//algorithm.initialize(designRect->getBoundary());
				tiling.add(designRect);
			}

			// Add rectangles to the space using the algorithm (Padding = 1)
			//
			else
			{
				// Get next space in which to create a room
				//gridRect largestSubRect = algorithm.getLargestUnusedRectangle();
				gridRect largestSubRect = _layout->getLargestUnusedRectangle();

				if (largestSubRect == default_value::value<gridRect>())
				{
					iteration++;
					continue;
				}

				brogueDesignRect* designRect = new brogueDesignRect(configuration, largestSubRect, 0);
				gridRect minSize = designRect->getMinSize();

				if (largestSubRect.width < minSize.width ||
					largestSubRect.height < minSize.height)
				{
					iteration++;

					delete designRect;
					continue;
				}

				// Give the algorithm a rectangle to tile
				//gridRect nextRect = designRect->getBoundary();

				// Success
				//if (algorithm.addRectangle(nextRect))
				//{
					// Use the rectangle to translate the design rect
				//	designRect->setOffset(nextRect.topLeft());

					tiling.add(designRect);
				//}
			}

			// Complete:
			//
			// 1) Create grid cells for the tile's room
			// 2) Translate the result inside of its design grid rect (towards the entrance room)
			// 3) Calculate the final boundary (setting it into the brogueDesignRect)
			// 4) Remove / Replace the rectangle in the algorithm with the final one
			//

			brogueDesignRect* designRect = tiling.get(tiling.count() - 1);

			// (MEMORY!) These must be deleted; and the stack-like gridLocator instances will be copied into the brogueLayout* grid.
			//gridRegion<gridLocator>* region = _roomGenerator->designRoom(*designRect, _layout->getBoundary());

			// Set the actual region boundary
			//designRect->complete(region->getBoundary());

			// Repack the rectangle if it is not the entrance
			if (iteration != 0)
			{
				// Since the design rect was built with a padded boundary (for the room generator), the
				// tile in the packing should still have room for the padded boundary of the actual rect.
				//
				//gridRect actualRect = designRect->getActualBoundary();
				////gridRect actualPaddedRect = actualRect.createExpanded(designRect->getPadding());				// Padded region must be within the bounds
				//gridRect actualPaddedRect = actualRect;
				//gridRect repackedRect = algorithm.repackRectangle(tiling.count() - 1, actualPaddedRect);
				//gridLocator translation = actualPaddedRect.getTranslation(repackedRect);						// Difference from the original boundary

				//// Translate the design rect
				//designRect->translate(translation);

				//// Translate the region
				//region->translate_StackLike(translation.column, translation.row);

				// (TODO: Heap the tile list) (Have to re-create tiling list entry due to stack memory copying)
				//tiling.removeAt(tiling.count() - 1);
				//tiling.add(designRect);
			}

			// Transfer data over to the brogueLayout*

			brogueLayout* layout = _layout;

			designRect->getBoundary().iterate([&layout] (short column, short row)
			{
				layout->createCells(gridLocator(column, row));
				return iterationCallback::iterate;
			});

			// (MEMORY!) Clean up heap memory
			//delete region;

			iteration++;
		}



		//// 1) Create 35 room (up front), 2) Attempt bolt-on until there are no more fits
		////
		//for (short index = 0; index < maxAttempts; index++)
		//{
		//	// Room configuration from the template
		//	brogueRoomInfo configuration = index == 0 ? _profile->getEntranceRoom(_randomGenerator) : _profile->getRandomRoomInfo(_randomGenerator);

		//	// (MEMORY!) (These regions are given an extra cell of padding for connection points)
		//	gridRegion<gridLocator>* nextRegion = _roomGenerator->designRoom(configuration, _layout->getParentBoundary(), _layout->getBoundary());

		//	// Connection Points (These are the edges, not the connection points)
		//	simpleOrderedList<gridLocator> northEdge = nextRegion->getBestEdges(brogueCompass::N);
		//	simpleOrderedList<gridLocator> southEdge = nextRegion->getBestEdges(brogueCompass::S);
		//	simpleOrderedList<gridLocator> eastEdge = nextRegion->getBestEdges(brogueCompass::E);
		//	simpleOrderedList<gridLocator> westEdge = nextRegion->getBestEdges(brogueCompass::W);

		//	// TODO: Situate these better. This will create an exponential draw that falls off quickly. 
		//	//		 So, the majority of attempts will be on the outer walls, closer to the boundary.
		//	//
		//	int randN = _randomGenerator->randomIndex_exp(0, northEdge.count(), 10);
		//	int randS = _randomGenerator->randomIndex_exp(0, southEdge.count(), 10);
		//	int randE = _randomGenerator->randomIndex_exp(0, eastEdge.count(), 10);
		//	int randW = _randomGenerator->randomIndex_exp(0, westEdge.count(), 10);

		//	// (MEMORY!) (Must delete region explicitly - see below)
		//	accretionTile* nextRoom = new accretionTile();

		//	// (Pointer to allocated region memory)
		//	nextRoom->region = nextRegion;

		//	gridLocator connectionN = northEdge.get(randN).add(0, -1);
		//	gridLocator connectionS = southEdge.get(randS).add(0, 1);
		//	gridLocator connectionE = eastEdge.get(randE).add(1, 0);
		//	gridLocator connectionW = westEdge.get(randW).add(-1, 0);

		//	// Make sure connection points lie OUTSIDE the region
		//	if (nextRegion->isDefined(connectionN) ||
		//		nextRegion->isDefined(connectionS) ||
		//		nextRegion->isDefined(connectionE) ||
		//		nextRegion->isDefined(connectionW))
		//	{
		//		delete nextRoom->region;
		//		delete nextRoom;

		//		continue;
		//	}


		//	nextRoom->connectionPointN = connectionN;		// Connection points lie in the padded area of the desisgn grid
		//	nextRoom->connectionPointS = connectionS;		// or on the interior. They are not part of the region itself; and			
		//	nextRoom->connectionPointE = connectionE;		// will become separate corridor cells, and regions, afterwards.
		//	nextRoom->connectionPointW = connectionW;

		//	attemptRegions.add(nextRoom);
		//}

		//// Bolt-on
		//for (int index = 0; index < attemptRegions.count(); index++)
		//{
		//	accretionTile* attemptRegion = attemptRegions.get(index);

		//	// First Room
		//	if (index == 0)
		//	{
		//		// NOTE:  Rooms are generated in their supposed 1st-attempt location. This may
		//		//        have been by design (Brogue v1.7.5); but probably only matters for
		//		//        the first level.
		//		//
		//		_roomTiles->add(attemptRegion);

		//		// Set final room location (creates new brogueCell* instances, so delete our local one afterwards)
		//		_layout->createCells(attemptRegion->region);
		//	}

		//	// Accrete rooms - translating the attempt room into place (modifies attempt region)
		//	else if (attemptConnection(attemptRegion, _layout->getBoundary(), interRoomPadding))
		//	{
		//		_roomTiles->add(attemptRegion);

		//		// Set final room location (creates new brogueCell* instances, so delete our local one afterwards)
		//		_layout->createCells(attemptRegion->region);
		//	}
		//}

		//// Add corridor cells for the connection points
		//for (int index = 0; index < _roomTiles->count(); index++)
		//{
		//	accretionTile* tile = _roomTiles->get(index);

		//	if (tile->hasNorthConnection)
		//		_layout->createCells(tile->connectionPointN);

		//	if (tile->hasSouthConnection)
		//		_layout->createCells(tile->connectionPointS);

		//	if (tile->hasEastConnection)
		//		_layout->createCells(tile->connectionPointE);

		//	if (tile->hasWestConnection)
		//		_layout->createCells(tile->connectionPointW);
		//}

		//// Cleanup Memory:  Delete unused room tiles
		//for (int index = 0; index < attemptRegions.count(); index++)
		//{
		//	if (!_roomTiles->contains(attemptRegions.get(index)))
		//		delete attemptRegions.get(index);
		//}
	}

	//bool layoutGenerator::attemptConnection(brogueDesignRect* roomTile, const gridRect& attemptRect, short interRoomPadding) const
	//{
	//	// Procedure
	//	//
	//	// 1) Combinatorically attempt connection points
	//	//      -> Start at beginning of room tile vector
	//	//      -> Test each possibility
	//	//          1) fits inside attemptRect
	//	//          2) no overlapping region locations
	//	// 
	//	// Success: Update room tile; Update attemptRect
	//	// 
	//	// Fail:    (TODO) Try positioning the tile using
	//	//                 padding.
	//	//

	//	for (int index = 0; index < _roomTiles->count(); index++)
	//	{
	//		brogueDesignRect* tile = _roomTiles->get(index);

	//		bool northAttempt = false;
	//		bool southAttempt = false;
	//		bool eastAttempt = false;
	//		bool westAttempt = false;

	//		for (int tileAttempts = 1; tileAttempts <= 4; tileAttempts++)
	//		{
	//			gridLocator translation = gridLocator::getEmpty();

	//			// Left, Top, Right, Bottom
	//			if (!roomTile->hasEastConnection && !tile->hasWestConnection && !westAttempt)
	//			{
	//				translation = tile->connectionPointW.subtract(roomTile->connectionPointE);

	//				// Adjust the connection point by one cell + padding
	//				//translation.column -= (1 + interRoomPadding);

	//				westAttempt = true;
	//			}

	//			else if (!roomTile->hasNorthConnection && !tile->hasSouthConnection && !northAttempt)
	//			{
	//				translation = tile->connectionPointN.subtract(roomTile->connectionPointS);

	//				// Adjust the connection point by one cell + padding
	//				//translation.row -= (1 + interRoomPadding);

	//				northAttempt = true;
	//			}

	//			else if (!roomTile->hasEastConnection && !tile->hasWestConnection && !eastAttempt)
	//			{
	//				translation = tile->connectionPointE.subtract(roomTile->connectionPointW);

	//				// Adjust the connection point by one cell + padding
	//				//translation.column += (1 + interRoomPadding);

	//				eastAttempt = true;
	//			}

	//			else if (!roomTile->hasSouthConnection && !tile->hasNorthConnection && !southAttempt)
	//			{
	//				translation = tile->connectionPointS.subtract(roomTile->connectionPointN);

	//				// Adjust the connection point by one cell + padding
	//				//translation.row += (1 + interRoomPadding);

	//				southAttempt = true;
	//			}
	//			else
	//				break;

	//			// Translate the region tile into position (this affects all grid locators, and connection points)
	//			if (!roomTile->attemptTranslastion(translation))
	//				continue;

	//			// Check the boundary:  1) outer gridRect first to reduce iteration, and 2) the grid overlap per cell
	//			if (attemptRect.contains(roomTile->region->getBoundary()))
	//			{
	//				if (tile->region->overlaps(roomTile->region))
	//				{
	//					//delete roomTile->region;
	//					continue;
	//				}

	//				bool gridOverlaps = false;
	//				brogueLayout* layout = _layout;

	//				// Double check the already-set rooms
	//				roomTile->region->iterateLocations([&layout, &gridOverlaps] (short column, short row, gridLocator item)
	//				{
	//					if (layout->isDefined(item.column, item.row))
	//					{
	//						gridOverlaps = true;
	//						return iterationCallback::breakAndReturn;
	//					}

	//					return iterationCallback::iterate;
	//				});

	//				if (gridOverlaps)
	//				{
	//					//delete roomTile->region;
	//					continue;
	//				}

	//				// Set connection point data (should be using a direction iterator) [1, 4] -> [W,N,E,S]
	//				if (index == 1)
	//				{
	//					roomTile->connectionPointE = tile->connectionPointW;
	//					roomTile->hasEastConnection = true;
	//				}
	//				else if (index == 2)
	//				{
	//					roomTile->connectionPointS = tile->connectionPointN;
	//					roomTile->hasSouthConnection = true;
	//				}
	//				else if (index == 3)
	//				{
	//					roomTile->connectionPointW = tile->connectionPointE;
	//					roomTile->hasWestConnection = true;
	//				}
	//				else
	//				{
	//					roomTile->connectionPointN = tile->connectionPointS;
	//					roomTile->hasNorthConnection = true;
	//				}

	//				return true;
	//			}
	//		}
	//	}

	//	return false;
	//}

	void layoutGenerator::triangulateRooms()
	{
		//// Create delaunay triangulator with graph edge constructor
		//delaunayAlgorithm<gridLocator, gridLocatorEdge> triangulator([] (gridLocator node1, gridLocator node2)
		//{
		//	gridLocatorEdge edge(node1, node2);

		//	return edge;
		//});

		//// Create connection point vertices
		//simpleList<gridLocator> connectionNodes;

		//_roomTiles->forEach([&connectionNodes] (brogueDesignRect* item)
		//{
		//	if (item->hasEastConnection)
		//		connectionNodes.add(gridLocator(item->connectionPointE));

		//	if (item->hasNorthConnection)
		//		connectionNodes.add(gridLocator(item->connectionPointN));

		//	if (item->hasSouthConnection)
		//		connectionNodes.add(gridLocator(item->connectionPointS));

		//	if (item->hasWestConnection)
		//		connectionNodes.add(gridLocator(item->connectionPointW));

		//	return iterationCallback::iterate;
		//});

		//_delaunayGraph = triangulator.run(connectionNodes);
	}

	void layoutGenerator::connectRooms()
	{
		//// Procedure
		////
		//// 1) Iterate Edges: DELAUNAY GRAPH
		////      -> Find edges that are not self-referential (use room tiles)
		////      -> Collect these edges to pass to dijkstra
		////
		//// 2) Run Dijkstra to set cells in the primary grid
		////

		//simpleList<gridLocatorEdge> corridorEdges;
		//simpleList<brogueDesignRect*>* roomTiles = _roomTiles;
		//brogueLayout* layout = _layout;

		//_delaunayGraph->iterateEdges([&roomTiles, &corridorEdges] (const gridLocatorEdge& edge)
		//{
		//	bool isCorridorEdge = false;

		//	roomTiles->forEach([&edge, &isCorridorEdge] (brogueDesignRect* tile)
		//	{
		//		int connectionCount = 0;

		//		// North
		//		if (tile->hasNorthConnection &&
		//			(tile->connectionPointN == edge.node1 ||
		//			tile->connectionPointN == edge.node2))
		//			connectionCount++;

		//		// South
		//		if (tile->hasSouthConnection &&
		//			(tile->connectionPointS == edge.node1 ||
		//			tile->connectionPointS == edge.node2))
		//			connectionCount++;

		//		// East
		//		if (tile->hasEastConnection &&
		//			(tile->connectionPointE == edge.node1 ||
		//			tile->connectionPointE == edge.node2))
		//			connectionCount++;

		//		// West
		//		if (tile->hasWestConnection &&
		//			(tile->connectionPointW == edge.node1 ||
		//			tile->connectionPointW == edge.node2))
		//			connectionCount++;

		//		if (connectionCount == 1)
		//		{
		//			isCorridorEdge = true;
		//			return iterationCallback::breakAndReturn;
		//		}

		//		return iterationCallback::iterate;
		//	});

		//	if (isCorridorEdge)
		//		corridorEdges.add(edge);

		//	return iterationCallback::iterate;
		//});

		//// (MEMORY!) Use the corridor edges to call dijkstra (on stack usgae has predicate copying)
		//dijkstra<gridLocator>* algorithm = new dijkstra<gridLocator>(
		//	_layout->getParentBoundary(),
		//	_layout->getBoundary(),
		//	true,                           // Cardinal Movement (for laying corridors)

		//// Primary Inclusion Predicate (is it in the grid?)
		//[&layout](short column, short row)
		//{
		//	return layout->isDefined(column, row);
		//},

		//// Then, the map cost is queried (what is the movement cost?)
		//[&layout](short column, short row)
		//{
		//	return 1;
		//},

		//// Then, it will need the locators from the grid to keep its internal data temporarily
		//[&layout](short column, short row)
		//{
		//	// May need design change for this problem (needed to copy grid locators back for dijkstra)
		//	return gridLocator(column, row);
		//});

		//// Iterate each corridor edge and run dijkstra to finalize the connection
		//corridorEdges.forEach([&algorithm, &layout] (const gridLocatorEdge& edge)
		//{
		//	gridLocator source = edge.node1;
		//	gridLocator targets[1] = { edge.node2 };

		//	// Run Dijkstra
		//	algorithm->initialize(source, simpleArray<gridLocator>(targets, 1));
		//	algorithm->run();

		//	simpleArray<gridLocator> resultPath = algorithm->getResultPath(targets[0]);

		//	// Set corridors
		//	resultPath.forEach([&layout] (gridLocator locator)
		//	{
		//		layout->createCells(locator);

		//		return iterationCallback::iterate;
		//	});

		//	return iterationCallback::iterate;
		//});

		//// Clean up memory
		//delete algorithm;
	}
}

