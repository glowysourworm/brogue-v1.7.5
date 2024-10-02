#pragma once

#include "triangle.h"
#include "graph.h"
#include "graphAlgorithm.h"
#include "gridLocatorEdge.h"
#include "simpleList.h"
#include "brogueGlobal.h"

namespace brogueHd::component
{
    template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
	class delaunayAlgorithm : public graphAlgorithm<TNode, TEdge>
	{
    public:

        delaunayAlgorithm(){};
        delaunayAlgorithm(graphEdgeConstructor<TNode, TEdge> graphEdgeConstructor);
        ~delaunayAlgorithm();

        graph<TNode, TEdge>* run(const simpleList<TNode>& vertices) override;

    protected:

        /// <summary>
        /// Creates Delaunay triangulation using the Bowyer-Watson algorithm O(n log n). 
        /// </summary>
        graph<TNode, TEdge>* bowyerWatson(const simpleList<TNode>& vertices);
	};

    template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
    delaunayAlgorithm<TNode, TEdge>::delaunayAlgorithm(graphEdgeConstructor<TNode, TEdge> edgeConstructor)
    {
    }

    template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
    delaunayAlgorithm<TNode, TEdge>::~delaunayAlgorithm()
    {

    }

    template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
    graph<TNode, TEdge>* delaunayAlgorithm<TNode, TEdge>::run(const simpleList<TNode>& vertices)
    {
        if (vertices.count() < 3)
            return this->createDefaultGraph(vertices);

        return this->bowyerWatson(vertices);
    }

    template<isGridLocatorNode TNode, isGridLocatorEdge<TNode> TEdge>
    graph<TNode, TEdge>* delaunayAlgorithm<TNode, TEdge>::bowyerWatson(const simpleList<TNode>& vertices)
    {
        if (vertices.count() < 3)
        {
            return this->createDefaultGraph(vertices);
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

        // Create a lookup to convert from gridLocator to mathPoint
        simpleHash<TNode, mathPoint<short>> vertexLookup;
        simpleHash<mathPoint<short>, TNode> nodeLookup;

        vertices.forEach([&top, &bottom, &left, &right, &vertexLookup, &nodeLookup](TNode vertex)
        {
            if (vertex.row < top)
                top = vertex.row;

            if (vertex.row > bottom)
                bottom = vertex.row;

            if (vertex.column < left)
                left = vertex.column;

            if (vertex.column > right)
                right = vertex.column;

            // Store these to use later on when creating edges
            mathPoint<short> point = mathPoint<short>(vertex.column, vertex.row);

            vertexLookup.add(vertex, point);
            nodeLookup.add(point, vertex);

            return iterationCallback::iterate;
        });

        // NOTE*** NULL VERTEX REFERENCE USED TO IDENTIFY SUPER TRIANGLE
        mathPoint<short> point1(0, 0);
        mathPoint<short> point2((short)((right * 2) + 1), 0);
        mathPoint<short> point3(0, (short)((bottom * 2) + 1));

        // Initialize the mesh (the "super-triangle" is removed as part of the algorithm)
        //
        triangle<short> superTriangle(point1, point2, point3);

        simpleList<triangle<short>> triangles;
        simpleList<triangle<short>> badTriangles;
        simpleList<triangle<short>> otherBadTriangles;

        triangles.add(superTriangle);

        // Add points: one-at-a-time
        //
        vertices.forEach([&triangles, &badTriangles, &otherBadTriangles, &vertexLookup](TNode graphVertex)
        {
            mathPoint<short> vertexPoint = vertexLookup[graphVertex];

            // Find triangles in the mesh whose circum-circle contains the new point
            //
            // Remove those triangles from the mesh and return them
            //
            badTriangles = triangles.remove([&vertexPoint](triangle<short> triangle)
            {
                return triangle.circumCircleContains(vertexPoint);
            });

            // Use edges from the polygon hole to create new triangles. This should be an "outline" of
            // the bad triangles. So, use all edges from the bad triangles except for shared edges.
            //
            badTriangles.forEach([&otherBadTriangles, &vertexPoint, &badTriangles, &triangles, &vertexLookup](triangle<short> badTriangle)
            {
                otherBadTriangles = badTriangles.except([&badTriangle](triangle<short> triangle)
                {
                    return triangle == badTriangle;
                });

                bool edge12 = otherBadTriangles.any([&badTriangle](triangle<short> triangle)
                {
                    return triangle.containsEqualEdge(triangle.point1, triangle.point2);
                });

                bool edge23 = otherBadTriangles.any([&badTriangle](triangle<short> triangle)
                {
                    return triangle.containsEqualEdge(triangle.point2, triangle.point3);
                });

                bool edge31 = otherBadTriangles.any([&badTriangle](triangle<short> triangle)
                {
                    return triangle.containsEqualEdge(triangle.point3, triangle.point1);
                });

                // Check Shared Edges 1 -> 2
                if (!edge12)
                    triangles.add(triangle<short>(badTriangle.point1, badTriangle.point2, vertexPoint));

                // 2 -> 3
                if (!edge23)
                    triangles.add(triangle<short>(badTriangle.point2, badTriangle.point3, vertexPoint));

                // 3 -> 1
                if (!edge31)
                    triangles.add(triangle<short>(badTriangle.point3, badTriangle.point1, vertexPoint));

                return iterationCallback::iterate;
            });

            return iterationCallback::iterate;
        });

        // Create the delaunay graph using distinct edges
        simpleList<TEdge> delaunayEdges;
        delaunayAlgorithm<TNode, TEdge>* that = this;

        triangles.forEach([&delaunayEdges, &point1, &point2, &point3, &that, &nodeLookup](triangle<short> triangle)
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
                if (!delaunayEdges.any([&triangle](TEdge edge)
                    {
                        return edge.isEquivalent(triangle.point1, triangle.point2);
                    }))
                {
                    TNode node1 = nodeLookup[triangle.point1];
                    TNode node2 = nodeLookup[triangle.point2];

                    delaunayEdges.add(that->edgeConstructor(node1, node2));
                }
            }

            // 2 -> 3
            if (triangle.point2 != point1 &&
                triangle.point3 != point1 &&
                triangle.point2 != point2 &&
                triangle.point3 != point2 &&
                triangle.point2 != point3 &&
                triangle.point3 != point3)
            {
                // Check for equivalent edge
                if (!delaunayEdges.any([&triangle](TEdge edge)
                    {
                        return edge.isEquivalent(triangle.point2, triangle.point3);
                    }))
                {
                    TNode node2 = nodeLookup[triangle.point2];
                    TNode node3 = nodeLookup[triangle.point3];

                    delaunayEdges.add(that->edgeConstructor(node2, node3));
                }
            }

            // 3 -> 1
            if (triangle.point3 != point1 &&
                triangle.point1 != point1 &&
                triangle.point3 != point2 &&
                triangle.point1 != point2 &&
                triangle.point3 != point3 &&
                triangle.point1 != point3)
            {
                // Check for equivalent edge
                if (!delaunayEdges.any([&triangle](TEdge edge)
                    {
                        return edge.isEquivalent(triangle.point3, triangle.point1);
                    }))
                {
                    TNode node3 = nodeLookup[triangle.point3];
                    TNode node1 = nodeLookup[triangle.point1];

                    delaunayEdges.add(that->edgeConstructor(node3, node1));
                }
            }

            return iterationCallback::iterate;
        });

        // Return a new graph with Delaunay edges
        return new graph<TNode, TEdge>(vertices.toArray(), delaunayEdges.toArray());
    }
}
