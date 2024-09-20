#pragma once

#include "itemConstants.h"
#include "color.h"
#include "mathdef.h"
#include "dungeonDeclaration.h"

using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model::item
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
		monsterTypes vorpalEnemy;
	};

	struct itemStaff : itemBase
	{
		staffKind kind;

		boltType bolt;
		short charges;
		monsterTypes vorpalEnemy;
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
		monsterTypes vorpalEnemy;
	};

	enum itemFlags {
		ITEM_IDENTIFIED = Fl(0),
		ITEM_EQUIPPED = Fl(1),
		ITEM_CURSED = Fl(2),
		ITEM_PROTECTED = Fl(3),
		// unused               = Fl(4),
		ITEM_RUNIC = Fl(5),
		ITEM_RUNIC_HINTED = Fl(6),
		ITEM_RUNIC_IDENTIFIED = Fl(7),
		ITEM_CAN_BE_IDENTIFIED = Fl(8),
		ITEM_PREPLACED = Fl(9),
		ITEM_FLAMMABLE = Fl(10),
		ITEM_MAGIC_DETECTED = Fl(11),
		ITEM_MAX_CHARGES_KNOWN = Fl(12),
		ITEM_IS_KEY = Fl(13),

		ITEM_ATTACKS_STAGGER = Fl(14),	// mace, hammer
		ITEM_ATTACKS_EXTEND = Fl(15),   // whip
		ITEM_ATTACKS_QUICKLY = Fl(16),   // rapier
		ITEM_ATTACKS_PENETRATE = Fl(17),	// spear, pike
		ITEM_ATTACKS_ALL_ADJACENT = Fl(18),	// axe, war axe
		ITEM_LUNGE_ATTACKS = Fl(19),   // rapier
		ITEM_SNEAK_ATTACK_BONUS = Fl(20),   // dagger
		ITEM_PASS_ATTACKS = Fl(21),   // flail

		ITEM_KIND_AUTO_ID = Fl(22),	// the item type will become known when the item is picked up.
		ITEM_PLAYER_AVOIDS = Fl(23),	// explore and travel will try to avoid picking the item up
	};
}