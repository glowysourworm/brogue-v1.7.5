#pragma once

#include "dungeonDeclaration.h"
#include "item.h"
#include "itemFlavor.h"
#include "itemConstants.h"

//typedef struct itemTable {
//	char *name;
//	char *flavor;
//	short frequency;
//	short marketValue;
//	short number;
//	randomRange range;
//} itemTable;

const itemTable keyTable[NUMBER_KEY_TYPES] = {
	{"door key",			"", "", 1, 0,	0, {0,0,0}, true, false, "The notches on this ancient iron key are well worn; its leather lanyard is battered by age. What door might it open?"},
	{"cage key",			"", "", 1, 0,	0, {0,0,0}, true, false, "The rust accreted on this iron key has been stained with flecks of blood; it must have been used recently. What cage might it open?"},
	{"crystal orb",			"", "", 1, 0,	0, {0,0,0}, true, false, "A faceted orb, seemingly cut from a single crystal, sparkling and perpetually warm to the touch. What manner of device might such an object activate?"},
};

const itemTable foodTable[NUMBER_FOOD_KINDS] = {
	{"ration of food",		"", "", 3, 25,	1800, {0,0,0}, true, false, "A ration of food. Was it left by former adventurers? Is it a curious byproduct of the subterranean ecosystem?"},
	{"mango",				"", "", 1, 15,	1550, {0,0,0}, true, false, "An odd fruit to be found so deep beneath the surface of the earth, but only slightly less filling than a ration of food."}
};

const itemTable weaponTable[NUMBER_WEAPON_KINDS] = {
	{"dagger",				"", "", 10, 190,		12,	{3,	4,	1},		true, false, "A simple iron dagger with a well-worn wooden handle. Daggers will deal quintuple damage upon a successful sneak attack instead of triple damage."},
	{"sword",				"", "", 10, 440,		14, {7,	9,	1},		true, false, "The razor-sharp length of steel blade shines reassuringly."},
	{"broadsword",			"", "", 10, 990,		19,	{14, 22, 1},	true, false, "This towering blade inflicts heavy damage by investing its heft into every cut."},

	{"whip",				"", "", 10, 440,		14, {3,	5,	1},		true, false, "The lash from this coil of braided leather can tear bark from trees, and it will reach opponents up to five spaces away."},
	{"rapier",				"", "", 10, 440,		15, {3,	5,	1},		true, false, "This blade is thin and flexible, designed for deft and rapid maneuvers. It inflicts less damage than comparable weapons, but permits you to attack twice as quickly. If there is one space between you and an enemy and you step directly toward it, you will perform a devastating lunge attack, which deals treble damage and never misses."},
	{"flail",				"", "", 10, 440,		17, {10,13,	1},		true, false, "This spiked iron ball can be whirled at the end of its chain in synchronicity with your movement, allowing you a free attack whenever moving between two spaces that are adjacent to an enemy."},

	{"mace",				"", "", 10, 660,		16, {16, 20, 1},	true, false, "The iron flanges at the head of this weapon inflict substantial damage with every weighty blow. Because of its heft, it takes an extra turn to recover when it hits, and will push your opponent backward if there is room."},
	{"war hammer",			"", "", 10, 1100,		20, {25, 35, 1},	true, false, "Few creatures can withstand the crushing blow of this towering mass of lead and steel, but only the strongest of adventurers can effectively wield it. Because of its heft, it takes an extra turn to recover when it hits, and will push your opponent backward if there is room."},

	{"spear",				"", "", 10, 330,		13, {4, 5, 1},		true, false, "A slender wooden rod tipped with sharpened iron. The reach of the spear permits you to simultaneously attack an adjacent enemy and the enemy directly behind it."},
	{"war pike",			"", "", 10, 880,		18, {11, 15, 1},	true, false, "A long steel pole ending in a razor-sharp point. The reach of the pike permits you to simultaneously attack an adjacent enemy and the enemy directly behind it."},

	{"axe",					"", "", 10, 550,		15, {7, 9, 1},		true, false, "The blunt iron edge on this axe glints in the darkness. The arc of its swing permits you to attack all adjacent enemies simultaneously."},
	{"war axe",				"", "", 10, 990,		19, {12, 17, 1},	true, false, "The enormous steel head of this war axe puts considerable heft behind each stroke. The arc of its swing permits you to attack all adjacent enemies simultaneously."},

	{"dart",				"", "",	0,	15,			10,	{2,	4,	1},		true, false, "These simple metal spikes are weighted to fly true and sting their prey with a flick of the wrist."},
	{"incendiary dart",		"", "",	10, 25,			12,	{1,	2,	1},		true, false, "The barbed spike on each of these darts is designed to stick to its target while the compounds strapped to its length explode into flame."},
	{"javelin",				"", "",	10, 40,			15,	{3, 11, 3},		true, false, "This length of metal is weighted to keep the spike at its tip foremost as it sails through the air."},
};

