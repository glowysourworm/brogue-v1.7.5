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
#include <simpleGraphDefinitions.h>
#include <windows.h>

#include "brogueCell.h"
#include "layoutDesignRect.h"
#include "brogueLayout.h"
#include "dijkstra.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRectAdjacency.h"
#include "gridRect.h"
#include "gridRegion.h"
#include "gridRegionGraphNode.h"
#include "gridRegionGraphEdge.h"
#include "gridRegionCollection.h"

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
		/// Creates the (approx) delaunay room graph based on "centroids" (center of largest sub-rect). This
		/// triangulation will be modified during the connection process when there are collisions. 
		/// </summary>
		void createRoomGraph(layoutGeneratorData* data);

		/// <summary>
		/// Creates nearest neighbor locator connections for the room graph. These are stored
		/// with the connection builder (see attemptRoomConnections)
		/// </summary>
		void createNearestNeighbors(layoutGeneratorData* data);

		/// <summary>
		/// Creates a dealunay graph from the nearest neighbor locations. This cleans up the nearest
		/// neighbors in case there are any intersecting nearest neighbors. Validation is then run to
		/// make sure there are no rooms left out (this would be an almost impossible corner case)
		/// </summary>
		void createNearestNeighborsGraph(layoutGeneratorData* data);

		/// <summary>
		/// Creates the data for the layout connection builder connections. This is drawn from
		/// the nearest neighbors collection. The resulting connection data will be used to
		/// create the trial layout.
		/// </summary>
		void initializeConnectionBuilder(layoutGeneratorData* data);

		/// <summary>
		/// Iterates on the nearest neighbors - which are now the connection builder connections.
		/// These are attempted, using dijsktra's algorithm, to create corridor trial cells. If
		/// there are any interruptions, partial connections are made to modify the original room
		/// graph. These are then reconciled to be sure that the (modified) room graph will still
		/// function.
		/// </summary>
		void attemptRoomConnections(layoutGeneratorData* data);

		/// <summary>
		/// Using the connection builder data from the trial room connection attempt, we: 1) define
		/// the (modified) room graph, 2) create the delaunay triangulated connection-point graph, 3)
		/// create the MST connection-point graph. Define and store these components in the layoutGeneratorData*.
		/// </summary>
		void defineConnectionLayer(layoutGeneratorData* data);

		/// <summary>
		/// Transfers the data to the final layout - also selecting corridors from the trial data using the
		/// MST and delaunay connection graphs. This will define the final "connection layer". The region cells,
		/// and corridor cells, are now transferred to the final layout, to create brogueCell* instances.
		/// </summary>
		brogueLayout* finalizeLayout(layoutGeneratorData* data);

	private:

		template<isHashable T, isGridLocatorNode<T> TNode, isGridLocatorEdge<TNode, T> TEdge>
		simpleGraph<TNode, TEdge>* triangulate(simpleGraphAlgorithmType algorithmType, 
											   const simpleList<TNode>& nodes, 
											   std::function<TEdge(const TNode& node1, const TNode& node2)> edgeConstructor);

		template<isGridLocator T>
		simpleArray<T> runDijkstra(const T& source, const T& destination, layoutDijkstraParameters<T>* parameters);

		template<isGridLocator T>
		bool validateConnection(layoutGeneratorData* data,
								 const gridRegionGraphNode& sourceNode,
								 const gridRegionGraphNode& destNode,
								 const simpleArray<T>& pathData,
								 gridRegionGraphNode& interruptingNode,
								 gridLocator& interruptingLocation);

		static gridRegionGraphEdge gridRegionGraphEdgeConstructor(const gridRegionGraphNode& node1, const gridRegionGraphNode& node2)
		{
			return gridRegionGraphEdge(node1, node2);
		}
		static gridConnectionEdge gridConnectionEdgeConstructor(const gridConnectionNode& node1, const gridConnectionNode& node2)
		{
			return gridConnectionEdge(node1, node2);
		}

	private:
		roomGenerator* _roomGenerator;
		randomGenerator* _randomGenerator;
		layoutCoordinateConverter* _coordinateConverter;
		regionOutlineGenerator* _regionOutlineGenerator;
	};


	layoutGenerator::layoutGenerator(randomGenerator* randomGenerator, const gridRect& layoutParentBoundary, int zoomLevel)
	{
		_randomGenerator = randomGenerator;
		_roomGenerator = new roomGenerator(randomGenerator);
		_coordinateConverter = new layoutCoordinateConverter(layoutParentBoundary, zoomLevel);
		_regionOutlineGenerator = new regionOutlineGenerator(_coordinateConverter);
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
		layoutGeneratorData* layoutData = new layoutGeneratorData(profile, levelBoundary, levelPaddedBoundary);

		// Create Rooms (new roomTiles)
		createRooms(layoutData);

		// Triangulate Rooms:  Creates Delaunay Triangulation of the connection point vertices
		createRoomGraph(layoutData);

		// Create Nearest Neighbor (edge locations):  Locate nearest neighbor edge cells from the delaunay graph.
		createNearestNeighbors(layoutData);

		// Creates a delaunay graph from the nearest neighbor locations (cleans up intersections)
		createNearestNeighborsGraph(layoutData);

		// Create data for the connections from nearest neighbors
		initializeConnectionBuilder(layoutData);

		// Uses trial grid (see layoutGeneratorData) to try the nearest neighbors. Finishes "partial" connections
		// after all "normal" connections are routed. If there are any lingering issues the layout processing will
		// have to re-locate it's bulk regions; and finish them as "larger rooms".
		attemptRoomConnections(layoutData);

		// Use the completed connections to assemble the "connection layer" data. Choose corridor data from between
		// the MST of the connection graph; and the full (approx) delaunay of the connection graph.
		defineConnectionLayer(layoutData);

		// (MEMORY!) Complete the trial grid inside the layoutGeneratorData* with the generated path data.
		brogueLayout* layout = finalizeLayout(layoutData);

		// (MEMORY!) Clean up layoutGeneratorData* (see destructors..)
		delete layoutData;

		return layout;
	}

	void layoutGenerator::createRooms(layoutGeneratorData* data)
	{
		// Brogue v1.7.5 Creates up to 35 rooms using accretion (bolt-on) to fill up the space
		//
		//
		// Update:  (see backlog.txt) This will now be done using a tiling method.
		//
		// Procedure:
		//
		// 1) Fetch a room template randomly from the configuration
		// 2) Utilize the rectangle packing algorithm to situate the
		//    next rectangle as close as possible to the first using
		//    using the focal point (gridLocator). Leave room-padding
		//	  for the actual grid cell layout afterwards.
		// 3) Set the tile into the packing.
		// 4) Layout the room cells using the roomGenerator*. There will
		//    be space left over for some layouts. 
		// 5) Repack the result into the packing; and use the translate
		//    method to put the region into place.
		// 6) Call the adjacency of room tiles from the packing algorithm.
		// 7) Use this adjacency to create the room graph and set this
		//    into the layoutGeneratorData*.
		//
		// MEMORY:  The gridRegion*, and gridRegionOutline* are the
		//		    primary bi-products of this phase of the layout. These
		//			should be maintained in the brogueLayout*, which is
		//			the primary output of this class' functions.
		//
		//			the layoutGeneratorData* will step around these two
		//			data structures - deleting only that which is not required
		//			by the brogueLayout*. (see class destructors)
		//
		rectanglePackingAlgorithm algorithm(_randomGenerator, data->getParentBoundary(), data->getBoundary());

		// Keep track of the actual tiles in the layout. The algorithm must have a copy with which to
		// work; and will not see the fidgeting with our copy of the gridRect on the stack.
		//
		simpleHash<layoutDesignRect*, gridRect> tiling;

		gridLocator rectPackingFocus;
		int maxIterations = 35;
		int iteration = 0;

		while (iteration < maxIterations)
		{
			int randomRoomIndex = _randomGenerator->randomRangeExclusive(0, data->getProfile()->getRoomInfoCount());

			brogueRoomTemplate configuration = (iteration == 0)
												? data->getProfile()->getEntranceRoom()
												: data->getProfile()->getRoomInfo(randomRoomIndex);
			layoutDesignRect* designRect = nullptr;
			int roomPadding = iteration == 0 ? 0 : 1;

			// Entrance Room Selection
			//
			if (iteration == 0)
			{
				gridRect roomBoundary = configuration.getMaxSize();

				gridLocator topLeft = gridLocator(
					data->getBoundary().centerX() - (roomBoundary.width / 2),
					data->getBoundary().bottom() - roomBoundary.height);

				roomBoundary.translate(topLeft.column, topLeft.row);

				// (MEMORY!) These will go to the layoutGeneratorData*
				designRect = new layoutDesignRect(_coordinateConverter, configuration, roomBoundary, roomPadding);

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

				layoutDesignRect designRectPreLayout(_coordinateConverter, configuration, largestSubRect, roomPadding);

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

				// (MEMORY!) Store the result as the constrained boundary (removing the padding)
				designRect = new layoutDesignRect(_coordinateConverter, configuration, paddedBoundary, 0);

				// Update the tiling
				tiling.add(designRect, paddedBoundary);
			}

			// Complete:
			//
			// 1) Create grid cells for the tile's room
			// 2) Translate the result inside of its design grid rect (towards the entrance room)
			// 3) Calculate the final boundary (setting it into the layoutDesignRect)
			// 4) Remove / Replace the rectangle in the algorithm with the final one
			//

			// (MEMORY!) These must be deleted; and the stack-like gridLocator instances will be copied into the brogueLayout* grid.
			gridRegion* region = _roomGenerator->designRoom(designRect->getConfiguration().getRoomType(),
															 designRect->getBoundary().createPadded(roomPadding),
															 designRect->getMinSize(),
															 data->getBoundary());

			// (MEMORY!) Creates a polygon outline (with interior polygons) for the region
			//gridRegionOutline* regionOutline = _regionOutlineGenerator->createOutline(region);
			gridRegionOutline* regionOutline = nullptr;

			// Set the actual region boundary (can still translate)
			designRect->complete(region, regionOutline);

			// Repack the rectangle if it is not the entrance
			if (iteration != 0)
			{
				// Padding:  Added during rectangle packing 1 (above), removed during layout (above), now
				//			 the "actual" boundary lives inside of that rectangle. So, re-pack the first rect
				//			 by 1) Removing it from the tiling; and 2) Adding the "actual" boundary, but
				//			 expanded by the roomPadding amount. Translate the final region into position
				//			 using gridRegion::translate; and update our hash of designRect instances.
				//

				//gridRect paddedBoundaryPacked = tiling.get(designRect);
				//gridRect actualBoundaryExpanded = designRect->getActualBoundary().createExpanded(roomPadding);

				//if (!paddedBoundaryPacked.contains(actualBoundaryExpanded))
				//	throw simpleException("Mishandled room layout boundary:  layoutGenerator.h");

				//// Pack this into position:  modifies the gridRect
				//algorithm.removeRectangle(paddedBoundaryPacked);
				//algorithm.addRectangle(actualBoundaryExpanded);

				//// Update our hash table
				//tiling.set(designRect, actualBoundaryExpanded);

				//gridLocator translation = designRect->getActualBoundary().createExpanded(roomPadding)
				//														 .getTranslation(actualBoundaryExpanded);

				//if (translation.column != 0 ||
				//	translation.row != 0)
				//{
				//	// Will also translate the region outline
				//	designRect->translate(translation);
				//}
			}

			iteration++;
		}

		// Transfer the tiling straight onto the room graph
		for (int index = 0; index < tiling.count(); index++)
		{
			gridRect rect = tiling.getAt(index)->getValue();
			layoutDesignRect* designRect = tiling.getAt(index)->getKey();

			// Add Largest Sub-Rectangle -> Center (as Node)
			//
			data->getRoomNodes()->add(gridRegionGraphNode(designRect, designRect->getRegion()->getLargestSubRectangle().center()));
		}
	}

	void layoutGenerator::createRoomGraph(layoutGeneratorData* data)
	{
		// (MEMORY!) The gridRegion* instances need to be put on the layoutGeneratorData* data structure.
		simpleGraph<gridRegionGraphNode, gridRegionGraphEdge>* graph = triangulate<layoutDesignRect*, gridRegionGraphNode, gridRegionGraphEdge>(simpleGraphAlgorithmType::DelaunayTriangulation, *data->getRoomNodes(), &layoutGenerator::gridRegionGraphEdgeConstructor);

		graph->iterateEdges([&data] (const gridRegionGraphEdge& edge)
		{
			brogueCompass direction;

			bool isAdjacentTile = edge.node1.getData()->getConstraintBoundary().isAdjacent(edge.node2.getData()->getConstraintBoundary(), direction);

			// Don't allow non-adjacent grid rects to produce a graph edge. Afterwards, check the intergrity
			// of the graph.
			if (isAdjacentTile)
				data->getRoomGraph()->addEdge(edge);

			return iterationCallback::iterate;
		});

		if (data->getRoomGraph()->getNodeCount() > 1)
		{
			// Validate the room graph (use Dijkstra's Algorithm to exhaust the graph)
			dijkstrasAlgorithm<gridRegionGraphNode, gridRegionGraphEdge> algorithm(data->getRoomGraph());

			// Any two nodes will work for the isConnection function
			algorithm.initialize(data->getRoomGraph()->getNodes().get(0), data->getRoomGraph()->getNodes().get(1));

			//if (!algorithm.isGraphConnected())
			//	throw simpleException("Invalid room graph:  layoutGenerator::createRoomGraph");
		}

		delete graph;
	}

	void layoutGenerator::createNearestNeighbors(layoutGeneratorData* data)
	{
		layoutCoordinateConverter* coordinateConverter = _coordinateConverter;

		// Room Graph (centroid center graph) -> Nearest neighbor edge points graph
		data->getRoomGraph()->iterateEdges([&data, &coordinateConverter] (const gridRegionGraphEdge& edge)
		{
			simpleArray<gridLocator> edges1 = edge.node1.getData()->getRegion()->getEdgeLocations();
			simpleArray<gridLocator> edges2 = edge.node2.getData()->getRegion()->getEdgeLocations();

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
						bool valid = true;
						simpleLine<int> lineUI(point1UI, point2UI);
						simpleLine<float> lineUIReal = coordinateConverter->convertToUIReal(lineUI);

						// Make sure the line doesn't enter another of the rooms
						data->getRoomGraph()->iterateEdges([&coordinateConverter, &edge, &valid, &lineUIReal] (const gridRegionGraphEdge& edgeCheck)
						{
							if (edge == edgeCheck)
								return iterationCallback::iterate;

							// Check that we're not iterating one of the rooms
							if (edge.node1 != edgeCheck.node1 &&
								edge.node2 != edgeCheck.node1)
							{
								simpleRectangle<float> rectUIReal = coordinateConverter->convertToUIReal(edgeCheck.node1.getData()->getBoundary());

								valid &= !rectUIReal.intersects(lineUIReal);
							}

							if (edge.node1 != edgeCheck.node2 &&
								edge.node2 != edgeCheck.node2)
							{
								simpleRectangle<float> rectUIReal = coordinateConverter->convertToUIReal(edgeCheck.node2.getData()->getBoundary());

								valid &= !rectUIReal.intersects(lineUIReal);
							}

							if (!valid)
								return iterationCallback::breakAndReturn;

							return iterationCallback::iterate;
						});

						if (valid)
						{
							distance = point1UI.distance(point2UI);
							minlocation1 = edges1.get(index1);
							minlocation2 = edges2.get(index2);
						}
					}
				}
			}

			// No valid nearest neighbor found (intersection with another room rectangle)
			if (minlocation1 == default_value::value<gridLocator>() ||
				minlocation2 == default_value::value<gridLocator>())
				return iterationCallback::iterate;

			gridRegionGraphNode node1(edge.node1.getData(), minlocation1);
			gridRegionGraphNode node2(edge.node2.getData(), minlocation2);
			gridRegionGraphEdge graphEdge(node1, node2);

			//if (!data->getRoomNearestNeighbors()->any([&graphEdge](const gridRegionGraphEdge& otherEdge)
			//{
			//	return graphEdge == otherEdge;	// Compares both orientations
			//}))
			//{
				data->getRoomNearestNeighbors()->add(graphEdge);
			//}

			return iterationCallback::iterate;
		});
	}

	void layoutGenerator::createNearestNeighborsGraph(layoutGeneratorData* data)
	{
		
	}

	void layoutGenerator::initializeConnectionBuilder(layoutGeneratorData* data)
	{
		// Procedure:
		//
		// 1) Iterate room nearest-neighbor edges
		// 2) Create normal connection for each (non-partial)
		//

		for (int index = 0; index < data->getRoomNearestNeighbors()->count(); index++)
		{
			gridRegionGraphEdge edge = data->getRoomNearestNeighbors()->get(index);

			// (MEMORY!) (see layoutGeneratorData* destructor)
			layoutConnectionData* connection = new layoutConnectionData(edge.node1, edge.node2);

			data->getConnectionBuilder()->addConnection(connection);
		}
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
				simpleArray<gridLocator> pathData = runDijkstra(nextConnection->getNode1().getLocator(),
																nextConnection->getNode2().getLocator(),
																data->getTrialDijkstraParameters());

				if (pathData.count() == 0)
					throw simpleException("Dijkstra created empty path:  layoutGenerator::attemptRoomConnections");

				gridRegionGraphNode interruptingRegion = default_value::value<gridRegionGraphNode>();
				gridLocator interruptingLocation = default_value::value<gridLocator>();

				// Validate the path data
				if (!validateConnection(data,
					nextConnection->getNode1(),
					nextConnection->getNode2(),
					pathData, interruptingRegion, 
					interruptingLocation))
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
				simpleArray<gridLocator> pathData = runDijkstra(nextConnection->getNode1().getLocator(),
																nextConnection->getInterruptingLocation(),
																data->getTrialDijkstraParameters());

				if (pathData.count() == 0)
					throw simpleException("Dijkstra created empty path:  layoutGenerator::attemptRoomConnections");

				gridRegionGraphNode nextInterruptingRegion = default_value::value<gridRegionGraphNode>();
				gridLocator nextInterruptingLocation = default_value::value<gridLocator>();

				// Validate the path data
				if (!validateConnection(data,
					nextConnection->getNode1(),
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
					nextConnection->completePartial(pathData);
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

	void layoutGenerator::defineConnectionLayer(layoutGeneratorData* data)
	{
		// Procedure
		//
		// 1) define the (modified) room graph
		// 2) create the delaunay triangulated connection-point graph
		// 3) create the MST connection-point graph.
		// 4) store these components in the layoutGeneratorData*.
		//

		simpleGraph<gridRegionGraphNode, gridRegionGraphEdge> modifiedRoomGraph;
		simpleHash<gridConnectionEdge, simpleArray<gridLocator>> connectionEdges;
		simpleList<gridConnectionNode> connectionNodes;

		// Normal Connections
		data->getConnectionBuilder()->iterateNormals([&data, &connectionNodes, &connectionEdges, &modifiedRoomGraph] (layoutConnectionData* normalConnection)
		{
			if (!normalConnection->isComplete())
				throw simpleException("Mishandled normal connection:  (not-completed) layoutGenerator::defineConnectionLayer");

			if (normalConnection->isFailed())
				throw simpleException("Normal connection failed. Must re-analyze the layout graph:  layoutGenerator::defineConnectionLayer");

			if (normalConnection->getNode1().getLocator() == normalConnection->getNode2().getLocator())
				throw simpleException("Layout connection has the same source and destination:  layoutGenerator::defineConnectionLayer");

			// Nodes may be used in more than one connection
			gridConnectionNode node1(normalConnection->getNode1().getData()->getRegion(), normalConnection->getNode1().getLocator());
			gridConnectionNode node2(normalConnection->getNode2().getData()->getRegion(), normalConnection->getNode2().getLocator());
			gridConnectionEdge nodeEdge(node1, node2);

			if (!connectionNodes.contains(node1))
				connectionNodes.add(node1);

			if (!connectionNodes.contains(node2))
				connectionNodes.add(node2);

			if (!connectionEdges.contains(nodeEdge))
				connectionEdges.add(nodeEdge, normalConnection->getPathData());

			// Modified Room Edge
			gridRegionGraphEdge edge(normalConnection->getNode1(), normalConnection->getNode2());

			if (!modifiedRoomGraph.containsEdge(edge))
				modifiedRoomGraph.addEdge(edge);

			return iterationCallback::iterate;
		});

		// Partial Connections (reconciled)
		data->getConnectionBuilder()->iteratePartials([&data, &connectionNodes, &connectionEdges, &modifiedRoomGraph] (layoutPartialConnectionData* partialConnection)
		{
			if (!partialConnection->isComplete())
				throw simpleException("Mishandled partial connection:  (not-completed) layoutGenerator::defineConnectionLayer");

			if (partialConnection->isFailed())
				throw simpleException("Partial connection failed. Must re-analyze the layout graph:  layoutGenerator::defineConnectionLayer");

			if (!partialConnection->getReconciled())
				throw simpleException("Mishandled partial connection:  (non-reconciled) layoutGenerator::defineConnectionLayer");

			if (partialConnection->getNode1().getLocator() == default_value::value<gridLocator>())
				throw simpleException("Invalid partial connection point:  layoutGenerator::defineConnectionLayer");

			if (partialConnection->getInterruptingLocation() == default_value::value<gridLocator>())
				throw simpleException("Invalid partial connection point:  layoutGenerator::defineConnectionLayer");

			if (partialConnection->getNode1().getLocator() == partialConnection->getInterruptingLocation())
				throw simpleException("Layout connection has the same source and destination:  layoutGenerator::defineConnectionLayer");

			// Nodes may be used in more than one connection
			gridConnectionNode node1(partialConnection->getNode1().getData()->getRegion(), partialConnection->getNode1().getLocator());
			gridConnectionNode node2(partialConnection->getInterruptingRegion().getData()->getRegion(), partialConnection->getInterruptingLocation());
			gridConnectionEdge nodeEdge(node1, node2, partialConnection->getPathData());

			if (!connectionNodes.contains(node1))
				connectionNodes.add(node1);

			if (!connectionNodes.contains(node2))
				connectionNodes.add(node2);

			if (!connectionEdges.contains(nodeEdge))
				connectionEdges.add(nodeEdge, partialConnection->getPathData());

			// Modified Room Edge
			gridRegionGraphEdge edge(partialConnection->getNode1(), partialConnection->getInterruptingRegion());

			if (!modifiedRoomGraph.containsEdge(edge))
				modifiedRoomGraph.addEdge(edge);

			return iterationCallback::iterate;
		});

		// (MEMORY!) Create connection point graph between MST (minimum spanning tree); and Delaunay.
		simpleGraph<gridConnectionNode, gridConnectionEdge>* connectionMST = triangulate<gridRegion*, gridConnectionNode, gridConnectionEdge>(simpleGraphAlgorithmType::PrimsAlgorithm, connectionNodes, &layoutGenerator::gridConnectionEdgeConstructor);
		simpleGraph<gridConnectionNode, gridConnectionEdge>* connectionDelaunay = triangulate<gridRegion*, gridConnectionNode, gridConnectionEdge>(simpleGraphAlgorithmType::DelaunayTriangulation, connectionNodes, &layoutGenerator::gridConnectionEdgeConstructor);

		randomGenerator* randGenerator = _randomGenerator;

		// Add all MST edges. These are required.
		connectionMST->iterateEdges([&data, &connectionEdges] (const gridConnectionEdge& edge)
		{
			// Find the edge data we already cached (the nodes should still match)
			simpleArray<gridLocator> pathData = connectionEdges.firstValue([&edge] (const gridConnectionEdge& otherEdge, 
																					const simpleArray<gridLocator>& pathData)
			{
				return (edge.node1 == otherEdge.node1 && edge.node2 == otherEdge.node2) ||
					   (edge.node1 == otherEdge.node2 && edge.node2 == otherEdge.node1);
			});

			gridConnectionEdge finalEdge(edge.node1, edge.node2, pathData);

			data->getConnectionGraph()->addEdge(finalEdge);

			return iterationCallback::iterate;
		});

		// Add Delaunay edges based on probability
		connectionDelaunay->iterateEdges([&data, &randGenerator, &connectionEdges] (const gridConnectionEdge& edge)
		{
			// Find the edge data we already cached (the nodes should still match)
			simpleArray<gridLocator> pathData = connectionEdges.firstValue([&edge] (const gridConnectionEdge& otherEdge,
																					const simpleArray<gridLocator>& pathData)
			{
				return (edge.node1 == otherEdge.node1 && edge.node2 == otherEdge.node2) ||
					   (edge.node1 == otherEdge.node2 && edge.node2 == otherEdge.node1);
			});

			gridConnectionEdge finalEdge(edge.node1, edge.node2, pathData);

			if (!data->getConnectionGraph()->containsEdge(finalEdge) &&
				randGenerator->next() < data->getProfile()->getExtraCorridorProbability())
			{
				data->getConnectionGraph()->addEdge(finalEdge);
			}

			return iterationCallback::iterate;
		});

		// (MEMORY!) Clean up allocated memory
		delete connectionMST;
		delete connectionDelaunay;
	}

	brogueLayout* layoutGenerator::finalizeLayout(layoutGeneratorData* data)
	{
		// Finalize the (new) data. Be sure we've garbage collected everything else!
		grid<brogueCell*>* layoutGrid = new grid<brogueCell*>(data->getParentBoundary(), data->getBoundary());

		simpleList<gridRegion*> layoutRegions;

		// NOTE:  We're instantiating ALL grid cells. So, nullptr has no meaning for the brogueLayout*
		data->getTrialGrid()->iterate([&layoutGrid] (int column, int row, const gridLocator& item)
		{
			// (Memory!) (see brogueLayout*)
			layoutGrid->set(column, row, new brogueCell(column, row));
			return iterationCallback::iterate;
		});

		randomGenerator* randGenerator = _randomGenerator;

		// NOTE: Sharing pointers for regions with the primary layout!
		data->getRoomNodes()->forEach([&layoutGrid, &layoutRegions, &randGenerator] (const gridRegionGraphNode& regionNode)
		{
			gridRegion* region = regionNode.getData()->getRegion();
			gridRect tileBoundary = regionNode.getData()->getConstraintBoundary();

			color tileColor = randGenerator->nextColor(colors::black(), colors::white());

			tileBoundary.iterate([&layoutGrid, &tileColor] (int column, int row)
			{
				layoutGrid->get(column, row)->setUI(brogueCellDisplay(column, row, tileColor));

				return iterationCallback::iterate;
			});

			region->iterateLocations([&layoutGrid, &region] (int column, int row, const gridLocator& location)
			{
				color backColor = location == region->getLargestSubRectangle().center() ? color(1, 0, 0.5f, 1.0f) : color(0, 0, 0.5f, 1.0f);
				color foreColor = region->isEdge(column, row) ? color(1, 1, 1, 1) : colors::transparent();
				char symbol = region->isEdge(column, row) ? '#' : '.';

				layoutGrid->get(column, row)->setUI(brogueCellDisplay(column, row, backColor, foreColor, symbol));

				return iterationCallback::iterate;
			});

			return iterationCallback::iterate;
		});

		// Transfer the path data to the grid
		data->getConnectionGraph()->iterateEdges([&layoutGrid] (const gridConnectionEdge& edge)
		{
			simpleArray<gridLocator> pathData = edge.getPathData();

			for (int index = 0; index < pathData.count(); index++)
			{
				gridLocator location = pathData.get(index);
				color backColor(0, 0.3f, 0.5f, 1.0f);
				layoutGrid->get(location.column, location.row)->setUI(brogueCellDisplay(location.column, location.row, backColor));
			}

			return iterationCallback::iterate;
		});

		// (MEMORY!) gridRegionCollection* -> gridLayer* -> gridConnectionLayer* -> brogueLayout*
		gridRegionCollection* regionCollection = new gridRegionCollection(data->getParentBoundary(), layoutRegions);

		gridLayer* cellLayer = new gridLayer(regionCollection);

		gridConnectionLayer* connectionLayer = new gridConnectionLayer(data->getConnectionGraph(), cellLayer);

		// DEBUG DATA
		simpleList<gridConnectionEdge> roomGraph = data->getRoomGraph()->getEdges().select<gridConnectionEdge>([] (const gridRegionGraphEdge& edge)
		{
			gridConnectionNode node1(edge.node1.getData()->getRegion(), edge.node1.getLocator());
			gridConnectionNode node2(edge.node2.getData()->getRegion(), edge.node2.getLocator());

			return gridConnectionEdge(node1, node2);
		});
		simpleList<gridConnectionEdge> nearestNeighbors = data->getRoomNearestNeighbors()->select<gridConnectionEdge>([] (const gridRegionGraphEdge& edge)
		{
			gridConnectionNode node1(edge.node1.getData()->getRegion(), edge.node1.getLocator());
			gridConnectionNode node2(edge.node2.getData()->getRegion(), edge.node2.getLocator());

			return gridConnectionEdge(node1, node2);
		});

		return new brogueLayout(layoutGrid, connectionLayer, nearestNeighbors, roomGraph);
	}

	template<isHashable T, isGridLocatorNode<T> TNode, isGridLocatorEdge<TNode, T> TEdge>
	simpleGraph<TNode, TEdge>* layoutGenerator::triangulate(simpleGraphAlgorithmType algorithmType, 
															const simpleList<TNode>& nodes, 
															std::function<TEdge(const TNode& node1, const TNode& node2)> edgeConstructor)
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

		// (Memory!) Create triangulator with graph edge constructor
		simpleGraphAlgorithm<simplePoint<float>, simpleLine<float>>* triangulator = nullptr;

		if (algorithmType == simpleGraphAlgorithmType::DelaunayTriangulation)
			triangulator = new delaunayAlgorithm();

		else if (algorithmType == simpleGraphAlgorithmType::PrimsAlgorithm)
			triangulator = new primsAlgorithm();

		else
			throw simpleException("Unhandled graph algorithm type:  layoutGenerator::triangulate");

		// Convert grid locators to real number coordinates: This is to run bowyer-watson
		//
		layoutCoordinateConverter* converter = _coordinateConverter;

		// Convert the locators
		//
		simpleList<simplePoint<float>> connectionNodes = nodes.select<simplePoint<float>>([&converter] (const gridLocator& center)
		{
			// Scales by the cell size; and flips the y-axis
			//
			return converter->convertToUIReal(center, true);
		});

		// (MEMORY!) Create the graph
		simpleGraph<simplePoint<float>, simpleLine<float>>* outputGraph = triangulator->run(connectionNodes,
		[] (const simplePoint<float>& node1, const simplePoint<float>& node2)
		{
			// This constructor is for default graphs (3 or less nodes)
			return simpleLine<float>(node1, node2);
		});

		// (MEMORY!) Re-scale back to the grid locators
		simpleGraph<TNode, TEdge>* resultGraph = new simpleGraph<TNode, TEdge>();

		// Convert Back / Validate
		//
		outputGraph->iterate([&converter, &resultGraph, &nodes, &edgeConstructor]
		(const simplePoint<float>& node,const simpleList<simpleLine<float>>& adjacentEdges)
		{
			// Iterate the graph's edges (per node) and convert back to the room graph
			for (int index = 0; index < adjacentEdges.count(); index++)
			{
				simpleLine<float> edge = adjacentEdges.get(index);

				// Converts back to grid locators
				gridLocator location1 = converter->convertUIRealToGrid(edge.node1, true);
				gridLocator location2 = converter->convertUIRealToGrid(edge.node2, true);

				bool node1Found = false;
				bool node2Found = false;

				TNode graphNode1;
				TNode graphNode2;

				// Match the locations with the locator nodes
				for (int searchIndex = 0; searchIndex < nodes.count() && (!node1Found || !node2Found); searchIndex++)
				{
					if (location1 == nodes.get(searchIndex).getLocator())
					{
						graphNode1 = nodes.get(searchIndex);
						node1Found = true;
					}


					if (location2 == nodes.get(searchIndex).getLocator())
					{
						graphNode2 = nodes.get(searchIndex);
						node2Found = true;
					}
				}

				if (!node1Found || !node2Found)
					throw simpleException("Invalid triangulation:  layoutGenerator::triangulate");

				if (location1 == location2)
					throw simpleException("Invalid self-referential node:  layoutGenerator::triangulate");

				// Call the user's edge constructor
				TEdge graphEdge = edgeConstructor(graphNode1, graphNode2);

				// Add the new edge to the result graph
				if (!resultGraph->containsEdge(graphEdge))
					resultGraph->addEdge(graphEdge);
			}

			return iterationCallback::iterate;
		});

		// VALIDATE:
		if (resultGraph->getNodeCount() != nodes.count())
			throw simpleException("Location node not found in delaunay graph:  layoutGenerator.h");

		// Verify that the graph contains each room 
		for (int index = 0; index < nodes.count(); index++)
		{
			if (!resultGraph->containsNode(nodes.get(index)))
			{
				throw simpleException("Room node not found in delaunay room graph:  layoutGenerator.h");
			}
			if (resultGraph->getAdjacentEdges(nodes.get(index)).count() == 0)
			{
				throw simpleException("Room node has zero adjacent edges in delaunay room graph:  layoutGenerator.h");
			}
		}

		// (MEMORY!) Clean up
		delete outputGraph;
		delete triangulator;

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
											 const gridRegionGraphNode& sourceNode,
											 const gridRegionGraphNode& destNode,
											 const simpleArray<T>& pathData,
											 gridRegionGraphNode& interruptingNode,
											 gridLocator& interruptingLocation)
	{
		// Verify that the path does not intersect any other region except for the source / destination
		for (int index = 0; index < pathData.count(); index++)
		{
			// Source Region
			if (sourceNode.getData()->getRegion()->isDefined(pathData.get(index)))
				continue;

			// Destination Region
			if (destNode.getData()->getRegion()->isDefined(pathData.get(index)))
				continue;

			for (int regionIndex = 0; regionIndex < data->getRoomGraph()->getNodes().count(); regionIndex++)
			{
				gridRegionGraphNode regionNode = data->getRoomGraph()->getNodes().get(regionIndex);

				if (regionNode == sourceNode ||
					regionNode == destNode)
					continue;

				if (regionNode.getData()->getRegion()->isDefined(pathData.get(index)))
				{
					interruptingLocation = pathData.get(index);
					interruptingNode = regionNode;
					return false;
				}
			}
		}

		return true;
	}
}
