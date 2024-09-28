#pragma once

#include "broguedef.h"
#include "simpleList.h"
#include "accretionTile.h"
#include "brogueLayout.h"
#include "brogueLevel.h"
#include "brogueLevelProfile.h"
#include "roomGenerator.h"
#include "randomGenerator.h"
#include "gridLocatorEdge.h"
#include "gridLocatorNode.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model;
using namespace brogueHd::backend::model::layout;

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

		void initialize(brogueLevelProfile* profile);
		void clear();

		brogueLayout* generateLayout();

	private:

		void createRooms();

		// (MEMORY!) Creates a new room according to the specified room info
		gridRegion<gridLocator>* createRoom(gridRect levelBoundary, gridRect levelPaddedBoundary, const brogueRoomInfo& roomInfo);

		bool attemptConnection(accretionTile& roomTile, const gridRect& attemptRect, short interRoomPadding) const;

		void designateMachineRooms();
		void triangulateRooms();
		void connectRooms();
		void createTerrain();

		/// <summary>
		/// Returns the Brogue level boundary with padding
		/// </summary>
		gridRect getPaddedBoundary(short padding) const;

	// Managed On: clear(), initialize(...)
	private:

		grid<gridLocator>* _grid;

		simpleList<accretionTile>* _roomTiles;

		graph<gridLocatorNode, gridLocatorEdge>* _delaunayGraph;

		brogueLevelProfile* _profile;

		bool _initialized;

	private:

		roomGenerator* _roomGenerator;
		randomGenerator* _randomGenerator;
	};
}

