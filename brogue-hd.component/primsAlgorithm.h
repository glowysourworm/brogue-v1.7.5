#pragma once

#include "graphAlgorithm.h"
#include "simpleList.h"

namespace brogueHd::component
{
    /// <summary>
    /// Defines the Minimum Spanning Tree (MST) algorithm Prim's Algorithm
    /// </summary>
    template<isGridLocatorNode TNode, isGridLocatorEdge TEdge>
	class primsAlgorithm : public graphAlgorithm<TNode, TEdge>
	{
    public:

        primsAlgorithm(graphEdgeConstructor<TNode, TEdge> graphEdgeConstructor);
        ~primsAlgorithm();

        graph<TNode, TEdge>* run(const simpleList<TNode>& vertices) override;

    protected:

        /// <summary>
        /// Creates MST using Prim's Algorithm - which takes O(n log n)
        /// </summary>
        graph<TNode, TEdge>* createMST(const simpleList<TNode>& vertices);
        
	};
}