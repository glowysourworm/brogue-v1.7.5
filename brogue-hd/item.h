#pragma once

#include "color.h"
#include "creatureConstants.h"
#include "itemConstants.h"
#include "randomRange.h"
#include "brogueGlobal.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::backend::model
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
		unsigned int stackableQuantity;
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
		int timesEnchanted;
		int strengthRequired;
	};

	struct itemArmor : itemBase
	{
		armorKind kind;
		armorEnchants enchant1;
		armorEnchants enchant2;
		int armor;
		int timesEnchanted;
		int strengthRequired;
	};

	struct itemWand : itemBase
	{
		wandKind kind;

		boltType bolt;
		int charges;
		creatureTypes vorpalEnemy;
	};

	struct itemStaff : itemBase
	{
		staffKind kind;

		boltType bolt;
		int charges;
		creatureTypes vorpalEnemy;
		int strengthRequired;
	};

	struct itemRing : itemBase
	{
		ringKind kind;
		int timesEnchanted;
	};

	struct itemCharm : itemBase
	{
		charmKind kind;
		int charges;
		int timesEnchanted;
	};

	struct itemScroll : itemBase
	{
		scrollKind kind;
		creatureTypes vorpalEnemy;
	};

	struct itemTable
	{
		simpleString name;
		simpleString flavor;
		simpleString callTitle;
		int frequency;
		int marketValue;
		int strengthRequired;
		randomRange range;
		bool identified;
		bool called;
		simpleString description;
	};
}