#pragma once

#include <simple.h>
#include <simpleArray.h>
#include <simpleException.h>
#include <simpleHash.h>
#include <simpleLine.h>
#include <simpleList.h>
#include <color.h>

#include <simplePoint.h>
#include <simpleRectangle.h>
#include <simpleGraph.h>
#include <delaunayAlgorithm.h>
#include <primsAlgorithm.h>
#include <dungeonConstants.h>
#include <brogueRoomTemplate.h>

#include <brogueLevelProfile.h>

#include "brogueDesignRect.h"
#include "brogueLayout.h"
#include "brogueCell.h"

#include "randomGenerator.h"
#include "roomGenerator.h"
#include "layoutCoordinateConverter.h"

#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRect.h"
#include "gridRegion.h"
#include "noiseGenerator.h"
#include "rectanglePackingAlgorithm.h"
#include <limits>
#include "dijkstra.h"

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;
	using namespace brogueHd::model;

	class layoutGenerator
	{
	public:
		/// <summary>
		/// Creates the base layout, terrain, and machine terrain for the level.
		/// </summary>
		layoutGenerator(randomGenerator* randomGenerator, const gridRect& layoutParentBoundary, int zoomLevel);
		~layoutGenerator();

		brogueLayout* generateLayout(brogueLevelProfile* profile);

		/// <summary>
		/// Clears the layout generation data out for another use. Must set up once per level.
		/// </summary>
		void clear();

	private:
		void initialize(brogueLevelProfile* profile);

		void createRooms();

		//bool attemptConnection(brogueDesignRect* roomTile, const gridRect& attemptRect, int interRoomPadding) const;

		//void designateMachineRooms();

		void triangulateRooms();
		void triangulateRoomConnections();

		void connectRooms();

	private:
		simpleGraph<gridLocator, gridLocatorEdge>* triangulate(const simpleList<gridLocator>& locations);

	private:
		roomGenerator* _roomGenerator;
		randomGenerator* _randomGenerator;
		layoutCoordinateConverter* _coordinateConverter;

		// Generation Data Stores:
		//
		brogueLayout* _layout;
		brogueLevelProfile* _profile;
		simpleList<brogueDesignRect*>* _roomTiles;
		simpleGraph<gridLocator, gridLocatorEdge>* _roomGraph;
		simpleGraph<gridLocator, gridLocatorEdge>* _connectionGraph;
	};


	layoutGenerator::layoutGenerator(randomGenerator* randomGenerator, const gridRect& layoutParentBoundary,
	                                 int zoomLevel)
	{
		_randomGenerator = randomGenerator;
		_roomGenerator = new roomGenerator(randomGenerator);
		_coordinateConverter = new layoutCoordinateConverter(layoutParentBoundary, zoomLevel);

		// Layout Data
		_layout = nullptr;
		_profile = nullptr;
		_roomTiles = nullptr;
		_roomGraph = nullptr;
		_connectionGraph = nullptr;
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
		delete _coordinateConverter;
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

		// Create the level boundary (add 1 to the layout (parent) boundary)
		gridRect levelBoundary = _coordinateConverter->getLayoutBoundary();
		gridRect levelPaddedBoundary = levelBoundary.createPadded(1);

		_layout = new brogueLayout(levelBoundary, levelPaddedBoundary);
		_profile = profile;
		_roomTiles = new simpleList<brogueDesignRect*>();
		_roomGraph = nullptr;
		_connectionGraph = nullptr;
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
		//		-> Calculate room region "centroids"
		//      -> Create Delaunay Triangulation of centroid centers (becomes room graph later)
		//      -> Create nearest neighbor pairs using region edge locations
		//		-> (Optional) Remove connections backwards towards MST using Prim's Algorithm
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
		triangulateRooms();
		triangulateRoomConnections();

		// Connect Rooms:  Create cells in the grid for the delaunay triangulation of the 
		//                 connection points of the room tiles. Runs dijkstra's algorithm to
		//				   create corridor cells inside the brogueLayout*
		connectRooms();

		return _layout;
	}

	void layoutGenerator::createRooms()
	{
		// Brogue v1.7.5 Creates up to 35 rooms using accretion (bolt-on) to fill up the space
		//
		//
		// Update:  (see backlog.txt) This will now be done using a tiling method.
		//

		rectanglePackingAlgorithm algorithm(_randomGenerator, _layout->getParentBoundary(), _layout->getBoundary());

		// Keep track of the actual tiles in the layout. The algorithm must have a copy with which to
		// work; and will not see the fidgeting with our copy of the gridRect on the stack.
		//
		simpleHash<brogueDesignRect*, gridRect> tiling;

		gridLocator rectPackingFocus;
		int maxIterations = 35;
		int iteration = 0;

		while (iteration < maxIterations)
		{
			int randomRoomIndex = _randomGenerator->randomRangeExclusive(0, _profile->getRoomInfoCount());

			brogueRoomTemplate configuration = (iteration == 0)
				                                   ? _profile->getEntranceRoom()
				                                   : _profile->getRoomInfo(randomRoomIndex);
			brogueDesignRect* designRect = nullptr;
			int roomPadding = iteration == 0 ? 0 : 1;

			// Entrance Room Selection
			//
			if (iteration == 0)
			{
				designRect = new brogueDesignRect(configuration, _layout->getBoundary().createPadded(1), roomPadding);

				gridLocator topLeft = gridLocator(
					_layout->getBoundary().centerX() - (designRect->getBoundary().width / 2),
					_layout->getBoundary().bottom() - designRect->getBoundary().height);
				designRect->setOffset(topLeft);

				rectPackingFocus.column = designRect->getBoundary().centerX();
				rectPackingFocus.row = designRect->getBoundary().bottom();

				// Keep track of our tiling
				//
				algorithm.initialize(designRect->getBoundary(), rectPackingFocus);
				tiling.add(designRect, designRect->getBoundary());
			}

			// Add rectangles to the space using the algorithm (Padding = 1)
			//
			else
			{
				// Get next space in which to create a room
				gridRect minRect(configuration.getMinSize());
				gridRect largestSubRect = algorithm.getLargestUnusedRectangle(minRect.createExpanded(roomPadding));

				if (largestSubRect == default_value::value<gridRect>())
				{
					iteration++;
					continue;
				}

				brogueDesignRect designRectPreLayout(configuration, largestSubRect, roomPadding);

				if (!largestSubRect.contains(designRectPreLayout.getBoundary())) // Check room space constraints
				{
					iteration++;

					delete designRect;
					continue;
				}

				// Use packing algorithm to place a room rectangle inside the constrained area; and 
				// put it as close to the focal point as possible.
				gridRect paddedBoundary = designRectPreLayout.getBoundary();
				algorithm.addRectangle(paddedBoundary);

				// Store the result as the constrained boundary (removing the padding)
				designRect = new brogueDesignRect(configuration, paddedBoundary, 0);

				// Update the tiling
				tiling.add(designRect, paddedBoundary);

				// Offset randomly inside the available space
				//gridLocator translation(_randomGenerator->randomRangeInclusive(0, largestSubRect.width - designRect->getBoundary().width),
				//						_randomGenerator->randomRangeInclusive(0, largestSubRect.height - designRect->getBoundary().height));


				//designRect->translate(translation);

				// Give the algorithm a rectangle to tile
				//gridRect nextRect = designRect->getBoundary();

				// Success
				//if (algorithm.addRectangle(nextRect))
				//{
				// Use the rectangle to translate the design rect
				//designRect->setOffset(nextRect.topLeft());

				//tiling.add(designRect);
				//}
			}

			// Complete:
			//
			// 1) Create grid cells for the tile's room
			// 2) Translate the result inside of its design grid rect (towards the entrance room)
			// 3) Calculate the final boundary (setting it into the brogueDesignRect)
			// 4) Remove / Replace the rectangle in the algorithm with the final one
			//

			// (MEMORY!) These must be deleted; and the stack-like gridLocator instances will be copied into the brogueLayout* grid.
			gridRegion<gridLocator>* region = _roomGenerator->designRoom(designRect->getConfiguration().getRoomType(),
			                                                             designRect->getBoundary().createPadded(
				                                                             roomPadding),
			                                                             designRect->getMinSize(),
			                                                             _layout->getBoundary());

			// Set the actual region boundary
			designRect->complete(region);

			// Repack the rectangle if it is not the entrance
			if (iteration != 0)
			{
				// Since the design rect was built with a padded boundary (for the room generator), the
				// tile in the packing should still have room for the padded boundary of the actual rect.
				// 
				// This "actual rect" will be the exact smallest boundary for the region. To fit this 
				// properly into the layout we have to expand by the size of the padding; and move it
				// inside the design rect's boundary.
				//
				gridRect actualRect = designRect->getActualBoundary();
				gridRect boundary = designRect->getBoundary();

				// Add the padding back to the actual rect
				gridRect actualRectPadded = actualRect.createExpanded(roomPadding);
				gridLocator translation(0, 0);

				// Check the boundary to see where it falls outside the prescribed bounds.
				if (actualRectPadded.left() < boundary.left())
					translation.column = boundary.left() - actualRectPadded.left();

				if (actualRectPadded.right() > boundary.right())
					translation.column = boundary.right() - actualRectPadded.right();

				if (actualRectPadded.top() < boundary.top())
					translation.row = boundary.top() - actualRectPadded.top();

				if (actualRectPadded.bottom() > boundary.bottom())
					translation.row = boundary.bottom() - actualRectPadded.bottom();

				// Modify the region using a translation
				designRect->translate(translation, true);
			}

			iteration++;
		}

		// Place Region Data
		brogueLayout* layout = _layout;
		simpleList<brogueDesignRect*>* roomTiles = _roomTiles;

		tiling.forEach([&layout, &roomTiles](brogueDesignRect* designRect, const gridRect& boundary)
		{
			// Design Tile
			boundary.iterate([&layout](int column, int row)
			{
				brogueCell prototype(column, row, color(0.0f, 0.0f, 0.3f, 0.2f), colors::white(),
				                     brogueGlyphMap::Empty);

				layout->createCells(gridLocator(column, row), prototype);

				return iterationCallback::iterate;
			});

			// Design Boundary
			designRect->getBoundary().iterate([&layout](int column, int row)
			{
				brogueCell prototype(column, row, color(0.3f, 0.0f, 0.3f, 0.2f), colors::white(),
				                     brogueGlyphMap::Empty);

				layout->createCells(gridLocator(column, row), prototype, true);

				return iterationCallback::iterate;
			});

			// Actual Boundary
			designRect->getActualBoundary().iterate([&layout](int column, int row)
			{
				brogueCell prototype(column, row, color(0.0f, 0.5f, 1.0f, 0.2f), colors::white(),
				                     brogueGlyphMap::Empty);

				layout->createCells(gridLocator(column, row), prototype, true);

				return iterationCallback::iterate;
			});

			// Room Floor
			designRect->getRegion()->iterateLocations(
				[&layout, &designRect](int column, int row, const gridLocator& location)
				{
					color backColor(0.3f, 0.3f, 0.3f, 0.5f);

					//if (designRect->getConfiguration().getRoomType() == brogueRoomType::ChunkyRoom)
					//{
					//	backColor.red = 1;
					//}

					brogueCell prototype(column, row, backColor, colors::white(), '.');

					layout->createCells(gridLocator(column, row), prototype, true);

					return iterationCallback::iterate;
				});

			// Set room tiles (DON'T FORGET REGION MEMORY)
			roomTiles->add(designRect);

			return iterationCallback::iterate;
		});
	}

	simpleGraph<gridLocator, gridLocatorEdge>* layoutGenerator::triangulate(const simpleList<gridLocator>& locations)
	{
		// Procedure
		//
		// 1) Create real-number coordinate transform of the locations
		// 2) Create delaunay graph of the locations
		// 3) Validate the result
		//		-> Ensure all locations have edge connections
		//		-> Ensure all locations are accounted for
		// 4) Translate the graph back into grid coordinates
		//

		// Create delaunay triangulator with graph edge constructor
		//delaunayAlgorithm triangulator;
		primsAlgorithm triangulator;

		// Convert grid locators to real number coordinates: This is to run bowyer-watson
		//
		layoutCoordinateConverter* converter = _coordinateConverter;

		// Convert the locators
		//
		simpleList<simplePoint<float>> connectionNodes = locations.select<simplePoint<float>>(
			[&converter](const gridLocator& center)
			{
				// Scales by the cell size; and flips the y-axis
				//
				return converter->convertToUIReal(center, true);
			});

		// (MEMORY!) Create delaunay graph
		simpleGraph<simplePoint<float>, simpleLine<float>>* delaunayGraph = triangulator.run(
			connectionNodes, [](simplePoint<float> node1, simplePoint<float> node2)
			{
				// This constructor is for default graphs (3 or less nodes)
				return simpleLine<float>(node1, node2);
			});

		// (MEMORY!) Re-scale back to the grid locators
		simpleGraph<gridLocator, gridLocatorEdge>* resultGraph = new simpleGraph<gridLocator, gridLocatorEdge>();

		// Convert Back / Validate
		//
		delaunayGraph->iterate(
			[&converter, &resultGraph, &locations](const simplePoint<float>& node,
			                                       const simpleList<simpleLine<float>>& adjacentEdges)
			{
				// Iterate the graph's edges (per node) and convert back to the room graph
				for (int index = 0; index < adjacentEdges.count(); index++)
				{
					// Converts back to grid locators
					gridLocatorEdge edge = converter->convertUIRealToGrid(adjacentEdges.get(index), true);
					bool node1Found = false;
					bool node2Found = false;

					// Validate the edge points with the room centers
					for (int searchIndex = 0; searchIndex < locations.count() && (!node1Found || !node2Found);
					     searchIndex++)
					{
						if (edge.node1 == locations.get(searchIndex))
							node1Found = true;

						if (edge.node2 == locations.get(searchIndex))
							node2Found = true;
					}

					if (!node1Found || !node2Found)
						throw simpleException("Invalid triangulation:  layoutGenerator::triangulate");

					// Will take care of the node(s)
					if (!resultGraph->containsEdge(edge))
						resultGraph->addEdge(edge);
				}

				return iterationCallback::iterate;
			});

		// VALIDATE:
		if (resultGraph->getNodeCount() != locations.count())
			throw simpleException("Location node not found in delaunay graph:  layoutGenerator.h");

		// Verify that the graph contains each room 
		for (int index = 0; index < locations.count(); index++)
		{
			if (!resultGraph->containsNode(locations.get(index)))
			{
				throw simpleException("Room node not found in delaunay room graph:  layoutGenerator.h");
			}
			if (resultGraph->getAdjacentEdges(locations.get(index)).count() == 0)
			{
				throw simpleException("Room node has zero adjacent edges in delaunay room graph:  layoutGenerator.h");
			}
		}

		// (MEMORY!) Clean up
		delete delaunayGraph;

		// (MEMORY!) This is now the caller's responsibility
		return resultGraph;
	}

	void layoutGenerator::triangulateRooms()
	{
		// What to triangulate? Utilize the "biggest sub-rectangle" center. Create corridors using nearest neighbors
		// from the region edge locations + dijkstra's algorithm.
		//
		simpleList<gridLocator> roomCenters = _roomTiles->select<gridLocator>([](brogueDesignRect* designRect)
		{
			return designRect->getRegion()->getLargestSubRectangle().center();
		});

		// (MEMORY!) Calculate the room graph
		_roomGraph = triangulate(roomCenters);

		_layout->setRoomConnectionGraph(_roomGraph);
	}

	void layoutGenerator::triangulateRoomConnections()
	{
		// _roomGraph must be initialized

		// Procedure
		//
		// 1) Create nearest neighbor connections between delaunay rooms using edge locations
		//		-> Condition 1: The room connection line must not intersect any other room 
		//						rectangle except the two rooms in question. (This may be improved
		//						by using outline geometry. TODO)
		// 
		// 2) Use these to re-triangulate the connections
		// 3) Remove self-referencing edges
		// 4) Store the result as corridor connections
		// 5) Store the larger result as the "connection graph" for the level.
		//		-> TODO
		//

		layoutCoordinateConverter* coordinateConverter = _coordinateConverter;

		simpleList<brogueDesignRect*>* roomTiles = _roomTiles;
		simpleList<gridLocator> connectionNodes;

		// Nearest neighbor edges
		_roomGraph->iterateEdges([&roomTiles, &connectionNodes, &coordinateConverter](const gridLocatorEdge& edge)
		{
			// Room 1
			brogueDesignRect* designRect1 = roomTiles->first([&edge](brogueDesignRect* rect)
			{
				return rect->getRegion()->getLargestSubRectangle().center() == edge.node1;
			});

			// Room 2
			brogueDesignRect* designRect2 = roomTiles->first([&edge](brogueDesignRect* rect)
			{
				return rect->getRegion()->getLargestSubRectangle().center() == edge.node2;
			});

			if (designRect1 == designRect2)
			{
				return iterationCallback::iterate;
			}

			simpleArray<gridLocator> edges1 = designRect1->getRegion()->getEdgeLocations();
			simpleArray<gridLocator> edges2 = designRect2->getRegion()->getEdgeLocations();

			gridLocator minlocation1 = default_value::value<gridLocator>();
			gridLocator minlocation2 = default_value::value<gridLocator>();

			float distance = std::numeric_limits<float>::max();

			for (int index1 = 0; index1 < edges1.count(); index1++)
			{
				for (int index2 = 0; index2 < edges2.count(); index2++)
				{
					simplePoint<int> point1UI = coordinateConverter->convertToUI(edges1.get(index1), true);
					simplePoint<int> point2UI = coordinateConverter->convertToUI(edges2.get(index2), true);

					if (point1UI.distance(point2UI) < distance)
					{
						distance = point1UI.distance(point2UI);
						minlocation1 = edges1.get(index1);
						minlocation2 = edges2.get(index2);
					}
				}
			}

			if (!connectionNodes.contains(minlocation1))
				connectionNodes.add(minlocation1);

			if (!connectionNodes.contains(minlocation2))
				connectionNodes.add(minlocation2);

			return iterationCallback::iterate;
		});

		// (MEMORY!) Re-triangulate with these nearest edge locations
		simpleGraph<gridLocator, gridLocatorEdge>* connectionGraph = triangulate(connectionNodes);

		_connectionGraph = connectionGraph;

		//_layout->setRoomConnectionGraph(connectionGraph);

		simpleList<gridLocatorEdge> corridorEdges;

		// Retrieve the room-edge where the connection was drawn from
		//
		_roomGraph->iterateEdges(
			[&connectionGraph, &corridorEdges, &roomTiles, &coordinateConverter](const gridLocatorEdge& roomEdge)
			{
				// Room 1
				brogueDesignRect* designRect1 = roomTiles->first([&roomEdge](brogueDesignRect* rect)
				{
					return rect->getRegion()->isDefined(roomEdge.node1);
				});

				// Room 2
				brogueDesignRect* designRect2 = roomTiles->first([&roomEdge](brogueDesignRect* rect)
				{
					return rect->getRegion()->isDefined(roomEdge.node2);
				});

				if (designRect1 == designRect2)
				{
					return iterationCallback::iterate;
				}

				float distance = std::numeric_limits<float>::max();
				gridLocatorEdge minEdge = default_value::value<gridLocatorEdge>();

				// Remove self-referential edges; look for the best edge connection; and store the result as corridor edges
				connectionGraph->iterateEdges(
					[&minEdge, &designRect1, &designRect2, &distance, &coordinateConverter](const gridLocatorEdge& edge)
					{
						if (edge.node1 == default_value::value<gridLocator>() ||
							edge.node2 == default_value::value<gridLocator>())
						{
							throw simpleException(
								"No edge found for corridor placement:  layoutGenerator::triangulateRoomConnections");
						}

						// Edge forward
						if (designRect1->getRegion()->isDefined(edge.node1) &&
							designRect2->getRegion()->isDefined(edge.node2))
						{
							simplePoint<int> point1UI = coordinateConverter->convertToUI(edge.node1);
							simplePoint<int> point2UI = coordinateConverter->convertToUI(edge.node2);

							if (point1UI.distance(point2UI) < distance)
							{
								distance = point1UI.distance(point2UI);
								minEdge = edge;
							}
						}

						// Edge reverse
						if (designRect1->getRegion()->isDefined(edge.node2) &&
							designRect2->getRegion()->isDefined(edge.node1))
						{
							simplePoint<int> point1UI = coordinateConverter->convertToUI(edge.node2);
							simplePoint<int> point2UI = coordinateConverter->convertToUI(edge.node1);

							if (point1UI.distance(point2UI) < distance)
							{
								distance = point1UI.distance(point2UI);
								minEdge = edge;
							}
						}

						return iterationCallback::iterate;
					});

				if (minEdge.node1 == default_value::value<gridLocator>() ||
					minEdge.node2 == default_value::value<gridLocator>())
				{
					// TODO: Figure out what happened to triangulation
					//throw simpleException("No edge found for corridor placement:  layoutGenerator::triangulateRoomConnections");
				}
				else
					corridorEdges.add(minEdge);

				return iterationCallback::iterate;
			});

		// Final Verification:  Make sure that each edge does not intersect any other region except for 
		//						the ones that they're connecting.
		//
		for (int index = corridorEdges.count() - 1; index >= 0; index--)
		{
			gridLocatorEdge edge = corridorEdges.get(index);

			// Room 1
			brogueDesignRect* designRect1 = roomTiles->first([&edge](brogueDesignRect* rect)
			{
				return rect->getRegion()->isDefined(edge.node1);
			});

			// Room 2
			brogueDesignRect* designRect2 = roomTiles->first([&edge](brogueDesignRect* rect)
			{
				return rect->getRegion()->isDefined(edge.node2);
			});

			for (int roomIndex = 0; roomIndex < roomTiles->count(); roomIndex++)
			{
				if (roomTiles->get(roomIndex) == designRect1 ||
					roomTiles->get(roomIndex) == designRect2)
					continue;

				simpleRectangle<float> roomRectUIReal = coordinateConverter->convertToUIReal(
					roomTiles->get(roomIndex)->getActualBoundary(), true);
				simpleLine<float> lineUIReal = coordinateConverter->convertToUIReal(edge, true);

				// Found Intersection with another room
				//if (roomRectUIReal.intersects(lineUIReal))
				//{
				//	corridorEdges.removeAt(index);
				//	break;
				//}
			}
		}


		_layout->setCorridorConnections(corridorEdges);
	}

	void layoutGenerator::connectRooms()
	{
		// Procedure
		//
		// 1) Iterate Edges: (DELAUNAY GRAPH) (actually, use the corridor edges.. need to clean up)
		//      -> Find edges that are not self-referential (use room tiles) (corridor edges)
		//      -> Collect these edges to pass to dijkstra
		//
		// 2) Run Dijkstra to set cells in the primary grid
		//

		brogueLayout* layout = _layout;
		const simpleList<brogueDesignRect*>* roomTiles = _roomTiles;

		// (MEMORY!) Use the corridor edges to call dijkstra (on stack usgae has predicate copying)
		dijkstra<gridLocator>* algorithm = new dijkstra<gridLocator>(
			_layout->getParentBoundary(),
			_layout->getBoundary(),
			true, // Cardinal Movement (for laying corridors)

			// Primary Inclusion Predicate (is it in the grid?)
			[&layout](int column, int row)
			{
				return layout->isDefined(column, row);
			},

			// Then, the map cost is queried (what is the movement cost?)
			[&layout, &roomTiles](int column, int row)
			{
				//// TODO: Remove this and clean up the layout generator
				//if (roomTiles->any([&column, &row] (brogueDesignRect* designRect)
				//{
				//	return designRect->getRegion()->isDefined(column, row);
				//}))
				//{
				//	return 10;
				//}

				//else
				return 1;
			},

			// Then, it will need the locators from the grid to keep its internal data temporarily
			[&layout](int column, int row)
			{
				// May need design change for this problem (needed to copy grid locators back for dijkstra)
				return gridLocator(column, row);
			});

		// Iterate each corridor edge and run dijkstra to finalize the connection
		_layout->getCorridorConnections().forEach([&algorithm, &layout](const gridLocatorEdge& edge)
		{
			gridLocator source = edge.node1;
			gridLocator targets[1] = {edge.node2};

			// Run Dijkstra
			algorithm->initialize(source, simpleArray<gridLocator>(targets, 1));
			algorithm->run();

			simpleArray<gridLocator> resultPath = algorithm->getResultPath(targets[0]);

			// Set corridors
			resultPath.forEach([&layout](gridLocator locator)
			{
				color backColor(0.3f, 0.3f, 0.3f, 0.5f);

				brogueCell cell(locator.column, locator.row, backColor, colors::white(), '.');

				layout->createCells(locator, cell, true);

				return iterationCallback::iterate;
			});

			return iterationCallback::iterate;
		});

		// Clean up memory
		delete algorithm;
	}
}
