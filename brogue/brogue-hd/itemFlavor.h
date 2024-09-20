#pragma once

#include "broguedef.h"

namespace brogueHd::backend::model::item
{
	char itemTitles[NUMBER_SCROLL_KINDS][30];

	const char titlePhonemes[NUMBER_TITLE_PHONEMES][30] = {
		"glorp",
		"snarg",
		"gana",
		"flin",
		"herba",
		"pora",
		"nuglo",
		"greep",
		"nur",
		"lofa",
		"poder",
		"nidge",
		"pus",
		"wooz",
		"flem",
		"bloto",
		"porta",
		"ermah",
		"gerd",
		"nurt",
		"flurx",
	};

	char itemColors[NUMBER_ITEM_COLORS][30];

	const char itemColorsRef[NUMBER_ITEM_COLORS][30] = {
		"crimson",
		"scarlet",
		"orange",
		"yellow",
		"green",
		"blue",
		"indigo",
		"violet",
		"puce",
		"mauve",
		"burgundy",
		"turquoise",
		"aquamarine",
		"gray",
		"pink",
		"white",
		"lavender",
		"tan",
		"brown",
		"cyan",
		"black"
	};

	char itemWoods[NUMBER_ITEM_WOODS][30];

	const char itemWoodsRef[NUMBER_ITEM_WOODS][30] = {
		"teak",
		"oak",
		"redwood",
		"rowan",
		"willow",
		"mahogany",
		"pinewood",
		"maple",
		"bamboo",
		"ironwood",
		"pearwood",
		"birch",
		"cherry",
		"eucalyptus",
		"walnut",
		"cedar",
		"rosewood",
		"yew",
		"sandalwood",
		"hickory",
		"hemlock",
	};

	char itemMetals[NUMBER_ITEM_METALS][30];

	const char itemMetalsRef[NUMBER_ITEM_METALS][30] = {
		"bronze",
		"steel",
		"brass",
		"pewter",
		"nickel",
		"copper",
		"aluminum",
		"tungsten",
		"titanium",
		"cobalt",
		"chromium",
		"silver",
	};

	char itemGems[NUMBER_ITEM_GEMS][30];

	const char itemGemsRef[NUMBER_ITEM_GEMS][30] = {
		"diamond",
		"opal",
		"garnet",
		"ruby",
		"amethyst",
		"topaz",
		"onyx",
		"tourmaline",
		"sapphire",
		"obsidian",
		"malachite",
		"aquamarine",
		"emerald",
		"jade",
		"alexandrite",
		"agate",
		"bloodstone",
		"jasper"
	};
}