const itemTable armorTable[NUMBER_ARMOR_KINDS] = {
	{"leather armor",	"", "", 10,	250,		10,	{30,30,0},		true, false, "This lightweight armor offers basic protection."},
	{"scale mail",		"", "", 10, 350,		12, {40,40,0},		true, false, "Bronze scales cover the surface of treated leather, offering greater protection than plain leather with minimal additional weight."},
	{"chain mail",		"", "", 10, 500,		13, {50,50,0},		true, false, "Interlocking metal links make for a tough but flexible suit of armor."},
	{"banded mail",		"", "", 10, 800,		15, {70,70,0},		true, false, "Overlapping strips of metal horizontally encircle a chain mail base, offering an additional layer of protection at the cost of greater weight."},
	{"splint mail",		"", "", 10, 1000,		17, {90,90,0},		true, false, "Thick plates of metal are embedded into a chain mail base, providing the wearer with substantial protection."},
	{"plate armor",		"", "", 10, 1300,		19, {110,110,0},	true, false, "Enormous plates of metal are joined together into a suit that provides unmatched protection to any adventurer strong enough to bear its staggering weight."}
};

const char weaponRunicNames[NUMBER_WEAPON_RUNIC_KINDS][30] = {
	"speed",
	"quietus",
	"paralysis",
	"multiplicity",
	"slowing",
	"confusion",
	"force",
	"slaying",
	"mercy",
	"plenty"
};

const char armorRunicNames[NUMBER_ARMOR_ENCHANT_KINDS][30] = {
	"multiplicity",
	"mutuality",
	"absorption",
	"reprisal",
	"immunity",
	"reflection",
	"respiration",
	"dampening",
	"burden",
	"vulnerability",
	"immolation",
};

itemTable scrollTable[NUMBER_SCROLL_KINDS] = {
	{"enchanting",			itemTitles[0], "",	0,	550,	0,{0,0,0}, false, false, "This ancient enchanting sorcery will imbue a single item with a powerful and permanent magical charge. A staff will increase in power and in number of charges; a weapon will inflict more damage and find its mark more easily; a suit of armor will deflect attacks more often; the magic of a ring will intensify; and a wand will gain expendable charges in the least amount that such a wand can be found with. Weapons and armor will also require less strength to use, and any curses on the item will be lifted."}, // frequency is dynamically adjusted
	{"identify",			itemTitles[1], "",	30,	300,	0,{0,0,0}, false, false, "This scrying magic will permanently reveal all of the secrets of a single item."},
	{"teleportation",		itemTitles[2], "",	10,	500,	0,{0,0,0}, false, false, "This escape spell will instantly relocate you to a random location on the dungeon level. It can be used to escape a dangerous situation with luck. The unlucky reader might find himself in an even more dangerous place."},
	{"remove curse",		itemTitles[3], "",	15,	150,	0,{0,0,0}, false, false, "This redemption spell will instantly strip from the reader's weapon, armor, rings and carried items any evil enchantments that might prevent the wearer from removing them."},
	{"recharging",			itemTitles[4], "",	12,	375,	0,{0,0,0}, false, false, "The power bound up in this parchment will instantly recharge all of your staffs and charms."},
	{"protect armor",		itemTitles[5], "",	10,	400,	0,{0,0,0}, false, false, "This ceremonial shielding magic will permanently proof your armor against degradation by acid."},
	{"protect weapon",		itemTitles[6], "",	10,	400,	0,{0,0,0}, false, false, "This ceremonial shielding magic will permanently proof your weapon against degradation by acid."},
	{"sanctuary",           itemTitles[7], "",  10, 500,    0,{0,0,0}, false, false, "This protection rite will imbue the area with powerful warding glyphs, when released over plain ground. Monsters will not willingly set foot on the affected area."},
	{"magic mapping",		itemTitles[8], "",	12,	500,	0,{0,0,0}, false, false, "This powerful scouting magic will etch a purple-hued image of crystal clarity into your memory, alerting you to the precise layout of the level and revealing all hidden secrets."},
	{"negation",			itemTitles[9], "",	8,	400,	0,{0,0,0}, false, false, "When this powerful anti-magic is released, all creatures (including yourself) and all items lying on the ground within your field of view will be exposed to its blast and stripped of magic. Creatures animated purely by magic will die. Potions, scrolls, items being held by other creatures and items in your inventory will not be affected."},
	{"shattering",			itemTitles[10],"",	8,	500,	0,{0,0,0}, false, false, "This strange incantation will alter the physical structure of nearby stone, causing it to evaporate into the air over the ensuing minutes."},
	{"discord",             itemTitles[11], "",	8,	400,	0,{0,0,0}, false, false, "This scroll will unleash a powerful blast of mind magic. Any creatures within line of sight will turn against their companions and attack indiscriminately for 30 turns."},
	{"aggravate monsters",	itemTitles[12], "",	15,	50,		0,{0,0,0}, false, false, "This scroll will unleash a piercing shriek that will awaken all monsters and alert them to the reader's location."},
	{"summon monsters",		itemTitles[13], "",	10,	50,		0,{0,0,0}, false, false, "This summoning incantation will call out to creatures in other planes of existence, drawing them through the fabric of reality to confront the reader."},
};

