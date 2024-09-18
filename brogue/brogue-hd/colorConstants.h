#pragma once

#include "color.h"
#include "dungeonDeclaration.h"

namespace brogueHd
{
	// Red		Green	Blue	RedRand	GreenRand	BlueRand	Rand	Dances?
	// basic colors
	const color whiteBrogue = { 100,	100,	100,	0,		0,			0,			0,		false };
	const color grayBrogue = { 50,	50,		50,		0,		0,			0,			0,		false };
	const color darkGrayBrogue = { 30,	30,		30,		0,		0,			0,			0,		false };
	const color veryDarkGrayBrogue = { 15,	15,		15,		0,		0,			0,			0,		false };
	const color black = { 0,		0,		0,		0,		0,			0,			0,		false };
	const color yellow = { 100,	100,	0,		0,		0,			0,			0,		false };
	const color darkYellow = { 50,	50,		0,		0,		0,			0,			0,		false };
	const color teal = { 30,	100,	100,	0,		0,			0,			0,		false };
	const color purple = { 100,	0,		100,	0,		0,			0,			0,		false };
	const color darkPurple = { 50,	0,		50,		0,		0,			0,			0,		false };
	const color brown = { 60,	40,		0,		0,		0,			0,			0,		false };
	const color green = { 0,		100,	0,		0,		0,			0,			0,		false };
	const color darkGreen = { 0,		50,		0,		0,		0,			0,			0,		false };
	const color orange = { 100,	50,		0,		0,		0,			0,			0,		false };
	const color darkOrange = { 50,	25,		0,		0,		0,			0,			0,		false };
	const color blue = { 0,		0,		100,	0,		0,			0,			0,		false };
	const color darkBlue = { 0,		0,		50,		0,		0,			0,			0,		false };
	const color darkTurquoise = { 0,		40,		65,		0,		0,			0,			0,		false };
	const color lightBlue = { 40,	40,		100,	0,		0,			0,			0,		false };
	const color pink = { 100,	60,		66,		0,		0,			0,			0,		false };
	const color red = { 100,	0,		0,		0,		0,			0,			0,		false };
	const color darkRed = { 50,	0,		0,		0,		0,			0,			0,		false };
	const color tanColor = { 80,	67,		15,		0,		0,			0,			0,		false };

	// bolt colors
	const color rainbow = { -70,	-70,	-70,	170,	170,		170,		0,		true };
	const color descentBoltColor = { -40,   -40,    -40,    0,      0,          80,         80,     true };
	const color discordColor = { 25,	0,		25,		66,		0,			0,			0,		true };
	const color poisonColor = { 0,		0,		0,		10,		50,			10,			0,		true };
	const color beckonColor = { 10,	10,		10,		5,		5,			5,			50,		true };
	const color invulnerabilityColor = { 25,	0,		25,		0,		0,			66,			0,		true };
	const color dominationColor = { 0,		0,		100,	80,		25,			0,			0,		true };
	const color empowermentColor = { 30,    100,	40,     25,		80,			25,			0,		true };
	const color fireBoltColor = { 500,	150,	0,		45,		30,			0,			0,		true };
	const color yendorLightColor = { 50,    -100,    30,     0,      0,          0,          0,      true };
	const color dragonFireColor = { 500,	150,	0,      45,		30,			45,			0,		true };
	const color flamedancerCoronaColor = { 500,	150,	100,	45,		30,			0,			0,		true };
	//const color shieldingColor =		{100,	50,		0,		0,		50,			100,		0,		true};
	const color shieldingColor = { 150,	75,		0,		0,		50,			175,		0,		true };

	// tile colors
	const color undiscoveredColor = { 0,		0,		0,		0,		0,			0,			0,		false };

	const color wallForeColor = { 7,		7,		7,		3,		3,			3,			0,		false };

	color wallBackColor;
	const color wallBackColorStart = { 45,	40,		40,		15,		0,			5,			20,		false };
	const color wallBackColorEnd = { 40,	30,		35,		0,		20,			30,			20,		false };

