#pragma once

#include "simple.h"
#include "simpleEnumString.h"
#include "simpleArray.h"
#include "simpleString.h"
#include "simpleList.h"
#include "simplePoint.h"

#include "brogueObject.h"
#include "brogueCellDisplay.h"
#include "gridLocator.h"
#include "graphNode.h"
#include "graphEdge.h"
#include "gridLocatorEdge.h"

#include "colorConstants.h"
#include "creatureBehaviorConstants.h"
#include "feat.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model;
using namespace brogueHd::backend::model::game;
using namespace brogueHd::backend::model::layout;
using namespace brogueHd::backend::modelConstant;

//#define SINGLE_ARG(...)		__VA_ARGS__

//MAKE_HASHABLE_CLASS(brogueHd::simple::simpleArray<short>);
//MAKE_HASHABLE_CLASS(brogueHd::simple::simpleArray<int>);
MAKE_HASHABLE_CLASS(brogueHd::simple::simpleArray<char>);
//MAKE_HASHABLE_CLASS(brogueHd::simple::simpleArray<float>);
//MAKE_HASHABLE_CLASS(brogueHd::simple::simpleArray<double>);
MAKE_HASHABLE_CLASS(brogueHd::simple::simpleArray<brogueHd::simple::simpleString>);
MAKE_HASHABLE_CLASS(brogueHd::simple::simpleList<brogueHd::simple::simpleString>);
//MAKE_HASHABLE_STRUCT(SINGLE_ARG(brogueHd::simple::simplePair<brogueHd::simple::simpleString,brogueHd::simple::simpleString>));
MAKE_HASHABLE_CLASS(brogueHd::simple::simpleString);
MAKE_HASHABLE_CLASS(brogueHd::backend::model::brogueObject)
MAKE_HASHABLE_STRUCT(brogueHd::backend::model::layout::brogueCellDisplay)
MAKE_HASHABLE_STRUCT(brogueHd::backend::model::game::color)
MAKE_HASHABLE_STRUCT(brogueHd::simple::simplePoint<short>)
MAKE_HASHABLE_STRUCT(brogueHd::component::graphEdge<gridLocator>)
MAKE_HASHABLE_STRUCT(brogueHd::component::graphNode)
MAKE_HASHABLE_STRUCT(brogueHd::component::gridLocator)
MAKE_HASHABLE_STRUCT(brogueHd::component::gridLocatorEdge)

// Macro Enum Definitions:  Used for string <-> value methods EnumString<...>::To and EnumString<...>::From

#pragma region colorConstants.h

//Begin_Enum_String(colorCollections)
//{
//	Enum_String(game);
//	Enum_String(bolt);
//	Enum_String(tile);
//	Enum_String(creature);
//	Enum_String(light);
//	Enum_String(flare);
//	Enum_String(multipliers);
//	Enum_String(blood);
//	Enum_String(gas);
//	Enum_String(bInterface);
//}
//End_Enum_String;

Begin_Enum_String(gameColors)
{
	Enum_String(white);
	Enum_String(gray);
	Enum_String(darkGray);
	Enum_String(veryDarkGray);
	Enum_String(black);
	Enum_String(yellow);
	Enum_String(darkYellow);
	Enum_String(teal);
	Enum_String(purple);
	Enum_String(darkPurple);
	Enum_String(brown);
	Enum_String(green);
	Enum_String(darkGreen);
	Enum_String(orange);
	Enum_String(darkOrange);
	Enum_String(blue);
	Enum_String(darkBlue);
	Enum_String(darkTurquoise);
	Enum_String(lightBlue);
	Enum_String(pink);
	Enum_String(red);
	Enum_String(darkRed);
	Enum_String(tanColor);
}
End_Enum_String;

Begin_Enum_String(boltColors)
{
	Enum_String(rainbow);
	Enum_String(descentBoltColor);
	Enum_String(discordColor);
	Enum_String(poisonColor);
	Enum_String(beckonColor);
	Enum_String(invulnerabilityColor);
	Enum_String(dominationColor);
	Enum_String(empowermentColor);
	Enum_String(fireBoltColor);
	Enum_String(yendorLightColor);
	Enum_String(dragonFireColor);
	Enum_String(flamedancerCoronaColor);
	Enum_String(shieldingColor);
}
End_Enum_String;

