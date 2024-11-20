#pragma once

#include "accretionTile.h"
#include "brogueLayout.h"
#include "brogueLevelProfile.h"
#include "randomGenerator.h"
#include "roomGenerator.h"

#include "brogueUIBuilder.h"
#include "delaunayAlgorithm.h"
#include "dijkstra.h"
#include "dungeon.h"
#include "graph.h"
#include "grid.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRect.h"
#include "gridRegion.h"
#include "noiseGenerator.h"
#include "simple.h"
#include "simpleArray.h"
#include "simpleHash.h"
#include "simpleList.h"
#include "simpleMath.h"
#include "simpleOrderedList.h"
#include "simplePoint.h"
#include "simpleVector.h"

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

		bool attemptConnection(accretionTile* roomTile, const gridRect& attemptRect, short interRoomPadding) const;

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
		simpleList<accretionTile*>* _roomTiles;
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
		_roomTiles = new simpleList<accretionTile*>();
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

		grid<gridLocator> tilingGrid(_layout->getParentBoundary(), _layout->getBoundary());

		simpleHash<brogueRoomInfo, gridRect> tiling;

		gridRect entranceBoundary;
		bool finished = false;
		int maxIterations = 35;
		int iteration = 0;

		while (!finished && iteration < maxIterations)
		{
			brogueRoomInfo configuration;
			gridRect boundary;

			// Tile the entrance room
			if (iteration == 0)
			{
				configuration = _profile->getEntranceRoom(_randomGenerator);
				boundary = _roomGenerator->getRoomDesiredSize(configuration, _layout->getBoundary());
				gridLocator topLeft = gridLocator(_layout->getBoundary().centerX() - (boundary.width / 2),
												  _layout->getBoundary().bottom() - boundary.height);
				boundary.translate(topLeft);

				// Mark the entrance tile
				entranceBoundary = boundary;
			}

			// Tile against any previous tile; but we're going to apply some random logic to the positining;
			// and, also, brute force movement once our random positioning doesn't get a much better heuristic.
			else
			{
				gridRect nextLargestBoundary = tilingGrid.calculateLargestRectangle();
				configuration = _profile->getRandomRoomInfo(_randomGenerator);
				boundary = _roomGenerator->getRoomDesiredSize(configuration, nextLargestBoundary);

				// No more space
				if (boundary == default_value::value<gridRect>())
					break;

				// Procedure:  Pseudo-Rectangular Packing Problem (w/ random positioning and distance heuristic)
				// 
				// 0) Check adjacent locations to the tile to see if it's well nestled into a decent spot
				//		-> True: Goto #3
				// 1) Calculate the distance vector between the new tile center and the first tile center
				// 2) Try random locations to minimize this vector - limit to a small number (we'll try 10)
				// 3) Check against the vector for edge adjacency to another tile. 
				//		-> True: Slide laterally (in the opposite dimension) to see if it minimizes the 
				//				 heuristic.
				//		-> False: Move against the vector (in both dimensions) using the slope to determine
				//				  the amount in each dimension until there is adjacency established. 
				// 4) Set the result and break the loop
				//

				int edgesFound = 0;

				// Condition:  2 / 4 edges are adjacent to another rectangle
				tiling.iterate([&boundary, &edgesFound] (brogueRoomInfo config, const gridRect& tile)
				{
					if (tile.isAdjacent(boundary))
						edgesFound++;

					if (edgesFound >= 2)
						return iterationCallback::breakAndReturn;

					return iterationCallback::iterate;
				});

				simpleVector<float> heuristic(boundary.centerX() - entranceBoundary.centerX(), boundary.centerY() - entranceBoundary.centerY());
				int counter = 0;

				// Try randomly a few times to minimize iteration work in the following loops
				while (edgesFound == 0 && counter++ < 10)
				{
					gridRect randomBoundary(_randomGenerator->randomIndex(nextLargestBoundary.left(), nextLargestBoundary.right() - boundary.width + 1),
											_randomGenerator->randomIndex(nextLargestBoundary.top(), nextLargestBoundary.bottom() - boundary.height + 1),
											boundary.width, boundary.height);

					simpleVector<float> nextHeuristic(randomBoundary.centerX() - entranceBoundary.centerX(),
													  randomBoundary.centerY() - entranceBoundary.centerY());

					if (nextHeuristic.magnitude() < heuristic.magnitude())
					{
						boundary = randomBoundary;
						heuristic = nextHeuristic;
					}
				}

				bool movement = true;

				// Slide into position
				while (movement)
				{
					int yMoves = simpleMath::floor(1 / heuristic.slope());
					int xMoves = simpleMath::floor(yMoves / heuristic.slope());

					// First, check overlapping tiles (before iterating positions independently)
					boundary.translate(xMoves, yMoves);

					bool validMove = true;

					tiling.iterate([&boundary, &validMove] (brogueRoomInfo config, const gridRect& tile)
					{
						if (boundary.overlaps(tile))
						{
							validMove = false;
							return iterationCallback::breakAndReturn;
						}
						return iterationCallback::iterate;
					});

					// Invalid:  Backout and try single dimension(s)
					if (!validMove)
					{
						boundary.translate(-1 * xMoves, -1 * yMoves);
					}
					else
						continue;

					// Move one-dimension, one-cell at a time; and check using the tile grid this time.
					//
					for (int index = 0; index < simpleMath::abs(xMoves) + simpleMath::abs(yMoves); index++)
					{
						bool yMove = (index >= simpleMath::abs(xMoves));

						if (!yMove)
							boundary.translate(simpleMath::sign(xMoves), 0);
						else
							boundary.translate(0, simpleMath::sign(yMoves));

						validMove = true;

						// Iterate just the tiles to save processing time (checking for invalid movement)
						tiling.iterate([&boundary, &validMove] (brogueRoomInfo config, const gridRect& tile)
						{
							if (boundary.overlaps(tile))
							{
								validMove = false;
								return iterationCallback::breakAndReturn;
							}
							return iterationCallback::iterate;
						});

						// Invalid:  Back out the translation and move onto the y-dimension
						if (!validMove)
						{
							// X-Dimension:  Can still check the y-dimension
							if (!yMove)
							{
								boundary.translate(-1 * simpleMath::sign(xMoves), 0);

								// Skip to the y-dimension
								index += (simpleMath::abs(xMoves) - index);
							}

							// Y-Dimension:  Must break the loop
							else
							{
								boundary.translate(0, -1 * simpleMath::sign(yMoves));
								movement = false;
								break;
							}
						}
					}
				}
			}

			// Fill in the tile on the grid (will be needed to calculate the largest sub-rectangle)
			//
			boundary.iterate([&tilingGrid] (short column, short row)
			{
				tilingGrid.set(column, row, gridLocator(column, row));
				return iterationCallback::iterate;
			});

			tiling.add(configuration, boundary);

			iteration++;
		}

		// Tiling is complete! Next, just iterate the tiles and fill in room cells
		//
		for (int index = 0; index < tiling.count(); index++)
		{
			brogueRoomInfo configuration = tiling.getAt(index)->key;
			gridRect boundary = tiling.getAt(index)->value;
			int padding = index == 0 ? 0 : 1;

			// (MEMORY!) These must be deleted; and the stack-like gridLocator instances will be copied into the brogueLayout* grid.
			gridRegion<gridLocator>* region = _roomGenerator->designRoom(configuration, _layout->getParentBoundary(), boundary, 0);

			_layout->createCells(region);

			delete region;
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

	bool layoutGenerator::attemptConnection(accretionTile* roomTile, const gridRect& attemptRect, short interRoomPadding) const
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

		for (int index = 0; index < _roomTiles->count(); index++)
		{
			accretionTile* tile = _roomTiles->get(index);

			bool northAttempt = false;
			bool southAttempt = false;
			bool eastAttempt = false;
			bool westAttempt = false;

			for (int tileAttempts = 1; tileAttempts <= 4; tileAttempts++)
			{
				gridLocator translation = gridLocator::getEmpty();

				// Left, Top, Right, Bottom
				if (!roomTile->hasEastConnection && !tile->hasWestConnection && !westAttempt)
				{
					translation = tile->connectionPointW.subtract(roomTile->connectionPointE);

					// Adjust the connection point by one cell + padding
					//translation.column -= (1 + interRoomPadding);

					westAttempt = true;
				}

				else if (!roomTile->hasNorthConnection && !tile->hasSouthConnection && !northAttempt)
				{
					translation = tile->connectionPointN.subtract(roomTile->connectionPointS);

					// Adjust the connection point by one cell + padding
					//translation.row -= (1 + interRoomPadding);

					northAttempt = true;
				}

				else if (!roomTile->hasEastConnection && !tile->hasWestConnection && !eastAttempt)
				{
					translation = tile->connectionPointE.subtract(roomTile->connectionPointW);

					// Adjust the connection point by one cell + padding
					//translation.column += (1 + interRoomPadding);

					eastAttempt = true;
				}

				else if (!roomTile->hasSouthConnection && !tile->hasNorthConnection && !southAttempt)
				{
					translation = tile->connectionPointS.subtract(roomTile->connectionPointN);

					// Adjust the connection point by one cell + padding
					//translation.row += (1 + interRoomPadding);

					southAttempt = true;
				}
				else
					break;

				// Translate the region tile into position (this affects all grid locators, and connection points)
				if (!roomTile->attemptTranslastion(translation))
					continue;

				// Check the boundary:  1) outer gridRect first to reduce iteration, and 2) the grid overlap per cell
				if (attemptRect.contains(roomTile->region->getBoundary()))
				{
					if (tile->region->overlaps(roomTile->region))
					{
						//delete roomTile->region;
						continue;
					}

					bool gridOverlaps = false;
					brogueLayout* layout = _layout;

					// Double check the already-set rooms
					roomTile->region->iterateLocations([&layout, &gridOverlaps] (short column, short row, gridLocator item)
					{
						if (layout->isDefined(item.column, item.row))
						{
							gridOverlaps = true;
							return iterationCallback::breakAndReturn;
						}

						return iterationCallback::iterate;
					});

					if (gridOverlaps)
					{
						//delete roomTile->region;
						continue;
					}

					// Set connection point data (should be using a direction iterator) [1, 4] -> [W,N,E,S]
					if (index == 1)
					{
						roomTile->connectionPointE = tile->connectionPointW;
						roomTile->hasEastConnection = true;
					}
					else if (index == 2)
					{
						roomTile->connectionPointS = tile->connectionPointN;
						roomTile->hasSouthConnection = true;
					}
					else if (index == 3)
					{
						roomTile->connectionPointW = tile->connectionPointE;
						roomTile->hasWestConnection = true;
					}
					else
					{
						roomTile->connectionPointN = tile->connectionPointS;
						roomTile->hasNorthConnection = true;
					}

					return true;
				}
			}
		}

		return false;
	}

	void layoutGenerator::triangulateRooms()
	{
		// Create delaunay triangulator with graph edge constructor
		delaunayAlgorithm<gridLocator, gridLocatorEdge> triangulator([] (gridLocator node1, gridLocator node2)
		{
			gridLocatorEdge edge(node1, node2);

			return edge;
		});

		// Create connection point vertices
		simpleList<gridLocator> connectionNodes;

		_roomTiles->forEach([&connectionNodes] (accretionTile* item)
		{
			if (item->hasEastConnection)
				connectionNodes.add(gridLocator(item->connectionPointE));

			if (item->hasNorthConnection)
				connectionNodes.add(gridLocator(item->connectionPointN));

			if (item->hasSouthConnection)
				connectionNodes.add(gridLocator(item->connectionPointS));

			if (item->hasWestConnection)
				connectionNodes.add(gridLocator(item->connectionPointW));

			return iterationCallback::iterate;
		});

		_delaunayGraph = triangulator.run(connectionNodes);
	}

	void layoutGenerator::connectRooms()
	{
		// Procedure
		//
		// 1) Iterate Edges: DELAUNAY GRAPH
		//      -> Find edges that are not self-referential (use room tiles)
		//      -> Collect these edges to pass to dijkstra
		//
		// 2) Run Dijkstra to set cells in the primary grid
		//

		simpleList<gridLocatorEdge> corridorEdges;
		simpleList<accretionTile*>* roomTiles = _roomTiles;
		brogueLayout* layout = _layout;

		_delaunayGraph->iterateEdges([&roomTiles, &corridorEdges] (const gridLocatorEdge& edge)
		{
			bool isCorridorEdge = false;

			roomTiles->forEach([&edge, &isCorridorEdge] (accretionTile* tile)
			{
				int connectionCount = 0;

				// North
				if (tile->hasNorthConnection &&
					(tile->connectionPointN == edge.node1 ||
					tile->connectionPointN == edge.node2))
					connectionCount++;

				// South
				if (tile->hasSouthConnection &&
					(tile->connectionPointS == edge.node1 ||
					tile->connectionPointS == edge.node2))
					connectionCount++;

				// East
				if (tile->hasEastConnection &&
					(tile->connectionPointE == edge.node1 ||
					tile->connectionPointE == edge.node2))
					connectionCount++;

				// West
				if (tile->hasWestConnection &&
					(tile->connectionPointW == edge.node1 ||
					tile->connectionPointW == edge.node2))
					connectionCount++;

				if (connectionCount == 1)
				{
					isCorridorEdge = true;
					return iterationCallback::breakAndReturn;
				}

				return iterationCallback::iterate;
			});

			if (isCorridorEdge)
				corridorEdges.add(edge);

			return iterationCallback::iterate;
		});

		// (MEMORY!) Use the corridor edges to call dijkstra (on stack usgae has predicate copying)
		dijkstra<gridLocator>* algorithm = new dijkstra<gridLocator>(
			_layout->getParentBoundary(),
			_layout->getBoundary(),
			true,                           // Cardinal Movement (for laying corridors)

		// Primary Inclusion Predicate (is it in the grid?)
		[&layout](short column, short row)
		{
			return layout->isDefined(column, row);
		},

		// Then, the map cost is queried (what is the movement cost?)
		[&layout](short column, short row)
		{
			return 1;
		},

		// Then, it will need the locators from the grid to keep its internal data temporarily
		[&layout](short column, short row)
		{
			// May need design change for this problem (needed to copy grid locators back for dijkstra)
			return gridLocator(column, row);
		});

		// Iterate each corridor edge and run dijkstra to finalize the connection
		corridorEdges.forEach([&algorithm, &layout] (const gridLocatorEdge& edge)
		{
			gridLocator source = edge.node1;
			gridLocator targets[1] = { edge.node2 };

			// Run Dijkstra
			algorithm->initialize(source, simpleArray<gridLocator>(targets, 1));
			algorithm->run();

			simpleArray<gridLocator> resultPath = algorithm->getResultPath(targets[0]);

			// Set corridors
			resultPath.forEach([&layout] (gridLocator locator)
			{
				layout->createCells(locator);

				return iterationCallback::iterate;
			});

			return iterationCallback::iterate;
		});

		// Clean up memory
		delete algorithm;
	}
}

