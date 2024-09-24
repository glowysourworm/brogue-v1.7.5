#pragma once

#include "delaunayAlgorithm.h"
#include "triangle.h"
#include "vectorExtension.h"
#include <vector>

using namespace std;

using namespace brogueHd::backend::extension;

namespace brogueHd::backend::math
{
    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    delaunayAlgorithm<TNode, TEdge>::delaunayAlgorithm(graphDelegates<TNode, TEdge>::edgeConstructor edgeConstructor)
    {
    }

    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    delaunayAlgorithm<TNode, TEdge>::~delaunayAlgorithm()
    {
        
    }

    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    graph<TNode, TEdge>* delaunayAlgorithm<TNode, TEdge>::run(const std::vector<TNode>& vertices)
    {
        if (vertices.count() < 3)
            return this->createDefaultGraph(vertices);

        return this->bowyerWatson(vertices);
    }

    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
    graph<TNode, TEdge>* delaunayAlgorithm<TNode, TEdge>::bowyerWatson(const std::vector<TNode>& vertices)
    {
        if (vertices.count() < 3)
        {
            return createDefaultGraph(vertices);
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
        short top = std::numeric_limits<short>::max();
        short bottom = std::numeric_limits<short>::min();
        short left = std::numeric_limits<short>::max();
        short right = std::numeric_limits<short>::min();

        vectorExtension<TNode>::forEach(vertices, [](TNode vertex)
        {
            if (vertex.locator.row < top)
                top = vertex.locator.row;

            if (vertex.locator.row > bottom)
                bottom = vertex.locator.row;

            if (vertex.locator.column < left)
                left = vertex.locator.column;

            if (vertex.locator.column > right)
                right = vertex.locator.column;
        });

        // NOTE*** NULL VERTEX REFERENCE USED TO IDENTIFY SUPER TRIANGLE
        point<short> point1(0, 0);
        point<short> point2((short)((right * 2) + 1), 0);
        point<short> point3(0, (short)((bottom * 2) + 1));

        // Initialize the mesh (the "super-triangle" is removed as part of the algorithm)
        //
        triangle<short> superTriangle(point1, point2, point3);

        std::vector<triangle<short>> triangles;
        std::vector<triangle<short>> badTriangles;
        std::vector<triangle<short>> otherBadTriangles;

        triangles.push_back(superTriangle);

        // Add points: one-at-a-time
        //
        vectorExtension<TNode>::forEach(vertices, [&triangles, &badTriangles, &otherBadTriangles](TNode graphVertex)
        {
            point<T> vertexPoint(graphVertex.locator.column, graphVertex.locator.row);
            
            // Find triangles in the mesh whose circum-circle contains the new point
            //
            // Remove those triangles from the mesh and return them
            //
            badTriangles = vectorExtension<triangle<short>>::remove(triangles, [&vertexPoint](triangle<short> triangle)
            {
                return triangle.circumCircleContains(vertexPoint);
            });

            // Use edges from the polygon hole to create new triangles. This should be an "outline" of
            // the bad triangles. So, use all edges from the bad triangles except for shared edges.
            //
            vectorExtension<triangle<short>>::forEach(badTriangles, [&otherBadTriangles, &vertexPoint](triangle<short> badTriangle)
            {
                otherBadTriangles = vectorExtension<triangle<short>>::except(badTriangles, [](triangle<short> triangle)
                {
                    return triangle == badTriangle;
                });

                bool edge12 = vectorExtension<triangle<short>>::any(otherBadTriangles, [&badTriangle](triangle<short> triangle)
                {
                    return triangle.containsEqualEdge(triangle.point1, triangle.point2);
                });

                bool edge23 = vectorExtension<triangle<short>>::any(otherBadTriangles, [&badTriangle](triangle<short> triangle)
                {
                    return triangle.containsEqualEdge(triangle.point2, triangle.point3);
                });

                bool edge31 = vectorExtension<triangle<short>>::any(otherBadTriangles, [&badTriangle](triangle<short> triangle)
                {
                    return triangle.containsEqualEdge(triangle.point3, triangle.point1);
                });

                // Check Shared Edges 1 -> 2
                if (!edge12)
                    triangles.push_back(triangle<short>(badTriangle.point1, badTriangle.point2, vertexPoint));

                // 2 -> 3
                if (!edge23)
                    triangles.push_back(triangle<short>(badTriangle.point2, badTriangle.point3, vertexPoint));

                // 3 -> 1
                if (!edge31)
                    triangles.push_back(triangle<short>(badTriangle.point3, badTriangle.point1, vertexPoint));
            });
        });

        // Create the delaunay graph using distinct edges
        std::vector<TEdge> delaunayEdges;

        vectorExtension<triangle<short>>::forEach(triangles, [&delaunayEdges, &point1, &point2, &point3](triangle<short> triangle)
        {
            // (Cleaning Up) Remove any edges shared with the "super-triangle" vertices
            //

            // 1 -> 2
            if (triangle.point1 != point1 &&
                triangle.point2 != point1 &&
                triangle.point1 != point2 &&
                triangle.point2 != point2 &&
                triangle.point1 != point3 &&
                triangle.point2 != point3)
            {
                // Check for equivalent edge
                if (!vectorExtension<TEdge>::any(delaunayEdges, [&triangle](TEdge edge) 
                {
                    return edge.isEquivalent(triangle.point1, triangle.point2);
                }))
                {
                    delaunayEdges.push_back(this->graphEdgeConstructor(triangle.point1, triangle.point2));
                }
            }

            // 2 -> 3
            if (triangle.Point2 != point1 &&
                triangle.Point3 != point1 &&
                triangle.Point2 != point2 &&
                triangle.Point3 != point2 &&
                triangle.Point2 != point3 &&
                triangle.Point3 != point3)
            {
                // Check for equivalent edge
                if (!vectorExtension<TEdge>::any(delaunayEdges, [&triangle](TEdge edge)
                {
                    return edge.isEquivalent(triangle.point2, triangle.point3);
                }))
                {
                    delaunayEdges.push_back(this->graphEdgeConstructor(triangle.point2, triangle.point3));
                }
            }

            // 3 -> 1
            if (triangle.Point3 != point1 &&
                triangle.Point1 != point1 &&
                triangle.Point3 != point2 &&
                triangle.Point1 != point2 &&
                triangle.Point3 != point3 &&
                triangle.Point1 != point3)
            {
                // Check for equivalent edge
                if (!vectorExtension<TEdge>::any(delaunayEdges, [&triangle](TEdge edge)
                {
                    return edge.isEquivalent(triangle.point3, triangle.point1);
                }))
                {
                    delaunayEdges.push_back(this->graphEdgeConstructor(triangle.point3, triangle.point1));
                }
            }
        }

        // Return a new graph with Delaunay edges
        return new graph<TNode, TEdge>(vertices, delaunayEdges);
    }

}