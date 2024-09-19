#pragma once

#include "grid.h"

namespace brogueHd::backend::model::layout
{
	template<gridCellConstraint T>
	struct floodFillData
	{
		std::vector<T> locations;
		std::vector<T> edgeLocations;
		std::vector<T> westExposedLocations;
		std::vector<T> northExposedLocations;
		std::vector<T> eastExposedLocations;
		std::vector<T> southExposedLocations;
		std::vector<T> northWestCornerLocations;
		std::vector<T> northEastCornerLocations;
		std::vector<T> southEastCornerLocations;
		std::vector<T> southWestCornerLocations;
		gridRect relativeBoundary;
	};
}