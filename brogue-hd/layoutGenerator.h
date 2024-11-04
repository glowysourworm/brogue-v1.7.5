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
#include "grid.h"
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
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::generator
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

		void initialize(brogueLevelProfile* profile);
		void clear();

		brogueLayout* generateLayout();

	private:

		void createRooms();

		// (MEMORY!) Creates a new room according to the specified room info
		gridRegion<gridLocator>* createRoom(gridRect levelBoundary, gridRect levelPaddedBoundary, const brogueRoomInfo& roomInfo);

		bool attemptConnection(accretionTile& roomTile, const gridRect& attemptRect, short interRoomPadding) const;

		//void designateMachineRooms();
		void triangulateRooms();
		void connectRooms();
		//void createTerrain();

		/// <summary>
		/// Returns the Brogue level boundary with padding
		/// </summary>
		gridRect getPaddedBoundary(short padding) const;

		// Managed On: clear(), initialize(...)
	private:

		grid<gridLocator>* _grid;

		simpleList<accretionTile>* _roomTiles;

		graph<gridLocator, gridLocatorEdge>* _delaunayGraph;

		brogueLevelProfile* _profile;

		bool _initialized;

	private:

		roomGenerator* _roomGenerator;
		randomGenerator* _randomGenerator;
	};


	layoutGenerator::layoutGenerator(randomGenerator* randomGenerator)
	{
		if (!randomGenerator->checkId(RANDOM_GENERATOR_MAIN))
			simpleException::showCstr("Improper use of random number generator:  layoutGenerator.cpp");

		_roomTiles = new simpleList<accretionTile>();
		_randomGenerator = randomGenerator;
		_initialized = false;
	}

	layoutGenerator::~layoutGenerator()
	{
		this->clear();

		delete _roomTiles;

		_initialized = false;
	}

	void layoutGenerator::clear()
	{
		_roomTiles->clear();

		delete _grid;

		_initialized = false;
	}

	void layoutGenerator::initialize(brogueLevelProfile* profile)
	{
		this->clear();

		gridRect levelBoundary = this->getPaddedBoundary(0);
		gridRect levelPaddedBoundary = this->getPaddedBoundary(1);

		_profile = profile;
		_grid = new grid<gridLocator>(levelBoundary, levelPaddedBoundary);

		_initialized = true;
	}

	gridRect layoutGenerator::getPaddedBoundary(short padding) const
	{
		if (padding >= 10)
			simpleException::showCstr("Invalid level padding:  must be <= 10");

		return gridRect(padding, padding, DCOLS - padding, DROWS - padding);
	}

	brogueLayout* layoutGenerator::generateLayout()
	{
		if (!_initialized)
			simpleException::showCstr("Layout generator not properly intiialized:  call initialize() before generateLayout()");

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

		// Create Rooms
		this->createRooms();

		// Machine Rooms

		// Triangulate Rooms:  Creates Delaunay Triangulation of the connection point vertices
		this->triangulateRooms();

		// Connect Rooms:  Create cells in the grid for the delaunay triangulation of the 
		//                 connection points of the room tiles
		this->connectRooms();

		return NULL;
	}

	void layoutGenerator::createRooms()
	{
		// Brogue v1.7.5 Creates up to 35 rooms using accretion (bolt-on) to fill up the space
		//

		short maxAttempts = 35;
		short interRoomPadding = 1;
		simpleList<accretionTile> attemptRegions;

		// 1) Create 35 room (up front), 2) Attempt bolt-on until there are no more fits
		//
		for (short index = 0; index < maxAttempts; index++)
		{
			// MEMORY!
			gridRegion<gridLocator>* nextRegion = index == 0 ? this->createRoom(_grid->getParentBoundary(), _grid->getRelativeBoundary(), _profile->getEntranceRoom()) :
				this->createRoom(_grid->getParentBoundary(), _grid->getRelativeBoundary(), _profile->getRandomRoomInfo());

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
				_roomTiles->add(attemptRegion);

				// Set final room locations
				attemptRegion.region->copyTo(*_grid);
			}

			// Accrete rooms - translating the attempt room into place (modifies attempt region)
			else if (attemptConnection(attemptRegion, _grid->getParentBoundary(), interRoomPadding))
			{
				_roomTiles->add(attemptRegion);

				// Set final room locations
				attemptRegion.region->copyTo(*_grid);
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

		//attachRooms(grid, &theDP, 35, 35);
	}

	bool layoutGenerator::attemptConnection(accretionTile& roomTile, const gridRect& attemptRect, short interRoomPadding) const
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
			accretionTile tile = _roomTiles->get(index);

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
				gridRegion<gridLocator>* translatedRegion = gridRegionConstructor<gridLocator>::translate(*roomTile.region, translation);

				// Check the boundary:  1) outer gridRect first to reduce iteration, and 2) the grid overlap per cell
				if (attemptRect.contains(translatedRegion->getBoundary()))
				{
					if (tile.region->overlaps(*translatedRegion))
					{
						delete translatedRegion;
						continue;
					}

					bool gridOverlaps = false;
					grid<gridLocator>* grid = _grid;

					// Double check the already-set rooms
					translatedRegion->iterateLocations([&grid, &gridOverlaps] (short column, short row, gridLocator item)
					{
						if (grid->isDefined(item.column, item.row))
						{
							gridOverlaps = true;
							return iterationCallback::breakAndReturn;
						}

						return iterationCallback::iterate;
					});

					if (gridOverlaps)
					{
						delete translatedRegion;
						continue;
					}

					roomTile.region = translatedRegion;

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

	gridRegion<gridLocator>* layoutGenerator::createRoom(gridRect levelBoundary, gridRect levelPaddedBoundary, const brogueRoomInfo& roomInfo)
	{
		gridRect minSize, maxSize;

		// Tracks the cell additions to the grid
		//
		gridRegionConstructor<gridLocator> constructor(levelBoundary, [] (short column, short row, gridLocator locator)
		{
			// Inclusion Predicate
			return true;
		});

		switch (roomInfo.type)
		{
		case roomTypes::CrossRoom:
		_roomGenerator->designCrossRoom(constructor);
		break;
		case roomTypes::SmallSymmetricalCrossRoom:
		_roomGenerator->designSymmetricalCrossRoom(constructor);
		break;
		case roomTypes::SmallRoom:
		_roomGenerator->designSmallRoom(constructor);
		break;
		case roomTypes::CircularRoom:
		_roomGenerator->designCircularRoom(constructor, levelBoundary, levelPaddedBoundary);
		break;
		case roomTypes::ChunkyRoom:
		_roomGenerator->designChunkyRoom(constructor, levelBoundary, levelPaddedBoundary);
		break;
		case roomTypes::Cave:

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
		maxSize.height = DROWS - 2;
		break;
		case 2:
		// Large east-west cave room.
		minSize.width = 20;
		minSize.height = 4;
		maxSize.width = DROWS - 2;
		maxSize.height = 8;
		break;
		default:
		break;
		}
		break;

		_roomGenerator->designCavern(constructor, levelBoundary, levelPaddedBoundary, minSize, maxSize);

		case roomTypes::Cavern:

		minSize.width = CAVE_MIN_WIDTH;
		minSize.height = CAVE_MIN_HEIGHT;
		maxSize.width = DCOLS - 2;
		maxSize.height = DROWS - 2;

		_roomGenerator->designCavern(constructor, levelBoundary, levelPaddedBoundary, minSize, maxSize);

		break;
		case roomTypes::MainEntranceRoom:
		_roomGenerator->designEntranceRoom(constructor);
		break;
		default:
		break;
		}

		return constructor.complete();
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

		_roomTiles->forEach([&connectionNodes] (accretionTile item)
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
		simpleList<accretionTile> roomTiles = *_roomTiles;

		_delaunayGraph->iterateEdges([&roomTiles, &corridorEdges] (gridLocatorEdge edge)
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

		grid<gridLocator> grid = *_grid;

		// Use the corridor edges to call dijkstra
		dijkstra<gridLocator> algorithm(
			_grid->getParentBoundary(),
			_grid->getRelativeBoundary(),
			true,                           // Cardinal Movement (for laying corridors)

			// Primary Inclusion Predicate (is it in the grid?)
			[&grid](short column, short row)
		{
			return grid.isDefined(column, row);
		},

			// Then, the map cost is queried (what is the movement cost?)
			[&grid](short column, short row)
		{
			return 1;
		},

			// Then, it will need the locators from the grid to keep its internal data temporarily
			[&grid](short column, short row)
		{
			return grid.get(column, row);
		});

		// Iterate each corridor edge and run dijkstra to finalize the connection
		corridorEdges.forEach([&algorithm, &grid] (gridLocatorEdge edge)
		{
			gridLocator source = edge.node1;
			gridLocator targets[1] = { edge.node2 };

			// Run Dijkstra
			algorithm.initialize(source, simpleArray<gridLocator>(targets, 1));
			algorithm.run();

			simpleArray<gridLocator> resultPath = algorithm.getResultPath(targets[0]);

			// Set corridors
			resultPath.forEach([&grid] (gridLocator locator)
			{
				grid.set(locator.column, locator.row, locator);

				return iterationCallback::iterate;
			});

			return iterationCallback::iterate;
		});
	}
}

