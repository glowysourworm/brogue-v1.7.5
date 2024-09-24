#pragma once

#include "accretionTile.h"
#include "brogueLayout.h"
#include "brogueLevel.h"
#include "brogueLevelProfile.h"
#include "roomGenerator.h"
#include "randomGenerator.h"

using namespace brogueHd::backend::model;

namespace brogueHd::backend::generator
{
	class layoutGenerator
	{
	public:

		/// <summary>
		/// Creates the base layout, terrain, and machine terrain for the leve
		/// </summary>
		/// <param name="profile">The dungeon profile:  must contain all parameters</param>
		layoutGenerator(randomGenerator* randomGenerator);
		~layoutGenerator();

		void initialize(const brogueLevelProfile& profile);
		void clear();

		brogueLayout* generateLayout();

	private:

		void createRooms();

		// (MEMORY!) Creates a new room according to the specified room info
		gridRegion<gridLocator>* createRoom(gridRect levelBoundary, gridRect levelPaddedBoundary, const brogueRoomInfo& roomInfo);

		bool attemptConnection(accretionTile& roomTile, const gridRect& attemptRect, short interRoomPadding) const;

		void designateMachineRooms();
		graph<gridLocatorNode, gridLocatorEdge> triangulateRooms();
		void connectRooms(graph<gridLocatorNode, gridLocatorEdge> delaunayGraph);
		void createTerrain();

		/// <summary>
		/// Returns the Brogue level boundary with padding
		/// </summary>
		gridRect getPaddedBoundary(short padding) const;

	// Managed On: clear(), initialize(...)
	private:

		grid<gridLocator>* _grid;

		std::vector<accretionTile>* _roomTiles;

		brogueLevelProfile _profile;

		bool _initialized;

	private:

		roomGenerator* _roomGenerator;
		randomGenerator* _randomGenerator;
	};
}

