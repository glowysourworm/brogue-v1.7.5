#pragma once



// Macro Enum Definitions:  Used for string <-> value methods EnumString<...>::To and EnumString<...>::From

//Register_Enum(colorCollections)
//	Declare_Enum_Name(colorCollections)
//		Enum_Name(colorCollections, game)
//		Enum_Name(colorCollections, bolt)
//		Enum_Name(colorCollections, tile)
//		Enum_Name(colorCollections, creature)
//		Enum_Name(colorCollections, light)
//		Enum_Name(colorCollections, flare)
//		Enum_Name(colorCollections, multipliers)
//		Enum_Name(colorCollections, blood)
//		Enum_Name(colorCollections, gas)
//		Enum_Name(colorCollections, bInterface)
//	End_Enum_Name;
//End_Enum;
//
//Register_Enum(gameColors)
//	Declare_Enum_Name(gameColors)
//		Enum_Name(gameColors, white)
//		Enum_Name(gameColors, gray)
//		Enum_Name(gameColors, darkGray)
//		Enum_Name(gameColors, veryDarkGray)
//		Enum_Name(gameColors, black)
//		Enum_Name(gameColors, yellow)
//		Enum_Name(gameColors, darkYellow)
//		Enum_Name(gameColors, teal)
//		Enum_Name(gameColors, purple)
//		Enum_Name(gameColors, darkPurple)
//		Enum_Name(gameColors, brown)
//		Enum_Name(gameColors, green)
//		Enum_Name(gameColors, darkGreen)
//		Enum_Name(gameColors, orange)
//		Enum_Name(gameColors, darkOrange)
//		Enum_Name(gameColors, blue)
//		Enum_Name(gameColors, darkBlue)
//		Enum_Name(gameColors, darkTurquoise)
//		Enum_Name(gameColors, lightBlue)
//		Enum_Name(gameColors, pink)
//		Enum_Name(gameColors, red)
//		Enum_Name(gameColors, darkRed)
//		Enum_Name(gameColors, tanColor)
//	End_Enum_Name;
//End_Enum;
//
//Register_Enum(boltColors)
//	Declare_Enum_Name(boltColors)
//		Enum_Name(boltColors, rainbow)
//		Enum_Name(boltColors, descentBoltColor)
//		Enum_Name(boltColors, discordColor)
//		Enum_Name(boltColors, poisonColor)
//		Enum_Name(boltColors, beckonColor)
//		Enum_Name(boltColors, invulnerabilityColor)
//		Enum_Name(boltColors, dominationColor)
//		Enum_Name(boltColors, empowermentColor)
//		Enum_Name(boltColors, fireBoltColor)
//		Enum_Name(boltColors, yendorLightColor)
//		Enum_Name(boltColors, dragonFireColor)
//		Enum_Name(boltColors, flamedancerCoronaColor)
//		Enum_Name(boltColors, shieldingColor)
//	End_Enum_Name;
//End_Enum;
//
//Register_Enum(tileColors)
//	Declare_Enum_Name(tileColors)
//		Enum_Name(tileColors, undiscoveredColor)
//		Enum_Name(tileColors, wallForeColor)
//		Enum_Name(tileColors, wallBackColorStart)
//		Enum_Name(tileColors, wallBackColorEnd)
//
//		Enum_Name(tileColors, mudWallForeColor)
//		Enum_Name(tileColors, mudWallBackColor)
//		Enum_Name(tileColors, graniteBackColor)
//		Enum_Name(tileColors, floorForeColor)
//
//		Enum_Name(tileColors, floorBackColorStart)
//		Enum_Name(tileColors, floorBackColorEnd)
//
//		Enum_Name(tileColors, stairsBackColor)
//		Enum_Name(tileColors, firstStairsBackColor)
//
//		Enum_Name(tileColors, refuseBackColor)
//		Enum_Name(tileColors, rubbleBackColor)
//		Enum_Name(tileColors, bloodflowerForeColor)
//		Enum_Name(tileColors, bloodflowerPodForeColor)
//		Enum_Name(tileColors, bloodflowerBackColor)
//		Enum_Name(tileColors, bedrollBackColor)
//
//		Enum_Name(tileColors, obsidianBackColor)
//		Enum_Name(tileColors, carpetForeColor)
//		Enum_Name(tileColors, carpetBackColor)
//		Enum_Name(tileColors, marbleForeColor)
//		Enum_Name(tileColors, marbleBackColor)
//		Enum_Name(tileColors, doorForeColor)
//		Enum_Name(tileColors, doorBackColor)
//		Enum_Name(tileColors, ironDoorForeColor)
//		Enum_Name(tileColors, ironDoorBackColor)
//		Enum_Name(tileColors, bridgeFrontColor)
//		Enum_Name(tileColors, bridgeBackColor)
//		Enum_Name(tileColors, statueBackColor)
//		Enum_Name(tileColors, glyphColor)
//		Enum_Name(tileColors, glyphLightColor)
//		Enum_Name(tileColors, sacredGlyphColor)
//		Enum_Name(tileColors, sacredGlyphLightColor)
//
//		Enum_Name(tileColors, deepWaterForeColor)
//		Enum_Name(tileColors, deepWaterBackColorStart)
//		Enum_Name(tileColors, deepWaterBackColorEnd)
//		Enum_Name(tileColors, shallowWaterForeColor)
//		Enum_Name(tileColors, shallowWaterBackColorStart)
//		Enum_Name(tileColors, shallowWaterBackColorEnd)
//
//		Enum_Name(tileColors, mudForeColor)
//		Enum_Name(tileColors, mudBackColor)
//		Enum_Name(tileColors, chasmForeColor)
//		Enum_Name(tileColors, chasmEdgeBackColorStart)
//		Enum_Name(tileColors, chasmEdgeBackColorEnd)
//		Enum_Name(tileColors, fireForeColor)
//		Enum_Name(tileColors, lavaForeColor)
//		Enum_Name(tileColors, brimstoneForeColor)
//		Enum_Name(tileColors, brimstoneBackColor)
//
//		Enum_Name(tileColors, lavaBackColor)
//		Enum_Name(tileColors, acidBackColor)
//
//		Enum_Name(tileColors, lightningColor)
//		Enum_Name(tileColors, fungusLightColor)
//		Enum_Name(tileColors, lavaLightColor)
//		Enum_Name(tileColors, deepWaterLightColor)
//
//		Enum_Name(tileColors, grassColor)
//		Enum_Name(tileColors, deadGrassColor)
//		Enum_Name(tileColors, fungusColor)
//		Enum_Name(tileColors, grayFungusColor)
//		Enum_Name(tileColors, foliageColor)
//		Enum_Name(tileColors, deadFoliageColor)
//		Enum_Name(tileColors, lichenColor)
//		Enum_Name(tileColors, hayColor)
//		Enum_Name(tileColors, ashForeColor)
//		Enum_Name(tileColors, bonesForeColor)
//		Enum_Name(tileColors, ectoplasmColor)
//		Enum_Name(tileColors, forceFieldColor)
//		Enum_Name(tileColors, wallCrystalColor)
//		Enum_Name(tileColors, altarForeColor)
//		Enum_Name(tileColors, altarBackColor)
//		Enum_Name(tileColors, greenAltarBackColor)
//		Enum_Name(tileColors, goldAltarBackColor)
//		Enum_Name(tileColors, pedestalBackColor)
//	End_Enum_Name;
//End_Enum;
//
//Register_Enum(creatureColors)
//	Declare_Enum_Name(creatureColors)
//		Enum_Name(creatureColors, goblinColor)
//		Enum_Name(creatureColors, jackalColor)
//		Enum_Name(creatureColors, ogreColor)
//		Enum_Name(creatureColors, eelColor)
//		Enum_Name(creatureColors, goblinConjurerColor)
//		Enum_Name(creatureColors, spectralBladeColor)
//		Enum_Name(creatureColors, spectralImageColor)
//		Enum_Name(creatureColors, toadColor)
//		Enum_Name(creatureColors, trollColor)
//		Enum_Name(creatureColors, centipedeColor)
//		Enum_Name(creatureColors, dragonColor)
//		Enum_Name(creatureColors, krakenColor)
//		Enum_Name(creatureColors, salamanderColor)
//		Enum_Name(creatureColors, pixieColor)
//		Enum_Name(creatureColors, darPriestessColor)
//		Enum_Name(creatureColors, darMageColor)
//		Enum_Name(creatureColors, wraithColor)
//		Enum_Name(creatureColors, pinkJellyColor)
//		Enum_Name(creatureColors, wormColor)
//		Enum_Name(creatureColors, sentinelColor)
//		Enum_Name(creatureColors, goblinMysticColor)
//		Enum_Name(creatureColors, ifritColor)
//		Enum_Name(creatureColors, phoenixColor)
//	End_Enum_Name;
//End_Enum;
//
//Register_Enum(lightColors)
//	Declare_Enum_Name(lightColors)
//		Enum_Name(lightColors, minersLightStartColor)
//		Enum_Name(lightColors, minersLightEndColor)
//		Enum_Name(lightColors, torchColor)
//		Enum_Name(lightColors, torchLightColor)
//		Enum_Name(lightColors, hauntedTorchColor)
//		Enum_Name(lightColors, hauntedTorchLightColor)
//		Enum_Name(lightColors, ifritLightColor)
//		Enum_Name(lightColors, unicornLightColor)
//		Enum_Name(lightColors, wispLightColor)
//		Enum_Name(lightColors, summonedImageLightColor)
//		Enum_Name(lightColors, spectralBladeLightColor)
//		Enum_Name(lightColors, ectoplasmLightColor)
//		Enum_Name(lightColors, explosionColor)
//		Enum_Name(lightColors, explosiveAuraColor)
//		Enum_Name(lightColors, sacrificeTargetColor)
//		Enum_Name(lightColors, dartFlashColor)
//		Enum_Name(lightColors, lichLightColor)
//		Enum_Name(lightColors, forceFieldLightColor)
//		Enum_Name(lightColors, crystalWallLightColor)
//		Enum_Name(lightColors, sunLightColor)
//		Enum_Name(lightColors, fungusForestLightColor)
//		Enum_Name(lightColors, fungusTrampledLightColor)
//		Enum_Name(lightColors, redFlashColor)
//		Enum_Name(lightColors, darknessPatchColor)
//		Enum_Name(lightColors, darknessCloudColor)
//		Enum_Name(lightColors, magicMapFlashColor)
//		Enum_Name(lightColors, sentinelLightColor)
//		Enum_Name(lightColors, telepathyColor)
//		Enum_Name(lightColors, confusionLightColor)
//		Enum_Name(lightColors, portalActivateLightColor)
//		Enum_Name(lightColors, descentLightColor)
//		Enum_Name(lightColors, algaeBlueLightColor)
//		Enum_Name(lightColors, algaeGreenLightColor)
//	End_Enum_Name;
//End_Enum;
//
//Register_Enum(flareColors)
//	Declare_Enum_Name(flareColors)
//		Enum_Name(flareColors, scrollProtectionColor)
//		Enum_Name(flareColors, scrollEnchantmentColor)
//		Enum_Name(flareColors, potionStrengthColor)
//		Enum_Name(flareColors, empowermentFlashColor)
//		Enum_Name(flareColors, genericFlashColor)
//		Enum_Name(flareColors, summoningFlashColor)
//		Enum_Name(flareColors, fireFlashColor)
//		Enum_Name(flareColors, explosionFlareColor)
//		Enum_Name(flareColors, quietusFlashColor)
//		Enum_Name(flareColors, slayingFlashColor)
//	End_Enum_Name;
//End_Enum;
//
//Register_Enum(colorMultipliers)
//	Declare_Enum_Name(colorMultipliers)
//		Enum_Name(colorMultipliers, colorDim25)
//		Enum_Name(colorMultipliers, colorMultiplier100)
//		Enum_Name(colorMultipliers, memoryColor)
//		Enum_Name(colorMultipliers, memoryOverlay)
//		Enum_Name(colorMultipliers, magicMapColor)
//		Enum_Name(colorMultipliers, clairvoyanceColor)
//		Enum_Name(colorMultipliers, telepathyMultiplier)
//		Enum_Name(colorMultipliers, omniscienceColor)
//		Enum_Name(colorMultipliers, basicLightColor)
//	End_Enum_Name;
//End_Enum;
//
//Register_Enum(bloodColors)
//	Declare_Enum_Name(bloodColors)
//		Enum_Name(bloodColors, humanBloodColor)
//		Enum_Name(bloodColors, insectBloodColor)
//		Enum_Name(bloodColors, vomitColor)
//		Enum_Name(bloodColors, urineColor)
//		Enum_Name(bloodColors, methaneColor)
//	End_Enum_Name;
//End_Enum;
//
//Register_Enum(gasColors)
//	Declare_Enum_Name(gasColors)
//		Enum_Name(gasColors, poisonGasColor)
//		Enum_Name(gasColors, confusionGasColor)
//	End_Enum_Name;
//End_Enum;
//
//Register_Enum(interfaceColors)
//	Declare_Enum_Name(interfaceColors)
//		Enum_Name(interfaceColors, itemColor)
//		Enum_Name(interfaceColors, blueBar)
//		Enum_Name(interfaceColors, redBar)
//		Enum_Name(interfaceColors, hiliteColor)
//		Enum_Name(interfaceColors, interfaceBoxColor)
//		Enum_Name(interfaceColors, interfaceButtonColor)
//		Enum_Name(interfaceColors, buttonHoverColor)
//		Enum_Name(interfaceColors, titleButtonColor)
//
//		Enum_Name(interfaceColors, playerInvisibleColor)
//		Enum_Name(interfaceColors, playerInLightColor)
//		Enum_Name(interfaceColors, playerInShadowColor)
//		Enum_Name(interfaceColors, playerInDarknessColor)
//
//		Enum_Name(interfaceColors, inLightMultiplierColor)
//		Enum_Name(interfaceColors, inDarknessMultiplierColor)
//
//		Enum_Name(interfaceColors, goodMessageColor)
//		Enum_Name(interfaceColors, badMessageColor)
//		Enum_Name(interfaceColors, advancementMessageColor)
//		Enum_Name(interfaceColors, itemMessageColor)
//		Enum_Name(interfaceColors, flavorTextColor)
//		Enum_Name(interfaceColors, backgroundMessageColor)
//
//		Enum_Name(interfaceColors, superVictoryColor)
//
//		Enum_Name(interfaceColors, flameSourceColor)
//		Enum_Name(interfaceColors, flameSourceColorSecondary)
//
//		Enum_Name(interfaceColors, flameTitleColor)
//	End_Enum_Name;
//End_Enum;
//
//#pragma endregion colorConstants.h
//
//#pragma region creatureBehaviorConstants.h
//
//Register_Enum(creatureAbilityFlags)
//	Declare_Enum_Name(creatureAbilityFlags)
//		Enum_Name(creatureAbilityFlags, MA_HIT_HALLUCINATE)
//		Enum_Name(creatureAbilityFlags, MA_HIT_STEAL_FLEE)
//		Enum_Name(creatureAbilityFlags, MA_HIT_BURN)
//		Enum_Name(creatureAbilityFlags, MA_ENTER_SUMMONS)
//		Enum_Name(creatureAbilityFlags, MA_HIT_DEGRADE_ARMOR)
//		Enum_Name(creatureAbilityFlags, MA_CAST_SUMMON)
//		Enum_Name(creatureAbilityFlags, MA_SEIZES)
//		Enum_Name(creatureAbilityFlags, MA_POISONS)
//		Enum_Name(creatureAbilityFlags, MA_DF_ON_DEATH)
//		Enum_Name(creatureAbilityFlags, MA_CLONE_SELF_ON_DEFEND)
//		Enum_Name(creatureAbilityFlags, MA_KAMIKAZE)
//		Enum_Name(creatureAbilityFlags, MA_TRANSFERENCE)
//		Enum_Name(creatureAbilityFlags, MA_CAUSES_WEAKNESS)
//
//		Enum_Name(creatureAbilityFlags, MA_ATTACKS_PENETRATE)
//		Enum_Name(creatureAbilityFlags, MA_ATTACKS_ALL_ADJACENT)
//		Enum_Name(creatureAbilityFlags, MA_ATTACKS_EXTEND)
//		Enum_Name(creatureAbilityFlags, MA_ATTACKS_STAGGER)
//		Enum_Name(creatureAbilityFlags, MA_AVOID_CORRIDORS)
//	End_Enum_Name;
//
//	Declare_Enum_Descr(creatureAbilityFlags)
//		Enum_Descr(creatureAbilityFlags, MA_HIT_HALLUCINATE, "can induce hallucinations");
//		Enum_Descr(creatureAbilityFlags, MA_HIT_STEAL_FLEE, "can steal items");
//		Enum_Descr(creatureAbilityFlags, MA_HIT_BURN, "lights enemies on fire when $HESHE hits");
//		Enum_Descr(creatureAbilityFlags, MA_ENTER_SUMMONS, "can possess $HISHER summoned allies");
//		Enum_Descr(creatureAbilityFlags, MA_HIT_DEGRADE_ARMOR, "corrodes armor when $HESHE hits");
//		Enum_Descr(creatureAbilityFlags, MA_CAST_SUMMON, "can summon allies");
//		Enum_Descr(creatureAbilityFlags, MA_SEIZES, "immobilizes $HISHER prey");
//		Enum_Descr(creatureAbilityFlags, MA_POISONS, "injects poison when $HESHE hits");
//		Enum_Descr(creatureAbilityFlags, MA_DF_ON_DEATH, "");
//		Enum_Descr(creatureAbilityFlags, MA_CLONE_SELF_ON_DEFEND, "divides in two when struck");
//		Enum_Descr(creatureAbilityFlags, MA_KAMIKAZE, "dies when $HESHE attacks");
//		Enum_Descr(creatureAbilityFlags, MA_TRANSFERENCE, "recovers health when $HESHE inflicts damage");
//		Enum_Descr(creatureAbilityFlags, MA_CAUSES_WEAKNESS, "saps strength when $HESHE inflicts damage");
//
//		Enum_Descr(creatureAbilityFlags, MA_ATTACKS_PENETRATE, "attacks up to two opponents in a line");
//		Enum_Descr(creatureAbilityFlags, MA_ATTACKS_ALL_ADJACENT, "attacks all adjacent opponents at once");
//		Enum_Descr(creatureAbilityFlags, MA_ATTACKS_EXTEND, "attacks with a whip");
//		Enum_Descr(creatureAbilityFlags, MA_ATTACKS_STAGGER, "pushes opponents backward when $HESHE hits");
//		Enum_Descr(creatureAbilityFlags, MA_AVOID_CORRIDORS, "avoids attacking in corridors in a group");
//	End_Enum_Descr;
//End_Enum;
//
//Register_Enum(creatureBehaviorFlags)
//	Declare_Enum_Name(creatureBehaviorFlags)
//		Enum_Name(creatureBehaviorFlags, MONST_INVISIBLE)
//		Enum_Name(creatureBehaviorFlags, MONST_INANIMATE)
//		Enum_Name(creatureBehaviorFlags, MONST_IMMOBILE)
//		Enum_Name(creatureBehaviorFlags, MONST_CARRY_ITEM_100)
//		Enum_Name(creatureBehaviorFlags, MONST_CARRY_ITEM_25)
//		Enum_Name(creatureBehaviorFlags, MONST_ALWAYS_HUNTING)
//		Enum_Name(creatureBehaviorFlags, MONST_FLEES_NEAR_DEATH)
//		Enum_Name(creatureBehaviorFlags, MONST_ATTACKABLE_THRU_WALLS)
//		Enum_Name(creatureBehaviorFlags, MONST_DEFEND_DEGRADE_WEAPON)
//		Enum_Name(creatureBehaviorFlags, MONST_IMMUNE_TO_WEAPONS)
//		Enum_Name(creatureBehaviorFlags, MONST_FLIES)
//		Enum_Name(creatureBehaviorFlags, MONST_FLITS)
//		Enum_Name(creatureBehaviorFlags, MONST_IMMUNE_TO_FIRE)
//		Enum_Name(creatureBehaviorFlags, MONST_CAST_SPELLS_SLOWLY)
//		Enum_Name(creatureBehaviorFlags, MONST_IMMUNE_TO_WEBS)
//		Enum_Name(creatureBehaviorFlags, MONST_REFLECT_4)
//		Enum_Name(creatureBehaviorFlags, MONST_NEVER_SLEEPS)
//		Enum_Name(creatureBehaviorFlags, MONST_FIERY)
//		Enum_Name(creatureBehaviorFlags, MONST_INVULNERABLE)
//		Enum_Name(creatureBehaviorFlags, MONST_IMMUNE_TO_WATER)
//		Enum_Name(creatureBehaviorFlags, MONST_RESTRICTED_TO_LIQUID)
//		Enum_Name(creatureBehaviorFlags, MONST_SUBMERGES)
//		Enum_Name(creatureBehaviorFlags, MONST_MAINTAINS_DISTANCE)
//		Enum_Name(creatureBehaviorFlags, MONST_WILL_NOT_USE_STAIRS)
//		Enum_Name(creatureBehaviorFlags, MONST_DIES_IF_NEGATED)
//		Enum_Name(creatureBehaviorFlags, MONST_MALE)
//		Enum_Name(creatureBehaviorFlags, MONST_FEMALE)
//		Enum_Name(creatureBehaviorFlags, MONST_NOT_LISTED_IN_SIDEBAR)
//		Enum_Name(creatureBehaviorFlags, MONST_GETS_TURN_ON_ACTIVATION)
//	End_Enum_Name;
//
//	Declare_Enum_Descr(creatureBehaviorFlags)
//		Enum_Descr(creatureBehaviorFlags, MONST_INVISIBLE, "is invisible");
//		Enum_Descr(creatureBehaviorFlags, MONST_INANIMATE, "is an inanimate object");
//		Enum_Descr(creatureBehaviorFlags, MONST_IMMOBILE, "cannot move");
//		Enum_Descr(creatureBehaviorFlags, MONST_CARRY_ITEM_100, "");
//		Enum_Descr(creatureBehaviorFlags, MONST_CARRY_ITEM_25, "");
//		Enum_Descr(creatureBehaviorFlags, MONST_ALWAYS_HUNTING, "");
//		Enum_Descr(creatureBehaviorFlags, MONST_FLEES_NEAR_DEATH, "flees at low health");
//		Enum_Descr(creatureBehaviorFlags, MONST_ATTACKABLE_THRU_WALLS, "");
//		Enum_Descr(creatureBehaviorFlags, MONST_DEFEND_DEGRADE_WEAPON, "corrodes weapons when hit");
//		Enum_Descr(creatureBehaviorFlags, MONST_IMMUNE_TO_WEAPONS, "is immune to weapon damage");
//		Enum_Descr(creatureBehaviorFlags, MONST_FLIES, "flies");
//		Enum_Descr(creatureBehaviorFlags, MONST_FLITS, "moves erratically");
//		Enum_Descr(creatureBehaviorFlags, MONST_IMMUNE_TO_FIRE, "is immune to fire");
//		Enum_Descr(creatureBehaviorFlags, MONST_CAST_SPELLS_SLOWLY, "");
//		Enum_Descr(creatureBehaviorFlags, MONST_IMMUNE_TO_WEBS, "cannot be entangled");
//		Enum_Descr(creatureBehaviorFlags, MONST_REFLECT_4, "can reflect magic spells");
//		Enum_Descr(creatureBehaviorFlags, MONST_NEVER_SLEEPS, "never sleeps");
//		Enum_Descr(creatureBehaviorFlags, MONST_FIERY, "burns unceasingly");
//		Enum_Descr(creatureBehaviorFlags, MONST_INVULNERABLE, "is invulnerable");
//		Enum_Descr(creatureBehaviorFlags, MONST_IMMUNE_TO_WATER, "is at home in water");
//		Enum_Descr(creatureBehaviorFlags, MONST_RESTRICTED_TO_LIQUID, "cannot venture onto dry land");
//		Enum_Descr(creatureBehaviorFlags, MONST_SUBMERGES, "submerges");
//		Enum_Descr(creatureBehaviorFlags, MONST_MAINTAINS_DISTANCE, "keeps $HISHER distance");
//		Enum_Descr(creatureBehaviorFlags, MONST_WILL_NOT_USE_STAIRS, "");
//		Enum_Descr(creatureBehaviorFlags, MONST_DIES_IF_NEGATED, "is animated purely by magic");
//		Enum_Descr(creatureBehaviorFlags, MONST_MALE, "");
//		Enum_Descr(creatureBehaviorFlags, MONST_FEMALE, "");
//		Enum_Descr(creatureBehaviorFlags, MONST_NOT_LISTED_IN_SIDEBAR, "");
//		Enum_Descr(creatureBehaviorFlags, MONST_GETS_TURN_ON_ACTIVATION, "moves only when activated");
//	End_Enum_Descr;
//End_Enum;
//
//Register_Enum(creatureBookkeepingFlags)
//	Declare_Enum_Name(creatureBookkeepingFlags)
//		Enum_Name(creatureBookkeepingFlags, MB_WAS_VISIBLE)
//		Enum_Name(creatureBookkeepingFlags, MB_TELEPATHICALLY_REVEALED)
//		Enum_Name(creatureBookkeepingFlags, MB_PREPLACED)
//		Enum_Name(creatureBookkeepingFlags, MB_APPROACHING_UPSTAIRS)
//		Enum_Name(creatureBookkeepingFlags, MB_APPROACHING_DOWNSTAIRS)
//		Enum_Name(creatureBookkeepingFlags, MB_APPROACHING_PIT)
//		Enum_Name(creatureBookkeepingFlags, MB_LEADER)
//		Enum_Name(creatureBookkeepingFlags, MB_FOLLOWER)
//		Enum_Name(creatureBookkeepingFlags, MB_CAPTIVE)
//		Enum_Name(creatureBookkeepingFlags, MB_SEIZED)
//		Enum_Name(creatureBookkeepingFlags, MB_SEIZING)
//		Enum_Name(creatureBookkeepingFlags, MB_SUBMERGED)
//		Enum_Name(creatureBookkeepingFlags, MB_JUST_SUMMONED)
//		Enum_Name(creatureBookkeepingFlags, MB_WILL_FLASH)
//		Enum_Name(creatureBookkeepingFlags, MB_BOUND_TO_LEADER)
//		Enum_Name(creatureBookkeepingFlags, MB_MARKED_FOR_SACRIFICE)
//	End_Enum_Name;
//
//	Declare_Enum_Descr(creatureBookkeepingFlags)
//		Enum_Descr(creatureBookkeepingFlags, MB_WAS_VISIBLE, "");
//		Enum_Descr(creatureBookkeepingFlags, MB_TELEPATHICALLY_REVEALED, "is telepathically bonded with you");
//		Enum_Descr(creatureBookkeepingFlags, MB_PREPLACED, "");
//		Enum_Descr(creatureBookkeepingFlags, MB_APPROACHING_UPSTAIRS, "");
//		Enum_Descr(creatureBookkeepingFlags, MB_APPROACHING_DOWNSTAIRS, "");
//		Enum_Descr(creatureBookkeepingFlags, MB_APPROACHING_PIT, "");
//		Enum_Descr(creatureBookkeepingFlags, MB_LEADER, "");
//		Enum_Descr(creatureBookkeepingFlags, MB_FOLLOWER, "");
//		Enum_Descr(creatureBookkeepingFlags, MB_CAPTIVE, "");
//		Enum_Descr(creatureBookkeepingFlags, MB_SEIZED, "has been immobilized");
//		Enum_Descr(creatureBookkeepingFlags, MB_SEIZING, "is currently holding $HISHER prey immobile");
//		Enum_Descr(creatureBookkeepingFlags, MB_SUBMERGED, "is submerged");
//		Enum_Descr(creatureBookkeepingFlags, MB_JUST_SUMMONED, "");
//		Enum_Descr(creatureBookkeepingFlags, MB_WILL_FLASH, "");
//		Enum_Descr(creatureBookkeepingFlags, MB_BOUND_TO_LEADER, "is anchored to reality by $HISHER summoner");
//		Enum_Descr(creatureBookkeepingFlags, MB_MARKED_FOR_SACRIFICE, "is marked for demonic sacrifice");
//	End_Enum_Descr;
//End_Enum;
//
//#pragma endregion creatureBehaviorConstants.h
//
//#pragma region feat.h
//
//Register_Enum(featTypes)
//	Declare_Enum_Name(featTypes)
//		Enum_Name(featTypes, FEAT_PURE_MAGE)
//		Enum_Name(featTypes, FEAT_PURE_WARRIOR)
//		Enum_Name(featTypes, FEAT_PACIFIST)
//		Enum_Name(featTypes, FEAT_ARCHIVIST)
//		Enum_Name(featTypes, FEAT_COMPANION)
//		Enum_Name(featTypes, FEAT_SPECIALIST)
//		Enum_Name(featTypes, FEAT_JELLYMANCER)
//		Enum_Name(featTypes, FEAT_INDOMITABLE)
//		Enum_Name(featTypes, FEAT_MYSTIC)
//		Enum_Name(featTypes, FEAT_DRAGONSLAYER)
//		Enum_Name(featTypes, FEAT_PALADIN)
//	End_Enum_Name;
//End_Enum;
//
//#pragma endregion feat.h