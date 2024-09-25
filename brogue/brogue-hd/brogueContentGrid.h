#pragma once

#include "broguePlayer.h"
#include "brogueItem.h"
#include "brogueCreature.h"

namespace brogueHd::backend::model
{
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
		grid<brogueObject*> _contentGrid;

		// Content Collections:  Sub-groupings of contents kept in parallel to the grid
		std::vector<brogueCreature*> _enemies;
		std::vector<brogueCreature*> _friendlies;
		std::vector<brogueCreature*> _dormantEnemies;

		std::vector<brogueItem*> _keys;
		std::vector<brogueItem*> _food;
		std::vector<brogueItem*> _potions;
		std::vector<brogueItem*> _weapons;
		std::vector<brogueItem*> _armor;
		std::vector<brogueItem*> _wands;
		std::vector<brogueItem*> _staves;
		std::vector<brogueItem*> _rings;
		std::vector<brogueItem*> _charms;
		std::vector<brogueItem*> _scrolls;
	};
}