	const color mudWallForeColor = { 55,	45,		0,		5,		5,			5,			1,		false };
	//const color mudWallForeColor =      {40,	34,		7,		0,		3,			0,			3,		false};
	const color mudWallBackColor = { 20,	12,		3,		8,		4,			3,			0,		false };

	const color graniteBackColor = { 10,	10,		10,		0,		0,			0,			0,		false };

	const color floorForeColor = { 30,	30,		30,		0,		0,			0,			35,		false };

	color floorBackColor;
	const color floorBackColorStart = { 2,		2,		10,		2,		2,			0,			0,		false };
	const color floorBackColorEnd = { 5,		5,		5,		2,		2,			0,			0,		false };

	const color stairsBackColor = { 15,	15,		5,		0,		0,			0,			0,		false };
	const color firstStairsBackColor = { 10,	10,		25,		0,		0,			0,			0,		false };

	const color refuseBackColor = { 6,		5,		3,		2,		2,			0,			0,		false };
	const color rubbleBackColor = { 7,		7,		8,		2,		2,			1,			0,		false };
	const color bloodflowerForeColor = { 30,    5,      40,     5,      1,          3,          0,      false };
	const color bloodflowerPodForeColor = { 50,  5,      25,     5,      1,          3,          0,      false };
	const color bloodflowerBackColor = { 15,    3,      10,     3,      1,          3,          0,      false };
	const color bedrollBackColor = { 10,	8,		5,		1,		1,			0,			0,		false };

	const color obsidianBackColor = { 6,		0,		8,		2,		0,			3,			0,		false };
	const color carpetForeColor = { 23,	30,		38,		0,		0,			0,			0,		false };
	const color carpetBackColor = { 15,	8,		5,		0,		0,			0,			0,		false };
	const color marbleForeColor = { 30,	23,		38,		0,		0,			0,			0,		false };
	const color marbleBackColor = { 6,     5,		13,		1,		0,			1,			0,		false };
	const color doorForeColor = { 70,	35,		15,		0,		0,			0,			0,		false };
	const color doorBackColor = { 30,	10,		5,		0,		0,			0,			0,		false };
	//const color ironDoorForeColor =		{40,	40,		40,		0,		0,			0,			0,		false};
	const color ironDoorForeColor = { 500,	500,	500,	0,		0,			0,			0,		false };
	const color ironDoorBackColor = { 15,	15,		30,		0,		0,			0,			0,		false };
	const color bridgeFrontColor = { 33,	12,		12,		12,		7,			2,			0,		false };
	const color bridgeBackColor = { 12,	3,		2,		3,		2,			1,			0,		false };
	const color statueBackColor = { 20,	20,		20,		0,		0,			0,			0,		false };
	const color glyphColor = { 20,    5,      5,      50,     0,          0,          0,      true };
	const color glyphLightColor = { 150,   0,      0,      150,    0,          0,          0,      true };
	const color sacredGlyphColor = { 5,     20,     5,      0,      50,         0,          0,      true };
	const color sacredGlyphLightColor = { 45,    150,	60,     25,		80,			25,			0,		true };

	//const color deepWaterForeColor =	{5,		5,		40,		0,		0,			10,			10,		true};
	//color deepWaterBackColor;
	//const color deepWaterBackColorStart = {5,	5,		55,		5,		5,			10,			10,		true};
	//const color deepWaterBackColorEnd =	{5,		5,		45,		2,		2,			5,			5,		true};
	//const color shallowWaterForeColor =	{40,	40,		90,		0,		0,			10,			10,		true};
	//color shallowWaterBackColor;
	//const color shallowWaterBackColorStart ={30,30,		80,		0,		0,			10,			10,		true};
	//const color shallowWaterBackColorEnd ={20,	20,		60,		0,		0,			5,			5,		true};