itemTable potionTable[NUMBER_POTION_KINDS] = {
	{"life",				itemColors[1], "",	0,	500,	0,{0,0,0}, false, false, "A swirling elixir that will instantly heal you, cure you of ailments, and permanently increase your maximum health."}, // frequency is dynamically adjusted
	{"strength",			itemColors[2], "",	0,	400,	0,{0,0,0}, false, false, "This powerful medicine will course through your muscles, permanently increasing your strength by one point."}, // frequency is dynamically adjusted
	{"telepathy",			itemColors[3], "",	20,	350,	0,{0,0,0}, false, false, "This mysterious liquid will attune your mind to the psychic signature of distant creatures. Its effects will not reveal inanimate objects, such as totems, turrets and traps."},
	{"levitation",			itemColors[4], "",	15,	250,	0,{0,0,0}, false, false, "This curious liquid will cause you to hover in the air, able to drift effortlessly over lava, water, chasms and traps. Flames, gases and spiderwebs fill the air, and cannot be bypassed while airborne. Creatures that dwell in water or mud will be unable to attack you while you levitate."},
	{"detect magic",		itemColors[5], "",	20,	500,	0,{0,0,0}, false, false, "This mysterious brew will sensitize your mind to the radiance of magic. Items imbued with helpful enchantments will be marked with a full sigil; items corrupted by curses or designed to bring misfortune upon the bearer will be marked with a hollow sigil. The Amulet of Yendor will be revealed by its unique aura."},
	{"speed",				itemColors[6], "",	10,	500,	0,{0,0,0}, false, false, "Quaffing the contents of this flask will enable you to move at blinding speed for several minutes."},
	{"fire immunity",		itemColors[7], "",	15,	500,	0,{0,0,0}, false, false, "This potion will render you impervious to heat and permit you to wander through fire and lava and ignore otherwise deadly bolts of flame. It will not guard against the concussive impact of an explosion, however."},
	{"invisibility",		itemColors[8], "",	15,	400,	0,{0,0,0}, false, false, "Drinking this potion will render you temporarily invisible. Enemies more than two spaces away will be unable to track you."},
	{"caustic gas",         itemColors[9], "",	15,	200,	0,{0,0,0}, false, false, "Uncorking or shattering this pressurized glass will cause its contents to explode into a deadly cloud of caustic purple gas. You might choose to fling this potion at distant enemies instead of uncorking it by hand."},
	{"paralysis",			itemColors[10], "",	10, 250,	0,{0,0,0}, false, false, "Upon exposure to open air, the liquid in this flask will vaporize into a numbing pink haze. Anyone who inhales the cloud will be paralyzed instantly, unable to move for some time after the cloud dissipates. This item can be thrown at distant enemies to catch them within the effect of the gas."},
	{"hallucination",		itemColors[11], "",	10,	500,	0,{0,0,0}, false, false, "This flask contains a vicious and long-lasting hallucinogen. Under its dazzling effect, you will wander through a rainbow wonderland, unable to discern the form of any creatures or items you see."},
	{"confusion",			itemColors[12], "",	15,	450,	0,{0,0,0}, false, false, "This unstable chemical will quickly vaporize into a glittering cloud upon contact with open air, causing any creature that inhales it to lose control of the direction of its movements until the effect wears off (although its ability to aim projectile attacks will not be affected). Its vertiginous intoxication can cause creatures and adventurers to careen into one another or into chasms or lava pits, so extreme care should be taken when under its effect. Its contents can be weaponized by throwing the flask at distant enemies."},
	{"incineration",		itemColors[13], "",	15,	500,	0,{0,0,0}, false, false, "This flask contains an unstable compound which will burst violently into flame upon exposure to open air. You might throw the flask at distant enemies -- or into a deep lake, to cleanse the cavern with scalding steam."},
	{"darkness",			itemColors[14], "",	7,	150,	0,{0,0,0}, false, false, "Drinking this potion will plunge you into darkness. At first, you will be completely blind to anything not illuminated by an independent light source, but over time your vision will regain its former strength. Throwing the potion will create a cloud of supernatural darkness, and enemies will have difficulty seeing or following you if you take refuge under its cover."},
	{"descent",				itemColors[15], "",	15,	500,	0,{0,0,0}, false, false, "When this flask is uncorked by hand or shattered by being thrown, the fog that seeps out will temporarily cause the ground in the vicinity to vanish."},
	{"creeping death",		itemColors[16], "",	7,	450,	0,{0,0,0}, false, false, "When the cork is popped or the flask is thrown, tiny spores will spill across the ground and begin to grow a deadly lichen. Anything that touches the lichen will be poisoned by its clinging tendrils, and the lichen will slowly grow to fill the area. Fire will purge the infestation."},
};

