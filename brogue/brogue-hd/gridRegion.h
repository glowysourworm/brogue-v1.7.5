#pragma once

#include "grid.h"
#include "gridDefinitions.h"
#include "gridExtension.h"
#include "extensionDefinitions.h"
#include "gridRect.h"
#include "dijkstra.h"
#include <functional>
#include <vector>

using namespace std;

using namespace brogueHd::backend::math;

namespace brogueHd::backend::model::layout
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
		std::vector<T>* getLocations();
		std::vector<T>* getEdgeLocations();

		/// <summary>
		/// Translates this instance of the grid region using the provided translator
		/// </summary>
		void translate(gridLocator translation);

		/// <summary>
		/// Checks for location overlap
		/// </summary>
		bool overlaps(const gridRegion<T>& region);

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
		std::vector<T>* getEdges(brogueCompass direction);

		/// <summary>
		/// Gets the locations that are also along the bounding rectangle
		/// </summary>
		std::vector<T>& getBoundaryEdges(brogueCompass direction);

		/// <summary>
		/// Gets locations with exposed corner
		/// </summary>
		std::vector<T>* getCorners(brogueCompass nonCardinalDirection);

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

		std::vector<T>* _locations;
		std::vector<T>* _edgeLocations;

		// Exposed Edge Faces
		std::vector<T>* _northExposedLocations;
		std::vector<T>* _southExposedLocations;
		std::vector<T>* _westExposedLocations;
		std::vector<T>* _eastExposedLocations;
		
		// Corners
		std::vector<T>* _northEastCornerLocations;
		std::vector<T>* _northWestCornerLocations;
		std::vector<T>* _southEastCornerLocations;
		std::vector<T>* _southWestCornerLocations;
	};
}

