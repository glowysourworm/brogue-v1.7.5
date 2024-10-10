#pragma once

#include "color.h"

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
		bInterface = 9
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