Begin_Enum_String(tileColors)
{
	Enum_String(undiscoveredColor);
	Enum_String(wallForeColor);
	Enum_String(wallBackColorStart);
	Enum_String(wallBackColorEnd);

	Enum_String(mudWallForeColor);
	Enum_String(mudWallForeColor);
	Enum_String(mudWallBackColor);
	Enum_String(graniteBackColor);
	Enum_String(floorForeColor);

	Enum_String(floorBackColorStart);
	Enum_String(floorBackColorEnd);

	Enum_String(stairsBackColor);
	Enum_String(firstStairsBackColor);

	Enum_String(refuseBackColor);
	Enum_String(rubbleBackColor);
	Enum_String(bloodflowerForeColor);
	Enum_String(bloodflowerPodForeColor);
	Enum_String(bloodflowerBackColor);
	Enum_String(bedrollBackColor);

	Enum_String(obsidianBackColor);
	Enum_String(carpetForeColor);
	Enum_String(carpetBackColor);
	Enum_String(marbleForeColor);
	Enum_String(marbleBackColor);
	Enum_String(doorForeColor);
	Enum_String(doorBackColor);
	Enum_String(ironDoorForeColor);
	Enum_String(ironDoorBackColor);
	Enum_String(bridgeFrontColor);
	Enum_String(bridgeBackColor);
	Enum_String(statueBackColor);
	Enum_String(glyphColor);
	Enum_String(glyphLightColor);
	Enum_String(sacredGlyphColor);
	Enum_String(sacredGlyphLightColor);

	Enum_String(deepWaterForeColor);
	Enum_String(deepWaterBackColorStart);
	Enum_String(deepWaterBackColorEnd);
	Enum_String(shallowWaterForeColor);
	Enum_String(shallowWaterBackColorStart);
	Enum_String(shallowWaterBackColorEnd);

	Enum_String(mudForeColor);
	Enum_String(mudBackColor);
	Enum_String(chasmForeColor);
	Enum_String(chasmEdgeBackColorStart);
	Enum_String(chasmEdgeBackColorEnd);
	Enum_String(fireForeColor);
	Enum_String(lavaForeColor);
	Enum_String(brimstoneForeColor);
	Enum_String(brimstoneBackColor);

	Enum_String(lavaBackColor);
	Enum_String(acidBackColor);

	Enum_String(lightningColor);
	Enum_String(fungusLightColor);
	Enum_String(lavaLightColor);
	Enum_String(deepWaterLightColor);

	Enum_String(grassColor);
	Enum_String(deadGrassColor);
	Enum_String(fungusColor);
	Enum_String(grayFungusColor);
	Enum_String(foliageColor);
	Enum_String(deadFoliageColor);
	Enum_String(lichenColor);
	Enum_String(hayColor);
	Enum_String(ashForeColor);
	Enum_String(bonesForeColor);
	Enum_String(ectoplasmColor);
	Enum_String(forceFieldColor);
	Enum_String(wallCrystalColor);
	Enum_String(altarForeColor);
	Enum_String(altarBackColor);
	Enum_String(greenAltarBackColor);
	Enum_String(goldAltarBackColor);
	Enum_String(pedestalBackColor);
}
End_Enum_String;

Begin_Enum_String(creatureColors)
{
	Enum_String(goblinColor);
	Enum_String(jackalColor);
	Enum_String(ogreColor);
	Enum_String(eelColor);
	Enum_String(goblinConjurerColor);
	Enum_String(spectralBladeColor);
	Enum_String(spectralImageColor);
	Enum_String(toadColor);
	Enum_String(trollColor);
	Enum_String(centipedeColor);
	Enum_String(dragonColor);
	Enum_String(krakenColor);
	Enum_String(salamanderColor);
	Enum_String(pixieColor);
	Enum_String(darPriestessColor);
	Enum_String(darMageColor);
	Enum_String(wraithColor);
	Enum_String(pinkJellyColor);
	Enum_String(wormColor);
	Enum_String(sentinelColor);
	Enum_String(goblinMysticColor);
	Enum_String(ifritColor);
	Enum_String(phoenixColor);
}
End_Enum_String;

Begin_Enum_String(lightColors)
{
	Enum_String(minersLightStartColor);
	Enum_String(minersLightEndColor);
	Enum_String(torchColor);
	Enum_String(torchLightColor);
	Enum_String(hauntedTorchColor);
	Enum_String(hauntedTorchLightColor);
	Enum_String(ifritLightColor);
	Enum_String(unicornLightColor);
	Enum_String(wispLightColor);
	Enum_String(summonedImageLightColor);
	Enum_String(spectralBladeLightColor);
	Enum_String(ectoplasmLightColor);
	Enum_String(explosionColor);
	Enum_String(explosiveAuraColor);
	Enum_String(sacrificeTargetColor);
	Enum_String(dartFlashColor);
	Enum_String(lichLightColor);
	Enum_String(forceFieldLightColor);
	Enum_String(crystalWallLightColor);
	Enum_String(sunLightColor);
	Enum_String(fungusForestLightColor);
	Enum_String(fungusTrampledLightColor);
	Enum_String(redFlashColor);
	Enum_String(darknessPatchColor);
	Enum_String(darknessCloudColor);
	Enum_String(magicMapFlashColor);
	Enum_String(sentinelLightColor);
	Enum_String(telepathyColor);
	Enum_String(confusionLightColor);
	Enum_String(portalActivateLightColor);
	Enum_String(descentLightColor);
	Enum_String(algaeBlueLightColor);
	Enum_String(algaeGreenLightColor);
}
End_Enum_String;

