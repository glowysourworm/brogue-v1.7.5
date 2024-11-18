#pragma once

#include "accretionTile.h"
#include "brogueGlobal.h"
#include "brogueLayout.h"
#include "brogueLevelProfile.h"
#include "randomGenerator.h"
#include "roomGenerator.h"

#include "delaunayAlgorithm.h"
#include "dijkstra.h"
#include "dungeon.h"
#include "dungeonConstants.h"
#include "graph.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRect.h"
#include "gridRegion.h"
#include "gridRegionConstructor.h"
#include "simple.h"
#include "simpleArray.h"
#include "simpleException.h"
#include "simpleList.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model;

namespace brogueHd::backend
{
	class layoutGenerator
	{
	public:

		/// <summary>
		/// Creates the base layout, terrain, and machine terrain for the leve
		/// </summary>
		/// <param name="profile">The dungeon profile:  must contain all parameters</param>
		layoutGenerator(randomGenerator* randomGenerator);
		~layoutGenerator();

		brogueLayout* generateLayout(brogueLevelProfile* profile);

	private:

		simpleList<accretionTile> createRooms(brogueLayout* layout, brogueLevelProfile* profile);

		bool attemptConnection(brogueLayout* layout, const simpleList<accretionTile>& roomTiles, accretionTile& roomTile, const gridRect& attemptRect, short interRoomPadding) const;

		//void designateMachineRooms();

		graph<gridLocator, gridLocatorEdge> triangulateRooms(brogueLayout* layout, const simpleList<accretionTile>& roomTiles);

		void connectRooms(brogueLayout* layout, const simpleList<accretionTile>& roomTiles, const graph<gridLocator, gridLocatorEdge>& delaunayGraph);
		//void createTerrain();

		/// <summary>
		/// Returns the Brogue level boundary with padding
		/// </summary>
		gridRect getPaddedBoundary(short padding) const;

	private:

		roomGenerator* _roomGenerator;
		randomGenerator* _randomGenerator;
	};


	layoutGenerator::layoutGenerator(randomGenerator* randomGenerator)
	{
		_randomGenerator = randomGenerator;
	}

	layoutGenerator::~layoutGenerator()
	{
	}

	gridRect layoutGenerator::getPaddedBoundary(short padding) const
	{
		if (padding >= 10)
			simpleException::showCstr("Invalid level padding:  must be <= 10");

		return gridRect(padding, padding, DCOLS - padding, DROWS - padding);
	}

	brogueLayout* layoutGenerator::generateLayout(brogueLevelProfile* profile)
	{
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

		gridRect levelBoundary = this->getPaddedBoundary(0);
		gridRect levelPaddedBoundary = this->getPaddedBoundary(1);

		brogueLayout* layout = new brogueLayout(levelBoundary, levelPaddedBoundary);

		// Create Rooms
		simpleList<accretionTile> roomTiles = createRooms(layout, profile);

		// Machine Rooms

		// Triangulate Rooms:  Creates Delaunay Triangulation of the connection point vertices
		graph<gridLocator, gridLocatorEdge> delaunayGraph = triangulateRooms(layout, roomTiles);

		// Connect Rooms:  Create cells in the grid for the delaunay triangulation of the 
		//                 connection points of the room tiles
		connectRooms(layout, roomTiles, delaunayGraph);

		return layout;
	}

