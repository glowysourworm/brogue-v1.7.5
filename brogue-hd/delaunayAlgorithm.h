#pragma once

#include "simpleList.h"
#include "simplePoint.h"
#include "simpleTriangle.h"

#include "graph.h"
#include "graphAlgorithm.h"
#include "graphDefinitions.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleLine.h"
#include <limits>

using namespace brogueHd::simple;

namespace brogueHd::component
{
	class delaunayAlgorithm : public graphAlgorithm<simplePoint<float>, simpleLine<float>>
	{
	public:

		delaunayAlgorithm() {};
		~delaunayAlgorithm() {};

		graph<simplePoint<float>, simpleLine<float>>* run(const simpleList<simplePoint<float>>& vertices, graphEdgeConstructor<simplePoint<float>, simpleLine<float>> edgeConstructor) override;

	protected:

		/// <summary>
		/// Creates Delaunay triangulation using the Bowyer-Watson algorithm O(n log n). 
		/// </summary>
		graph<simplePoint<float>, simpleLine<float>>* bowyerWatson(const simpleList<simplePoint<float>>& vertices, graphEdgeConstructor<simplePoint<float>, simpleLine<float>> edgeConstructor);
	};

	graph<simplePoint<float>, simpleLine<float>>* delaunayAlgorithm::run(const simpleList<simplePoint<float>>& vertices, graphEdgeConstructor<simplePoint<float>, simpleLine<float>> edgeConstructor)
	{
		if (vertices.count() < 3)
			return this->createDefaultGraph(vertices, edgeConstructor);

		return this->bowyerWatson(vertices, edgeConstructor);
	}