itemTable wandTable[NUMBER_WAND_KINDS] = {
	{"teleportation",	itemMetals[0], "",	3,	800,	BOLT_TELEPORT,      {3,5,1}, false, false, "This wand will teleport a creature to a random place on the level. Aquatic or mud-bound creatures will be rendered helpless on dry land."},
	{"slowness",		itemMetals[1], "",	3,	800,	BOLT_SLOW,          {2,5,1}, false, false, "This wand will cause a creature to move at half its ordinary speed for 30 turns."},
	{"polymorphism",	itemMetals[2], "",	3,	700,	BOLT_POLYMORPH,     {3,5,1}, false, false, "This mischievous magic will transform a creature into another creature at random. Beware: the tamest of creatures might turn into the most fearsome. The horror of the transformation will turn an allied victim against you."},
	{"negation",		itemMetals[3], "",	3,	550,	BOLT_NEGATION,      {4,6,1}, false, false, "This powerful anti-magic will strip a creature of a host of magical traits, including flight, invisibility, acidic corrosiveness, telepathy, magical speed or slowness, hypnosis, magical fear, immunity to physical attack, fire resistance and the ability to blink. Spellcasters will lose their magical abilities and magical totems will be rendered inert. Creatures animated purely by magic will die."},
	{"domination",		itemMetals[4], "",	1,	1000,	BOLT_DOMINATION,    {1,2,1}, false, false, "This wand can forever bind an enemy to the caster's will, turning it into a steadfast ally. However, the magic works only against enemies that are near death."},
	{"beckoning",		itemMetals[5], "",	3,	500,	BOLT_BECKONING,     {2,4,1}, false, false, "The force of this wand will draw the targeted creature into direct proximity."},
	{"plenty",			itemMetals[6], "",	2,	700,	BOLT_PLENTY,        {1,2,1}, false, false, "The creature at the other end of this mischievous bit of cloning magic, friend or foe, will be beside itself -- literally!"},
	{"invisibility",	itemMetals[7], "",	3,	100,	BOLT_INVISIBILITY,  {3,5,1}, false, false, "This wand will render a creature temporarily invisible to the naked eye. Only with telepathy or in the silhouette of a thick gas will an observer discern the creature's hazy outline."},
	{"empowerment",     itemMetals[8], "",	1,	100,	BOLT_EMPOWERMENT,   {1,1,1}, false, false, "This sacred magic will permanently improve the mind and body of any monster it hits. A wise adventurer will use it on allies, making them stronger in combat and able to learn a new talent from a fallen foe. If the bolt is reflected back at you, it will have no effect."},
};

