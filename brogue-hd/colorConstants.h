#pragma once

#include "color.h"
#include "brogueGlobal.h"

namespace brogueHd::backend::modelConstant
{
	enum colorCollections
	{
		game = 0,
		bolt = 1,
		tile = 2,
		creature = 3,
		light = 4,
		flare = 5,
		multipliers = 6,
		blood = 7,
		gas = 8,
		interface = 9
	};

	enum gameColors
	{
		white,
		gray,
		darkGray,
		veryDarkGray,
		black,
		yellow,
		darkYellow,
		teal,
		purple,
		darkPurple,
		brown,
		green,
		darkGreen,
		orange,
		darkOrange,
		blue,
		darkBlue,
		darkTurquoise,
		lightBlue,
		pink,
		red,
		darkRed,
		tanColor
	};

	enum boltColors
	{
		rainbow,
		descentBoltColor,
		discordColor,
		poisonColor,
		beckonColor,
		invulnerabilityColor,
		dominationColor,
		empowermentColor,
		fireBoltColor,
		yendorLightColor,
		dragonFireColor,
		flamedancerCoronaColor,
		shieldingColor
	};

	enum tileColors
	{
		undiscoveredColor,
		wallForeColor,
		wallBackColorStart,
		wallBackColorEnd,

		mudWallForeColor,
		//mudWallForeColor,
		mudWallBackColor,
		graniteBackColor,
		floorForeColor,

		floorBackColorStart,
		floorBackColorEnd,

		stairsBackColor,
		firstStairsBackColor,

		refuseBackColor,
		rubbleBackColor,
		bloodflowerForeColor,
		bloodflowerPodForeColor,
		bloodflowerBackColor,
		bedrollBackColor,

		obsidianBackColor,
		carpetForeColor,
		carpetBackColor,
		marbleForeColor,
		marbleBackColor,
		doorForeColor,
		doorBackColor,
		ironDoorForeColor,
		ironDoorBackColor,
		bridgeFrontColor,
		bridgeBackColor,
		statueBackColor,
		glyphColor,
		glyphLightColor,
		sacredGlyphColor,
		sacredGlyphLightColor,

		deepWaterForeColor,
		deepWaterBackColorStart,
		deepWaterBackColorEnd,
		shallowWaterForeColor,
		shallowWaterBackColorStart,
		shallowWaterBackColorEnd,

		mudForeColor,
		mudBackColor,
		chasmForeColor,
		chasmEdgeBackColorStart,
		chasmEdgeBackColorEnd,
		fireForeColor,
		lavaForeColor,
		brimstoneForeColor,
		brimstoneBackColor,

		lavaBackColor,
		acidBackColor,

		lightningColor,
		fungusLightColor,
		lavaLightColor,
		deepWaterLightColor,

		grassColor,
		deadGrassColor,
		fungusColor,
		grayFungusColor,
		foliageColor,
		deadFoliageColor,
		lichenColor,
		hayColor,
		ashForeColor,
		bonesForeColor,
		ectoplasmColor,
		forceFieldColor,
		wallCrystalColor,
		altarForeColor,
		altarBackColor,
		greenAltarBackColor,
		goldAltarBackColor,
		pedestalBackColor,
	};

	enum creatureColors
	{
		goblinColor,
		jackalColor,
		ogreColor,
		eelColor,
		goblinConjurerColor,
		spectralBladeColor,
		spectralImageColor,
		toadColor,
		trollColor,
		centipedeColor,
		dragonColor,
		krakenColor,
		salamanderColor,
		pixieColor,
		darPriestessColor,
		darMageColor,
		wraithColor,
		pinkJellyColor,
		wormColor,
		sentinelColor,
		goblinMysticColor,
		ifritColor,
		phoenixColor,
	};

	enum lightColors
	{
		minersLightStartColor,
		minersLightEndColor,
		torchColor,
		torchLightColor,
		hauntedTorchColor,
		hauntedTorchLightColor,
		ifritLightColor,
		unicornLightColor,
		wispLightColor,
		summonedImageLightColor,
		spectralBladeLightColor,
		ectoplasmLightColor,
		explosionColor,
		explosiveAuraColor,
		sacrificeTargetColor,
		dartFlashColor,
		lichLightColor,
		forceFieldLightColor,
		crystalWallLightColor,
		sunLightColor,
		fungusForestLightColor,
		fungusTrampledLightColor,
		redFlashColor,
		darknessPatchColor,
		darknessCloudColor,
		magicMapFlashColor,
		sentinelLightColor,
		telepathyColor,
		confusionLightColor,
		portalActivateLightColor,
		descentLightColor,
		algaeBlueLightColor,
		algaeGreenLightColor
	};

	enum flareColors
	{
		scrollProtectionColor,
		scrollEnchantmentColor,
		potionStrengthColor,
		empowermentFlashColor,
		genericFlashColor,
		summoningFlashColor,
		fireFlashColor,
		explosionFlareColor,
		quietusFlashColor,
		slayingFlashColor
	};

	enum colorMultipliers
	{
		colorDim25,
		colorMultiplier100,
		memoryColor,
		memoryOverlay,
		magicMapColor,
		clairvoyanceColor,
		telepathyMultiplier,
		omniscienceColor,
		basicLightColor,
	};

	enum bloodColors
	{
		humanBloodColor,
		insectBloodColor,
		vomitColor,
		urineColor,
		methaneColor
	};

	enum gasColors
	{
		poisonGasColor,
		confusionGasColor
	};

	enum interfaceColors
	{
		itemColor,
		blueBar,
		redBar,
		hiliteColor,
		interfaceBoxColor,
		interfaceButtonColor,
		buttonHoverColor,
		titleButtonColor,

		playerInvisibleColor,
		playerInLightColor,
		playerInShadowColor,
		playerInDarknessColor,

		inLightMultiplierColor,
		inDarknessMultiplierColor,

		goodMessageColor,
		badMessageColor,
		advancementMessageColor,
		itemMessageColor,
		flavorTextColor,
		backgroundMessageColor,

		superVictoryColor,

		flameSourceColor,
		flameSourceColorSecondary,

		flameTitleColor
	};

	// Macro Enum Definitions:  Used for string <-> value methods EnumString<...>::To and EnumString<...>::From

	Begin_Enum_String(colorCollections)
	{
		Enum_String(game);
		Enum_String(bolt);
		Enum_String(tile);
		Enum_String(creature);
		Enum_String(light);
		Enum_String(flare);
		Enum_String(multipliers);
		Enum_String(blood);
		Enum_String(gas);
		Enum_String(interface);
	}
	End_Enum_String;

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

	// Color array was changed - creating a compiler errro
	//color* dynamicColors[NUMBER_DYNAMIC_COLORS][3] = {
	//	// used color			shallow color				deep color
	//	{&minersLightColor,		&minersLightStartColor,		&minersLightEndColor},
	//	{&wallBackColor,		&wallBackColorStart,		&wallBackColorEnd},
	//	{&deepWaterBackColor,	&deepWaterBackColorStart,	&deepWaterBackColorEnd},
	//	{&shallowWaterBackColor,&shallowWaterBackColorStart,&shallowWaterBackColorEnd},
	//	{&floorBackColor,		&floorBackColorStart,		&floorBackColorEnd},
	//	{&chasmEdgeBackColor,	&chasmEdgeBackColorStart,	&chasmEdgeBackColorEnd},
	//};
}
