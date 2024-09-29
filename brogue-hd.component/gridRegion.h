#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "simpleArray.h"
#include <functional>

using namespace std;
using namespace brogueHd::component::math;

namespace brogueHd::component
{
	/// <summary>
	/// Grid component that creates a subgrid, using the same coordinate space, from a parent grid. This
	/// means the result will have the same width and height; but will be a subgrid on a specified gridRect
	/// of the parent grid. Each function and call will be protected to utilize only this region. No grid
	/// offsets are required for use.
	/// </summary>
	template<isGridLocator T>
	class gridRegion
	{
	public:
		gridRegion(){}
		gridRegion(const gridRegion& copy);
		gridRegion(T* locations,
				   T* edgeLocations,
				   T* westExposedLocations,
				   T* northExposedLocations,
				   T* eastExposedLocations,
				   T* southExposedLocations,
				   T* northWestCornerLocations,
				   T* northEastCornerLocations,
				   T* southEastCornerLocations,
				   T* southWestCornerLocations,
				   gridRect parentBoundary,
				   gridRect relativeBoundary,
				   gridRect largestRectangularSubRegion);


		~gridRegion();

		gridRect getBoundary() const;
		gridRect getParentBoundary() const;
		gridRect getLargestSubRectangle() const;
		simpleArray<T>* getLocations();
		simpleArray<T>* getEdgeLocations();

		/// <summary>
		/// Checks for location overlap
		/// </summary>
		bool overlaps(const gridRegion<T>& region);

		/// <summary>
		/// Copies grid region to another grid
		/// </summary>
		void copyTo(grid<T>& grid);

	public:

		/// <summary>
		/// Returns value from the underlying grid. This is the index space of the parent grid.
		/// </summary>
		T get(short column, short row) const;

		/// <summary>
		/// Returns true if a cell is defined for the region
		/// </summary>
		bool isDefined(short column, short row) const;

		/// <summary>
		/// Returns true if an edge cell is defined
		/// </summary>
		bool isEdge(short column, short row) const;

		/// <summary>
		/// Iterates the locations of the region and calls the user method
		/// </summary>
		void iterateLocations(gridDelegates<T>::callback callback);

		/// <summary>
		/// Iterates the edge locations of the region and calls the user method
		/// </summary>
		void iterateEdges(gridDelegates<T>::callback callback);

		/// <summary>
		/// Gets locations with an exposed edge in the specified direction
		/// </summary>
		simpleArray<T>* getEdges(brogueCompass direction);

		/// <summary>
		/// Gets the locations that are also along the bounding rectangle
		/// </summary>
		simpleArray<T>* getBoundaryEdges(brogueCompass direction);

		/// <summary>
		/// Gets locations with exposed corner
		/// </summary>
		simpleArray<T>* getCorners(brogueCompass nonCardinalDirection);

	private:

		// Needed a mutator for the translate() function
		void initialize(T* locations,
						T* edgeLocations,
						T* westExposedLocations,
						T* northExposedLocations,
						T* eastExposedLocations,
						T* southExposedLocations,
						T* northWestCornerLocations,
						T* northEastCornerLocations,
						T* southEastCornerLocations,
						T* southWestCornerLocations,
						gridRect parentBoundary,
						gridRect relativeBoundary,
						gridRect largestRectangularSubRegion);

	private:

		grid<T>* _grid;
		grid<T>* _edgeGrid;

		gridRect _largestRectangularSubRegion;

		simpleArray<T>* _locations;
		simpleArray<T>* _edgeLocations;

		// Exposed Edge Faces
		simpleArray<T>* _northExposedLocations;
		simpleArray<T>* _southExposedLocations;
		simpleArray<T>* _westExposedLocations;
		simpleArray<T>* _eastExposedLocations;
		
		// Corners
		simpleArray<T>* _northEastCornerLocations;
		simpleArray<T>* _northWestCornerLocations;
		simpleArray<T>* _southEastCornerLocations;
		simpleArray<T>* _southWestCornerLocations;
	};
}