	const color deepWaterForeColor = { 5,		8,		20,		0,		4,			15,			10,		true };
	color deepWaterBackColor;
	const color deepWaterBackColorStart = { 5,	10,		31,		5,		5,			5,			6,		true };
	const color deepWaterBackColorEnd = { 5,		8,		20,		2,		3,			5,			5,		true };
	const color shallowWaterForeColor = { 28,	28,		60,		0,		0,			10,			10,		true };
	color shallowWaterBackColor;
	const color shallowWaterBackColorStart = { 20,20,		60,		0,		0,			10,			10,		true };
	const color shallowWaterBackColorEnd = { 12,	15,		40,		0,		0,			5,			5,		true };

	const color mudForeColor = { 18,	14,		5,		5,		5,			0,			0,		false };
	const color mudBackColor = { 23,	17,		7,		5,		5,			0,			0,		false };
	const color chasmForeColor = { 7,		7,		15,		4,		4,			8,			0,		false };
	color chasmEdgeBackColor;
	const color chasmEdgeBackColorStart = { 5,	5,		25,		2,		2,			2,			0,		false };
	const color chasmEdgeBackColorEnd = { 8,		8,		20,		2,		2,			2,			0,		false };
	const color fireForeColor = { 70,	20,		0,		15,		10,			0,			0,		true };
	const color lavaForeColor = { 20,	20,		20,		100,	10,			0,			0,		true };
	const color brimstoneForeColor = { 100,	50,		10,		0,		50,			40,			0,		true };
	const color brimstoneBackColor = { 18,	12,		9,		0,		0,			5,			0,		false };

	const color lavaBackColor = { 70,	20,		0,		15,		10,			0,			0,		true };
	const color acidBackColor = { 15,	80,		25,		5,		15,			10,			0,		true };

	const color lightningColor = { 100,	150,	500,	50,		50,			0,			50,		true };
	const color fungusLightColor = { 2,		11,		11,		4,		3,			3,			0,		true };
	const color lavaLightColor = { 47,	13,		0,		10,		7,			0,			0,		true };
	const color deepWaterLightColor = { 10,	30,		100,	0,		30,			100,		0,		true };

	const color grassColor = { 15,	40,		15,		15,		50,			15,			10,		false };
	const color deadGrassColor = { 20,	13,		0,		20,		10,			5,			10,		false };
	const color fungusColor = { 15,	50,		50,		0,		25,			0,			30,		true };
	const color grayFungusColor = { 30,	30,		30,		5,		5,			5,			10,		false };
	const color foliageColor = { 25,	100,	25,		15,		0,			15,			0,		false };
	const color deadFoliageColor = { 20,	13,		0,		30,		15,			0,			20,		false };
	const color lichenColor = { 50,	5,		25,		10,		0,			5,			0,		true };
	const color hayColor = { 70,	55,		5,		0,		20,			20,			0,		false };
	const color ashForeColor = { 20,	20,		20,		0,		0,			0,			20,		false };
	const color bonesForeColor = { 80,	80,		30,		5,		5,			35,			5,		false };
	const color ectoplasmColor = { 45,	20,		55,		25,		0,			25,			5,		false };
	const color forceFieldColor = { 0,		25,		25,		0,		25,			25,			0,		true };
	const color wallCrystalColor = { 40,	40,		60,		20,		20,			40,			0,		true };
	const color altarForeColor = { 5,		7,		9,		0,		0,			0,			0,		false };
	const color altarBackColor = { 35,	18,		18,		0,		0,			0,			0,		false };
	const color greenAltarBackColor = { 18,	25,		18,		0,		0,			0,			0,		false };
	const color goldAltarBackColor = { 25,	24,		12,		0,		0,			0,			0,		false };
	const color pedestalBackColor = { 10,	5,		20,		0,		0,			0,			0,		false };

