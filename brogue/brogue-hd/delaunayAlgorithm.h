#pragma once

#include "graph.h"
#include "graphAlgorithm.h"
#include "gridLocatorEdge.h"
#include "gridLocatorNode.h"
#include "simpleList.h"
#include "exceptionHandler.h"

using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::math
{
    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
	class delaunayAlgorithm : public graphAlgorithm<TNode, TEdge>
	{
    public:

        delaunayAlgorithm(graphDelegates<TNode, TEdge>::edgeConstructor graphEdgeConstructor);
        ~delaunayAlgorithm();

        graph<TNode, TEdge>* run(const simpleList<TNode>& vertices) override;

    protected:

        /// <summary>
        /// Creates Delaunay triangulation using the Bowyer-Watson algorithm O(n log n). 
        /// </summary>
        graph<TNode, TEdge>* bowyerWatson(const simpleList<TNode>& vertices);
	};
}
