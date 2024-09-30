#pragma once

#include "color.h"
#include "itemConstants.h"
#include "creatureConstants.h"

#include "randomRange.h"

#include <string>

using namespace std;

using namespace brogueHd::component::math;
using namespace brogueHd::backend::modelConstant;

namespace brogueHd::backend::model::game
{
	// Item Declarations:  This struct hierarchy will take care of item types (or "kinds" from Brogue)
	//					   with enums galore! ...just separated by struct type inference. 
	//

	struct itemBase
	{
		// TODO: Parse all the different flags.s.s.s.sss, and put them in the hierarchy
		//unsigned long flags;

		// Category to decide type inspection
		itemCategory category;

		// Damage left to base class (see throwing v.s. melee)
		randomRange damage;
		
		// Inventory color: NOT brogueObject color definition.
		color inventoryColor;

		// Inventory display character:  NOT brogueObject definition.
		char inventoryLetter;

		// Inscription (?): This should probably go with brogueObject (for all dungeon mapped objects)
		char* inscription[DCOLS];

		// Item is stackable in inventory
		bool stackableInventory;

		// Quantity of items that are currently stacked (quiver or quantity)		
		unsigned short stackableQuantity;
	};

	struct itemKey : itemBase
	{
		keyKind kind;
	};

	struct itemFood : itemBase
	{
		foodKind kind;
	};

	struct itemPotion : itemBase
	{
		potionKind kind;
	};

	struct itemWeapon : itemBase
	{
		weaponKind kind;
		weaponEnchants enchant1;
		weaponEnchants enchant2;
		short timesEnchanted;
		short strengthRequired;
	};

	struct itemArmor : itemBase
	{
		armorKind kind;
		armorEnchants enchant1;
		armorEnchants enchant2;
		short armor;
		short timesEnchanted;
		short strengthRequired;
	};

	struct itemWand : itemBase
	{
		wandKind kind;

		boltType bolt;
		short charges;
		creatureTypes vorpalEnemy;
	};

	struct itemStaff : itemBase
	{
		staffKind kind;

		boltType bolt;
		short charges;
		creatureTypes vorpalEnemy;
		short strengthRequired;
	};

	struct itemRing : itemBase
	{
		ringKind kind;
		short timesEnchanted;
	};

	struct itemCharm : itemBase
	{
		charmKind kind;
		short charges;
		short timesEnchanted;
	};

	struct itemScroll : itemBase
	{
		scrollKind kind;
		creatureTypes vorpalEnemy;
	};

	struct itemTable
	{
		std::string name;
		std::string flavor;
		std::string callTitle;
		short frequency;
		short marketValue;
		short strengthRequired;
		randomRange range;
		bool identified;
		bool called;
		std::string description;
	};
}