Begin_Enum_String(flareColors)
{
	Enum_String(scrollProtectionColor);
	Enum_String(scrollEnchantmentColor);
	Enum_String(potionStrengthColor);
	Enum_String(empowermentFlashColor);
	Enum_String(genericFlashColor);
	Enum_String(summoningFlashColor);
	Enum_String(fireFlashColor);
	Enum_String(explosionFlareColor);
	Enum_String(quietusFlashColor);
	Enum_String(slayingFlashColor);
}
End_Enum_String;

Begin_Enum_String(colorMultipliers)
{
	Enum_String(colorDim25);
	Enum_String(colorMultiplier100);
	Enum_String(memoryColor);
	Enum_String(memoryOverlay);
	Enum_String(magicMapColor);
	Enum_String(clairvoyanceColor);
	Enum_String(telepathyMultiplier);
	Enum_String(omniscienceColor);
	Enum_String(basicLightColor);
}
End_Enum_String;

Begin_Enum_String(bloodColors)
{
	Enum_String(humanBloodColor);
	Enum_String(insectBloodColor);
	Enum_String(vomitColor);
	Enum_String(urineColor);
	Enum_String(methaneColor);
}
End_Enum_String;

Begin_Enum_String(gasColors)
{
	Enum_String(poisonGasColor);
	Enum_String(confusionGasColor);
}
End_Enum_String;

Begin_Enum_String(interfaceColors)
{
	Enum_String(itemColor);
	Enum_String(blueBar);
	Enum_String(redBar);
	Enum_String(hiliteColor);
	Enum_String(interfaceBoxColor);
	Enum_String(interfaceButtonColor);
	Enum_String(buttonHoverColor);
	Enum_String(titleButtonColor);

	Enum_String(playerInvisibleColor);
	Enum_String(playerInLightColor);
	Enum_String(playerInShadowColor);
	Enum_String(playerInDarknessColor);

	Enum_String(inLightMultiplierColor);
	Enum_String(inDarknessMultiplierColor);

	Enum_String(goodMessageColor);
	Enum_String(badMessageColor);
	Enum_String(advancementMessageColor);
	Enum_String(itemMessageColor);
	Enum_String(flavorTextColor);
	Enum_String(backgroundMessageColor);

	Enum_String(superVictoryColor);

	Enum_String(flameSourceColor);
	Enum_String(flameSourceColorSecondary);

	Enum_String(flameTitleColor);
}
End_Enum_String;

#pragma endregion colorConstants.h

#pragma region creatureBehaviorConstants.h

Begin_Enum_String(creatureAbilityFlags)
{
	Enum_String_With_Description(MA_HIT_HALLUCINATE, "can induce hallucinations");
	Enum_String_With_Description(MA_HIT_STEAL_FLEE, "can steal items");
	Enum_String_With_Description(MA_HIT_BURN, "lights enemies on fire when $HESHE hits");
	Enum_String_With_Description(MA_ENTER_SUMMONS, "can possess $HISHER summoned allies");
	Enum_String_With_Description(MA_HIT_DEGRADE_ARMOR, "corrodes armor when $HESHE hits");
	Enum_String_With_Description(MA_CAST_SUMMON, "can summon allies");
	Enum_String_With_Description(MA_SEIZES, "immobilizes $HISHER prey");
	Enum_String_With_Description(MA_POISONS, "injects poison when $HESHE hits");
	Enum_String_With_Description(MA_DF_ON_DEATH, "");
	Enum_String_With_Description(MA_CLONE_SELF_ON_DEFEND, "divides in two when struck");
	Enum_String_With_Description(MA_KAMIKAZE, "dies when $HESHE attacks");
	Enum_String_With_Description(MA_TRANSFERENCE, "recovers health when $HESHE inflicts damage");
	Enum_String_With_Description(MA_CAUSES_WEAKNESS, "saps strength when $HESHE inflicts damage");

	Enum_String_With_Description(MA_ATTACKS_PENETRATE, "attacks up to two opponents in a line");
	Enum_String_With_Description(MA_ATTACKS_ALL_ADJACENT, "attacks all adjacent opponents at once");
	Enum_String_With_Description(MA_ATTACKS_EXTEND, "attacks with a whip");
	Enum_String_With_Description(MA_ATTACKS_STAGGER, "pushes opponents backward when $HESHE hits");
	Enum_String_With_Description(MA_AVOID_CORRIDORS, "avoids attacking in corridors in a group");
}
End_Enum_String;

