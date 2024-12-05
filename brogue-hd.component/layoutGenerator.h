#pragma once

#include <brogueLevelProfile.h>
#include <brogueRoomTemplate.h>
#include <color.h>
#include <delaunayAlgorithm.h>
#include <dungeonConstants.h>
#include <limits>
#include <primsAlgorithm.h>
#include <simple.h>
#include <simpleArray.h>
#include <simpleException.h>
#include <simpleGraph.h>
#include <simpleHash.h>
#include <simpleLine.h>
#include <simpleList.h>
#include <simplePoint.h>
#include <simpleRectangle.h>

#include "brogueCell.h"
#include "layoutDesignRect.h"
#include "brogueLayout.h"
#include "dijkstra.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRect.h"
#include "gridRegion.h"

#include "layoutConnectionBuilder.h"
#include "layoutPartialConnectionData.h"
#include "layoutCoordinateConverter.h"
#include "layoutDijkstraParameters.h"
#include "layoutGeneratorData.h"

#include "noiseGenerator.h"
#include "randomGenerator.h"
#include "rectanglePackingAlgorithm.h"
#include "regionOutlineGenerator.h"
#include "roomGenerator.h"

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

	private:	// Steps 1-7 in order, see below.

		/// <summary>
		/// Creates the room regions; and packs them using the rectangle packing algorithm. Also, creates
		/// region outlilne polygons and stores them with the data store.
		/// </summary>
		void createRooms(layoutGeneratorData* data);

		/// <summary>
		/// Creates the base delaunay region graph for the layout. This is the room graph.
		/// </summary>
		void triangulateRooms(layoutGeneratorData* data);

		/// <summary>
		/// Creates nearest neighbor locator connections for the delaunay region graph. These are stored
		/// with the connection builder (see attemptRoomConnections)
		/// </summary>
		void createNearestNeighbors(layoutGeneratorData* data);

		/// <summary>
		/// Checks nearest neighbors for polygon intersection with other rooms. Creates partial
		/// connection data inside of the connection builder for later use. (see attemptRoomConnections)
		/// </summary>
		void checkRoomConnections(layoutGeneratorData* data);

		/// <summary>
		/// Iterates on the room connections - checking for partials - until the trial grid is complete.
		/// (see the function implementation for notes). The idea being:  the full delaunay must be tried
		/// in a planning phase to be sure there are viable corridors. Each of these will be validated
		/// before transferring cells to the layout grid. So, the final graph is still modifiable
		/// between the MST / Delaunay; and the corridors are already planned. So, the final step will
		/// be just to create the final grid, graph, and connection layer that define the level layout.
		/// </summary>
		void attemptRoomConnections(layoutGeneratorData* data);

		/// <summary>
		/// Selects the final graph of rooms from the (modified) delaunay graph - with partial connections
		/// already accounted for. The MST graph must be drawn from the modified delaunay graph of connection
		/// points. (e.g. source -> (partial) interrupting location -> destination) This modified graph is
		/// still a "room graph". So, the first step is to create the (modified delaunay) graph of connection
		/// points. This graph is, tentatively, the "connection layer" graph. The MST will be a subset of this
		/// graph that still connects the level. Once this MST is derived, the finalization of the layout will
		/// choose corridors at random from between the MST (and) modified delaunay.
		/// </summary>
		/// <param name="data"></param>
		void defineConnectionLayer(layoutGeneratorData* data);

		/// <summary>
		/// Transfers the data to the final layout - also selecting corridors from the trial data using the
		/// MST and modified delaunay connection graphs. This will define the final "connection layer". The
		/// region cells, and corridor cells, are now transferred to the final layout, to create brogueCell*
		/// instances.
		/// </summary>
		void finalizeLayout(layoutGeneratorData* data);

	private:

		simpleGraph<gridLocator, gridLocatorEdge>* triangulate(const simpleList<gridLocator>& locations);

		template<isGridLocator T>
		simpleArray<T> runDijkstra(const T& source, const T& destination, layoutDijkstraParameters<T>* parameters);

		template<isGridLocator T>
		bool validateConnection(layoutGeneratorData* data,
								 layoutDesignRect* sourceRect,
								 layoutDesignRect* destRect,
								 const simpleArray<T>& pathData,
								 layoutDesignRect* interruptingRegion,
								 gridLocator& interruptingLocation);

	private:
		roomGenerator* _roomGenerator;
		randomGenerator* _randomGenerator;
		layoutCoordinateConverter* _coordinateConverter;
		regionOutlineGenerator* _regionOutlineGenerator;
	};


	layoutGenerator::layoutGenerator(randomGenerator* randomGenerator, const gridRect& layoutParentBoundary,int zoomLevel)
	{
		_randomGenerator = randomGenerator;
		_roomGenerator = new roomGenerator(randomGenerator);
		_coordinateConverter = new layoutCoordinateConverter(layoutParentBoundary, zoomLevel);
		_regionOutlineGenerator = new regionOutlineGenerator<gridLocator>(_coordinateConverter);
	}

	layoutGenerator::~layoutGenerator()
	{
		delete _roomGenerator;
		delete _coordinateConverter;
		delete _regionOutlineGenerator;
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

		// Create the level boundary (add 1 to the layout (parent) boundary)
		gridRect levelBoundary = _coordinateConverter->getLayoutBoundary();
		gridRect levelPaddedBoundary = levelBoundary.createPadded(1);

		// Generation Data Stores: These will be "garbage collected" by another component (TODO)
		//
		layoutGeneratorData* layoutData = new layoutGeneratorData(levelBoundary, levelPaddedBoundary);

		// Create Rooms (new roomTiles)
		createRooms(layoutData);

		// Triangulate Rooms:  Creates Delaunay Triangulation of the connection point vertices
		triangulateRooms(layoutData);

		// Create Nearest Neighbor (edge locations):  Locate nearest neighbor edge cells from the delaunay graph.
		createNearestNeighbors(layoutData);

		// Filter these nearest neighbors to avoid collisions with other rooms (modifies the graphs)
		filterRoomConnections(layoutData);

		// Create the graph of these points (delaunay, and MST, to provide further corridor filtering)
		triangulateRoomConnections(layoutData);

		// Connect Rooms:  Create cells in the grid for the delaunay triangulation of the 
		//                 connection points (remaining) of the room tiles. Runs dijkstra's algorithm to
		//				   create corridor cells inside the layout grid.
		connectRooms(layoutData);

		//gridRegionCollection()
		//gridLayer<brogueCell*>* cellLayer = new gridLayer<brogueCell*>()

		//gridConnectionLayer<brogueCell*>* connectionLayer = new gridConnectionLayer<brogueCell*>(_connectionGraph, )

		return new brogueLayout(_layoutGrid, _connectionLayer);
	}

	void layoutGenerator::createRooms(layoutGeneratorData* data)
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
		simpleHash<layoutDesignRect*, gridRect> tiling;

		gridLocator rectPackingFocus;
		int maxIterations = 35;
		int iteration = 0;

		while (iteration < maxIterations)
		{
			int randomRoomIndex = _randomGenerator->randomRangeExclusive(0, _profile->getRoomInfoCount());

			brogueRoomTemplate configuration = (iteration == 0)
				                                   ? _profile->getEntranceRoom()
				                                   : _profile->getRoomInfo(randomRoomIndex);
			layoutDesignRect* designRect = nullptr;
			int roomPadding = iteration == 0 ? 0 : 1;

			// Entrance Room Selection
			//
			if (iteration == 0)
			{
				designRect = new layoutDesignRect(configuration, _layout->getBoundary().createPadded(1), roomPadding);

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

				layoutDesignRect designRectPreLayout(configuration, largestSubRect, roomPadding);

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
				designRect = new layoutDesignRect(configuration, paddedBoundary, 0);

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
			// 3) Calculate the final boundary (setting it into the layoutDesignRect)
			// 4) Remove / Replace the rectangle in the algorithm with the final one
			//

			// (MEMORY!) These must be deleted; and the stack-like gridLocator instances will be copied into the brogueLayout* grid.
			gridRegion<gridLocator>* region = _roomGenerator->designRoom(designRect->getConfiguration().getRoomType(),
			                                                             designRect->getBoundary().createPadded(roomPadding),
			                                                             designRect->getMinSize(),
			                                                             _layout->getBoundary());

			// (MEMORY!) Creates a polygon outline (with interior polygons) for the region
			gridRegionOutline* regionOutline _regionOutlineGenerator->createOutline(region);

			// Set the actual region boundary
			designRect->complete(region, regionOutline);

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
		simpleList<layoutDesignRect*>* roomTiles = _roomTiles;

		tiling.forEach([&layout, &roomTiles](layoutDesignRect* designRect, const gridRect& boundary)
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

	void layoutGenerator::triangulateRooms(layoutGeneratorData* data)
	{
		// What to triangulate? Utilize the "biggest sub-rectangle" center. Create corridors using nearest neighbors
		// from the region edge locations + dijkstra's algorithm.
		//
		simpleList<gridLocator> roomCenters = roomTiles->select<gridLocator>([](layoutDesignRect* designRect)
		{
			return designRect->getRegion()->getLargestSubRectangle().center();
		});

		// (MEMORY!) Calculate the room graph
		data->roomGraph = triangulate(roomCenters);
	}

	void layoutGenerator::createNearestNeighbors(layoutGeneratorData* data)
	{
		layoutCoordinateConverter* coordinateConverter = _coordinateConverter;

		// Nearest neighbor edges
		data->roomGraph->iterateEdges([&data, &coordinateConverter] (const gridLocatorEdge& edge)
		{
			// Room 1
			layoutDesignRect* designRect1 = data->roomTiles->first([&edge] (layoutDesignRect* rect)
			{
				return rect->getRegion()->getLargestSubRectangle().center() == edge.node1;
			});

			// Room 2
			layoutDesignRect* designRect2 = data->roomTiles->first([&edge] (layoutDesignRect* rect)
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

			if (!data->roomNearestNeighbors->contains(minlocation1))
				data->roomNearestNeighbors->add(minlocation1);

			if (!data->roomNearestNeighbors->contains(minlocation2))
				data->roomNearestNeighbors->add(minlocation2);

			return iterationCallback::iterate;
		});
	}

	void layoutGenerator::attemptRoomConnections(layoutGeneratorData* data)
	{
		// Procedure:  Iterate all delaunay room connections
		//
		// 0) Create the trialGrid data structure with room regions
		//
		// 1) If a connection has no partials, then attempt dijkstra's algorithm
		//	  on each connection:
		//		-> Pass: Cement the connection data (including cells in the trialGrid)
		//		-> Fail: Keep track of failure in connection data (see step #4)
		//
		// 2) If a connection has partials
		//		-> If there are no more base connections:  Goto #3
		//		-> Else: Skip these until base connections are finished
		//
		// 3) Process Partial:
		//		-> Process either piece of the partial
		//			-> Success: Store the data!
		//			-> Fail:	Put on the failed connections (see step #4)
		//
		// 4) Failed Attempts: Start a loop of failed connections
		//		-> Re-attempt Success:  Store the data!
		//		-> Re-attempt Failure:
		//			-> Any Other Successes this iteration? Then, Re-attempt
		//			-> Otherwise, Goto #5
		//
		// 5) Forced Connection: These will be acheived using a re-triangulation
		//						 of the entire trialGrid to find the nearest raw
		//						 connection points hooking the two pieces together.
		//						 There may be preferences to just use the original
		//						 connection w/ a dijkstra iteration. Either way,
		//						 there will be a simple way to fix this issue, re-
		//						 triangulate the result; and store the new layout
		//						 data - either as partial, or as the original connection
		//						 points.
		//

		// Use the layoutConnectionBuilder* class to manage the connection process
		//
		while (data->getConnectionBuilder()->hasPendingWork())
		{
			// Normal Connections (first priority)(aids is partial connections afterwards)
			//
			if (data->getConnectionBuilder()->hasPendingNormal())
			{
				layoutConnectionData* nextConnection = data->getConnectionBuilder()->getNextNormal();

				// Run Dijkstra on the trial grid
				simpleArray<gridLocator> pathData = runDijkstra(nextConnection->getConnectionPoint1(),
																nextConnection->getConnectionPoint2(),
																data->getTrialDijkstraParameters());

				layoutDesignRect* interruptingRegion = nullptr;
				gridLocator interruptingLocation = default_value::value<gridLocator>();

				// Validate the path data
				if (!validateConnection(data,
					nextConnection->getRegion1Rect(),
					nextConnection->getRegion2Rect(),
					pathData, interruptingRegion, interruptingLocation))
				{
					// Change connection to partial; and continue
					data->getConnectionBuilder()->changeConnectionToPartial(nextConnection, interruptingRegion, interruptingLocation);

					// Delete this connection
					delete nextConnection;
				}

				// Success! (Normal)
				else
				{
					nextConnection->complete(pathData);
				}
			}

			// Partial Connections
			//
			else if (data->getConnectionBuilder()->hasPendingPartial())
			{
				layoutPartialConnectionData* nextConnection = data->getConnectionBuilder()->getNextPartial();

				// Run Dijkstra (Partial 1) on the trial grid (using closest interrupting region to source location)
				simpleArray<gridLocator> pathData = runDijkstra(nextConnection->getConnectionPoint1(),
																nextConnection->getInterruptingLocation(),
																data->getTrialDijkstraParameters());

				layoutDesignRect* nextInterruptingRegion = nullptr;
				gridLocator nextInterruptingLocation = default_value::value<gridLocator>();

				// Validate the path data
				if (!validateConnection(data,
					nextConnection->getRegion1Rect(),
					nextConnection->getInterruptingRegion(),
					pathData, nextInterruptingRegion, nextInterruptingLocation))
				{
					// CONNECTION FAILURE! (Need to investigate...) These may be forced; but it is better to
					// fix underlying layout issues.
					//
					nextConnection->fail();
				}

				// Success! Mark that this partial has been solved. We must know that the interrupting region
				//			has a path to the original destination region. So, we must consolidate partials
				//			at the beginning of the next iteration; and check to see that the normal route has
				//			been solved.
				else
				{
					nextConnection->completePartial(closestRegionRect, pathData);
				}
			}
			else
				throw simpleException("Improper use of the layoutConnectionBuilder:  layoutGenerator.h");
		}

		// Consolidate Connection(s):  Find any completed partial connections and try
		// to reconcile the graph. Complete these connections; and skip the if / else below
		// if there is nothing more to do.
		if (data->getConnectionBuilder()->hasCompletedPartial())
		{
			// This will run several iterations of Dijkstra's Algorithm on the room graph.
			data->getConnectionBuilder()->reconcilePartials(data->getRoomGraph());
		}

		// Final Validation
		if (data->getConnectionBuilder()->hasCompletedUnreconciledPartials())
		{
			// (TODO) This should be a forced connection
			throw simpleException("Unfinished partial connection:  layoutGenerator.h");
		}
	}

	void layoutGenerator::triangulateRoomConnections(layoutGeneratorData* data)
	{
		// Procedure
		//
		// 1) Create nearest neighbor connections between delaunay rooms using edge locations
		//		-> Condition 1: The room connection line must not intersect any other room 
		//						polygon except the two rooms in question. 
		// 
		// 2) Use these to re-triangulate the connections
		// 3) Remove self-referencing edges
		// 4) Store the result as corridor connections
		// 5) Store the larger result as the "connection graph" for the level.
		//		-> TODO
		//

		simpleList<gridLocatorEdge> roomEdges = roomGraph->getEdges();

		// Final Verification:  Make sure that each edge does not intersect any other region except for 
		//						the ones that they're connecting.
		//
		for (int index = roomEdges.count() - 1; index >= 0; index--)
		{
			gridLocatorEdge edge = roomEdges.get(index);

			// Room 1
			layoutDesignRect* designRect1 = roomTiles->first([&edge](layoutDesignRect* rect)
			{
				return rect->getRegion()->isDefined(edge.node1);
			});

			// Room 2
			layoutDesignRect* designRect2 = roomTiles->first([&edge](layoutDesignRect* rect)
			{
				return rect->getRegion()->isDefined(edge.node2);
			});

			bool intersectionFound = false;

			for (int roomIndex = 0; roomIndex < roomTiles->count() && !intersectionFound; roomIndex++)
			{
				if (roomTiles->get(roomIndex) == designRect1 ||
					roomTiles->get(roomIndex) == designRect2)
					continue;

				// (Memory!) Create deep copy of the polygon data (needed for stack operation convenience)
				simplePolygon<int>* regionPolygonUI = roomTiles->get(roomIndex)->getRegionOutline()->getExterior()->copy();

				// (Memory!) Create a real number version of the polygon for this stack check
				simplePolygon<float>* regionPolygonUIReal = coordinateConverter->convertToUIReal(regionPolygonUI);

				// Move to the center to avoid co-linear points with the outline
				simpleLine<float> lineUIReal = coordinateConverter->convertToUIReal(edge, true);

				// Found Intersection with another room
				if (regionPolygonUIReal->intersects(lineUIReal))
				{
					//roomEdges.removeAt(index);
					intersectionFound = true;
				}

				delete regionPolygonUI;
				delete regionPolygonUIReal;
			}
		}


	}

	void layoutGenerator::checkRoomConnections(layoutGeneratorData* data)
	{
		// Procedure
		//
		// 1) Take roomGraph (delaunay) and look for edges that don't intersect
		//	  other room-region polygons
		//		-> If Intersection:  Create partial connection to the graph
		//							 where it intersects the first room node.
		//		-> Else: (continue)
		//
		// 2) Store these partials for later use
		//

		// Nearest neighbor edges (also, fill out modified graph)
		for (int index = 0; index < data->roomNearestNeighbors->count(); index++)
		{
			gridLocatorEdge edge = data->roomNearestNeighbors->get(index);

			// Room 1
			layoutDesignRect* designRect1 = data->roomTiles->first([&edge] (layoutDesignRect* rect)
			{
				return rect->getRegion()->isDefined(edge.node1);
			});

			// Room 2
			layoutDesignRect* designRect2 = data->roomTiles->first([&edge] (layoutDesignRect* rect)
			{
				return rect->getRegion()->isDefined(edge.node2);
			});

			// Non-modified data
			gridLocator node1 = designRect1->getRegion()->getLargestSubRectangle().center();
			gridLocator node2 = designRect1->getRegion()->getLargestSubRectangle().center();
			gridLocatorEdge originalEdge(node1, node2);

			// Fill in the modified room graph
			if (!data->roomGraphModified->containsEdge(originalEdge))
				data->roomGraphModified->addEdge(originalEdge);

			// Filter self-referential edges
			if (designRect1 == designRect2)
			{
				continue;
			}

			// Calculate intersection in real numbers
			simpleLine<float> edgeUIReal = _coordinateConverter->convertToUIReal(edge, true);

			// Look for other polygon intersections
			for (int roomIndex = 0; roomIndex < data->roomTiles->count(); roomIndex++)
			{
				layoutDesignRect* nextRoom = data->roomTiles->get(roomIndex);

				// These are the source and destination rooms
				if (nextRoom == designRect1 ||
					nextRoom == designRect2)
					continue;

				// (MEMORY!) Need to use some heap allocation for these polygons...
				simplePolygon<float>* regionUIReal = _coordinateConverter->convertToUIReal(nextRoom->getRegionOutline()->getExterior());

				// Could either be single-point or collinear
				if (regionUIReal->calculateIntersection(edgeUIReal) != simpleLineIntersectionResult::None)
				{
					// Create a partial connection for this intersection; and continue. These will be used
					// later on.
					layoutPartialConnectionData* partial = data->connectionPartials->firstOrDefault(
					[&node1, &node2] (gridDesignPartialConnection* partial)
					{
						// Checks both directions
						return partial->hasOriginalConnection(node1, node2);
					});

					// Create / Update Partial
					if (partial != nullptr)
						partial->addInterruptingRegion(nextRoom);

					else
					{
						// (MEMORY!) Add this to our layout data heap
						partial = new layoutPartialConnectionData(designRect1, designRect2, node1, node2);
						partial->addInterruptingRegion(nextRoom);
						data->connectionPartials->add(partial);
					}
				}

				// Be sure to clean up local memory...
				delete regionUIReal;

				return result;
			}
		}
	}

	void layoutGenerator::connectRooms(layoutGeneratorData* data)
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
		const simpleList<layoutDesignRect*>* roomTiles = _roomTiles;

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
				//if (roomTiles->any([&column, &row] (layoutDesignRect* designRect)
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
			[&converter] (const gridLocator& center)
		{
			// Scales by the cell size; and flips the y-axis
			//
			return converter->convertToUIReal(center, true);
		});

		// (MEMORY!) Create delaunay graph
		simpleGraph<simplePoint<float>, simpleLine<float>>* delaunayGraph = triangulator.run(connectionNodes,
			[] (const simplePoint<float>& node1, const simplePoint<float>& node2)
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

	template<isGridLocator T>
	simpleArray<T> layoutGenerator::runDijkstra(const T& source, const T& destination, layoutDijkstraParameters<T>* parameters)
	{
		// Procedure
		//
		// 1) Make use of the cost callback; and encapsulate the rest of dijkstra's map here.
		//

		// (MEMORY!) Be sure to delete the algorithm heap afterwards
		dijkstra<T>* algorithm = new dijkstra<T>(
			parameters->getParentBoundary(),
			parameters->getRelativeBoundary(),
			parameters->getObeyCardinalMovement(),
			parameters->getMapPredicate(),
			parameters->getMapCostPredicate(),
			parameters->getLocatorCallback());

		simpleArray<T> targets(1);
		targets.set(0, destination);

		algorithm->initialize(source, targets);
		algorithm->run();
		simpleArray<T> pathData = algorithm->getResultPath(targets.get(0));

		// Clean up memory
		delete algorithm;

		// Copy these up the stack
		return pathData;
	}

	template<isGridLocator T>
	bool layoutGenerator::validateConnection(layoutGeneratorData* data,
											 layoutDesignRect* sourceRect,
											 layoutDesignRect* destRect,
											 const simpleArray<T>& pathData, 
											 layoutDesignRect* interruptingRegion,
											 gridLocator& interruptingLocation)
	{
		// Verify that the path does not intersect any other region except for the source / destination
		for (int index = 0; index < pathData.count(); index++)
		{
			// Source Region
			if (sourceRect->getRegion()->isDefined(pathData.get(index)))
				continue;

			// Destination Region
			if (destRect->getRegion()->isDefined(pathData.get(index)))
				continue;

			for (int regionIndex = 0; regionIndex < data->getRoomTiles()->count(); regionIndex++)
			{
				layoutDesignRect* regionRect = data->getRoomTiles()->get(regionIndex);

				if (regionRect == sourceRect ||
					regionRect == destRect)
					continue;

				if (regionRect->getRegion()->isDefined(pathData.get(index)))
				{
					interruptingLocation = pathData.get(index);
					interruptingRegion = regionRect;
					return false;
				}
			}
		}

		return true;
	}
}
