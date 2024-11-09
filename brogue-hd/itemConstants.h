#pragma once

#include "brogueGlobal.h"

namespace brogueHd::backend::model
{
	enum itemFlags 
	{
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

	enum itemCategory 
	{
		FOOD = Fl(0),
		WEAPON = Fl(1),
		ARMOR = Fl(2),
		POTION = Fl(3),
		SCROLL = Fl(4),
		STAFF = Fl(5),
		WAND = Fl(6),
		RING = Fl(7),
		CHARM = Fl(8),
		GOLD = Fl(9),
		AMULET = Fl(10),
		GEM = Fl(11),
		KEY = Fl(12)

		//CAN_BE_DETECTED = (WEAPON | ARMOR | POTION | SCROLL | RING | CHARM | WAND | STAFF | AMULET),
		//PRENAMED_CATEGORY = (FOOD | GOLD | AMULET | GEM | KEY),
		//NEVER_IDENTIFIABLE = (FOOD | CHARM | GOLD | AMULET | GEM | KEY),
		//CAN_BE_SWAPPED = (WEAPON | ARMOR | STAFF | CHARM | RING),
		//ALL_ITEMS = (FOOD | POTION | WEAPON | ARMOR | STAFF | WAND | SCROLL | RING | CHARM | GOLD | AMULET | GEM | KEY),
	};

	enum keyKind {
		KEY_DOOR,
		KEY_CAGE,
		KEY_PORTAL,
		NUMBER_KEY_TYPES
	};

	enum foodKind {
		RATION,
		FRUIT,
		NUMBER_FOOD_KINDS
	};

	enum potionKind {
		POTION_LIFE,
		POTION_STRENGTH,
		POTION_TELEPATHY,
		POTION_LEVITATION,
		POTION_DETECT_MAGIC,
		POTION_HASTE_SELF,
		POTION_FIRE_IMMUNITY,
		POTION_INVISIBILITY,
		POTION_POISON,
		POTION_PARALYSIS,
		POTION_HALLUCINATION,
		POTION_CONFUSION,
		POTION_INCINERATION,
		POTION_DARKNESS,
		POTION_DESCENT,
		POTION_LICHEN,
		NUMBER_POTION_KINDS
	};

	enum weaponKind {
		DAGGER,
		SWORD,
		BROADSWORD,

		WHIP,
		RAPIER,
		FLAIL,

		MACE,
		HAMMER,

		SPEAR,
		PIKE,

		AXE,
		WAR_AXE,

		DART,
		INCENDIARY_DART,
		JAVELIN,
		NUMBER_WEAPON_KINDS
	};

	enum weaponEnchants {
		W_SPEED,
		W_QUIETUS,
		W_PARALYSIS,
		W_MULTIPLICITY,
		W_SLOWING,
		W_CONFUSION,
		W_FORCE,
		W_SLAYING,
		W_MERCY,
		NUMBER_GOOD_WEAPON_ENCHANT_KINDS = W_MERCY,
		W_PLENTY,
		NUMBER_WEAPON_RUNIC_KINDS
	};

	enum armorKind {
		LEATHER_ARMOR,
		SCALE_MAIL,
		CHAIN_MAIL,
		BANDED_MAIL,
		SPLINT_MAIL,
		PLATE_MAIL,
		NUMBER_ARMOR_KINDS
	};

	enum armorEnchants {
		A_MULTIPLICITY,
		A_MUTUALITY,
		A_ABSORPTION,
		A_REPRISAL,
		A_IMMUNITY,
		A_REFLECTION,
		A_RESPIRATION,
		A_DAMPENING,
		A_BURDEN,
		NUMBER_GOOD_ARMOR_ENCHANT_KINDS = A_BURDEN,
		A_VULNERABILITY,
		A_IMMOLATION,
		NUMBER_ARMOR_ENCHANT_KINDS,
	};

	enum wandKind {
		WAND_TELEPORT,
		WAND_SLOW,
		WAND_POLYMORPH,
		WAND_NEGATION,
		WAND_DOMINATION,
		WAND_BECKONING,
		WAND_PLENTY,
		WAND_INVISIBILITY,
		WAND_EMPOWERMENT,
		NUMBER_WAND_KINDS
	};

	enum staffKind {
		STAFF_LIGHTNING,
		STAFF_FIRE,
		STAFF_POISON,
		STAFF_TUNNELING,
		STAFF_BLINKING,
		STAFF_ENTRANCEMENT,
		STAFF_OBSTRUCTION,
		STAFF_DISCORD,
		STAFF_CONJURATION,
		STAFF_HEALING,
		STAFF_HASTE,
		STAFF_PROTECTION,
		NUMBER_STAFF_KINDS
	};

	// these must be wand bolts, in order, and then staff bolts, in order:
	enum boltType {
		BOLT_NONE = 0,
		BOLT_TELEPORT,
		BOLT_SLOW,
		BOLT_POLYMORPH,
		BOLT_NEGATION,
		BOLT_DOMINATION,
		BOLT_BECKONING,
		BOLT_PLENTY,
		BOLT_INVISIBILITY,
		BOLT_EMPOWERMENT,
		BOLT_LIGHTNING,
		BOLT_FIRE,
		BOLT_POISON,
		BOLT_TUNNELING,
		BOLT_BLINKING,
		BOLT_ENTRANCEMENT,
		BOLT_OBSTRUCTION,
		BOLT_DISCORD,
		BOLT_CONJURATION,
		BOLT_HEALING,
		BOLT_HASTE,
		BOLT_SLOW_2,
		BOLT_SHIELDING,
		BOLT_SPIDERWEB,
		BOLT_SPARK,
		BOLT_DRAGONFIRE,
		BOLT_DISTANCE_ATTACK,
		BOLT_POISON_DART,
		BOLT_ANCIENT_SPIRIT_VINES,
		BOLT_WHIP,
		NUMBER_BOLT_KINDS
	};

	enum ringKind {
		RING_CLAIRVOYANCE,
		RING_STEALTH,
		RING_REGENERATION,
		RING_TRANSFERENCE,
		RING_LIGHT,
		RING_AWARENESS,
		RING_WISDOM,
		RING_REAPING,
		NUMBER_RING_KINDS
	};

	enum charmKind {
		CHARM_HEALTH,
		CHARM_PROTECTION,
		CHARM_HASTE,
		CHARM_FIRE_IMMUNITY,
		CHARM_INVISIBILITY,
		CHARM_TELEPATHY,
		CHARM_LEVITATION,
		CHARM_SHATTERING,
		CHARM_GUARDIAN,
		CHARM_TELEPORTATION,
		CHARM_RECHARGING,
		CHARM_NEGATION,
		NUMBER_CHARM_KINDS
	};

	enum scrollKind {
		SCROLL_ENCHANTING,
		SCROLL_IDENTIFY,
		SCROLL_TELEPORT,
		SCROLL_REMOVE_CURSE,
		SCROLL_RECHARGING,
		SCROLL_PROTECT_ARMOR,
		SCROLL_PROTECT_WEAPON,
		SCROLL_SANCTUARY,
		SCROLL_MAGIC_MAPPING,
		SCROLL_NEGATION,
		SCROLL_SHATTERING,
		SCROLL_DISCORD,
		SCROLL_AGGRAVATE_MONSTER,
		SCROLL_SUMMON_MONSTER,
		NUMBER_SCROLL_KINDS
	};
}