	simpleList<accretionTile> layoutGenerator::createRooms(brogueLayout* layout, brogueLevelProfile* profile)
	{
		// Brogue v1.7.5 Creates up to 35 rooms using accretion (bolt-on) to fill up the space
		//

		short maxAttempts = 35;
		short interRoomPadding = 1;
		simpleList<accretionTile> attemptRegions;
		simpleList<accretionTile> roomTiles;

		// 1) Create 35 room (up front), 2) Attempt bolt-on until there are no more fits
		//
		for (short index = 0; index < maxAttempts; index++)
		{
			// Room configuration from the template
			brogueRoomInfo configuration = index == 0 ? profile->getEntranceRoom(_randomGenerator) : profile->getRandomRoomInfo(_randomGenerator);

			// MEMORY!
			gridRegion<gridLocator>* nextRegion = _roomGenerator->designRoom(configuration, layout->getParentBoundary(), layout->getBoundary());

			// Connection Points					
			simpleArray<gridLocator> northEdge = nextRegion->getBoundaryEdges(brogueCompass::N);
			simpleArray<gridLocator> southEdge = nextRegion->getBoundaryEdges(brogueCompass::S);
			simpleArray<gridLocator> eastEdge = nextRegion->getBoundaryEdges(brogueCompass::E);
			simpleArray<gridLocator> westEdge = nextRegion->getBoundaryEdges(brogueCompass::W);

			int randN = _randomGenerator->randomRange(0, northEdge.count());
			int randS = _randomGenerator->randomRange(0, southEdge.count());
			int randE = _randomGenerator->randomRange(0, eastEdge.count());
			int randW = _randomGenerator->randomRange(0, westEdge.count());

			accretionTile nextRoom;

			// (Pointer to allocated region memory)
			nextRoom.region = nextRegion;
			nextRoom.connectionPointN = northEdge.get(randN);
			nextRoom.connectionPointS = southEdge.get(randS);
			nextRoom.connectionPointE = eastEdge.get(randE);
			nextRoom.connectionPointW = westEdge.get(randW);

			attemptRegions.add(nextRoom);
		}

		// Bolt-on
		for (int index = 0; index < attemptRegions.count(); index++)
		{
			accretionTile attemptRegion = attemptRegions.get(index);

			// First Room
			if (index == 0)
			{
				// NOTE:  Rooms are generated in their supposed 1st-attempt location. This may
				//        have been by design (Brogue v1.7.5); but probably only matters for
				//        the first level.
				//
				roomTiles.add(attemptRegion);

				// Set final room locations
				layout->createCells(*attemptRegion.region);
			}

			// Accrete rooms - translating the attempt room into place (modifies attempt region)
			else if (attemptConnection(layout, roomTiles, attemptRegion, layout->getParentBoundary(), interRoomPadding))
			{
				roomTiles.add(attemptRegion);

				// Set final room locations
				layout->createCells(*attemptRegion.region);
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

		//attachRooms(grid, &theDP, 35, 35);\\

		return roomTiles;
	}

	bool layoutGenerator::attemptConnection(brogueLayout* layout,
											const simpleList<accretionTile>& roomTiles,
											accretionTile& roomTile,
											const gridRect& attemptRect,
											short interRoomPadding) const
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

		for (int index = 0; index < roomTiles.count(); index++)
		{
			accretionTile tile = roomTiles.get(index);

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

				// Translate the region into position (HEAP ALLOCATED! ALSO CHANGES LOCATOR INSTANCES)
				roomTile.region->translate_StackLike(translation.column, translation.row);

				// Check the boundary:  1) outer gridRect first to reduce iteration, and 2) the grid overlap per cell
				if (attemptRect.contains(roomTile.region->getBoundary()))
				{
					if (tile.region->overlaps(*roomTile.region))
					{
						delete roomTile.region;
						continue;
					}

					bool gridOverlaps = false;

					// Double check the already-set rooms
					roomTile.region->iterateLocations([&layout, &gridOverlaps] (short column, short row, gridLocator item)
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
						delete roomTile.region;
						continue;
					}

					// Set connection point data (should be using a direction iterator) [1, 4] -> [W,N,E,S]
					if (index == 1)
					{
						roomTile.connectionPointE = tile.connectionPointW;
						roomTile.hasEastConnection = true;
					}
					else if (index == 2)
					{
						roomTile.connectionPointS = tile.connectionPointN;
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

		return false;
	}

	graph<gridLocator, gridLocatorEdge> layoutGenerator::triangulateRooms(brogueLayout* layout, const simpleList<accretionTile>& roomTiles)
	{
		// Create delaunay triangulator with graph edge constructor
		delaunayAlgorithm<gridLocator, gridLocatorEdge> triangulator([] (gridLocator node1, gridLocator node2)
		{
			gridLocatorEdge edge(node1, node2);

			return edge;
		});

		// Create connection point vertices
		simpleList<gridLocator> connectionNodes;

		roomTiles.forEach([&connectionNodes] (accretionTile item)
		{
			if (item.hasEastConnection)
				connectionNodes.add(gridLocator(item.connectionPointE));

			if (item.hasNorthConnection)
				connectionNodes.add(gridLocator(item.connectionPointN));

			if (item.hasSouthConnection)
				connectionNodes.add(gridLocator(item.connectionPointS));

			if (item.hasWestConnection)
				connectionNodes.add(gridLocator(item.connectionPointW));

			return iterationCallback::iterate;
		});

		return triangulator.run(connectionNodes);
	}

	void layoutGenerator::connectRooms(brogueLayout* layout, const simpleList<accretionTile>& roomTiles, const graph<gridLocator, gridLocatorEdge>& delaunayGraph)
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

		delaunayGraph.iterateEdges([&roomTiles, &corridorEdges] (gridLocatorEdge edge)
		{
			bool isCorridorEdge = false;

			roomTiles.forEach([&edge, &isCorridorEdge] (accretionTile tile)
			{
				int connectionCount = 0;

				// North
				if (tile.hasNorthConnection &&
					(tile.connectionPointN == edge.node1 ||
					tile.connectionPointN == edge.node2))
					connectionCount++;

				// South
				if (tile.hasSouthConnection &&
					(tile.connectionPointS == edge.node1 ||
					tile.connectionPointS == edge.node2))
					connectionCount++;

				// East
				if (tile.hasEastConnection &&
					(tile.connectionPointE == edge.node1 ||
					tile.connectionPointE == edge.node2))
					connectionCount++;

				// West
				if (tile.hasWestConnection &&
					(tile.connectionPointW == edge.node1 ||
					tile.connectionPointW == edge.node2))
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
			layout->getParentBoundary(),
			layout->getBoundary(),
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
		corridorEdges.forEach([&algorithm, &layout] (gridLocatorEdge edge)
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