Begin_Enum_String(creatureBehaviorFlags)
{
	Enum_String_With_Description(MONST_INVISIBLE, "is invisible");
	Enum_String_With_Description(MONST_INANIMATE, "is an inanimate object");
	Enum_String_With_Description(MONST_IMMOBILE, "cannot move");
	Enum_String_With_Description(MONST_CARRY_ITEM_100, "");
	Enum_String_With_Description(MONST_CARRY_ITEM_25, "");
	Enum_String_With_Description(MONST_ALWAYS_HUNTING, "");
	Enum_String_With_Description(MONST_FLEES_NEAR_DEATH, "flees at low health");
	Enum_String_With_Description(MONST_ATTACKABLE_THRU_WALLS, "");
	Enum_String_With_Description(MONST_DEFEND_DEGRADE_WEAPON, "corrodes weapons when hit");
	Enum_String_With_Description(MONST_IMMUNE_TO_WEAPONS, "is immune to weapon damage");
	Enum_String_With_Description(MONST_FLIES, "flies");
	Enum_String_With_Description(MONST_FLITS, "moves erratically");
	Enum_String_With_Description(MONST_IMMUNE_TO_FIRE, "is immune to fire");
	Enum_String_With_Description(MONST_CAST_SPELLS_SLOWLY, "");
	Enum_String_With_Description(MONST_IMMUNE_TO_WEBS, "cannot be entangled");
	Enum_String_With_Description(MONST_REFLECT_4, "can reflect magic spells");
	Enum_String_With_Description(MONST_NEVER_SLEEPS, "never sleeps");
	Enum_String_With_Description(MONST_FIERY, "burns unceasingly");
	Enum_String_With_Description(MONST_INVULNERABLE, "is invulnerable");
	Enum_String_With_Description(MONST_IMMUNE_TO_WATER, "is at home in water");
	Enum_String_With_Description(MONST_RESTRICTED_TO_LIQUID, "cannot venture onto dry land");
	Enum_String_With_Description(MONST_SUBMERGES, "submerges");
	Enum_String_With_Description(MONST_MAINTAINS_DISTANCE, "keeps $HISHER distance");
	Enum_String_With_Description(MONST_WILL_NOT_USE_STAIRS, "");
	Enum_String_With_Description(MONST_DIES_IF_NEGATED, "is animated purely by magic");
	Enum_String_With_Description(MONST_MALE, "");
	Enum_String_With_Description(MONST_FEMALE, "");
	Enum_String_With_Description(MONST_NOT_LISTED_IN_SIDEBAR, "");
	Enum_String_With_Description(MONST_GETS_TURN_ON_ACTIVATION, "moves only when activated");
}
End_Enum_String;

Begin_Enum_String(creatureBookkeepingFlags)
{
	Enum_String_With_Description(MB_WAS_VISIBLE, "");
	Enum_String_With_Description(MB_TELEPATHICALLY_REVEALED, "is telepathically bonded with you");
	Enum_String_With_Description(MB_PREPLACED, "");
	Enum_String_With_Description(MB_APPROACHING_UPSTAIRS, "");
	Enum_String_With_Description(MB_APPROACHING_DOWNSTAIRS, "");
	Enum_String_With_Description(MB_APPROACHING_PIT, "");
	Enum_String_With_Description(MB_LEADER, "");
	Enum_String_With_Description(MB_FOLLOWER, "");
	Enum_String_With_Description(MB_CAPTIVE, "");
	Enum_String_With_Description(MB_SEIZED, "has been immobilized");
	Enum_String_With_Description(MB_SEIZING, "is currently holding $HISHER prey immobile");
	Enum_String_With_Description(MB_SUBMERGED, "is submerged");
	Enum_String_With_Description(MB_JUST_SUMMONED, "");
	Enum_String_With_Description(MB_WILL_FLASH, "");
	Enum_String_With_Description(MB_BOUND_TO_LEADER, "is anchored to reality by $HISHER summoner");
	Enum_String_With_Description(MB_MARKED_FOR_SACRIFICE, "is marked for demonic sacrifice");
}
End_Enum_String;

#pragma endregion creatureBehaviorConstants.h

#pragma region feat.h

Begin_Enum_String(featTypes)
{
	Enum_String(FEAT_PURE_MAGE);
	Enum_String(FEAT_PURE_WARRIOR);
	Enum_String(FEAT_PACIFIST);
	Enum_String(FEAT_ARCHIVIST);
	Enum_String(FEAT_COMPANION);
	Enum_String(FEAT_SPECIALIST);
	Enum_String(FEAT_JELLYMANCER);
	Enum_String(FEAT_INDOMITABLE);
	Enum_String(FEAT_MYSTIC);
	Enum_String(FEAT_DRAGONSLAYER);
	Enum_String(FEAT_PALADIN);
}
End_Enum_String;

#pragma endregion feat.h