	// monster colors
	const color goblinColor = { 40,	30,		20,		0,		0,			0,			0,		false };
	const color jackalColor = { 60,	42,		27,		0,		0,			0,			0,		false };
	const color ogreColor = { 60,	25,		25,		0,		0,			0,			0,		false };
	const color eelColor = { 30,	12,		12,		0,		0,			0,			0,		false };
	const color goblinConjurerColor = { 67,	10,		100,	0,		0,			0,			0,		false };
	const color spectralBladeColor = { 15,	15,		60,		0,		0,			70,			50,		true };
	const color spectralImageColor = { 13,	0,		0,		25,		0,			0,			0,		true };
	const color toadColor = { 40,	65,		30,		0,		0,			0,			0,		false };
	const color trollColor = { 40,	60,		15,		0,		0,			0,			0,		false };
	const color centipedeColor = { 75,	25,		85,		0,		0,			0,			0,		false };
	const color dragonColor = { 20,	80,		15,		0,		0,			0,			0,		false };
	const color krakenColor = { 100,	55,		55,		0,		0,			0,			0,		false };
	const color salamanderColor = { 40,	10,		0,		8,		5,			0,			0,		true };
	const color pixieColor = { 60,	60,		60,		40,		40,			40,			0,		true };
	const color darPriestessColor = { 0,		50,		50,		0,		0,			0,			0,		false };
	const color darMageColor = { 50,	50,		0,		0,		0,			0,			0,		false };
	const color wraithColor = { 66,	66,		25,		0,		0,			0,			0,		false };
	const color pinkJellyColor = { 100,	40,		40,		5,		5,			5,			20,		true };
	const color wormColor = { 80,	60,		40,		0,		0,			0,			0,		false };
	const color sentinelColor = { 3,		3,		30,		0,		0,			10,			0,		true };
	const color goblinMysticColor = { 10,	67,		100,	0,		0,			0,			0,		false };
	const color ifritColor = { 50,	10,		100,	75,		0,			20,			0,		true };
	const color phoenixColor = { 100,	0,		0,		0,		100,		0,			0,		true };

	// light colors
	color minersLightColor;
	const color minersLightStartColor = { 180,	180,	180,	0,		0,			0,			0,		false };
	const color minersLightEndColor = { 90,	90,		120,	0,		0,			0,			0,		false };
	const color torchColor = { 150,	75,		30,		0,		30,			20,			0,		true };
	const color torchLightColor = { 75,	38,		15,		0,		15,			7,			0,		true };
	//const color hauntedTorchColor =     {75,	30,		150,	30,		20,			0,			0,		true};
	const color hauntedTorchColor = { 75,	20,		40,     30,		10,			0,			0,		true };
	//const color hauntedTorchLightColor ={19,     7,		37,		8,		4,			0,			0,		true};
	const color hauntedTorchLightColor = { 67,    10,		10,		20,		4,			0,			0,		true };
	const color ifritLightColor = { 0,		10,		150,	100,	0,			100,		0,		true };
	//const color unicornLightColor =		{-50,	-50,	-50,	200,	200,		200,		0,		true};
	const color unicornLightColor = { -50,	-50,	-50,	250,	250,		250,		0,		true };
	const color wispLightColor = { 75,	100,	250,	33,		10,			0,			0,		true };
	const color summonedImageLightColor = { 200,	0,		75,		0,		0,			0,			0,		true };
	const color spectralBladeLightColor = { 40,	0,		230,	0,		0,			0,			0,		true };
	const color ectoplasmLightColor = { 23,	10,		28,		13,		0,			13,			3,		false };
	const color explosionColor = { 10,	8,		2,		0,		2,			2,			0,		true };
	const color explosiveAuraColor = { 2000,	0,      -1000,  200,    200,		0,          0,		true };
	const color sacrificeTargetColor = { 100,	-100,   -300,   0,      100,		100,        0,		true };
	const color dartFlashColor = { 500,	500,	500,	0,		2,			2,			0,		true };
	const color lichLightColor = { -50,	80,		30,		0,		0,			20,			0,		true };
	const color forceFieldLightColor = { 10,	10,		10,		0,		50,			50,			0,		true };
	const color crystalWallLightColor = { 10,	10,		10,		0,		0,			50,			0,		true };
	const color sunLightColor = { 100,	100,	75,		0,		0,			0,			0,		false };
	const color fungusForestLightColor = { 30,	40,		60,		0,		0,			0,			40,		true };
	const color fungusTrampledLightColor = { 10,	10,		10,		0,		50,			50,			0,		true };
	const color redFlashColor = { 100,	10,		10,		0,		0,			0,			0,		false };
	const color darknessPatchColor = { -10,	-10,	-10,	0,		0,			0,			0,		false };
	const color darknessCloudColor = { -20,	-20,	-20,	0,		0,			0,			0,		false };
	const color magicMapFlashColor = { 60,	20,		60,		0,		0,			0,			0,		false };
	const color sentinelLightColor = { 20,	20,		120,	10,		10,			60,			0,		true };
	const color telepathyColor = { 30,	30,		130,	0,		0,			0,			0,		false };
	const color confusionLightColor = { 10,	10,		10,		10,		10,			10,			0,		true };
	const color portalActivateLightColor = { 300,	400,	500,	0,		0,			0,			0,		true };
	const color descentLightColor = { 20,    20,     70,     0,      0,          0,          0,      false };
	const color algaeBlueLightColor = { 20,    15,     50,     0,      0,          0,          0,      false };
	const color algaeGreenLightColor = { 15,    50,     20,     0,      0,          0,          0,      false };

