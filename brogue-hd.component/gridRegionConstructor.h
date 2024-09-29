#pragma once

#include "grid.h"
#include "gridRect.h"
#include "gridDefinitions.h"
#include "gridRegion.h"
#include "simpleHash.h"

namespace brogueHd::component
{
    template<isGridLocator T>
    class gridRegionConstructor
    {
    public:

        gridRegionConstructor(gridRect parentBoundary, gridPredicate<T> inclusionPredicate);
        ~gridRegionConstructor();

        /// <summary>
        /// Completes the region collections, finalizes boundary, and validates connectedness.
        /// </summary>
        gridRegion<T>* complete();

        /// <summary>
        /// Adds new cell to the region, and updates collections.
        /// </summary>
        void add(short column, short row, T item);

        /// <summary>
        /// Sets new cells in the grid using the parameterless constructor
        /// </summary>
        void add(const grid<T>& grid);

        /// <summary>
        /// Sets new cells in the grid using the parameterless constructor
        /// </summary>
        void add(T* itemArray);

        /// <summary>
        /// Checks to see whether or not the region constructor contains the specified item
        /// </summary>
        bool contains(T item);

        /// <summary>
        /// Checks to see whether the constructor is defined at the specified coordinates
        /// </summary>
        bool isDefined(short column, short row);

        /// <summary>
        /// Translates a gridRegion using the specified offset - creating a new gridRegion instance
        /// </summary>
        /// <param name="region"></param>
        /// <returns></returns>
        static gridRegion<T>* translate(const gridRegion<T>& region, gridLocator translation);

    private:

        void completeImpl();
        bool isConnected(short column, short row, T item);
        void addEdges(short column, short row, T item);
        void addBoundary(short column, short row, T item);
        void validate();
        void validateRegionCollection(simpleHash<T, T>* collection);
        gridRect calculateLargestRectangle();

    private:

        grid<T>* _grid;

        simpleHash<T, T>* _locations;
        simpleHash<T, T>* _edgeLocations;

        simpleHash<T, T>* _northEdges;
        simpleHash<T, T>* _southEdges;
        simpleHash<T, T>* _eastEdges;
        simpleHash<T, T>* _westEdges;

        simpleHash<T, T>* _nwCorners;
        simpleHash<T, T>* _neCorners;
        simpleHash<T, T>* _seCorners;
        simpleHash<T, T>* _swCorners;

        gridRect _calculatedBoundary;

        short _top;
        short _left;
        short _right;
        short _bottom;

        bool _completed;

        gridPredicate<T> _predicate;
    };
}