itemTable staffTable[NUMBER_STAFF_KINDS] = {
	{"lightning",		itemWoods[0], "",	15,	1300,	BOLT_LIGHTNING,     {2,4,1}, false, false, "This staff conjures forth deadly arcs of electricity to damage to any number of creatures in a straight line."},
	{"firebolt",		itemWoods[1], "",	15,	1300,	BOLT_FIRE,          {2,4,1}, false, false, "This staff unleashes bursts of magical fire. It will ignite flammable terrain and burn any creature that it hits. Creatures with an immunity to fire will be unaffected by the bolt."},
	{"poison",			itemWoods[3], "",	10,	1200,	BOLT_POISON,        {2,4,1}, false, false, "The vile blast of this twisted staff will imbue its target with a deadly venom. Each turn, a creature that is poisoned will suffer one point of damage per dose of poison it has received, and poisoned creatures will not regenerate lost health until the poison clears."},
	{"tunneling",		itemWoods[4], "",	10,	1000,	BOLT_TUNNELING,     {2,4,1}, false, false, "Bursts of magic from this staff will pass harmlessly through creatures but will reduce obstructions to rubble."},
	{"blinking",		itemWoods[5], "",	11,	1200,	BOLT_BLINKING,      {2,4,1}, false, false, "This staff will allow you to teleport in the chosen direction. Creatures and inanimate obstructions will block the teleportation."},
	{"entrancement",	itemWoods[6], "",	6,	1000,	BOLT_ENTRANCEMENT,  {2,4,1}, false, false, "This staff will send creatures into a temporary trance, causing them to mindlessly mirror your movements. You can use the effect to cause one creature to attack another or to step into hazardous terrain, but the spell will be broken if you attack the creature under the effect."},
	{"obstruction",		itemWoods[7], "",	10,	1000,	BOLT_OBSTRUCTION,   {2,4,1}, false, false, "This staff will conjure a mass of impenetrable green crystal, preventing anything from moving through the affected area and temporarily entombing anything that is already there. The crystal will dissolve into the air as time passes. Higher level staffs will create larger obstructions."},
	{"discord",			itemWoods[8], "",	10,	1000,	BOLT_DISCORD,       {2,4,1}, false, false, "This staff will alter the perception of a creature and cause it to lash out indiscriminately. Strangers and allies alike will turn on the victim."},
	{"conjuration",		itemWoods[9], "",	8,	1000,	BOLT_CONJURATION,   {2,4,1}, false, false, "A flick of this staff will summon a number of phantom blades to fight on your behalf."},
	{"healing",			itemWoods[10], "",	5,	1100,	BOLT_HEALING,       {2,4,1}, false, false, "This staff will heal any creature, friend or foe. Unfortunately, you cannot use this or any staff on yourself except by reflecting the bolt."},
	{"haste",			itemWoods[11], "",	5,	900,	BOLT_HASTE,         {2,4,1}, false, false, "This staff will temporarily double the speed of any creature, friend or foe. Unfortunately, you cannot use this or any staff on yourself except by reflecting the bolt."},
	{"protection",		itemWoods[12], "",	5,	900,	BOLT_SHIELDING,     {2,4,1}, false, false, "This staff will bathe a creature in a protective light that will absorb all damage until it is depleted. Unfortunately, you cannot use this or any staff on yourself except by reflecting the bolt."},
};