	// flare colors
	const color scrollProtectionColor = { 375,	750,	0,		0,		0,			0,          0,		true };
	const color scrollEnchantmentColor = { 250,	225,	300,	0,		0,			450,        0,		true };
	const color potionStrengthColor = { 1000,  0,      400,	600,	0,			0,          0,		true };
	const color empowermentFlashColor = { 500,   1000,   600,	0,      500,		0,          0,		true };
	const color genericFlashColor = { 800,   800,    800,    0,      0,          0,          0,      false };
	const color summoningFlashColor = { 0,     0,      0,      600,    0,          1200,       0,      true };
	const color fireFlashColor = { 750,	225,	0,		100,	50,			0,			0,		true };
	const color explosionFlareColor = { 10000, 6000,   1000,   0,      0,          0,          0,      false };
	const color quietusFlashColor = { 0,     -1000,  -200,   0,      0,          0,          0,      true };
	const color slayingFlashColor = { -1000, -200,   0,      0,      0,          0,          0,      true };

	// color multipliers
	const color colorDim25 = { 25,	25,		25,		25,		25,			25,			25,		false };
	const color colorMultiplier100 = { 100,	100,	100,	100,	100,		100,		100,	false };
	const color memoryColor = { 25,	25,		50,		20,		20,			20,			0,		false };
	const color memoryOverlay = { 25,	25,		50,		0,		0,			0,			0,		false };
	const color magicMapColor = { 60,	20,		60,		60,		20,			60,			0,		false };
	const color clairvoyanceColor = { 50,	90,		50,		50,		90,			50,			66,		false };
	const color telepathyMultiplier = { 30,	30,		130,	30,		30,			130,		66,		false };
	const color omniscienceColor = { 140,	100,	60,		140,	100,		60,			90,		false };
	const color basicLightColor = { 180,	180,	180,	180,	180,		180,		180,	false };

	// blood colors
	const color humanBloodColor = { 60,	20,		10,		15,		0,			0,			15,		false };
	const color insectBloodColor = { 10,	60,		20,		0,		15,			0,			15,		false };
	const color vomitColor = { 60,	50,		5,		0,		15,			15,			0,		false };
	const color urineColor = { 70,	70,		40,		0,		0,			0,			10,		false };
	const color methaneColor = { 45,	60,		15,		0,		0,			0,			0,		false };

	// gas colors
	const color poisonGasColor = { 75,	25,		85,		0,		0,			0,			0,		false };
	const color confusionGasColor = { 60,	60,		60,		40,		40,			40,			0,		true };