	graph<simplePoint<float>, simpleLine<float>>* delaunayAlgorithm::bowyerWatson(const simpleList<simplePoint<float>>& vertices, graphEdgeConstructor<simplePoint<float>, simpleLine<float>> edgeConstructor)
	{
		if (vertices.count() < 3)
		{
			return this->createDefaultGraph(vertices, edgeConstructor);
		}

		// NOTE*** The graph of regions is over the VERTICES of edge connections between two regions (NOT THE 
		//         REGIONS THEMSELVES). 
		//
		//         The result of this algorithm will be a Delaunay Triangulation - giving nearest neighbor graph
		//         relationships - between two VERTICES - which are from two separate REGIONS.
		//          

		// Procedure - https://en.wikipedia.org/wiki/Bowyer%E2%80%93Watson_algorithm
		//
		// 0) Create "super-triangle" that encompasses all the points (the mesh)
		// 1) Add points one-at-a-time to the mesh
		// 2) Find the affected triangles in the mesh
		//      - Point lies inside the circumcircle of the triangle
		//
		// 3) For each edge in each affected triangle
		//      - If edge is not shared by any other affected triangle, Then add to "polygon hole"
		//
		// 4) Remove bad triangles from mesh
		// 5) For each edge in "polygon hole"
		//      - Form new triangle with edge and the added point
		//      - Add this triangle to the mesh
		//
		// 6) For each triangle in the mesh
		//      - If triangle contains a vertex from the original "super-triangle", 
		//        Then remove the triangle from the mesh
		//
		// 7) (Cleaning Up) Remove self-referential edges and keep only the lowest-weight edge between
		//                  two regions (NOT VERTICES)
		//

		// 0) Create "super-triangle" by using the bounding rectangle for the points inscribed inside of a triangle
		//

		// Create an enclosing rectangle for the points
		float top = std::numeric_limits<float>::min();
		float bottom = std::numeric_limits<float>::max();
		float left = std::numeric_limits<float>::max();
		float right = std::numeric_limits<float>::min();

		// Real Coordinates
		vertices.forEach([&top, &bottom, &left, &right] (const simplePoint<float>& vertex)
		{
			if (vertex.y > top)
				top = vertex.y;

			if (vertex.y < bottom)
				bottom = vertex.y;

			if (vertex.x < left)
				left = vertex.x;

			if (vertex.x > right)
				right = vertex.x;

			return iterationCallback::iterate;
		});

		float width = right - left;
		float height = top - bottom;

		// Create super triangle to encompass all points (put rectangle in super triangle and solve)
		simplePoint<float> triangleBottomLeft(left - (width / 2.0f), bottom);
		simplePoint<float> triangleBottomRight(right + (width / 2.0f), bottom);
		simplePoint<float> triangleTop(left + (width / 2.0f), 2 * height);

		simplePoint<float> point1 = triangleBottomLeft;
		simplePoint<float> point2 = triangleTop;
		simplePoint<float> point3 = triangleBottomRight;

		// Initialize the mesh (the "super-triangle" is removed as part of the algorithm)
		//
		simpleTriangle<float> superTriangle(point1, point2, point3);

		simpleList<simpleTriangle<float>> triangles;

		triangles.add(superTriangle);

		// Add points: one-at-a-time
		//
		for (int index = 0; index < vertices.count(); index++)
		{
			simplePoint<float> vertexPoint = vertices.get(index);
			simpleList<simpleLine<float>> polygon;

			// Validate that super triangle was large enough to encompass vertices
			if (!superTriangle.isPointInTriangle(vertexPoint))
				throw simpleException("Ill-formed bowyer-watson super triangle, or did not use real numbers:  delaunayAlgorithm.h");

			// Find triangles in the mesh whose circum-circle contains the new point
			//
			// Remove those triangles from the mesh and return them
			//
			simpleList<simpleTriangle<float>> badTriangles = triangles.remove([&vertexPoint] (const simpleTriangle<float>& triangle)
			{
				return triangle.circumCircleContains(vertexPoint);
			});

			// Use edges from the polygon hole to create new triangles. This should be an "outline" of
			// the bad triangles. So, use all edges from the bad triangles except for shared edges.
			//
			for (int badIndex = 0; badIndex < badTriangles.count(); badIndex++)
			{
				simpleTriangle<float> badTriangle = badTriangles.get(badIndex);

				// All other bad triangles except the current
				simpleList<simpleTriangle<float>> otherBadTriangles = badTriangles.except([&badTriangle] (const simpleTriangle<float>& triangle)
				{
					//return triangle.isEquivalentTo(badTriangle);
					return triangle == badTriangle;
				});

				bool edge12 = otherBadTriangles.any([&badTriangle] (const simpleTriangle<float>& triangle)
				{
					return triangle.containsEqualEdge(badTriangle.point1, badTriangle.point2);
				});

				bool edge23 = otherBadTriangles.any([&badTriangle] (const simpleTriangle<float>& triangle)
				{
					return triangle.containsEqualEdge(badTriangle.point2, badTriangle.point3);
				});

				bool edge31 = otherBadTriangles.any([&badTriangle] (const simpleTriangle<float>& triangle)
				{
					return triangle.containsEqualEdge(badTriangle.point3, badTriangle.point1);
				});

				// Check Shared Edges 1 -> 2
				if (!edge12)
					polygon.add(simpleLine<float>(badTriangle.point1, badTriangle.point2));

				// 2 -> 3
				if (!edge23)
					polygon.add(simpleLine<float>(badTriangle.point2, badTriangle.point3));

				// 3 -> 1
				if (!edge31)
					polygon.add(simpleLine<float>(badTriangle.point3, badTriangle.point1));
			}

			// Create new triangles from the polygon hole
			for (int polygonIndex = 0; polygonIndex < polygon.count(); polygonIndex++)
			{
				triangles.add(simpleTriangle<float>(polygon.get(polygonIndex).node1, polygon.get(polygonIndex).node2, vertexPoint));
			}
		}

		// Create the delaunay graph using distinct edges
		simpleList<simpleLine<float>> delaunayEdges;
		delaunayAlgorithm* that = this;

		// Remove any triangle that contains a vertex from the original triangle
		triangles.remove([&point1, &point2, &point3] (const simpleTriangle<float>& triangle)
		{
			return triangle.point1 == point1 ||
					triangle.point1 == point2 ||
					triangle.point1 == point3 ||

					triangle.point2 == point1 ||
					triangle.point2 == point2 ||
					triangle.point2 == point3 ||

					triangle.point3 == point1 ||
					triangle.point3 == point2 ||
					triangle.point3 == point3;
		});

		// Add the result to the delaunay edges
		triangles.forEach([&delaunayEdges, &that] (const simpleTriangle<float>& triangle)
		{
			simpleLine<float> edge1(triangle.point1, triangle.point2);
			simpleLine<float> edge2(triangle.point2, triangle.point3);
			simpleLine<float> edge3(triangle.point3, triangle.point1);

			if (!delaunayEdges.contains(edge1))
				delaunayEdges.add(edge1);

			if (!delaunayEdges.contains(edge2))
				delaunayEdges.add(edge2);

			if (!delaunayEdges.contains(edge3))
				delaunayEdges.add(edge3);

			return iterationCallback::iterate;
		});

		// Return a new graph with Delaunay edges
		return new graph<simplePoint<float>, simpleLine<float>>(vertices.toArray(), delaunayEdges.toArray());
	}
}
