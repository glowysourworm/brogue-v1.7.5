#pragma once

#include "graphAlgorithm.h"

namespace brogueHd::backend::math
{
    /// <summary>
    /// Defines the Minimum Spanning Tree (MST) algorithm Prim's Algorithm
    /// </summary>
    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
	class primsAlgorithm : public graphAlgorithm<TNode, TEdge>
	{
    public:

        primsAlgorithm(graphDelegates<TNode, TEdge>::edgeConstructor graphEdgeConstructor);
        ~primsAlgorithm();

        graph<TNode, TEdge>* run(const std::vector<TNode>& vertices) override;

    protected:

        /// <summary>
        /// Creates MST using Prim's Algorithm - which takes O(n log n)
        /// </summary>
        graph<TNode, TEdge>* createMST(const std::vector<TNode>& vertices);
        
	};
}