	// interface colors
	const color itemColor = { 100,	95,		-30,	0,		0,			0,			0,		false };
	const color blueBar = { 15,	10,		50,		0,		0,			0,			0,		false };
	const color redBar = { 45,	10,		15,		0,		0,			0,			0,		false };
	const color hiliteColor = { 100,	100,	0,		0,		0,			0,			0,		false };
	const color interfaceBoxColor = { 7,		6,		15,		0,		0,			0,			0,		false };
	const color interfaceButtonColor = { 18,	15,		38,		0,		0,			0,			0,		false };
	const color buttonHoverColor = { 100,	70,		40,		0,		0,			0,			0,		false };
	const color titleButtonColor = { 23,	15,		30,		0,		0,			0,			0,		false };

	const color playerInvisibleColor = { 20,    20,     30,     0,      0,          80,         0,      true };
	const color playerInLightColor = { 100,	90,     30,		0,		0,			0,			0,		false };
	const color playerInShadowColor = { 60,	60,		100,	0,		0,			0,			0,		false };
	const color playerInDarknessColor = { 30,	30,		65,		0,		0,			0,			0,		false };

	const color inLightMultiplierColor = { 150,   150,    75,     150,    150,        75,         100,    true };
	const color inDarknessMultiplierColor = { 66,  66,     120,    66,     66,         120,        66,     true };

	const color goodMessageColor = { 60,	50,		100,	0,		0,			0,			0,		false };
	const color badMessageColor = { 100,	50,		60,		0,		0,			0,			0,		false };
	const color advancementMessageColor = { 50,	100,	60,		0,		0,			0,			0,		false };
	const color itemMessageColor = { 100,	100,	50,		0,		0,			0,			0,		false };
	const color flavorTextColor = { 50,	40,		90,		0,		0,			0,			0,		false };
	const color backgroundMessageColor = { 60,	20,		70,		0,		0,			0,			0,		false };

	const color superVictoryColor = { 150,	100,	300,	0,		0,			0,			0,		false };

	//const color flameSourceColor = {0, 0, 0, 65, 40, 100, 0, true}; // 1
	//const color flameSourceColor = {0, 0, 0, 80, 50, 100, 0, true}; // 2
	//const color flameSourceColor = {25, 13, 25, 50, 25, 50, 0, true}; // 3
	//const color flameSourceColor = {20, 20, 20, 60, 20, 40, 0, true}; // 4
	//const color flameSourceColor = {30, 18, 18, 70, 36, 36, 0, true}; // 7**
	const color flameSourceColor = { 20, 7, 7, 60, 40, 40, 0, true }; // 8
	const color flameSourceColorSecondary = { 7, 2, 0, 10, 0, 0, 0, true };

	//const color flameTitleColor = {0, 0, 0, 17, 10, 6, 0, true}; // pale orange
	//const color flameTitleColor = {0, 0, 0, 7, 7, 10, 0, true}; // *pale blue*
	const color flameTitleColor = { 0, 0, 0, 9, 9, 15, 0, true }; // *pale blue**
	//const color flameTitleColor = {0, 0, 0, 11, 11, 18, 0, true}; // *pale blue*
	//const color flameTitleColor = {0, 0, 0, 15, 15, 9, 0, true}; // pale yellow
	//const color flameTitleColor = {0, 0, 0, 15, 9, 15, 0, true}; // pale purple

	// Color array was changed - creating a compiler errro
	color* dynamicColors[NUMBER_DYNAMIC_COLORS][3] = {
		// used color			shallow color				deep color
		{&minersLightColor,		&minersLightStartColor,		&minersLightEndColor},
		{&wallBackColor,		&wallBackColorStart,		&wallBackColorEnd},
		{&deepWaterBackColor,	&deepWaterBackColorStart,	&deepWaterBackColorEnd},
		{&shallowWaterBackColor,&shallowWaterBackColorStart,&shallowWaterBackColorEnd},
		{&floorBackColor,		&floorBackColorStart,		&floorBackColorEnd},
		{&chasmEdgeBackColor,	&chasmEdgeBackColorStart,	&chasmEdgeBackColorEnd},
	};
}
