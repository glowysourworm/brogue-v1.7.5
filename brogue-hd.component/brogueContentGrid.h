#pragma once

#include "grid.h"
#include <brogueCreature.h>
#include <brogueItem.h>
#include <brogueObject.h>
#include <broguePlayer.h>
#include <simpleList.h>

namespace brogueHd::component
{
	using namespace simple;
	using namespace brogueHd::model;

	/// <summary>
	/// Component that handles collections of brogueObject types; and sets them
	/// into the grid by storing private collections.
	/// </summary>
	class brogueContentGrid
	{
	public:
		brogueContentGrid();
		~brogueContentGrid();

	private:
		broguePlayer* _player;
		//brogueGrid* _grid;

		// All contents
		grid<brogueObject*>* _contentGrid;

		// Content Collections:  Sub-groupings of contents kept in parallel to the grid
		simpleList<brogueCreature*>* _enemies;
		simpleList<brogueCreature*>* _friendlies;
		simpleList<brogueCreature*>* _dormantEnemies;

		simpleList<brogueItem*>* _keys;
		simpleList<brogueItem*>* _food;
		simpleList<brogueItem*>* _potions;
		simpleList<brogueItem*>* _weapons;
		simpleList<brogueItem*>* _armor;
		simpleList<brogueItem*>* _wands;
		simpleList<brogueItem*>* _staves;
		simpleList<brogueItem*>* _rings;
		simpleList<brogueItem*>* _charms;
		simpleList<brogueItem*>* _scrolls;
	};

	brogueContentGrid::brogueContentGrid()
	{
	}

	brogueContentGrid::~brogueContentGrid()
	{
	}
}
