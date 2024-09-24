#pragma once

#include "gridDefinitions.h"
#include "array2D.h"
#include "accretionTile.h"
#include "gridRect.h"
#include <vector>

using namespace std;

namespace brogueHd::backend::model::layout
{
	class accretionTiling
	{
	public:

		/// <summary>
		/// Creates an instance of the accretion tiling
		/// </summary>
		/// <param name="boundary">Parent (level) boundary</param>
		/// <param name="minBoundary">Minimum boundary for a tile (room)</param>
		accretionTiling(gridRect boundary, gridRect minBoundary);
		~accretionTiling();

		/// <summary>
		/// Adds tile to the accretion and marks used space
		/// </summary>
		void addTile(const accretionTile& tile);

		/// <summary>
		/// Returns true if space in this direction meets minimum criteria
		/// </summary>
		bool hasAvailableSpace(const accretionTile& from, brogueCompass direction);

		/// <summary>
		/// Gets available expansion space in a certain direction
		/// </summary>
		gridRect getAvailableSpace(const accretionTile& from, brogueCompass direction);

	private:

		gridRect calculateAvailableSpace(const accretionTile& from, brogueCompass direction);

		std::vector<gridRect> calculateUnusedSpace();

	private:

		array2D<bool>* _usedGrid;

		gridRect _minBoundary;

		std::vector<accretionTile>* _tiles;
	};
}