itemTable ringTable[NUMBER_RING_KINDS] = {
	{"clairvoyance",	itemGems[0], "",	1,	900,	0,{1,3,1}, false, false, "This ring of eldritch scrying will permit you to see through nearby walls and doors, within a radius determined by the level of the ring. A cursed ring of clairvoyance will blind you to your immediate surroundings."},
	{"stealth",			itemGems[1], "",	1,	800,	0,{1,3,1}, false, false, "This ring of silent passage will reduce your stealth range, making enemies less likely to notice you and more likely to lose your trail. Staying motionless and lurking in the shadows will make you even harder to spot. Cursed rings of stealth will increase your stealth range, making you easier to spot and to track."},
	{"regeneration",	itemGems[2], "",	1,	750,	0,{1,3,1}, false, false, "This ring of sacred life will allow you to recover lost health at an accelerated rate. Cursed rings will decrease or even halt your natural regeneration."},
	{"transference",	itemGems[3], "",	1,	750,	0,{1,3,1}, false, false, "This ring of blood magic will heal you in proportion to the damage you inflict on others. Cursed rings will cause you to lose health when inflicting damage."},
	{"light",			itemGems[4], "",	1,	600,	0,{1,3,1}, false, false, "This ring of preternatural vision will allow you to see farther in the dimming light of the deeper dungeon levels. It will not make you more noticeable to enemies."},
	{"awareness",		itemGems[5], "",	1,	700,	0,{1,3,1}, false, false, "This ring of effortless vigilance will enable you to notice hidden secrets (traps, secret doors and hidden levers) more often and from a greater distance. Cursed rings of awareness will dull your senses, making it harder to notice secrets without actively searching for them."},
	{"wisdom",			itemGems[6], "",	1,	700,	0,{1,3,1}, false, false, "This ring of arcane power will cause your staffs to recharge at an accelerated rate. Cursed rings of wisdom will cause your staffs to recharge more slowly."},
	{"reaping",         itemGems[7], "",	1,	700,	0,{1,3,1}, false, false, "This ring of blood magic will recharge your staffs and charms every time you hit an enemy. Cursed rings of reaping will drain your staffs and charms with every hit."},
};

itemTable charmTable[NUMBER_CHARM_KINDS] = {
	{"health",          "", "",	5,	900,	0,{1,2,1}, true, false, "A handful of dried bloodwort and mandrake root has been bound together with leather cord and imbued with a powerful healing magic."},
	{"protection",		"", "",	5,	800,	0,{1,2,1}, true, false, "Four copper rings have been joined into a tetrahedron. The construct is oddly warm to the touch."},
	{"haste",           "", "",	5,	750,	0,{1,2,1}, true, false, "Various animals have been etched into the surface of this brass bangle. It emits a barely audible hum."},
	{"fire immunity",	"", "",	3,	750,	0,{1,2,1}, true, false, "Eldritch flames flicker within this polished crystal bauble."},
	{"invisibility",	"", "",	5,	700,	0,{1,2,1}, true, false, "A jade figurine depicts a strange humanoid creature. It has a face on both sides of its head, but all four eyes are closed."},
	{"telepathy",		"", "",	3,	700,	0,{1,2,1}, true, false, "Seven tiny glass eyes roll freely within this glass sphere. Somehow, they always come to rest facing outward."},
	{"levitation",      "", "",	1,	700,	0,{1,2,1}, true, false, "Sparkling dust and fragments of feather waft and swirl endlessly inside this small glass sphere."},
	{"shattering",      "", "",	1,	700,	0,{1,2,1}, true, false, "This turquoise crystal, fixed to a leather lanyard, hums with an arcane energy that sets your teeth on edge."},
	{"guardian",        "", "",	5,	700,	0,{1,2,1}, true, false, "When you touch this tiny granite statue, a rhythmic booming echoes in your mind."},
	//    {"fear",            "", "",	3,	700,	0,{1,2,1}, true, false, "When you gaze into the murky interior of this obsidian cube, you feel as though something predatory is watching you."},
		{"teleportation",   "", "",	4,	700,	0,{1,2,1}, true, false, "The surface of this nickel sphere has been etched with a perfect grid pattern. Somehow, the squares of the grid are all exactly the same size."},
		{"recharging",      "", "",	5,	700,	0,{1,2,1}, true, false, "A strip of bronze has been wound around a rough wooden sphere. Each time you touch it, you feel a tiny electric shock."},
		{"negation",        "", "",	5,	700,	0,{1,2,1}, true, false, "A featureless gray disc hangs from a lanyard. When you touch it, your hand and arm go numb."},
};
